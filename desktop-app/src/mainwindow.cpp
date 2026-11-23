#include "mainwindow.h"
#include "taskdialog.h"
#include "taskstorage.h"

#include <QAbstractItemView>
#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateTime>
#include <QFormLayout>
#include <QFrame>
#include <QHeaderView>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

namespace
{
QFrame *makeCard(QWidget *parent = nullptr)
{
    QFrame *card = new QFrame(parent);
    card->setObjectName("card");
    return card;
}

QLabel *makeHeading(const QString &text, QWidget *parent = nullptr)
{
    QLabel *label = new QLabel(text, parent);
    label->setObjectName("sectionHeading");
    return label;
}

int priorityValue(const QString &priority)
{
    if (priority == "High") return 3;
    if (priority == "Medium") return 2;
    return 1;
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      remainingSeconds(25 * 60),
      timerRunning(false),
      focusMinutes(25),
      notificationsEnabled(true)
{
    setWindowTitle("Smart Study Time Optimizer");
    resize(1120, 720);
    setMinimumSize(900, 600);

    tasks = TaskStorage::loadTasks();
    sessions = TaskStorage::loadSessions();
    focusMinutes = TaskStorage::loadFocusMinutes();
    notificationsEnabled = TaskStorage::loadNotificationsEnabled();
    remainingSeconds = focusMinutes * 60;

    studyTimer = new QTimer(this);
    studyTimer->setInterval(1000);
    connect(studyTimer, &QTimer::timeout, this, [this]() { timerTick(); });

    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(14, 14, 14, 14);
    mainLayout->setSpacing(14);

    QFrame *sidebar = new QFrame(central);
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(190);
    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(12, 18, 12, 14);
    sideLayout->setSpacing(9);

    QLabel *appName = new QLabel("STUDY\nOPTIMIZER", sidebar);
    appName->setObjectName("appName");
    appName->setAlignment(Qt::AlignCenter);
    sideLayout->addWidget(appName);
    sideLayout->addSpacing(18);

    dashboardNav = new QPushButton("  Dashboard", sidebar);
    tasksNav = new QPushButton("  My Tasks", sidebar);
    focusNav = new QPushButton("  Focus Mode", sidebar);
    progressNav = new QPushButton("  Progress", sidebar);
    settingsNav = new QPushButton("  Settings", sidebar);

    const QList<QPushButton *> navButtons = {
        dashboardNav, tasksNav, focusNav, progressNav, settingsNav
    };

    for (QPushButton *button : navButtons)
    {
        button->setObjectName("navButton");
        button->setCheckable(true);
        button->setMinimumHeight(43);
        sideLayout->addWidget(button);
    }

    sideLayout->addStretch();
    QLabel *offlineLabel = new QLabel("LOCAL DESKTOP APP\nYour data stays on this PC", sidebar);
    offlineLabel->setObjectName("sidebarNote");
    offlineLabel->setAlignment(Qt::AlignCenter);
    sideLayout->addWidget(offlineLabel);

    pageStack = new QStackedWidget(central);
    pageStack->addWidget(createDashboardPage());
    pageStack->addWidget(createTasksPage());
    pageStack->addWidget(createFocusPage());
    pageStack->addWidget(createProgressPage());
    pageStack->addWidget(createSettingsPage());

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(pageStack, 1);
    setCentralWidget(central);

    connect(dashboardNav, &QPushButton::clicked, this, [this]() { changePage(0); });
    connect(tasksNav, &QPushButton::clicked, this, [this]() { changePage(1); });
    connect(focusNav, &QPushButton::clicked, this, [this]() { changePage(2); });
    connect(progressNav, &QPushButton::clicked, this, [this]() { changePage(3); });
    connect(settingsNav, &QPushButton::clicked, this, [this]() { changePage(4); });

    changePage(0);
    refreshAll();
}

QWidget *MainWindow::createDashboardPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(10, 8, 10, 8);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Good day!", page);
    title->setObjectName("pageTitle");
    QLabel *subtitle = new QLabel("Here is your study plan for today.", page);
    subtitle->setObjectName("mutedText");
    layout->addWidget(title);
    layout->addWidget(subtitle);

    QGridLayout *topGrid = new QGridLayout;
    topGrid->setSpacing(12);

    QFrame *focusCard = makeCard(page);
    QVBoxLayout *focusLayout = new QVBoxLayout(focusCard);
    focusLayout->setContentsMargins(18, 16, 18, 16);
    focusLayout->addWidget(makeHeading("YOUR DAILY FOCUS", focusCard));
    recommendedTaskLabel = new QLabel("Add a task to get started.", focusCard);
    recommendedTaskLabel->setObjectName("recommendationText");
    recommendedTaskLabel->setWordWrap(true);
    focusLayout->addWidget(recommendedTaskLabel);
    QPushButton *startRecommendedButton = new QPushButton("Start Studying", focusCard);
    startRecommendedButton->setObjectName("primaryButton");
    focusLayout->addWidget(startRecommendedButton, 0, Qt::AlignLeft);
    connect(startRecommendedButton, &QPushButton::clicked, this, [this]() {
        changePage(2);
        if (focusTaskCombo && recommendedTaskIndex() >= 0)
            focusTaskCombo->setCurrentText(tasks[recommendedTaskIndex()].name);
    });

    QFrame *progressCard = makeCard(page);
    QVBoxLayout *progressLayout = new QVBoxLayout(progressCard);
    progressLayout->setContentsMargins(18, 16, 18, 16);
    progressLayout->addWidget(makeHeading("DAILY PROGRESS", progressCard));
    dashboardProgressLabel = new QLabel("0 of 0 tasks completed", progressCard);
    dashboardProgressLabel->setObjectName("largeNumber");
    progressLayout->addWidget(dashboardProgressLabel);
    dashboardProgressBar = new QProgressBar(progressCard);
    dashboardProgressBar->setRange(0, 100);
    dashboardProgressBar->setValue(0);
    dashboardProgressBar->setTextVisible(false);
    dashboardProgressBar->setFixedHeight(12);
    progressLayout->addWidget(dashboardProgressBar);
    QLabel *progressNote = new QLabel("Complete your tasks to build your daily progress.", progressCard);
    progressNote->setObjectName("mutedText");
    progressNote->setWordWrap(true);
    progressLayout->addWidget(progressNote);

    topGrid->addWidget(focusCard, 0, 0);
    topGrid->addWidget(progressCard, 0, 1);
    topGrid->setColumnStretch(0, 3);
    topGrid->setColumnStretch(1, 2);
    layout->addLayout(topGrid);

    QFrame *tasksCard = makeCard(page);
    QVBoxLayout *tasksLayout = new QVBoxLayout(tasksCard);
    tasksLayout->setContentsMargins(18, 14, 18, 14);
    QHBoxLayout *tasksHeadingLayout = new QHBoxLayout;
    tasksHeadingLayout->addWidget(makeHeading("TODAY'S TASKS", tasksCard));
    tasksHeadingLayout->addStretch();
    QPushButton *viewTasksButton = new QPushButton("View all tasks", tasksCard);
    viewTasksButton->setObjectName("textButton");
    tasksHeadingLayout->addWidget(viewTasksButton);
    tasksLayout->addLayout(tasksHeadingLayout);
    dashboardTaskList = new QListWidget(tasksCard);
    dashboardTaskList->setObjectName("simpleList");
    tasksLayout->addWidget(dashboardTaskList);
    connect(viewTasksButton, &QPushButton::clicked, this, [this]() { changePage(1); });
    layout->addWidget(tasksCard, 1);

    return page;
}

QWidget *MainWindow::createTasksPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(10, 8, 10, 8);
    layout->setSpacing(12);

