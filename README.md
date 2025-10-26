# Clean Empty Folders Tool

## Overview

`cef.exe` is a lightweight Windows utility designed to recursively scan and remove empty directories within a given folder.  
It intelligently preserves directories containing files (at any depth) and only deletes folder structures that contain no files at all.

This tool can operate in **two modes**:
1. **Integration Mode** – Called by a parent GUI application via `CreateProcess` with redirected I/O (pipe).  
   - No console window appears.  
   - Logs are streamed to `stdout` for real-time GUI display.
2. **Standalone Mode** – Executed directly (double-click).  
   - No console window.  
   - A dialog box (`MessageBox`) displays the operation summary and result logs.

---

## Features

- Recursive scan of all subdirectories.
- Automatically removes nested folders that contain no files.
- Logs every deleted folder and any encountered errors.
- Dual-mode operation:  
  - Works as a **background tool** for GUI integration.  
  - Works as a **standalone cleaner** with GUI feedback via MessageBox.
- Compatible with **Windows 10+**, built using **C++17** and `std::filesystem`.

---

## Usage

### 1. As a standalone executable
Simply place `cef.exe` inside the target directory and double-click it.  
It will:
- Start scanning from the **current working directory**.
- Delete all empty nested folders.
- Display a summary log via MessageBox.

### 2. As a child process (for GUI integration)
From your main application:
- Call the executable via `CreateProcess` or `ShellExecute`.
- Redirect `stdout` and `stderr` using pipes to capture live log output.
- The tool will not display any UI or console window.

Example (pseudocode):
```cpp
// Simplified CreateProcess example
CreatePipe(&hRead, &hWrite, &saAttr, 0);
STARTUPINFOA si = { sizeof(si) };
si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
si.hStdOutput = hWrite;
si.hStdError = hWrite;
PROCESS_INFORMATION pi;
CreateProcessA("cef.exe", nullptr, nullptr, nullptr, TRUE,
               CREATE_NO_WINDOW, nullptr, "C:\\TargetDir", &si, &pi);
```

---

## Build Instructions

### Requirements

- Compiler: GCC (MinGW), MSVC, or Clang

- C++ Standard: C++17 or higher

- OS: Windows 10 or newer

### Command (GCC / MinGW)
```bash
g++ -st d=c++17 -O2 -Wall src/cef.cpp -o src/cef.exe -mwindows
```

> The -mwindows flag ensures the program runs without opening a console window.

---

## Logging Behavior

Mode|Output Method
-|-
GUI Integration (Pipe)|Writes logs to stdout
Standalone Execution|Shows logs via MessageBox

Example log output:

```bash
[START] Checking directory: C:\Projects\Test
[DELETE] C:\Projects\Test\EmptyFolder1
[DELETE] C:\Projects\Test\Sub\EmptyFolder2
[DONE] Completed.
```

---

## Notes

- Only directories with zero files (at all levels) are deleted.

- Error handling is implemented for permission issues or locked files.

- The tool performs a read-only recursive scan first, then deletes as needed.

- Safe to integrate into existing GUI file managers or cleanup tools.

---

## Future Improvements (Planned)

- Configurable target directory via CLI argument.

- Optional dry-run mode.

- JSON or XML structured logging for GUI consumption.

- Localization for English/Vietnamese message strings.

## License

This code is provided under the MIT License.
You are free to use, modify, and distribute it within your application.