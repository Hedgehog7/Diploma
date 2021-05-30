#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../forms/YOSDI_form.h"
#include "../YOSDIInterpreter/IInterpreter.h"
#include "diffconcepts.h"


namespace Ui {
class Instruction;
}

class Instruction : public YOSDIForm
{
    Q_OBJECT

public:
    explicit Instruction(QWidget *parent, IYOSDIInterpreter* interpreter);
    ~Instruction();

private slots:
    void on_okButton_clicked();

    void on_skipButton_clicked();

private:
    Ui::Instruction *ui;
    QWidget* parent;
};

#endif // INSTRUCTION_H