    QLabel *title = new QLabel("My Tasks", page);
    title->setObjectName("pageTitle");
    QLabel *subtitle = new QLabel("Organize your study work and priorities.", page);
    subtitle->setObjectName("mutedText");
    layout->addWidget(title);
    layout->addWidget(subtitle);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *addButton = new QPushButton("+ Add Task", page);
    addButton->setObjectName("primaryButton");
    QPushButton *editButton = new QPushButton("Edit", page);
    QPushButton *completeButton = new QPushButton("Mark Complete", page);
    QPushButton *deleteButton = new QPushButton("Delete", page);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(completeButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout);

    taskTable = new QTableWidget(page);
    taskTable->setColumnCount(5);
    taskTable->setHorizontalHeaderLabels({"Task", "Subject", "Priority", "Deadline", "Status"});
    taskTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    taskTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    taskTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    taskTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    taskTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    taskTable->setSelectionMode(QAbstractItemView::SingleSelection);
    taskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    taskTable->setAlternatingRowColors(true);
    taskTable->verticalHeader()->setVisible(false);
    layout->addWidget(taskTable, 1);

    connect(addButton, &QPushButton::clicked, this, [this]() { addTask(); });
    connect(editButton, &QPushButton::clicked, this, [this]() { editTask(); });
    connect(completeButton, &QPushButton::clicked, this, [this]() { markTaskComplete(); });
    connect(deleteButton, &QPushButton::clicked, this, [this]() { deleteTask(); });

