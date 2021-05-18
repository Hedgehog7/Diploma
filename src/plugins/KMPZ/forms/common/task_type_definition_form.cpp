#include "task_type_definition_form.h"
#include "ui_task_type_definition_form.h"
#include "../diagnostics/question_answer_form.h"
#include "../ui_question_answer_form.h"
#include "../../YOSDIInterpreter/IInterpreter.h"
#include "../../dictionary/temporal_dictionary.h"
#include "../../YOSDIInterpreter/YOSDIInterpreter.h"
#include "../../target_selector/target_selector.h"
#include "../../src/ATGUI/ATaskExecutionWindow.cpp"

#include "task_type_alert_dialog.h"

//#include <QtScript>

TaskTypeDefinitionForm::TaskTypeDefinitionForm(QWidget *parent, std::function<void(const std::string)> taskTypeDelegate)
    : QWidget(parent)
    , ui(new Ui::TaskTypeDefinitionForm)
    , _taskTypeDelegate(taskTypeDelegate)
{
    ui->setupUi(this);
}

TaskTypeDefinitionForm::~TaskTypeDefinitionForm()
{
    delete ui;
}

void TaskTypeDefinitionForm::on_nextButton_clicked()
{
    try {
        NAT::NMA::TTargetSelector targetSelector("dict.db");

        const QString text = ui->problemEdit->toPlainText();
        if (text.length() == 0)
            throw "no problem info";

        QString type = targetSelector.Select(text);
      //  return new QuestionAnswerForm (nullptr,);
        /*APlanTask* task = new APlanTask();
        ATaskExecutionWindow* task_wnd = new ATaskExecutionWindow(task, 0);
        YOSDIInterpreter* itr = new YOSDIInterpreter(task_wnd);
        QuestionAnswerForm* w = new QuestionAnswerForm(nullptr, itr);
        w->show();*/


       TaskTypeAlertDialog* alert = new TaskTypeAlertDialog(nullptr, type, this);
       alert->show();
//        TaskTypeAlertDialog* alert  = new TaskTypeAlertDialog(nullptr, "diagnostics");
//        alert->show();

//       _taskTypeDelegate("diagnostics");
    } catch (...) {
        TaskTypeAlertDialog* alert  = new TaskTypeAlertDialog(nullptr, "diagnostics", this);
        alert->show();
//        _taskTypeDelegate("diagnostics");
    }

    //QScriptEngine engine;
    //auto res = engine.evaluate("var a = \"с 10 часов\"; var re = /[0-9]+/gi; return re.exec(a);").toString();
    //qDebug() << "the magic number is:" << res;
}
