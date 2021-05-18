#include "YOSDI_forms_manager.h"

#include "../forms/diagnostics/question_answer_form.h"
#include "../forms/diagnostics/can_make_conclusion_form.h"
#include "../forms/diagnostics/has_other_answer.h"
#include "../forms/diagnostics/clearifying_information_form.h"
#include "../forms/diagnostics/conclusion_form.h"

#include <assert.h>

YOSDIFormsManager::YOSDIFormsManager(IYOSDIInterpreter* interpreter, QWidget* widgetsParent, const std::string& taskType)
    : _interpreter(interpreter)
    , _widgetsParent(widgetsParent)
{
    if( taskType == "diagnostics" )
        _type = TaskType::DIAGNOSTICS;

    assert(_type != TaskType::UNKNOWN);
}

YOSDIFormsManager::~YOSDIFormsManager()
{
}

YOSDIForm* YOSDIFormsManager::getForm(const std::string& formName)
{
    switch (_type)
    {
    case TaskType::DIAGNOSTICS:
        return getDiagnosticsForm(formName);
        break;
    default:
        assert(!"unhandled task type");
    }

    return getDiagnosticsForm(formName);
}

YOSDIForm* YOSDIFormsManager::getDiagnosticsForm(const std::string& formName)
{
    if( formName == "question_answer" )
        return new QuestionAnswerForm(_widgetsParent, _interpreter);

    if( formName == "can_make_conclusion" )
        return new CanMakeConclusionForm(_widgetsParent, _interpreter);

    if( formName == "clearifying_information" )
        return new ClearifyingInformationForm(_widgetsParent, _interpreter);

    if( formName == "conclusion" )
        return new ConclusionForm(_widgetsParent, _interpreter);

    if( formName == "has_other_answer" )
        return new HasOtherAnswerForm(_widgetsParent, _interpreter);

    assert(!"unknown form name");
    return nullptr;
}
