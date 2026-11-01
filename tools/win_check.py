#!/usr/bin/env python3
"""Runs the egg-built Windows programs on real Windows, from WSL.

  win_check.py crosscheck   every scenario through lofta_headless.exe; hashes must match Linux
  win_check.py smoke EXE    the real window on a hidden desktop, nothing shows on the screen

Programs are copied to a Windows temp folder first: a Windows process cannot start from a
\\\\wsl.localhost path. Outputs come back to build/egg/win/.
"""
import pathlib
import shutil
import subprocess
import sys
import uuid

ROOT = pathlib.Path(__file__).resolve().parent.parent
PS = shutil.which("powershell.exe") or "/mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0/powershell.exe"
OUT = ROOT / "build/egg/win"
SMOKE_FRAMES = 180
SMOKE_TIMEOUT = 60


def wpath(path):
    return subprocess.run(["wslpath", "-w", str(path)], capture_output=True, text=True, check=True).stdout.strip()


def windows_temp():
    win = subprocess.run([PS, "-NoProfile", "-Command", "[IO.Path]::GetTempPath()"],
                         capture_output=True, text=True, check=True).stdout.strip()
    return pathlib.Path(subprocess.run(["wslpath", "-u", win], capture_output=True, text=True, check=True).stdout.strip())


def workdir():
    path = windows_temp() / ("lofta_" + uuid.uuid4().hex[:8])
    path.mkdir(parents=True)
    return path


def crosscheck():
    exe = ROOT / "build/egg/lofta_headless.exe"
    work = workdir()
    try:
        shutil.copy(exe, work / exe.name)
        shutil.copytree(ROOT / "tests/lib", work / "tests/lib")
        shutil.copytree(ROOT / "tests/scripts", work / "tests/scripts")
        failed = 0
        hashes = []
        for script in sorted((work / "tests/scripts").glob("*.lua")):
            rel = "tests/scripts/" + script.name
            r = subprocess.run(["./" + exe.name, rel, "build/test_shots"], cwd=work, capture_output=True, text=True)
            out = r.stdout.replace("\r", "")
            sys.stdout.write(out)
            sys.stderr.write(r.stderr)
            hashes += [line for line in out.splitlines() if line.startswith("hash")]
            failed += r.returncode != 0
        remote = subprocess.run(
            ["ssh", "egg", "cd dev/game-dev/Lofta && build/lofta_headless tests/scripts/determinism.lua build/test_shots"],
            capture_output=True, text=True, check=True).stdout
        linux = [line for line in remote.splitlines() if line.startswith("hash")]
        print("linux  ", linux)
        print("windows", hashes)
        if hashes != linux or not hashes:
            print("RESULT hash mismatch")
            return 1
        print("RESULT crosscheck ok" if failed == 0 else "RESULT scenario failures")
        return 1 if failed else 0
    finally:
        shutil.rmtree(work, ignore_errors=True)


def smoke(exe_path):
    exe = pathlib.Path(exe_path).resolve()
    work = workdir()
    OUT.mkdir(parents=True, exist_ok=True)
    try:
        shutil.copy(exe, work / exe.name)
        shutil.copy(ROOT / "tools/win_hidden_run.ps1", work / "win_hidden_run.ps1")
        win = wpath(work)
        args = (f'--smoke-frames {SMOKE_FRAMES} --smoke-shot "{win}\\smoke.png" '
                f'--smoke-result "{win}\\smoke.txt" --save-dir "{win}\\saves"')
        r = subprocess.run([PS, "-NoProfile", "-ExecutionPolicy", "Bypass", "-File", win + "\\win_hidden_run.ps1",
                            "-Exe", win + "\\" + exe.name, "-WorkDir", win, "-TimeoutSec", str(SMOKE_TIMEOUT),
                            "-Arguments", args], capture_output=True, text=True, errors="replace",
                           timeout=SMOKE_TIMEOUT + 30)
        sys.stdout.write(r.stdout)
        sys.stderr.write(r.stderr)
        print("exit", r.returncode)
        result = work / "smoke.txt"
        if result.is_file():
            print(result.read_text(encoding="utf-8").strip())
        for name in ("smoke.png", "smoke.txt", "lofta_error.txt"):
            if (work / name).is_file():
                shutil.copy(work / name, OUT / name)
        ok = r.returncode == 0 and result.is_file() and (work / "smoke.png").is_file()
        print("RESULT smoke ok" if ok else "RESULT smoke failed")
        return 0 if ok else 1
    finally:
        shutil.rmtree(work, ignore_errors=True)


def main(argv):
    if len(argv) >= 2 and argv[1] == "crosscheck":
        return crosscheck()
    if len(argv) == 3 and argv[1] == "smoke":
        return smoke(argv[2])
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv))
