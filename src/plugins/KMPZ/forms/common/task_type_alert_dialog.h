#ifndef TASK_TYPE_ALERT_DIALOG_H
#define TASK_TYPE_ALERT_DIALOG_H

#include <QDialog>
#include"task_type_definition_form.h"

namespace Ui {
class TaskTypeAlertDialog;
}

class TaskTypeAlertDialog : public QDialog
{
    Q_OBJECT

public:
    TaskTypeAlertDialog(QWidget* parent, const QString& type, TaskTypeDefinitionForm* parent_wind);
    virtual ~TaskTypeAlertDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TaskTypeAlertDialog* ui;
    TaskTypeDefinitionForm* parent_wind;
};

#endif // TASK_TYPE_ALERT_DIALOG_H
