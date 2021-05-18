#include "can_make_conclusion_form.h"
#include "ui_can_make_conclusion_form.h"

#include "../../YOSDIInterpreter/IInterpreter.h"

CanMakeConclusionForm::CanMakeConclusionForm(QWidget* parent, IYOSDIInterpreter* interpreter) :
    YOSDIForm(parent, interpreter),
    ui(new Ui::CanMakeConclusionForm)
{
    ui->setupUi(this);

    QString lastQuestion(interpreter->getLastQuestion().c_str());
    ui->questionEdit->setText(lastQuestion);

    QString lastAnswer(interpreter->getLastAnswer().c_str());
    ui->answerEdit->setText(lastAnswer);
}

CanMakeConclusionForm::~CanMakeConclusionForm()
{
    delete ui;
}

void CanMakeConclusionForm::setText(const std::string& identifier, const std::string& text)
{
    if( identifier == "question_label" )
        ui->questionLabel->setText(QString(text.c_str()));
    else if( identifier == "answer_label" )
        ui->answerLabel->setText(QString(text.c_str()));
    else if( identifier == "conclusion_label" )
        ui->conclusionEdit->setPlainText(QString(text.c_str()));
}

void CanMakeConclusionForm::on_yesButton_clicked()
{
    _interpreter->event("yes");
}

void CanMakeConclusionForm::on_noButton_clicked()
{
    _interpreter->event("no");
}

void CanMakeConclusionForm::on_visualiseButton_clicked()
{
    _interpreter->event("visualise");
}

void CanMakeConclusionForm::setParam(const std::string& paramName, const std::string& paramValue)
{
    if( paramName == "remove_condition" && paramValue == "true" )
    {
        _removeCondition = true;
    }
}
