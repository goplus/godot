#!/usr/bin/env python3
"""Run with requirements.txt, locked SCons, and a native C++ compiler."""

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
        write(self.root / "core/engine.cpp", '#include "engine.gen.h"\nint engine_value() { return ENGINE_VALUE; }\n')
        write(self.root / ".github/scripts/spx_coverage.py", SCRIPT.read_text())
        write(
            self.root / "SConstruct",
            f"""import os
import pickle
from pathlib import Path
env = Environment(CXX={self.compiler!r}, ENV=os.environ,
    CCFLAGS={self.compiler_flags + ["--coverage", "-O0"]!r},
    LINKFLAGS={self.compiler_flags + ["--coverage"]!r}, CPPPATH=[{str(self.module)!r}])
env.SConsignFile('.sconsign%d.dblite' % pickle.HIGHEST_PROTOCOL)
env.Decider('MD5-timestamp')
module = env.StaticLibrary('modules/module_spx.fixture',
    Glob('.spx-source/godot_modules/spx/*.cpp') + Glob('.spx-source/godot_modules/spx/web/*.cpp'))
def generate_header(target, source, env):
    Path(str(target[0])).write_text(source[0].read())
header = env.Command('core/engine.gen.h', env.Value('#define ENGINE_VALUE 0\\n'), generate_header)
engine = env.Object('core/engine.cpp')
env.Depends(engine, header)
env.Program('fixture', ['tests/test_main.cpp', engine, module])
""",
        )
        self.build()

    def build(self):
        return subprocess.run(
            [sys.executable, "-m", "SCons", "-Q"], cwd=self.root, check=True, text=True, capture_output=True
        )

    def prepare_build(self, cache_hit):
        workflow = load_yaml(REPO / ".github/workflows/spx_module_checks.yml")
        steps = workflow["jobs"]["linux-editor-tests"]["steps"]
        script = next(step["run"] for step in steps if step["name"] == "Prepare restored coverage build")
        subprocess.run(
            ["bash", "-c", script],
            cwd=self.root,
            env={
                **os.environ,
                "PATH": str(Path(sys.executable).parent) + os.pathsep + os.environ["PATH"],
                "CACHE_HIT": "true" if cache_hit else "false",
            },
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

    def test_cached_build_reuses_objects_without_reusing_counts(self):
        subprocess.run([str(self.binary)], check=True)
        self.report()
        original_report = ET.parse(self.output).getroot()
        workflow = load_yaml(REPO / ".github/workflows/spx_module_checks.yml")
        products = {
            path
            for pattern in workflow["jobs"]["linux-editor-tests"]["env"]["COVERAGE_BUILD_PATHS"].splitlines()
            for path in self.root.glob(pattern)
            if path.is_file()
        }
        self.assertTrue(any(path.name.startswith(".sconsign") for path in products))
        self.assertEqual({p for p in products if p.suffix == ".gcno"}, set(self.root.rglob("*.gcno")))
        self.assertNotIn(self.binary, products)
        self.assertTrue(all(path.suffix in (".dblite", ".o", ".a", ".gcno") for path in products))
        object_times = {path: path.stat().st_mtime_ns for path in self.root.rglob("*.o")}
        engine_object = self.root / "core/engine.o"
        with tempfile.TemporaryDirectory() as directory:
            cache = Path(directory)
            for path in products:
                saved = cache / path.relative_to(self.root)
                saved.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(path, saved)
                path.unlink()
            self.binary.unlink()
            (self.root / "core/engine.gen.h").unlink()
            # Fresh checkouts have new timestamps; SCons must compare content.
            for path in self.root.rglob("*"):
                if path.suffix in (".cpp", ".h"):
                    path.write_text(path.read_text())
            for path in products:
                shutil.copy2(cache / path.relative_to(self.root), path)

        with self.subTest("exact restore reuses objects and metadata, then relinks"):
            self.assertTrue(list(self.root.rglob("*.gcda")))
            self.prepare_build(True)
            self.assertFalse(list(self.root.rglob("*.gcda")))
            self.build()
            self.assertTrue(self.binary.exists())
            self.assertEqual(object_times, {path: path.stat().st_mtime_ns for path in object_times})
            subprocess.run([str(self.binary)], check=True)
            self.report()
            report = ET.parse(self.output).getroot()
            self.assertEqual(ET.tostring(original_report.find("packages")), ET.tostring(report.find("packages")))

        with self.subTest("changed header recompiles its consumer"):
            header = self.module / "web/godot_js_spx_util.h"
            write(header, "inline int spx_inline(int x) {\n  return x * 2 + 0;\n}\n")
            # Godot's MD5-timestamp decider uses whole seconds.
            stamp = header.stat().st_mtime + 2
            os.utime(header, (stamp, stamp))
            self.build()
            test_object = self.root / "tests/test_main.o"
            self.assertNotEqual(test_object.stat().st_mtime_ns, object_times[test_object])
            self.assertEqual(engine_object.stat().st_mtime_ns, object_times[engine_object])

        with self.subTest("fallback removes deleted source metadata but preserves engine objects"):
            (self.module / "spx_uncovered.cpp").unlink()
            self.prepare_build(False)
            self.assertFalse(list(self.root.rglob("*.gcda")))
            self.assertFalse(list(self.module.rglob("*.gcno")))
            self.assertFalse(list((self.root / "tests").rglob("*.o")))
            self.assertFalse(list((self.root / "modules").glob("libmodule_spx.*.a")))
            self.build()
            self.assertEqual(engine_object.stat().st_mtime_ns, object_times[engine_object])
            self.assertFalse((self.module / "spx_uncovered.gcno").exists())
            subprocess.run([str(self.binary)], check=True)
            self.report()
            classes = {entry.get("filename") for entry in ET.parse(self.output).findall(".//class")}
            self.assertNotIn("godot_modules/spx/spx_uncovered.cpp", classes)


class CoverageWorkflowTest(unittest.TestCase):
    def test_cache_and_coverage_inputs(self):
        context = load_yaml(REPO / ".github/actions/setup-spx-module/build-context/action.yml")
        self.assertEqual(context["inputs"]["cache-enabled"]["default"], "true")
        cache = next(step for step in context["runs"]["steps"] if step.get("uses", "").startswith("actions/cache@"))
        self.assertEqual(cache["if"], "inputs.cache-enabled == 'true'")
        jobs = load_yaml(REPO / ".github/workflows/spx_module_checks.yml")["jobs"]
        native_job = jobs["linux-editor-tests"]
        self.assertEqual(
            {key: native_job["env"][key] for key in ("CC", "CXX", "GCOV")},
            {"CC": "gcc", "CXX": "g++", "GCOV": "gcov"},
        )
        native = native_job["steps"]
        context_step = next(step for step in native if step.get("id") == "spx-build")
        self.assertEqual(context_step["with"]["cache-enabled"], "false")
        web_context = next(step for step in jobs["web-normal-smoke"]["steps"] if step.get("id") == "spx-build")
        self.assertEqual(web_context["with"].get("cache-enabled", "true"), "true")
        build = next(step for step in native if step.get("uses") == "./.github/actions/godot-build")
        self.assertEqual(build["with"]["tests"], "yes")
        self.assertEqual(build["with"]["scons-cache"], "")
        for flag in ("use_coverage=yes", "optimize=none", "fast_unsafe=no"):
            self.assertIn(flag, build["with"]["sconsflags"].split())
        for compiler in ("CC", "CXX"):
            self.assertIn(f"{compiler}=${{{{ env.{compiler} }}}}", build["with"]["sconsflags"])
        restore = next(step for step in native if step.get("id") == "coverage-cache")
        save = next(step for step in native if step.get("uses", "") == "actions/cache/save@v5")
        self.assertEqual(restore["uses"], "actions/cache/restore@v5")
        self.assertEqual(save["with"]["path"], restore["with"]["path"])
        self.assertEqual(restore["with"]["path"], "${{ env.COVERAGE_BUILD_PATHS }}")
        self.assertEqual(save["with"]["key"], "${{ steps.coverage-cache.outputs.cache-primary-key }}")
        self.assertEqual(save["if"], "steps.coverage-cache.outputs.cache-hit != 'true'")
        self.assertLess(native.index(build), native.index(save))
        tests = next(step for step in native if step["name"] == "Run SPX module unit tests")
        self.assertLess(native.index(save), native.index(tests))
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
