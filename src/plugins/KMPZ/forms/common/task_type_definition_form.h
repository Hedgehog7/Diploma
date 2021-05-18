#ifndef TASK_TYPE_DEFINITION_DIALOG_H
#define TASK_TYPE_DEFINITION_DIALOG_H

#include <functional>

#include <QDialog>

namespace Ui {
class TaskTypeDefinitionForm;
}

class TaskTypeDefinitionForm : public QWidget
{
    Q_OBJECT

public:
    TaskTypeDefinitionForm(QWidget *parent, std::function<void(const std::string)> taskTypeDelegate);
    ~TaskTypeDefinitionForm();
    std::function<void(const std::string&)> _taskTypeDelegate;

private slots:
    void on_nextButton_clicked();

private:
    Ui::TaskTypeDefinitionForm *ui;

};

#endif // TASK_TYPE_DEFINITION_DIALOG_H
