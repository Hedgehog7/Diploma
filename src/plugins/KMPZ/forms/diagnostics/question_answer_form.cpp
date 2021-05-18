#include "question_answer_form.h"
#include "ui_question_answer_form.h"

#include "../../YOSDIInterpreter/IInterpreter.h"
#include "../common/temporal_form.h"

#include <iostream>

#include <QString>
#include <QTextCodec>

QuestionAnswerForm::QuestionAnswerForm(QWidget* parent, IYOSDIInterpreter* interpreter) :
    YOSDIForm(parent, interpreter),
    ui(new Ui::QuestionAnswerForm)
{
    ui->setupUi(this);
    ui->tempInfoButton->hide();
}

QuestionAnswerForm::~QuestionAnswerForm()
{
    delete ui;
}

void QuestionAnswerForm::on_nextButton_clicked()
{
	auto answer = ui->answerEdit->text().toStdString();

	if (_isNew)
	{
		//ui->questionEdit->setText("???????");
		auto question = ui->questionEdit->text().toStdString();

		//auto question = ru("???????").toStdString();

        _interpreter->addCondition(question, answer);// PROBLEM IS HERE (????????)
	}
	else
		_interpreter->addAnswer(answer);


	_interpreter->event("next");
}

void QuestionAnswerForm::on_tempInfoButton_clicked()
{
    //QDialog* temporalDialog = new TemporalForm(this, _interpreter, ui->questionEdit->text(), ui->answerEdit->text());
   // temporalDialog->show();
}

void QuestionAnswerForm::setText(const std::string& identifier, const std::string& text)
{
    if( identifier == "question_label" )
        ui->questionLabel->setPlainText(QString(text.c_str()));
    else if( identifier == "answer_label" )
        ui->answerLabel->setPlainText(QString(text.c_str()));
}

void QuestionAnswerForm::setParam(const std::string& paramName, const std::string& paramValue)
{
    if( paramName == "previous_question" )
    {
        if( paramValue == "true" )
        {
            const std::string& lastQuestion =_interpreter->getLastQuestion();

            ui->questionEdit->setText( QString(lastQuestion.c_str()) );
            ui->questionEdit->setReadOnly(true);
        }
    }
    else if( paramName == "show_previous_answers" )
    {
        // TODO
    }
    else if( paramName == "new_question" )
    {
        if( paramValue == "true" )
            _isNew = true;
        else
            _isNew = false;
    }
    else if( paramName == "debug" && paramValue == "true" )
    {
        //ui->tempInfoButton->show();
    }
}

//??? ??????????? ??????????? ???????? ?????? ? Qt
QString QuestionAnswerForm::ru(const char * text)
{
	return QTextCodec::codecForName("CP1251")->toUnicode(text);
}
