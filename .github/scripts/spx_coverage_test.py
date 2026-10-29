#!/usr/bin/env python3
"""Run with the dependencies in requirements.txt and a native C++ compiler."""

import os
import shlex
import shutil
import subprocess
import sys
import tempfile
import unittest
import xml.etree.ElementTree as ET
from pathlib import Path

import spx_coverage
import yaml

SCRIPT = Path(__file__).with_name("spx_coverage.py").resolve()
REPO = SCRIPT.parents[2]


def write(path, text, executable=False):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)
    if executable:
        path.chmod(0o755)


def load_yaml(path):
    return yaml.load(path.read_text(), Loader=yaml.BaseLoader)


class CoverageReportTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler_flags = []
        if sys.platform == "darwin":
            cls.compiler = subprocess.check_output(["xcrun", "--find", "clang++"], text=True).strip()
            llvm_cov = subprocess.check_output(["xcrun", "--find", "llvm-cov"], text=True).strip()
            cls.gcov = shlex.join([llvm_cov, "gcov"])
            sdk = subprocess.check_output(["xcrun", "--show-sdk-path"], text=True).strip()
            cls.compiler_flags = ["-isysroot", sdk]
        else:
            cls.compiler = shutil.which(os.environ.get("CXX", "g++"))
            cls.gcov = os.environ.get("GCOV", "gcov")
            if not cls.compiler or not shutil.which(cls.gcov):
                raise unittest.SkipTest("GCC and gcov are required")

    def setUp(self):
        self.directory = tempfile.TemporaryDirectory(prefix="spx coverage ")
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name).resolve()
        self.source = self.root / ".spx-source"
        self.module = self.source / spx_coverage.MODULE
        self.output = self.root / "coverage.xml"
        self.binary = self.root / "fixture"
        write(self.module / "spx_sample.cpp", "int spx_value(int x) {\n  if (x > 0) return x + 1;\n  return 0;\n}\n")
        write(self.module / "spx_uncovered.cpp", "int spx_uncovered() {\n  return 10;\n}\n")
        write(self.module / "web/godot_js_spx_util.h", "inline int spx_inline(int x) {\n  return x * 2;\n}\n")
        for path, function in (
            ("tests/test_fixture.h", "test_helper"),
            ("thirdparty/vendor.h", "vendor_helper"),
            ("spx_fixture.gen.h", "generated_helper"),
            ("gdextension_spx_ext.h", "binding_helper"),
        ):
            write(self.module / path, f"inline int {function}() {{ return 1; }}\n")
        write(self.module / "web/godot_js_spx.cpp", "int generated_web() { return 1; }\n")
        write(
            self.root / "tests/test_main.cpp",
            """#include "web/godot_js_spx_util.h"
#include "tests/test_fixture.h"
#include "thirdparty/vendor.h"
#include "spx_fixture.gen.h"
#include "gdextension_spx_ext.h"
int spx_value(int);
int generated_web();
int main(int argc, char **) {
    if (argc > 1) return 0;
    return spx_value(1) + spx_inline(2) + test_helper() + vendor_helper()
        + generated_helper() + binding_helper() + generated_web() == 11 ? 0 : 1;
}
""",
        )
        sources = [
            self.module / "spx_sample.cpp",
            self.module / "spx_uncovered.cpp",
            self.module / "web/godot_js_spx.cpp",
            self.root / "tests/test_main.cpp",
        ]
        objects = []
        for source in sources:
            obj = source.with_suffix(".o")
            subprocess.run(
                [
                    self.compiler,
                    *self.compiler_flags,
                    "--coverage",
                    "-O0",
                    "-I",
                    str(self.module),
                    "-c",
                    str(source),
                    "-o",
                    str(obj),
                ],
                cwd=self.root,
                check=True,
            )
            objects.append(str(obj))
        subprocess.run(
            [self.compiler, *self.compiler_flags, "--coverage", *objects, "-o", str(self.binary)],
            cwd=self.root,
            check=True,
        )

    def report(self, success=True):
        result = subprocess.run(
            [sys.executable, str(SCRIPT), "report"],
            cwd=self.root,
            env={**os.environ, "GCOV": self.gcov},
            text=True,
            capture_output=True,
        )
        self.assertEqual(result.returncode, 0 if success else 1, result.stdout + result.stderr)
        self.assertEqual(self.output.exists(), success)
        return result

    def test_native_coverage_lifecycle(self):
        with self.subTest("report keeps unexecuted files and inline module headers"):
            subprocess.run([str(self.binary)], check=True)
            self.report()
            report = ET.parse(self.output).getroot()
            classes = {entry.get("filename"): entry for entry in report.findall(".//class")}
            self.assertEqual(
                set(classes),
                {
                    "godot_modules/spx/spx_sample.cpp",
                    "godot_modules/spx/spx_uncovered.cpp",
                    "godot_modules/spx/web/godot_js_spx_util.h",
                },
            )
            uncovered = classes["godot_modules/spx/spx_uncovered.cpp"].findall("./lines/line")
            self.assertTrue(uncovered)
            self.assertTrue(all(line.get("hits") == "0" for line in uncovered))
            inline = classes["godot_modules/spx/web/godot_js_spx_util.h"].findall("./lines/line")
            self.assertTrue(any(int(line.get("hits")) > 0 for line in inline))
            self.assertEqual([source.text for source in report.findall("./sources/source")], ["."])
            (source,) = report.findall("./sources/source")
            source.text = str(self.source)
            original = ET.tostring(report)
            spx_coverage.validate_report(report, self.source)
            self.assertEqual(ET.tostring(report), original)

        with self.subTest("reset clears both directories and missing execution fails"):
            self.assertTrue(list(self.module.rglob("*.gcda")))
            self.assertTrue(list((self.root / "tests").glob("*.gcda")))
            subprocess.run([sys.executable, str(SCRIPT), "reset"], cwd=self.root, check=True)
            self.assertFalse(list(self.root.rglob("*.gcda")))
            self.assertIn("execution data (.gcda) is missing", self.report(False).stderr)

        with self.subTest("zero executed production lines fails"):
            subprocess.run([str(self.binary), "skip-module"], check=True)
            self.assertIn("no executed SPX production lines", self.report(False).stderr)

        with self.subTest("missing metadata fails without retaining a stale report"):
            subprocess.run([str(self.binary)], check=True)
            self.output.write_text("stale report")
            for path in (self.root / "tests").glob("*.gcno"):
                path.unlink()
            self.assertIn("metadata (.gcno) is missing", self.report(False).stderr)


