#include "taskstorage.h"

#include <QCoreApplication>
#include <QDateTime>
#include <fstream>
#include <sstream>
#include <string>

QString TaskStorage::filePath(const QString &fileName)
{
    return QCoreApplication::applicationDirPath() + "/" + fileName;
}

QString TaskStorage::cleanField(const QString &value)
{
    QString result = value;
    result.replace("|", "/");
    result.replace("\n", " ");
    result.replace("\r", " ");
    return result;
}

std::vector<Task> TaskStorage::loadTasks()
{
    std::vector<Task> tasks;
    std::ifstream file(filePath("tasks.txt").toStdString());

    if (!file.is_open())
        return tasks;

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream stream(line);
        std::string name, subject, priority, hasDeadline, deadline, completed;

        if (!std::getline(stream, name, '|')) continue;
        if (!std::getline(stream, subject, '|')) continue;
        if (!std::getline(stream, priority, '|')) continue;
        if (!std::getline(stream, hasDeadline, '|')) continue;
        if (!std::getline(stream, deadline, '|')) continue;
        if (!std::getline(stream, completed, '|')) continue;

        Task task;
        task.name = QString::fromUtf8(name.c_str());
        task.subject = QString::fromUtf8(subject.c_str());
        task.priority = QString::fromUtf8(priority.c_str());
        task.hasDeadline = (hasDeadline == "1");
        task.deadline = QString::fromUtf8(deadline.c_str());
        task.completed = (completed == "1");
        tasks.push_back(task);
    }

    return tasks;
}

bool TaskStorage::saveTasks(const std::vector<Task> &tasks)
{
    std::ofstream file(filePath("tasks.txt").toStdString(), std::ios::trunc);
    if (!file.is_open())
        return false;

    for (const Task &task : tasks)
    {
        file << cleanField(task.name).toUtf8().toStdString() << '|'
             << cleanField(task.subject).toUtf8().toStdString() << '|'
             << cleanField(task.priority).toUtf8().toStdString() << '|'
             << (task.hasDeadline ? "1" : "0") << '|'
             << cleanField(task.deadline).toUtf8().toStdString() << '|'
             << (task.completed ? "1" : "0") << '\n';
    }

    return file.good();
}

std::vector<StudySession> TaskStorage::loadSessions()
{
    std::vector<StudySession> sessions;
    std::ifstream file(filePath("progress.txt").toStdString());

    if (!file.is_open())
        return sessions;

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream stream(line);
        std::string date, taskName, minutesText;

        if (!std::getline(stream, date, '|')) continue;
        if (!std::getline(stream, taskName, '|')) continue;
        if (!std::getline(stream, minutesText, '|')) continue;

        try
        {
            StudySession session;
            session.date = QString::fromUtf8(date.c_str());
            session.taskName = QString::fromUtf8(taskName.c_str());
            session.minutes = std::stoi(minutesText);
            sessions.push_back(session);
        }
        catch (...)
        {
            // Ignore a damaged progress line and continue reading the file.
        }
    }

    return sessions;
}

bool TaskStorage::addSession(const StudySession &session)
{
    std::ofstream file(filePath("progress.txt").toStdString(), std::ios::app);
    if (!file.is_open())
        return false;

    file << cleanField(session.date).toUtf8().toStdString() << '|'
         << cleanField(session.taskName).toUtf8().toStdString() << '|'
         << session.minutes << '\n';

    return file.good();
}

int TaskStorage::loadFocusMinutes()
{
    std::ifstream file(filePath("settings.txt").toStdString());
    int minutes = 25;
    bool notifications = true;

    if (file.is_open())
        file >> minutes >> notifications;

    if (minutes < 5 || minutes > 180)
        minutes = 25;

    return minutes;
}

bool TaskStorage::loadNotificationsEnabled()
{
    std::ifstream file(filePath("settings.txt").toStdString());
    int minutes = 25;
    bool notifications = true;

    if (file.is_open())
        file >> minutes >> notifications;

    return notifications;
}

bool TaskStorage::saveSettings(int focusMinutes, bool notificationsEnabled)
{
    std::ofstream file(filePath("settings.txt").toStdString(), std::ios::trunc);
    if (!file.is_open())
        return false;

    file << focusMinutes << ' ' << (notificationsEnabled ? 1 : 0) << '\n';
    return file.good();
}
