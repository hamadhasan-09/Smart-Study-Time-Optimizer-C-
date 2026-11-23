# Smart Study Time Optimizer — Desktop App

This folder contains the Qt Widgets desktop version of the Smart Study Time Optimizer mini-project. It is kept separate from the original console-based C++ version in the repository root.

## Features in this starter version

- Dashboard with a next-task recommendation based on priority and deadlines.
- Add, edit, delete and complete study tasks.
- High, Medium and Low task priorities.
- Optional deadline selector (Yes / No). Date and time fields appear only when Yes is selected.
- Focus timer linked to a selected task.
- Local progress tracking for completed focus sessions.
- Settings for focus-session length and timer-finished message.

## Technology

- C++17
- Qt 6 Widgets
- C++ file handling with `ifstream` and `ofstream`
- Local TXT files (no SQLite, cloud database or AI model)

## Build on Windows

Open this `desktop-app` folder in VS Code. Make sure the MSVC C++ compiler, CMake, Ninja and a matching Qt 6 MSVC kit are installed.

Example commands for Qt installed at `C:\Qt\6.12.0\msvc2022_64`:

```powershell
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH="C:/Qt/6.12.0/msvc2022_64"
cmake --build build
$env:PATH = "C:\Qt\6.12.0\msvc2022_64\bin;" + $env:PATH
.\build\SmartStudyTimeOptimizer.exe
```

Use the actual Qt installation path on your computer. The Qt kit and compiler must match (MSVC with MSVC, or MinGW with MinGW).

## Local data files

The application creates these files beside the executable when needed:

- `tasks.txt` — study tasks, priority, optional deadline and completion status.
- `progress.txt` — completed focus sessions.
- `settings.txt` — focus duration and timer-finished notification preference.

Keep these files to preserve saved data.

## Current limitations

- Recommendations use simple priority and deadline rules, not AI.
- Deadline notifications are not implemented; the current notification is shown when a focus timer finishes.
- A focus session is saved when the timer reaches zero. Pausing or resetting does not save a session.
- This source has not yet been compiled on the target Windows PC. Build and test it locally before treating it as submission-ready.
