#include "amrr_form.h"
#include "ui_amrr_form.h"


AMRR_form::AMRR_form(QWidget* parent, IYOSDIInterpreter* interpreter) :
    YOSDIForm(parent, interpreter),
    ui(new Ui::AMRR_form),
    parent(parent)
{
    ui->setupUi(this);
    auto current_branch = _interpreter->getLastBranch();
    if (current_branch.second.size() > 1){
        need_to_call = true;
    }
}

void AMRR_form::showEvent(QShowEvent *event){
    QWidget::showEvent(event);
    if (need_to_call){
        QTimer::singleShot(10, this, SLOT(on_to_amrr_clicked()));
    } else {
        QTimer::singleShot(10, this, SLOT(on_close_amrr_clicked()));
    }
}

AMRR_form::~AMRR_form()
{
    delete ui;
}

void AMRR_form::on_close_amrr_clicked()
{
    _interpreter->event("to_next");
}

void AMRR_form::on_to_amrr_clicked()
{
    _interpreter->switchForm(new Instruction(parent, _interpreter));
}