    return page;
}

QWidget *MainWindow::createFocusPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(10, 8, 10, 8);
    layout->setSpacing(14);

    QLabel *title = new QLabel("Focus Mode", page);
    title->setObjectName("pageTitle");
    QLabel *subtitle = new QLabel("Choose a task, start the timer and concentrate.", page);
    subtitle->setObjectName("mutedText");
    layout->addWidget(title);
    layout->addWidget(subtitle);

    QFrame *timerCard = makeCard(page);
    QVBoxLayout *timerLayout = new QVBoxLayout(timerCard);
    timerLayout->setContentsMargins(25, 25, 25, 25);
    timerLayout->setSpacing(16);

    QLabel *taskLabel = new QLabel("STUDY TASK", timerCard);
    taskLabel->setObjectName("smallHeading");
    focusTaskCombo = new QComboBox(timerCard);
    focusTaskCombo->setMinimumHeight(38);
    timerLabel = new QLabel("25:00", timerCard);
    timerLabel->setObjectName("timerDisplay");
    timerLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *controls = new QHBoxLayout;
    startPauseButton = new QPushButton("Start", timerCard);
    startPauseButton->setObjectName("primaryButton");
    QPushButton *resetButton = new QPushButton("Reset", timerCard);
    controls->addStretch();
    controls->addWidget(startPauseButton);
    controls->addWidget(resetButton);
    controls->addStretch();

    QLabel *tip = new QLabel("Your completed study session will be saved on this computer.", timerCard);
    tip->setObjectName("mutedText");
    tip->setAlignment(Qt::AlignCenter);
    tip->setWordWrap(true);

    timerLayout->addWidget(taskLabel);
    timerLayout->addWidget(focusTaskCombo);
    timerLayout->addSpacing(10);
    timerLayout->addWidget(timerLabel);
    timerLayout->addLayout(controls);
    timerLayout->addWidget(tip);
    layout->addWidget(timerCard, 1);

    connect(startPauseButton, &QPushButton::clicked, this, [this]() { startOrPauseTimer(); });
    connect(resetButton, &QPushButton::clicked, this, [this]() { resetTimer(); });

    return page;
}

QWidget *MainWindow::createProgressPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(10, 8, 10, 8);
    layout->setSpacing(12);

    QLabel *title = new QLabel("Progress", page);
    title->setObjectName("pageTitle");
    QLabel *subtitle = new QLabel("A simple overview of your study activity.", page);
    subtitle->setObjectName("mutedText");
    layout->addWidget(title);
    layout->addWidget(subtitle);

    QGridLayout *statsLayout = new QGridLayout;
    statsLayout->setSpacing(12);

    QFrame *completedCard = makeCard(page);
    QVBoxLayout *completedLayout = new QVBoxLayout(completedCard);
    completedLayout->setContentsMargins(16, 16, 16, 16);
    completedLayout->addWidget(makeHeading("TASKS COMPLETED", completedCard));
    completedCountLabel = new QLabel("0", completedCard);
    completedCountLabel->setObjectName("largeNumber");
    completedLayout->addWidget(completedCountLabel);

    QFrame *pendingCard = makeCard(page);
    QVBoxLayout *pendingLayout = new QVBoxLayout(pendingCard);
    pendingLayout->setContentsMargins(16, 16, 16, 16);
    pendingLayout->addWidget(makeHeading("TASKS PENDING", pendingCard));
    pendingCountLabel = new QLabel("0", pendingCard);
    pendingCountLabel->setObjectName("largeNumber");
    pendingLayout->addWidget(pendingCountLabel);

    QFrame *timeCard = makeCard(page);
    QVBoxLayout *timeLayout = new QVBoxLayout(timeCard);
    timeLayout->setContentsMargins(16, 16, 16, 16);
    timeLayout->addWidget(makeHeading("TOTAL STUDY TIME", timeCard));
    totalStudyTimeLabel = new QLabel("0 min", timeCard);
    totalStudyTimeLabel->setObjectName("largeNumber");
    timeLayout->addWidget(totalStudyTimeLabel);

    statsLayout->addWidget(completedCard, 0, 0);
    statsLayout->addWidget(pendingCard, 0, 1);
    statsLayout->addWidget(timeCard, 0, 2);
    layout->addLayout(statsLayout);

    QFrame *historyCard = makeCard(page);
    QVBoxLayout *historyLayout = new QVBoxLayout(historyCard);
    historyLayout->setContentsMargins(18, 14, 18, 14);
    historyLayout->addWidget(makeHeading("RECENT STUDY SESSIONS", historyCard));
    sessionList = new QListWidget(historyCard);
    sessionList->setObjectName("simpleList");
    historyLayout->addWidget(sessionList);
    layout->addWidget(historyCard, 1);

    return page;
}

