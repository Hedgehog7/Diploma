#include "conclusion_form.h"
#include "ui_conclusion_form.h"

#include "../../YOSDIInterpreter/IInterpreter.h"

ConclusionForm::ConclusionForm(QWidget* parent, IYOSDIInterpreter* interpreter)
    : YOSDIForm(parent, interpreter)
    , ui(new Ui::ConclusionForm)
{
    ui->setupUi(this);
}

ConclusionForm::~ConclusionForm()
{
    delete ui;
}

void ConclusionForm::on_inputButton_clicked()
{
    auto conclusion = ui->conclusion->text().toStdString();

    _interpreter->addConclusion(conclusion);
    _interpreter->addRule();

    _interpreter->event("input");
}

void ConclusionForm::on_visualiseButton_clicked()
{
    _interpreter->event("visualise");
}

void ConclusionForm::on_abortButton_clicked()
{
    _interpreter->event("abort");
}

void ConclusionForm::on_fuzzyButton_clicked()
{
    _interpreter->event("fuzzy");
}

void ConclusionForm::on_temporalButton_clicked()
{
   //_interpreter->event("temporal");
}
