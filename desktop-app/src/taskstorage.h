#ifndef TASKSTORAGE_H
#define TASKSTORAGE_H

#include "task.h"
#include <vector>

class TaskStorage
{
public:
    static std::vector<Task> loadTasks();
    static bool saveTasks(const std::vector<Task> &tasks);

    static std::vector<StudySession> loadSessions();
    static bool addSession(const StudySession &session);

    static int loadFocusMinutes();
    static bool loadNotificationsEnabled();
    static bool saveSettings(int focusMinutes, bool notificationsEnabled);

private:
    static QString filePath(const QString &fileName);
    static QString cleanField(const QString &value);
};

#endif // TASKSTORAGE_H