QWidget *MainWindow::createSettingsPage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(10, 8, 10, 8);
    layout->setSpacing(12);

    QLabel *title = new QLabel("Settings", page);
    title->setObjectName("pageTitle");
    QLabel *subtitle = new QLabel("Adjust a few basic study preferences.", page);
    subtitle->setObjectName("mutedText");
    layout->addWidget(title);
    layout->addWidget(subtitle);

    QFrame *settingsCard = makeCard(page);
    QVBoxLayout *cardLayout = new QVBoxLayout(settingsCard);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(16);
    cardLayout->addWidget(makeHeading("STUDY PREFERENCES", settingsCard));

    QFormLayout *form = new QFormLayout;
    focusMinutesSpin = new QSpinBox(settingsCard);
    focusMinutesSpin->setRange(5, 180);
    focusMinutesSpin->setSuffix(" minutes");
    focusMinutesSpin->setValue(focusMinutes);
    notificationsCheck = new QCheckBox("Show a message when a study timer finishes", settingsCard);
    notificationsCheck->setChecked(notificationsEnabled);
    form->addRow("Default focus session:", focusMinutesSpin);
    form->addRow("Reminders:", notificationsCheck);
    cardLayout->addLayout(form);

    QPushButton *saveButton = new QPushButton("Save Settings", settingsCard);
    saveButton->setObjectName("primaryButton");
    cardLayout->addWidget(saveButton, 0, Qt::AlignLeft);
    QLabel *storageNote = new QLabel("Your tasks, progress and settings are saved in local TXT files beside the application.", settingsCard);
    storageNote->setObjectName("mutedText");
    storageNote->setWordWrap(true);
    cardLayout->addWidget(storageNote);

    layout->addWidget(settingsCard);
    layout->addStretch();
    connect(saveButton, &QPushButton::clicked, this, [this]() { saveSettings(); });

    return page;
}

void MainWindow::changePage(int pageIndex)
{
    pageStack->setCurrentIndex(pageIndex);
    dashboardNav->setChecked(pageIndex == 0);
    tasksNav->setChecked(pageIndex == 1);
    focusNav->setChecked(pageIndex == 2);
    progressNav->setChecked(pageIndex == 3);
    settingsNav->setChecked(pageIndex == 4);

    if (pageIndex == 2)
        refreshFocusTasks();
    if (pageIndex == 3)
        refreshProgress();
}

void MainWindow::refreshAll()
{
    refreshDashboard();
    refreshTaskTable();
    refreshFocusTasks();
    refreshProgress();
}

int MainWindow::recommendedTaskIndex() const
{
    int bestIndex = -1;

    for (int i = 0; i < static_cast<int>(tasks.size()); ++i)
    {
        if (tasks[i].completed)
            continue;

        if (bestIndex == -1)
        {
            bestIndex = i;
            continue;
        }

        int currentPriority = priorityValue(tasks[i].priority);
        int bestPriority = priorityValue(tasks[bestIndex].priority);

        if (currentPriority > bestPriority)
        {
            bestIndex = i;
        }
        else if (currentPriority == bestPriority && tasks[i].hasDeadline && !tasks[bestIndex].hasDeadline)
        {
            bestIndex = i;
        }
        else if (currentPriority == bestPriority && tasks[i].hasDeadline && tasks[bestIndex].hasDeadline)
        {
            QDateTime currentDate = QDateTime::fromString(tasks[i].deadline, Qt::ISODate);
            QDateTime bestDate = QDateTime::fromString(tasks[bestIndex].deadline, Qt::ISODate);
            if (currentDate.isValid() && bestDate.isValid() && currentDate < bestDate)
                bestIndex = i;
        }
    }

    return bestIndex;
}