class CoverageWorkflowTest(unittest.TestCase):
    def test_cache_and_coverage_inputs(self):
        context = load_yaml(REPO / ".github/actions/setup-spx-module/build-context/action.yml")
        self.assertEqual(context["inputs"]["cache-enabled"]["default"], "true")
        cache = next(step for step in context["runs"]["steps"] if step.get("uses", "").startswith("actions/cache@"))
        self.assertEqual(cache["if"], "inputs.cache-enabled == 'true'")
        jobs = load_yaml(REPO / ".github/workflows/spx_module_checks.yml")["jobs"]
        native_job = jobs["linux-editor-tests"]
        self.assertEqual(native_job["env"], {"CC": "gcc", "CXX": "g++", "GCOV": "gcov"})
        native = native_job["steps"]
        context_step = next(step for step in native if step.get("id") == "spx-build")
        self.assertEqual(context_step["with"]["cache-enabled"], "false")
        web_context = next(step for step in jobs["web-normal-smoke"]["steps"] if step.get("id") == "spx-build")
        self.assertEqual(web_context["with"].get("cache-enabled", "true"), "true")
        build = next(step for step in native if step.get("uses") == "./.github/actions/godot-build")
        self.assertEqual(build["with"]["tests"], "yes")
        self.assertEqual(build["with"]["scons-cache"], "")
        for flag in ("use_coverage=yes", "optimize=none"):
            self.assertIn(flag, build["with"]["sconsflags"].split())
        for compiler in ("CC", "CXX"):
            self.assertIn(f"{compiler}=${{{{ env.{compiler} }}}}", build["with"]["sconsflags"])
        upload = next(step for step in native if step.get("uses", "").startswith("actions/upload-artifact@"))
        self.assertEqual(
            upload["with"],
            {
                "name": "coverage-spx-cpp",
                "path": "coverage.xml",
                "if-no-files-found": "error",
                "retention-days": "1",
                "overwrite": "true",
            },
        )

    def test_build_action_passes_empty_cache_to_scons(self):
        action = load_yaml(REPO / ".github/actions/godot-build/action.yml")
        script = action["runs"]["steps"][0]["run"].replace("${{ github.event.number }}", "")
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "bin").mkdir()
            record = root / "record"
            write(root / "tools/scons", '#!/bin/sh\nprintf "%s\\n" "$SCONS_CACHE" "$@" > "$RECORD"\n', True)
            env = {
                **os.environ,
                "PATH": str(root / "tools") + os.pathsep + os.environ["PATH"],
                "RECORD": str(record),
                "BUILD_PLATFORM": "linuxbsd",
                "BUILD_TARGET": "editor",
                "BUILD_TESTS": "yes",
                "SCONSFLAGS": "use_coverage=yes optimize=none",
                "CUSTOM_MODULES": "",
                "SCONS_CACHE": "",
                "SCONS_CACHE_LIMIT": "7",
            }
            subprocess.run(["bash", "-c", script], cwd=root, env=env, check=True, capture_output=True)
            self.assertEqual(
                record.read_text().splitlines(),
                [
                    "",
                    "platform=linuxbsd",
                    "target=editor",
                    "tests=yes",
                    "use_coverage=yes",
                    "optimize=none",
                    "cache_path=",
                    "cache_limit=7",
                ],
            )

    def test_report_rejects_invalid_or_excluded_paths(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory).resolve()
            write(root / "godot_modules/spx/spx_sample.cpp", "int sample;\n")
            for filename, reason in (
                ("/tmp/source.cpp", "invalid SPX coverage path"),
                ("../source.cpp", "invalid SPX coverage path"),
                ("godot_modules/spx/../source.cpp", "invalid SPX coverage path"),
                ("godot_modules/spx/tests/test.cpp", "excluded source leaked into coverage"),
                ("godot_modules/spx/thirdparty/lib.cpp", "excluded source leaked into coverage"),
                ("godot_modules/spx/spx_fixture.gen.h", "excluded source leaked into coverage"),
                ("godot_modules/spx/gdextension_spx_ext.cpp", "excluded source leaked into coverage"),
                ("godot_modules/spx/web/godot_js_spx.cpp", "excluded source leaked into coverage"),
                ("godot_modules/spx/missing.cpp", "coverage source is not in the SPX checkout"),
            ):
                with self.subTest(filename=filename):
                    report = ET.Element("coverage")
                    entry = ET.SubElement(report, "class", filename=filename)
                    ET.SubElement(ET.SubElement(entry, "lines"), "line", number="1", hits="1")
                    with self.assertRaisesRegex(ValueError, reason):
                        spx_coverage.validate_report(report, root)

    def test_workflow_resets_startup_counts_before_running_tests(self):
        workflow = load_yaml(REPO / ".github/workflows/spx_module_checks.yml")
        steps = workflow["jobs"]["linux-editor-tests"]["steps"]
        script = next(step["run"] for step in steps if step["name"] == "Run SPX module unit tests")
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            module = root / ".spx-source/godot_modules/spx"
            module.mkdir(parents=True)
            (root / "tests").mkdir()
            write(root / ".github/scripts/spx_coverage.py", SCRIPT.read_text())
            write(
                root / "bin/godot.linuxbsd.editor.dev.x86_64",
                """#!/bin/sh
set -eu
case "$*" in
  *--version*|*--list-test-cases*)
    printf startup > .spx-source/godot_modules/spx/startup.gcda
    printf startup > tests/startup.gcda
    printf '[SPX] fixture\n'
    ;;
  *)
    test ! -e .spx-source/godot_modules/spx/startup.gcda
    test ! -e tests/startup.gcda
    printf tested > tests/executed
    ;;
esac
""",
                True,
            )
            subprocess.run(
                ["bash", "-c", script],
                cwd=root,
                env={**os.environ, "RUNNER_TEMP": directory},
                check=True,
                capture_output=True,
            )
            self.assertEqual((root / "tests/executed").read_text(), "tested")


if __name__ == "__main__":
    unittest.main()
