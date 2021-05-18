#include "clearifying_information_form.h"
#include "ui_clearifying_information_form.h"

#include "../../YOSDIInterpreter/IInterpreter.h"

ClearifyingInformationForm::ClearifyingInformationForm(QWidget* parent, IYOSDIInterpreter* interpreter) :
    YOSDIForm(parent, interpreter),
    ui(new Ui::ClearifyingInformationForm)
{
    ui->setupUi(this);
}

ClearifyingInformationForm::~ClearifyingInformationForm()
{
    delete ui;
}

void ClearifyingInformationForm::on_inputButton_clicked()
{
    _interpreter->event("input");
}

void ClearifyingInformationForm::on_visualiseButton_clicked()
{
    _interpreter->event("visualise");
}
