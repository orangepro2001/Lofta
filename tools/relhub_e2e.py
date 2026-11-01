#!/usr/bin/env python3
"""End to end test of in-game updates against a throwaway relhub, never the live service.

  relhub_e2e.py --relhub BIN --old OLD.exe --new NEW.exe [--write-fixture]

NEW is published as 0.1.1 (101). The server runs in WSL on a fixed local port with a temporary
data directory, and OLD runs on real Windows against it: latest, wrong key, offer only, tampered
package, then the real swap of the running program. Builds stamped with code 0 work too, because
the flags set the installed code; pass --new-stamp 0 for them.
"""
import argparse
import hashlib
import json
import pathlib
import shutil
import subprocess
import sys
import tempfile
import time
import urllib.request
import uuid

ROOT = pathlib.Path(__file__).resolve().parent.parent
PS = shutil.which("powershell.exe") or "/mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0/powershell.exe"
PORT = 28791
CHANNEL = "beta"
LIVE_KEY = "k4thZR+OVYSGAT/9j5F39csgGqTL0t3X6GYdNCmuRI8="
NOTES = '第一行说明\n带 "引号"、反斜杠 \\ 和换行的第二行\n'
WAIT_MARKER_S = 20
PROGRAM = "Lofta.exe"


def wpath(path):
    return subprocess.run(["wslpath", "-w", str(path)], capture_output=True, text=True, check=True).stdout.strip()


def windows_temp():
    win = subprocess.run([PS, "-NoProfile", "-Command", "[IO.Path]::GetTempPath()"],
                         capture_output=True, text=True, check=True).stdout.strip()
    return pathlib.Path(subprocess.run(["wslpath", "-u", win], capture_output=True, text=True, check=True).stdout.strip())


def sha256(path):
    return hashlib.sha256(pathlib.Path(path).read_bytes()).hexdigest()


class Checks:
    def __init__(self):
        self.failed = 0

    def check(self, condition, message):
        print(("ok   " if condition else "FAIL ") + message, flush=True)
        self.failed += 0 if condition else 1


def http(url):
    with urllib.request.urlopen(url, timeout=5) as r:
        return r.read()


