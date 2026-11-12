#include <iostream>
#include <string>
using namespace std;

int main() {
    string subjects[10];
    int difficulty[10], daysLeft[10], priority[10];
    int count = 0;
    float hours = 0;

    cout << "=== Smart Study Time Optimizer ===\n";

    cout << "Available study hours: ";
    cin >> hours;
    if (!cin || hours <= 0) {
        cout << "Please enter valid study hours.\n";
        return 1;
    }

    cout << "Number of subjects (1-10): ";
    cin >> count;
    if (!cin || count < 1 || count > 10) {
        cout << "Please enter 1 to 10 subjects.\n";
        return 1;
    }

    for (int i = 0; i < count; i++) {
        cout << "\nSubject " << i + 1 << "\n";
        cout << "Name: ";
        getline(cin >> ws, subjects[i]); // Allows names with spaces

        cout << "Difficulty (1-5): ";
        cin >> difficulty[i];
        if (!cin || difficulty[i] < 1 || difficulty[i] > 5) {
            cout << "Difficulty must be from 1 to 5.\n";
            return 1;
        }

        cout << "Days until deadline (0 = today): ";
        cin >> daysLeft[i];
        if (!cin || daysLeft[i] < 0) {
            cout << "Days cannot be negative.\n";
            return 1;
        }

        // Harder subjects and closer deadlines get higher scores.
        priority[i] = difficulty[i];
        if (daysLeft[i] <= 2) {
            priority[i] += 3;
        } else if (daysLeft[i] <= 7) {
            priority[i] += 2;
        } else {
            priority[i] += 1;
        }
    }

    cout << "\n=== Study Priorities ===\n";
    cout << "Available time: " << hours << " hours\n";

    int best = 0;
    for (int i = 0; i < count; i++) {
        string level;
        if (priority[i] >= 7) {
            level = "High";
        } else if (priority[i] >= 5) {
            level = "Medium";
        } else {
            level = "Low";
        }

        cout << subjects[i] << " - Score: " << priority[i]
             << " (" << level << ")\n";

        if (priority[i] > priority[best]) {
            best = i;
        }
    }

    cout << "\nStudy first: " << subjects[best] << "\n";
    cout << "Time allocation will be added in the next phase.\n";
    return 0;
}
