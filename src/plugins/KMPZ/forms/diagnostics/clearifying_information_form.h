#ifndef CLARIFYING_INFORMATION_FORM_H
#define CLARIFYING_INFORMATION_FORM_H

#include "../YOSDI_form.h"

namespace Ui {
    class ClearifyingInformationForm;
}

class ClearifyingInformationForm : public YOSDIForm
{
    Q_OBJECT

public:
    ClearifyingInformationForm(QWidget* parent, IYOSDIInterpreter* interpreter);
    virtual ~ClearifyingInformationForm();

private slots:
    void on_inputButton_clicked();

    void on_visualiseButton_clicked();

private:
    Ui::ClearifyingInformationForm *ui;
};

#endif // CLARIFYING_INFORMATION_FORM_H