void MainWindow::refreshDashboard()
{
    int completed = 0;
    for (const Task &task : tasks)
    {
        if (task.completed)
            ++completed;
    }

    int total = static_cast<int>(tasks.size());
    int percent = total == 0 ? 0 : (completed * 100 / total);
    dashboardProgressLabel->setText(QString("%1 of %2 tasks completed").arg(completed).arg(total));
    dashboardProgressBar->setValue(percent);

    int nextTask = recommendedTaskIndex();
    if (nextTask == -1)
        recommendedTaskLabel->setText(total == 0 ? "Add your first study task to get started." : "All tasks are completed. Good work!");
    else
        recommendedTaskLabel->setText(tasks[nextTask].name + "\n" + tasks[nextTask].subject + "  |  " + tasks[nextTask].priority + " priority");
    dashboardTaskList->clear();
    if (tasks.empty())
    {
        dashboardTaskList->addItem("No tasks yet. Add a task from My Tasks.");
    }
    else
    {
        for (const Task &task : tasks)
        {
            QString status = task.completed ? "Completed" : "Pending";
            QString line = QString("%1   |   %2   |   %3")
                               .arg(task.name).arg(task.priority).arg(status);
            if (task.hasDeadline)
            {
                QDateTime date = QDateTime::fromString(task.deadline, Qt::ISODate);
                if (date.isValid())
                    line += "   |   " + date.toString("dd MMM, hh:mm AP");
            }
            dashboardTaskList->addItem(line);
        }
    }
}

void MainWindow::refreshTaskTable()
{
    taskTable->setRowCount(static_cast<int>(tasks.size()));

    for (int i = 0; i < static_cast<int>(tasks.size()); ++i)
    {
        const Task &task = tasks[i];
        QString deadlineText = "No deadline";
        if (task.hasDeadline)
        {
            QDateTime date = QDateTime::fromString(task.deadline, Qt::ISODate);
            deadlineText = date.isValid() ? date.toString("dd MMM yyyy, hh:mm AP") : "Set";
        }

        taskTable->setItem(i, 0, new QTableWidgetItem(task.name));
        taskTable->setItem(i, 1, new QTableWidgetItem(task.subject));
        taskTable->setItem(i, 2, new QTableWidgetItem(task.priority));
        taskTable->setItem(i, 3, new QTableWidgetItem(deadlineText));
        taskTable->setItem(i, 4, new QTableWidgetItem(task.completed ? "Completed" : "Pending"));
    }
}

void MainWindow::refreshFocusTasks()
{
    if (!focusTaskCombo)
        return;

    QString previousTask = focusTaskCombo->currentText();
    focusTaskCombo->clear();

    for (const Task &task : tasks)
    {
        if (!task.completed)
            focusTaskCombo->addItem(task.name);
    }

    if (focusTaskCombo->count() == 0)
        focusTaskCombo->addItem("General Study Session");
    else if (focusTaskCombo->findText(previousTask) >= 0)
        focusTaskCombo->setCurrentText(previousTask);
}

void MainWindow::refreshProgress()
{
    int completed = 0;
    int pending = 0;

    for (const Task &task : tasks)
    {
        if (task.completed) ++completed;
        else ++pending;
    }

    int totalMinutes = 0;
    for (const StudySession &session : sessions)
        totalMinutes += session.minutes;

    completedCountLabel->setText(QString::number(completed));
    pendingCountLabel->setText(QString::number(pending));

    if (totalMinutes >= 60)
    {
        int hours = totalMinutes / 60;
        int minutes = totalMinutes % 60;
        totalStudyTimeLabel->setText(QString("%1h %2m").arg(hours).arg(minutes));
    }
    else
    {
        totalStudyTimeLabel->setText(QString("%1 min").arg(totalMinutes));
    }

    sessionList->clear();
    if (sessions.empty())
    {
        sessionList->addItem("No completed study sessions yet.");
    }
    else
    {
        int shown = 0;
        for (auto it = sessions.rbegin(); it != sessions.rend() && shown < 10; ++it, ++shown)
        {
            QDate date = QDate::fromString(it->date, Qt::ISODate);
            QString dateText = date.isValid() ? date.toString("dd MMM yyyy") : it->date;
            sessionList->addItem(QString("%1  |  %2  |  %3 minutes")
                                     .arg(dateText).arg(it->taskName)
                                     .arg(it->minutes));
        }
    }
}

int MainWindow::selectedTaskIndex() const
{
    if (!taskTable)
        return -1;
    int row = taskTable->currentRow();
    if (row < 0 || row >= static_cast<int>(tasks.size()))
        return -1;
    return row;
}

