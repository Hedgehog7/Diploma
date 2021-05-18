#ifndef CAN_MAKE_CONCLUSION_FORM_H
#define CAN_MAKE_CONCLUSION_FORM_H

#include "../YOSDI_form.h"

namespace Ui {
class CanMakeConclusionForm;
}

class CanMakeConclusionForm : public YOSDIForm
{
    Q_OBJECT

public:
    CanMakeConclusionForm(QWidget* parent, IYOSDIInterpreter* interpreter);
    virtual ~CanMakeConclusionForm();

    virtual void setText(const std::string& identifier, const std::string& text) override;
    virtual void setParam(const std::string& paramName, const std::string& paramValue) override;

private slots:
    void on_yesButton_clicked();
    void on_noButton_clicked();
    void on_visualiseButton_clicked();

private:
    Ui::CanMakeConclusionForm* ui;
    bool _removeCondition = false;
};

#endif // CAN_MAKE_CONCLUSION_FORM_H
