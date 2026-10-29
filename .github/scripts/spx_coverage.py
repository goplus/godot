#!/usr/bin/env python3
"""Collect native SPX coverage with paths relative to the SPX checkout."""

import argparse
import re
import subprocess
import sys
import xml.etree.ElementTree as ET
from pathlib import Path, PurePosixPath

MODULE = PurePosixPath("godot_modules/spx")
EXCLUDES = (
    r"(?:.*/)?(?:tests|thirdparty)/",
    r".*\.gen\.[^/]+$",
    r"gdextension_spx_ext\.[^/]+$",
    r"web/godot_js_spx\.cpp$",
)


def coverage_paths(build_root):
    spx_root = build_root / ".spx-source"
    module = spx_root / MODULE
    if not module.is_dir():
        raise ValueError(f"SPX module source is missing: {module}")
    return spx_root, module, (module, build_root / "tests")


def reset(build_root):
    _, _, directories = coverage_paths(build_root)
    for directory in directories:
        for path in directory.rglob("*.gcda"):
            path.unlink()


def validate_report(report, spx_root):
    classes = report.findall(".//class")
    if not classes:
        raise ValueError("coverage report contains no SPX source files")
    covered = valid = 0
    for entry in classes:
        filename = entry.get("filename", "")
        path = PurePosixPath(filename)
        if path.is_absolute() or ".." in path.parts or "\\" in filename or not path.is_relative_to(MODULE):
            raise ValueError(f"invalid SPX coverage path: {filename}")
        relative = path.relative_to(MODULE).as_posix()
        if any(re.match(pattern, relative) for pattern in EXCLUDES):
            raise ValueError(f"excluded source leaked into coverage: {filename}")
        source = (spx_root / path).resolve()
        if not source.is_relative_to(spx_root) or not source.is_file():
            raise ValueError(f"coverage source is not in the SPX checkout: {filename}")
        lines = entry.findall("./lines/line")
        valid += len(lines)
        covered += sum(int(line.get("hits", "0")) > 0 for line in lines)
    if not valid or not covered:
        raise ValueError("coverage report has no executed SPX production lines")
    # The report is uploaded from a different checkout in the SPX workflow.
    for source in report.findall("./sources/source"):
        source.text = "."
    return valid, covered


def collect(build_root, output):
    output.unlink(missing_ok=True)
    spx_root, module, directories = coverage_paths(build_root)
    for directory in directories:
        if not any(directory.rglob("*.gcno")):
            raise ValueError(f"coverage metadata (.gcno) is missing: {directory}")
    if not any(path for directory in directories for path in directory.rglob("*.gcda")):
        raise ValueError("coverage execution data (.gcda) is missing")
    command = [
        sys.executable,
        "-m",
        "gcovr",
        "--root",
        str(spx_root),
        "--filter",
        re.escape(module.as_posix()) + "/",
        "--cobertura",
        str(output),
        "--print-summary",
    ]
    for pattern in EXCLUDES:
        command.extend(("--exclude", re.escape(module.as_posix()) + "/" + pattern))
    command.extend(str(directory) for directory in directories)
    try:
        subprocess.run(command, cwd=build_root, check=True)
        report = ET.parse(output)
        valid, covered = validate_report(report.getroot(), spx_root)
        report.write(output, encoding="utf-8", xml_declaration=True)
    except Exception:
        output.unlink(missing_ok=True)
        raise
    print(f"SPX coverage: {covered}/{valid} production lines covered")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("reset", "report"))
    args = parser.parse_args()
    root = Path.cwd().resolve()
    try:
        if args.command == "reset":
            reset(root)
        else:
            collect(root, root / "coverage.xml")
    except (OSError, ValueError, ET.ParseError, subprocess.CalledProcessError) as error:
        print(f"[error] {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
