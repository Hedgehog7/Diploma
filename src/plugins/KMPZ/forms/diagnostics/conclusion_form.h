#ifndef CONCLUSION_FORM_H
#define CONCLUSION_FORM_H

#include "../YOSDI_form.h"

namespace Ui {
class ConclusionForm;
}

class ConclusionForm : public YOSDIForm
{
    Q_OBJECT

public:
    ConclusionForm(QWidget* parent, IYOSDIInterpreter* interpreter);
    virtual ~ConclusionForm();

private slots:
    void on_inputButton_clicked();
    void on_visualiseButton_clicked();
    void on_abortButton_clicked();
    void on_fuzzyButton_clicked();
    void on_temporalButton_clicked();

private:
    Ui::ConclusionForm *ui;
};

#endif // CONCLUSION_FORM_H
