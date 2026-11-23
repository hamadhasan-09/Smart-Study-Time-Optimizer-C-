#include "taskdialog.h"

#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>

TaskDialog::TaskDialog(QWidget *parent, const Task *task)
    : QDialog(parent)
{
    setWindowTitle(task ? "Edit Task" : "Add New Task");
    setMinimumWidth(390);

    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("For example: Revise Chapter 2");

    subjectEdit = new QLineEdit(this);
    subjectEdit->setPlaceholderText("For example: Applied Physics");

    priorityCombo = new QComboBox(this);
    priorityCombo->addItems({"High", "Medium", "Low"});

    deadlineCombo = new QComboBox(this);
    deadlineCombo->addItems({"No", "Yes"});

    deadlineEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    deadlineEdit->setCalendarPopup(true);
    deadlineEdit->setDisplayFormat("dd MMM yyyy, hh:mm AP");
    deadlineEdit->setMinimumDateTime(QDateTime::currentDateTime());
    deadlineEdit->setVisible(false);
    QLabel *deadlineLabel = new QLabel("Deadline date and time:", this);
    deadlineLabel->setVisible(false);

    connect(deadlineCombo, &QComboBox::currentTextChanged, this,
            [this, deadlineLabel](const QString &value) {
                bool showDeadline = (value == "Yes");
                deadlineEdit->setVisible(showDeadline);
                deadlineLabel->setVisible(showDeadline);
            });

    QFormLayout *form = new QFormLayout;
    form->addRow("Task name:", nameEdit);
    form->addRow("Subject:", subjectEdit);
    form->addRow("Priority:", priorityCombo);
    form->addRow("Does this task have a deadline?", deadlineCombo);
    form->addRow(deadlineLabel, deadlineEdit);

    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons);
    setLayout(layout);

    if (task)
    {
        nameEdit->setText(task->name);
        subjectEdit->setText(task->subject);
        priorityCombo->setCurrentText(task->priority);
        deadlineCombo->setCurrentText(task->hasDeadline ? "Yes" : "No");

        if (task->hasDeadline)
        {
            QDateTime savedDeadline = QDateTime::fromString(task->deadline, Qt::ISODate);
            if (savedDeadline.isValid())
                deadlineEdit->setDateTime(savedDeadline);
        }
    }
}

Task TaskDialog::getTask() const
{
    Task task;
    task.name = nameEdit->text().trimmed();
    task.subject = subjectEdit->text().trimmed();
    task.priority = priorityCombo->currentText();
    task.hasDeadline = (deadlineCombo->currentText() == "Yes");
    task.deadline = task.hasDeadline
        ? deadlineEdit->dateTime().toString(Qt::ISODate)
        : "";
    task.completed = false;
    return task;
}
