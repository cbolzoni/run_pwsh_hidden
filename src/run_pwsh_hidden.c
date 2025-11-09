// run_pwsh_hidden.c
// Build with (MSYS2 MinGW-w64 or similar):
//   x86_64-w64-mingw32-gcc -O2 -s -static -municode -mwindows -o run_pwsh_hidden.exe run_pwsh_hidden.c
// The program launches a PowerShell script hidden (no flashing console)
// and returns the script's exit code to the caller (e.g., StExBar).

#include <windows.h>
#include <shellapi.h>   // CommandLineToArgvW

static void dbg(const wchar_t *s) {
    OutputDebugStringW(s); // view via DebugView if needed
}

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPWSTR lpCmdLine, int nShowCmd) {
    int argc = 0;
    LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!argv || argc < 2) {
        // Silent failure (no message boxes)
        return 2;
    }

    // argv[1] = script path, argv[2..] = args
    const wchar_t *pwsh_default = L"C:\\Program Files\\PowerShell\\7\\pwsh.exe";
    wchar_t app[MAX_PATH];
    if (GetFileAttributesW(pwsh_default) != INVALID_FILE_ATTRIBUTES) {
        lstrcpynW(app, pwsh_default, MAX_PATH);
    } else {
        lstrcpynW(app, L"powershell.exe", MAX_PATH); // fallback to Windows PowerShell
    }

    // Build command line: "<app>" -NoProfile -ExecutionPolicy Bypass -File "<script>" [args...]
    wchar_t cmd[32768];
    int pos = wsprintfW(cmd,
                        L"\"%s\" -NoProfile -ExecutionPolicy Bypass -File \"%s\"",
                        app, argv[1]);
    for (int i = 2; i < argc; ++i) {
        pos += wsprintfW(cmd + pos, L" \"%s\"", argv[i]);
    }

    STARTUPINFOW si = {0};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi = {0};

    BOOL ok = CreateProcessW(
        NULL,             // application name
        cmd,              // full command line (mutable buffer)
        NULL, NULL, FALSE,
        CREATE_NO_WINDOW, // no console window
        NULL, NULL,
        &si, &pi
    );

    if (!ok) {
        DWORD e = GetLastError();
        wchar_t buf[128];
        wsprintfW(buf, L"CreateProcess failed: %lu\n", e);
        dbg(buf);
        return 3;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return (int)exitCode;
}