void MainWindow::addTask()
{
    TaskDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    Task task = dialog.getTask();
    if (task.name.isEmpty() || task.subject.isEmpty())
    {
        QMessageBox::warning(this, "Missing Information", "Please enter both the task name and subject.");
        return;
    }

    tasks.push_back(task);
    saveTasks();
    refreshAll();
}

void MainWindow::editTask()
{
    int index = selectedTaskIndex();
    if (index == -1)
    {
        QMessageBox::information(this, "Select a Task", "Select a task from the table first.");
        return;
    }

    TaskDialog dialog(this, &tasks[index]);
    if (dialog.exec() != QDialog::Accepted)
        return;

    Task updatedTask = dialog.getTask();
    if (updatedTask.name.isEmpty() || updatedTask.subject.isEmpty())
    {
        QMessageBox::warning(this, "Missing Information", "Please enter both the task name and subject.");
        return;
    }

    updatedTask.completed = tasks[index].completed;
    tasks[index] = updatedTask;
    saveTasks();
    refreshAll();
}

void MainWindow::deleteTask()
{
    int index = selectedTaskIndex();
    if (index == -1)
    {
        QMessageBox::information(this, "Select a Task", "Select a task from the table first.");
        return;
    }

    QMessageBox::StandardButton answer = QMessageBox::question(
        this, "Delete Task", "Are you sure you want to delete this task?");
    if (answer != QMessageBox::Yes)
        return;

    tasks.erase(tasks.begin() + index);
    saveTasks();
    refreshAll();
}

void MainWindow::markTaskComplete()
{
    int index = selectedTaskIndex();
    if (index == -1)
    {
        QMessageBox::information(this, "Select a Task", "Select a task from the table first.");
        return;
    }

    tasks[index].completed = !tasks[index].completed;
    saveTasks();
    refreshAll();
}

void MainWindow::saveTasks()
{
    if (!TaskStorage::saveTasks(tasks))
    {
        QMessageBox::critical(this, "Save Error", "Could not save tasks.txt. Check the application folder permissions.");
    }
}

void MainWindow::updateTimerLabel()
{
    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;
    timerLabel->setText(QString("%1:%2")
                            .arg(minutes, 2, 10, QChar('0'))
                            .arg(seconds, 2, 10, QChar('0')));
}

void MainWindow::startOrPauseTimer()
{
    if (timerRunning)
    {
        studyTimer->stop();
        timerRunning = false;
        startPauseButton->setText("Resume");
        return;
    }

    if (remainingSeconds <= 0)
        remainingSeconds = focusMinutes * 60;

    studyTimer->start();
    timerRunning = true;
    startPauseButton->setText("Pause");
}

void MainWindow::resetTimer()
{
    studyTimer->stop();
    timerRunning = false;
    remainingSeconds = focusMinutes * 60;
    startPauseButton->setText("Start");
    updateTimerLabel();
}

void MainWindow::timerTick()
{
    if (remainingSeconds > 0)
        --remainingSeconds;

    updateTimerLabel();

    if (remainingSeconds == 0)
        finishStudySession();
}

void MainWindow::finishStudySession()
{
    studyTimer->stop();
    timerRunning = false;
    startPauseButton->setText("Start");

    StudySession session;
    session.date = QDate::currentDate().toString(Qt::ISODate);
    session.taskName = focusTaskCombo->currentText();
    session.minutes = focusMinutes;

    if (TaskStorage::addSession(session))
        sessions.push_back(session);
    else
        QMessageBox::warning(this, "Save Error", "The study session could not be saved to progress.txt.");

    remainingSeconds = focusMinutes * 60;
    updateTimerLabel();
    refreshProgress();
    refreshDashboard();

    if (notificationsEnabled)
        QMessageBox::information(this, "Study Session Complete", "Your focus session is complete. Take a short break.");
}

void MainWindow::saveSettings()
{
    int newFocusMinutes = focusMinutesSpin->value();
    bool newNotifications = notificationsCheck->isChecked();

    if (!TaskStorage::saveSettings(newFocusMinutes, newNotifications))
    {
        QMessageBox::critical(this, "Save Error", "Could not save settings.txt.");
        return;
    }

    focusMinutes = newFocusMinutes;
    notificationsEnabled = newNotifications;
    resetTimer();
    QMessageBox::information(this, "Settings Saved", "Your settings have been saved.");
}
