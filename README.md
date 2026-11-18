# Smart-Study-Time-Optimizer-C++

A beginner-level C++ console project that helps students decide what to study and how much time to give each subject.

## Current progress (approximately 40%)
- Enter available study hours.
- Enter up to 10 subjects, their difficulty (1-5), and days until deadline.
- Calculate a priority score for each subject.
- Divide study hours between subjects according to their priority.
- Show the study time for each subject and suggest which one to study first.

## How the priority score works
Start with the subject's difficulty (1 to 5).

- Deadline in 0-2 days: add 3.
- Deadline in 3-7 days: add 2.
- Deadline in 8 or more days: add 1.

The program gives more study time to subjects with higher priority scores.

## Example
If a student has 4 available hours:

| Subject | Difficulty | Days left | Priority |
| --- | ---: | ---: | ---: |
| Linear Algebra | 5 | 2 | 8 |
| Programming | 4 | 5 | 6 |
| Applied Physics | 3 | 9 | 4 |

The program divides the 4 hours using these priority scores. Linear Algebra receives the most time and is suggested as the first subject.

## Run in VS Code (Windows)
1. Open this project folder in VS Code.
2. Install Microsoft's C/C++ extension if needed.
3. Press **Ctrl+Shift+B** to compile using the included MSVC build task (requires Visual Studio C++ Build Tools).
4. Run `.\study.exe` in the VS Code PowerShell terminal.

## Development plan
1. Subject input and display (done)
2. Priority calculation using difficulty and deadlines (done)
3. Priority-based study time allocation (done)
4. Testing, improvements, and a clearer daily schedule (next)

## C++ concepts
Variables, input/output, `if-else` conditions, loops, arrays, and strings.
