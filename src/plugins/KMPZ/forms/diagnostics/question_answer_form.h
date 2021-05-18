#ifndef QUESTION_ANSWER_FORM_H
#define QUESTION_ANSWER_FORM_H

#include "../YOSDI_form.h"

namespace Ui {
class QuestionAnswerForm;
}

class QuestionAnswerForm : public YOSDIForm
{
    Q_OBJECT

public:
    QuestionAnswerForm(QWidget* parent, IYOSDIInterpreter* interpreter);
    virtual ~QuestionAnswerForm();

    virtual void setText(const std::string& identifier, const std::string& text) override;
    virtual void setParam(const std::string& paramName, const std::string& paramValue) override;

	QString ru(const char * text);

private:
    Ui::QuestionAnswerForm *ui;

private slots:
    void on_nextButton_clicked();
    void on_tempInfoButton_clicked();

private:
    bool _isNew = true;
};

#endif // QUESTION_ANSWER_FORM_H
