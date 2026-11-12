#include <iostream>
#include <string>
using namespace std;

int main() {
    const int MAX_SUBJECTS = 10;
    string subjects[MAX_SUBJECTS];
    int difficulty[MAX_SUBJECTS];
    int daysLeft[MAX_SUBJECTS];

    int subjectCount;
    float studyHours;

    cout << "=== Smart Study Time Optimizer ===\n";

    cout << "Enter available study hours: ";
    cin >> studyHours;

    cout << "Enter number of subjects (1-10): ";
    cin >> subjectCount;

    if (!cin || studyHours <= 0 || subjectCount < 1 || subjectCount > MAX_SUBJECTS) {
        cout << "Invalid study hours or number of subjects.\n";
        return 1;
    }

    for (int i = 0; i < subjectCount; i++) {
        cout << "\nSubject " << i + 1 << "\n";

        cout << "Name: ";
        getline(cin >> ws, subjects[i]);

        cout << "Difficulty (1-5): ";
        cin >> difficulty[i];

        cout << "Days until deadline (0 for today): ";
        cin >> daysLeft[i];

        if (!cin || subjects[i].empty() || difficulty[i] < 1 ||
            difficulty[i] > 5 || daysLeft[i] < 0) {
            cout << "Invalid subject information.\n";
            return 1;
        }
    }

    cout << "\n=== Your Study Details ===\n";
    cout << "Available time: " << studyHours << " hours\n";

    for (int i = 0; i < subjectCount; i++) {
        cout << i + 1 << ". " << subjects[i]
             << " | Difficulty: " << difficulty[i]
             << " | Days left: " << daysLeft[i] << "\n";
    }

    cout << "\nNext step: calculate priorities and generate a schedule.\n";
    return 0;
}
