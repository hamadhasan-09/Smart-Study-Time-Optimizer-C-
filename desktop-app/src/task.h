#ifndef TASK_H
#define TASK_H

#include <QString>

struct Task
{
    QString name;
    QString subject;
    QString priority;
    bool hasDeadline = false;
    QString deadline;
    bool completed = false;
};

struct StudySession
{
    QString date;
    QString taskName;
    int minutes = 0;
};

#endif // TASK_H
