# Run a program on a HIDDEN Windows desktop (CreateDesktop): the process renders on a separate
# desktop, so nothing appears on the screen the user is looking at. Used to verify the Windows exe
# (smoke runs) without disturbing the user. Call it from WSL via tools/win_check.py.
#
# NOTE: keep this file ASCII-only. Windows PowerShell 5.1 reads BOM-less UTF-8 scripts as the
# system code page, and non-ASCII characters break parsing.
#
#   powershell.exe -NoProfile -ExecutionPolicy Bypass -File win_hidden_run.ps1 `
#       -Exe C:\path\Game.exe -Arguments '-- --autoplay --shots=C:\tmp\shots' -TimeoutSec 240
#
# Exit code = the program's exit code; 124 if it timed out and was killed; 2 if the exe is missing.
param(
    [Parameter(Mandatory = $true)][string]$Exe,
    [string]$Arguments = "",
    [string]$WorkDir = "",
    [int]$TimeoutSec = 180
)

Add-Type -TypeDefinition @"
using System;
using System.Runtime.InteropServices;

public static class HiddenDesktop {
    [DllImport("user32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    static extern IntPtr CreateDesktopW(string name, IntPtr device, IntPtr devmode, int flags, uint access, IntPtr sa);
    [DllImport("user32.dll", SetLastError = true)] static extern bool CloseDesktop(IntPtr h);

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    struct STARTUPINFO {
        public int cb; public string lpReserved; public string lpDesktop; public string lpTitle;
        public int dwX, dwY, dwXSize, dwYSize, dwXCountChars, dwYCountChars, dwFillAttribute, dwFlags;
        public short wShowWindow, cbReserved2;
        public IntPtr lpReserved2, hStdInput, hStdOutput, hStdError;
    }
    [StructLayout(LayoutKind.Sequential)]
    struct PROCESS_INFORMATION { public IntPtr hProcess, hThread; public int dwProcessId, dwThreadId; }

    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    static extern bool CreateProcessW(string app, string cmd, IntPtr pa, IntPtr ta, bool inherit, uint flags,
                                      IntPtr env, string cwd, ref STARTUPINFO si, out PROCESS_INFORMATION pi);
    [DllImport("kernel32.dll")] static extern uint WaitForSingleObject(IntPtr h, uint ms);
    [DllImport("kernel32.dll")] static extern bool GetExitCodeProcess(IntPtr h, out uint code);
    [DllImport("kernel32.dll")] static extern bool TerminateProcess(IntPtr h, uint code);
    [DllImport("kernel32.dll")] static extern bool CloseHandle(IntPtr h);

    public static int Run(string exe, string args, string cwd, int timeoutMs) {
        string name = "lofta_hidden_" + Guid.NewGuid().ToString("N").Substring(0, 8);
        IntPtr desk = CreateDesktopW(name, IntPtr.Zero, IntPtr.Zero, 0, 0x10000000u /* GENERIC_ALL */, IntPtr.Zero);
        if (desk == IntPtr.Zero) throw new Exception("CreateDesktop failed, error " + Marshal.GetLastWin32Error());
        try {
            STARTUPINFO si = new STARTUPINFO();
            si.cb = Marshal.SizeOf(typeof(STARTUPINFO));
            si.lpDesktop = "WinSta0\\" + name;
            PROCESS_INFORMATION pi;
            string cmd = "\"" + exe + "\"" + (string.IsNullOrEmpty(args) ? "" : " " + args);
            if (!CreateProcessW(null, cmd, IntPtr.Zero, IntPtr.Zero, false, 0, IntPtr.Zero,
                                string.IsNullOrEmpty(cwd) ? null : cwd, ref si, out pi))
                throw new Exception("CreateProcess failed, error " + Marshal.GetLastWin32Error());
            try {
                uint w = WaitForSingleObject(pi.hProcess, (uint)timeoutMs);
                if (w != 0) { TerminateProcess(pi.hProcess, 124); return 124; }   // WAIT_TIMEOUT
                uint code; GetExitCodeProcess(pi.hProcess, out code);
                return unchecked((int)code);
            } finally { CloseHandle(pi.hProcess); CloseHandle(pi.hThread); }
        } finally { CloseDesktop(desk); }
    }
}
"@

if (-not (Test-Path -LiteralPath $Exe)) { Write-Error "exe not found: $Exe"; exit 2 }
exit ([HiddenDesktop]::Run($Exe, $Arguments, $WorkDir, $TimeoutSec * 1000))
