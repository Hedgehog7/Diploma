#ifndef AMRR_FORM_H
#define AMRR_FORM_H

#include "../forms/YOSDI_form.h"
#include "../YOSDIInterpreter/IInterpreter.h"
#include "instruction.h"
#include "diffconcepts.h"

#include<QTimer>


namespace Ui {
class AMRR_form;
}

class AMRR_form : public YOSDIForm
{
    Q_OBJECT

public:
    explicit AMRR_form(QWidget* parent, IYOSDIInterpreter* interpreter);
    ~AMRR_form() override;

private slots:
    void showEvent(QShowEvent *event) override;

    void on_close_amrr_clicked();

    void on_to_amrr_clicked();

private:
    Ui::AMRR_form *ui;
    QWidget* parent;
    bool need_to_call = false;
};

#endif // AMRR_FORM_H
