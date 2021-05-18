#ifndef HAS_OTHER_ANSWER_FORM_H
#define HAS_OTHER_ANSWER_FORM_H

#include "../YOSDI_form.h"

namespace Ui {
class HasOtherAnswerForm;
}

class HasOtherAnswerForm : public YOSDIForm
{
    Q_OBJECT

public:
    HasOtherAnswerForm(QWidget* parent, IYOSDIInterpreter* interpreter);
    virtual ~HasOtherAnswerForm();

private slots:
    void on_yesButton_clicked();
    void on_noButton_clicked();
    void on_visualiseButton_clicked();

private:
    Ui::HasOtherAnswerForm* ui;
};

#endif // HAS_OTHER_ANSWER_FORM_H
