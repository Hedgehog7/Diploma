#include "has_other_answer.h"
#include "ui_has_other_answer.h"

#include "../../YOSDIInterpreter/IInterpreter.h"

HasOtherAnswerForm::HasOtherAnswerForm(QWidget* parent, IYOSDIInterpreter* interpreter) :
    YOSDIForm(parent, interpreter),
    ui(new Ui::HasOtherAnswerForm)
{
    ui->setupUi(this);

    ui->questionEdit->setText(QString(_interpreter->getLastQuestion().c_str()));
}

HasOtherAnswerForm::~HasOtherAnswerForm()
{
    delete ui;
}

void HasOtherAnswerForm::on_yesButton_clicked()
{
    _interpreter->event("yes");
}

void HasOtherAnswerForm::on_noButton_clicked()
{
    _interpreter->removeCondition();

    _interpreter->event("no");
}

void HasOtherAnswerForm::on_visualiseButton_clicked()
{
    _interpreter->event("visualise");
}