def run_client(work, marker_name, *flags):
    marker = work / marker_name
    marker.unlink(missing_ok=True)
    win_marker = wpath(marker)
    subprocess.run([str(work / PROGRAM), *flags, f"--update-test-marker={win_marker}"], cwd=work,
                   capture_output=True, timeout=60)
    deadline = time.time() + WAIT_MARKER_S
    while time.time() < deadline:
        if marker.is_file() and marker.stat().st_size > 0:
            return marker.read_text(encoding="utf-8", errors="replace").strip()
        time.sleep(0.2)
    return ""


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--relhub", required=True)
    ap.add_argument("--old", required=True)
    ap.add_argument("--new", required=True)
    ap.add_argument("--write-fixture", action="store_true")
    ap.add_argument("--new-stamp", default="101", help="the version code stamped into NEW")
    ap.add_argument("--name", default="Lofta.exe", help="the name the program runs under")
    a = ap.parse_args()

    global PROGRAM
    PROGRAM = a.name
    checks = Checks()
    relhub = str(pathlib.Path(a.relhub).resolve())
    data = pathlib.Path(tempfile.mkdtemp(prefix="relhub-e2e-"))
    base = f"http://127.0.0.1:{PORT}/release"
    server = subprocess.Popen([relhub, "serve", "-addr", f"127.0.0.1:{PORT}", "-data", str(data), "-prefix", "/release"],
                              stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    work = windows_temp() / ("lofta_e2e_" + uuid.uuid4().hex[:8])
    work.mkdir(parents=True)
    try:
        for _ in range(100):
            try:
                if http(base + "/healthz"):
                    break
            except OSError:
                time.sleep(0.1)
        key = json.loads(http(base + "/v1/info"))["keys"][0]["publicKey"]
        common = ["-url", base, "-data", str(data)]
        subprocess.run([relhub, "app", "create", "-id", "lofta", "-name", "Lofta", *common], check=True, capture_output=True)
        notes = data.parent / (data.name + ".notes")
        notes.write_text(NOTES, encoding="utf-8")
        subprocess.run([relhub, "publish", *common, "-app", "lofta", "-platform", "win-x64", "-file", a.new,
                        "-version", "0.1.1", "-code", "101", "-channel", CHANNEL, "-notes-file", str(notes),
                        "-name", "Lofta-0.1.1-win-x64.exe"], check=True, capture_output=True)
        answer = http(f"{base}/v1/apps/lofta/check?platform=win-x64&channel={CHANNEL}&versionCode=100")
        if a.write_fixture:
            (ROOT / "tests/fixtures").mkdir(exist_ok=True)
            (ROOT / "tests/fixtures/check.json").write_bytes(answer)
            (ROOT / "tests/fixtures/key.txt").write_text(key + "\n", encoding="ascii")
            print("fixture written")

        shutil.copy(a.old, work / PROGRAM)
        time.sleep(2)
        if not (work / PROGRAM).is_file():
            print("RESULT blocked: Lofta.exe was removed from the temp folder (Windows Defender?)")
            return 3
        try:
            old_sha = sha256(work / PROGRAM)
        except OSError:
            print("RESULT blocked: Lofta.exe cannot be read (Windows Defender quarantine?)")
            return 3
        flags = [f"--update={base}", f"--update-channel={CHANNEL}", f"--update-keys={key}"]

        got = run_client(work, "m1.txt", *flags, "--update-installed-code=101")
        checks.check(got == "latest 0.1.1 101", f"up to date: {got!r}")

        got = run_client(work, "m2.txt", f"--update={base}", f"--update-channel={CHANNEL}", f"--update-keys={LIVE_KEY}")
        checks.check("signature is not from a trusted release key" in got, f"wrong key refused: {got!r}")

        got = run_client(work, "m3.txt", *flags)
        checks.check(got == "offered 0.1.1 101 mandatory=0", f"offer without install: {got!r}")
        checks.check(sha256(work / PROGRAM) == old_sha, "the program is untouched after an offer")

        stored = [p for p in data.rglob("*") if p.is_file() and p.stat().st_size == pathlib.Path(a.new).stat().st_size]
        checks.check(len(stored) == 1, f"found the stored package ({len(stored)})")
        original = stored[0].read_bytes()
        tampered = bytearray(original)
        tampered[len(tampered) // 2] ^= 0xFF
        stored[0].write_bytes(bytes(tampered))
        got = run_client(work, "m4.txt", *flags, "--update-auto-install")
        stored[0].write_bytes(original)
        checks.check("does not match the signed size and hash" in got, f"tampered package refused: {got!r}")
        checks.check(sha256(work / PROGRAM) == old_sha, "the program is untouched after a tampered download")
        checks.check(not (work / "updates/Lofta-0.1.1-win-x64.exe").exists(), "the tampered file is gone")

        got = run_client(work, "m5.txt", *flags, "--update-auto-install")
        checks.check(got == f"0.1.1 {a.new_stamp} old-removed=true", f"swap and restart: {got!r}")
        checks.check(sha256(work / PROGRAM) == sha256(a.new), "the program on disk is the new one")
        checks.check(not (work / (PROGRAM + ".old")).exists(), "the old program is gone")
        checks.check(not (work / "updates").exists(), "the updates folder is gone")
    finally:
        server.terminate()
        server.wait(timeout=10)
        shutil.rmtree(data, ignore_errors=True)
        pathlib.Path(str(data) + ".notes").unlink(missing_ok=True)
        shutil.rmtree(work, ignore_errors=True)
    print("RESULT e2e " + ("ok" if checks.failed == 0 else f"{checks.failed} failed"))
    return 1 if checks.failed else 0


if __name__ == "__main__":
    sys.exit(main())
