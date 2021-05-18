#include "no_diff_warning.h"
#include "ui_no_diff_warning.h"

No_diff_warning::No_diff_warning(QWidget *parent, IYOSDIInterpreter* interpreter) :
    YOSDIForm(parent, interpreter),
    ui(new Ui::No_diff_warning),
    parent(parent)
{
    ui->setupUi(this);
}

No_diff_warning::~No_diff_warning()
{
    delete ui;
}

void No_diff_warning::on_returnButton_clicked()
{
    _interpreter->switchForm(new DiffConcepts(parent, _interpreter));
}

void No_diff_warning::on_endButton_clicked()
{
    _interpreter->event("to_next");
}
