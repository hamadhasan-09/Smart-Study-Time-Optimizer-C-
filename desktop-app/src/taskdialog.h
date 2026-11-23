#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include "task.h"
#include <QDialog>

class QLineEdit;
class QComboBox;
class QDateTimeEdit;

class TaskDialog : public QDialog
{
public:
    explicit TaskDialog(QWidget *parent = nullptr, const Task *task = nullptr);
    Task getTask() const;

private:
    QLineEdit *nameEdit;
    QLineEdit *subjectEdit;
    QComboBox *priorityCombo;
    QComboBox *deadlineCombo;
    QDateTimeEdit *deadlineEdit;
};

#endif // TASKDIALOG_H
