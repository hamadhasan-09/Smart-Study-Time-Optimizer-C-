# Smart-Study-Time-Optimizer-C++

A simple C++ console project that helps students decide what subject to study first.

## Current progress (approximately 30%)
- Enter available study hours.
- Enter up to 10 subjects, their difficulty (1-5), and days until deadline.
- Calculate a priority score for each subject.
- Show High, Medium, or Low priority and suggest which subject to study first.

**Not built yet:** dividing study hours between subjects and generating a daily schedule.

## How the priority score works
Start with the subject's difficulty (1 to 5).

- Deadline in 0-2 days: add 3.
- Deadline in 3-7 days: add 2.
- Deadline in 8 or more days: add 1.

Scores of 7-8 are **High**, 5-6 **Medium**, and 2-4 **Low**.

If several subjects share the highest score, the program suggests the first one entered.

Example:

| Subject | Difficulty | Days left | Score |
| --- | ---: | ---: | ---: |
| Linear Algebra | 5 | 2 | 8 (High) |
| C++ Programming | 4 | 5 | 6 (Medium) |
| Applied Physics | 3 | 9 | 4 (Low) |

## Run in VS Code (Windows)
1. Open the cloned repository in VS Code.
2. Install Microsoft's C/C++ extension if needed.
3. Press **Ctrl+Shift+B** to compile using the included MSVC build task (requires Visual Studio C++ Build Tools).
4. Run `.\study.exe` in the VS Code PowerShell terminal. You can also press **F5** to debug.

## Development plan
1. Subject input and display (done)
2. Priority calculation using difficulty and deadlines (done)
3. Study-time allocation and daily schedule (next)
4. Additional testing and improvements

## C++ concepts
Variables, input/output, `if-else` conditions, loops, arrays, and strings. Functions can be introduced in a later phase.
