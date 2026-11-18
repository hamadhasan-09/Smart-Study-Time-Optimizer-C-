#include <iostream>
#include <string>
using namespace std;

int main() {
    string subjects[10];
    int difficulty[10], daysLeft[10], priority[10];
    float studyTime[10];
    int count;
    int totalPriority = 0;
    float hours;

    cout << "=== Smart Study Time Optimizer ===\n";

    cout << "Enter available study hours: ";
    cin >> hours;

    if (hours <= 0) {
        cout << "Please enter valid study hours.\n";
        return 0;
    }

    cout << "Enter number of subjects (1-10): ";
    cin >> count;

    if (count < 1 || count > 10) {
        cout << "Please enter 1 to 10 subjects.\n";
        return 0;
    }

    for (int i = 0; i < count; i++) {
        cout << "\nSubject " << i + 1 << "\n";

        cout << "Enter subject name: ";
        getline(cin >> ws, subjects[i]);

        cout << "Enter difficulty (1-5): ";
        cin >> difficulty[i];

        cout << "Enter days until deadline: ";
        cin >> daysLeft[i];

        if (difficulty[i] < 1 || difficulty[i] > 5 || daysLeft[i] < 0) {
            cout << "Invalid information. Please try again.\n";
            return 0;
        }

        priority[i] = difficulty[i];

        if (daysLeft[i] <= 2) {
            priority[i] = priority[i] + 3;
        }
        else if (daysLeft[i] <= 7) {
            priority[i] = priority[i] + 2;
        }
        else {
            priority[i] = priority[i] + 1;
        }

        totalPriority = totalPriority + priority[i];
    }

    cout << "\n=== Your Study Plan ===\n";

    int highest = 0;

    for (int i = 0; i < count; i++) {
        studyTime[i] = hours * priority[i] / totalPriority;

        cout << "\nSubject: " << subjects[i];
        cout << "\nPriority score: " << priority[i];
        cout << "\nStudy time: " << studyTime[i] << " hours\n";

        if (priority[i] > priority[highest]) {
            highest = i;
        }
    }

    cout << "\nYou should study first: " << subjects[highest] << "\n";

    return 0;
}
