#include "task_type_alert_dialog.h"
#include "ui_task_type_alert_dialog.h"

#include <QStandardItem>

#include <iostream>

TaskTypeAlertDialog::TaskTypeAlertDialog(QWidget* parent, const QString& type, TaskTypeDefinitionForm* parent_wind)
    : QDialog(parent)
    , ui(new Ui::TaskTypeAlertDialog)
{
    ui->setupUi(this);
    this->parent_wind = parent_wind;
    ui->lineEdit->setText(type);
}

TaskTypeAlertDialog::~TaskTypeAlertDialog()
{
    delete ui;
}

void TaskTypeAlertDialog::on_pushButton_clicked()
{
    ui->lineEdit->setText("Все ок.");
    parent_wind->_taskTypeDelegate("diagnostics");
    this->close();
}
