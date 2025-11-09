# run_pwsh_hidden

`run_pwsh_hidden` is a tiny Windows utility that **launches a PowerShell script with no visible console window** and returns the script's exit code to the caller. It is ideal for **context-menu integrations** (e.g., StExBar) or scheduled tasks where a blinking console is undesirable.

## What it does

- Prefers **PowerShell 7** at `C:\Program Files\PowerShell\7\pwsh.exe`; falls back to **Windows PowerShell** (`powershell.exe`) if pwsh.exe is not found.
- Executes: `"pwsh" -NoProfile -ExecutionPolicy Bypass -File "<script.ps1>" [args...]`
- Runs hidden (`CREATE_NO_WINDOW` + `SW_HIDE`) and **propagates the script's exit code**.
- Silent failures (no message boxes); view debug output via **DebugView** if needed.

## Usage

```
run_pwsh_hidden.exe <script.ps1> [args...]
```

Examples:

```powershell
# Run a script without a console; %ERRORLEVEL% reflects the script's exit code
run_pwsh_hidden.exe "C:\Tools\Publish\deploy.ps1" -Env Prod -Force

# Use from StExBar: pass the selected path(s)
run_pwsh_hidden.exe "C:\Tools\Zip\zip.ps1" "%sel%"

# About / version dialog
run_pwsh_hidden.exe --about
run_pwsh_hidden.exe --version
```

Exit codes:
- `0` — the PowerShell script exited successfully
- `2` — missing command-line arguments
- `3` — process creation failed (see DebugView)

## Build

### MSYS2 / MinGW‑w64

```bash
pacman -S --needed mingw-w64-x86_64-toolchain
make
# -> build/run_pwsh_hidden.exe
```

### CMake (Windows)

```bash
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
# -> build/Release/run_pwsh_hidden.exe (or build/run_pwsh_hidden.exe)
```

## Integration ideas

- **StExBar custom command**:  
  Command: `C:\path\to\run_pwsh_hidden.exe`  
  Parameters: `"C:\\path\\to\\script.ps1" "%sel%"`

- **Task Scheduler**: call `run_pwsh_hidden.exe` with your script; no console flashes when the task triggers.

- **Explorer context menu**: register a verb pointing to this EXE and pass `%1`/`%V`.

## Security

- Uses `-ExecutionPolicy Bypass` to ensure the script runs without policy prompts. Only use with **trusted scripts**.
- No elevation is performed; if the script requires admin privileges, start the EXE under elevated context.

## License

MIT — see `LICENSE`.