#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "task.h"
#include <QMainWindow>
#include <QTimer>
#include <vector>

class QLabel;
class QListWidget;
class QPushButton;
class QStackedWidget;
class QTableWidget;
class QComboBox;
class QProgressBar;
class QSpinBox;
class QCheckBox;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    std::vector<Task> tasks;
    std::vector<StudySession> sessions;
    QTimer *studyTimer;
    int remainingSeconds;
    bool timerRunning;
    int focusMinutes;
    bool notificationsEnabled;

    QStackedWidget *pageStack;
    QPushButton *dashboardNav;
    QPushButton *tasksNav;
    QPushButton *focusNav;
    QPushButton *progressNav;
    QPushButton *settingsNav;

    QLabel *recommendedTaskLabel;
    QLabel *dashboardProgressLabel;
    QProgressBar *dashboardProgressBar;
    QListWidget *dashboardTaskList;

    QTableWidget *taskTable;

    QLabel *timerLabel;
    QComboBox *focusTaskCombo;
    QPushButton *startPauseButton;

    QLabel *completedCountLabel;
    QLabel *pendingCountLabel;
    QLabel *totalStudyTimeLabel;
    QListWidget *sessionList;

    QSpinBox *focusMinutesSpin;
    QCheckBox *notificationsCheck;

    QWidget *createDashboardPage();
    QWidget *createTasksPage();
    QWidget *createFocusPage();
    QWidget *createProgressPage();
    QWidget *createSettingsPage();

    void changePage(int pageIndex);
    void refreshAll();
    void refreshDashboard();
    void refreshTaskTable();
    void refreshFocusTasks();
    void refreshProgress();
    int recommendedTaskIndex() const;
    int selectedTaskIndex() const;
    void addTask();
    void editTask();
    void deleteTask();
    void markTaskComplete();
    void saveTasks();
    void updateTimerLabel();
    void startOrPauseTimer();
    void resetTimer();
    void timerTick();
    void finishStudySession();
    void saveSettings();
};

#endif // MAINWINDOW_H
