#include "instruction.h"
#include "ui_instruction.h"


Instruction::Instruction(QWidget *parent, IYOSDIInterpreter* interpreter)
    : YOSDIForm(parent, interpreter),
      ui(new Ui::Instruction),
      parent(parent)
{
    ui->setupUi(this);
}

Instruction::~Instruction()
{
    delete ui;
}

void Instruction::on_okButton_clicked()
{
    _interpreter->switchForm(new DiffConcepts(parent, _interpreter));
}

void Instruction::on_skipButton_clicked()
{
    _interpreter->event("to_next");
}
