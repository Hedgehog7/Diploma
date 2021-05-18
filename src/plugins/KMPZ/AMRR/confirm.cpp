#include "confirm.h"
#include "ui_confirm.h"


Confirm::Confirm(QWidget* parent, IYOSDIInterpreter* interpreter, std::vector<std::pair<std::string, std::pair<std::string, int>>>& mark_list) :
    YOSDIForm(parent, interpreter),
    ui(new Ui::Confirm),
    parent(parent)
{
    ui->setupUi(this);
    std::string result;
    for (auto premise_marks: mark_list)
    {
        std::string premise = premise_marks.first;
        std::pair<std::string, int> mark = premise_marks.second;
        result += premise + ": [" + mark.first + " (" + std::to_string(mark.second) + ")]\n";
    }
    ui->textEdit->setText(QString(result.c_str()));
}

Confirm::~Confirm()
{
    delete ui;
}

void Confirm::on_acceptButton_clicked()
{
    for (auto premise_marks: mark_list)
    {
        std::string premise = premise_marks.first;
        std::pair<std::string, int> mark = premise_marks.second;
        if (mark.second < 33)
        {
//            _interpreter->add_pos_diff_cond();
        }
    }
    _interpreter->event("to_next");
}

void Confirm::on_skipButton_clicked()
{
    _interpreter->event("to_next");
}
