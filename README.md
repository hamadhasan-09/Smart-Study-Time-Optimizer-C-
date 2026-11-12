# Smart-Study-Time-Optimizer-C++

A simple C++ program that creates daily study schedules based on subject difficulty and deadlines.

## Project idea
The student enters available study hours, subjects, difficulty levels (1-5), and days remaining until deadlines. The final program will give more time to difficult subjects and subjects with closer deadlines.

## Current progress
**Phase 1 complete:** The console program collects and displays study details for up to 10 subjects. It does not calculate a schedule yet.

## How to run
Open `main.cpp` in Visual Studio, or compile it using g++:

```bash
g++ -std=c++11 main.cpp -o study-optimizer
```

Then run `./study-optimizer` (or `study-optimizer.exe` on Windows).

## Development plan
1. Subject input and display (done)
2. Priority calculation using difficulty and deadlines
3. Study-time allocation and daily schedule
4. Testing and input validation

## C++ concepts
Variables, input/output, conditions, loops, arrays, strings, and functions.
