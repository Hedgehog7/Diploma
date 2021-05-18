#include "YOSDIInterpreter.h"

#include "../forms/common/task_type_definition_form.h"
#include "../forms/common/debug_pz.h"
#include "../forms/diagnostics/question_answer_form.h"
#include "YOSDI_forms_manager.h"
#include "scenario_loader.h"
#include "../pz_creator/pz_creator.h"
#include <ATGUI/ATaskExecutionWindow.h>

#include <assert.h>

YOSDIInterpreter::YOSDIInterpreter(ATaskExecutionWindow* window)
    : _mainWindow(window)
    , _currentWidget( new TaskTypeDefinitionForm(window, std::bind(&YOSDIInterpreter::taskTypeDefinitionFormDelegate, this, std::placeholders::_1) ) )
    , _PZCreator(new PZCreator)
{
    _mainWindow->setCentralWidget(_currentWidget.get());

}

YOSDIInterpreter::~YOSDIInterpreter()
{}

void YOSDIInterpreter::event(const std::string& eventName)
{
    if( _isFinished )
    {
        _currentWidget.reset(nullptr);

        DebugPZ* pz = new DebugPZ(nullptr, QString::fromStdString(_PZCreator->getBZ()));
        pz->show();

		//запись ПЗ в xml файл
        _PZCreator->writeToFile();

        //добавить верификатор


        return;
    }
    auto connection = _connections.find(eventName);
    if( connection != _connections.end() )
        executeProcedure(_scenarioLoader->getProcedure(connection->second));
}

void YOSDIInterpreter::addCondition(const std::string& attributeType, const std::string& attributeValue)
{
    _PZCreator->addQuestionAndAnswer(attributeType, attributeValue);
}

void YOSDIInterpreter::addAnswer(const std::string& answer)
{
    _PZCreator->addAnswer(answer);
}

void YOSDIInterpreter::addRule()
{
    _PZCreator->addRule();
}

void YOSDIInterpreter::removeCondition()
{
    _PZCreator->removeQuestion();
    if( !_PZCreator->hasQuestions() )
        _isFinished = true;
}

void YOSDIInterpreter::addConclusion(const std::string& conclusion)
{
    _PZCreator->addConclusion(conclusion);
}

const std::string& YOSDIInterpreter::getLastQuestion()
{
    return _PZCreator->getLastQuestion().attributeType;
}

const std::string& YOSDIInterpreter::getLastAnswer()
{
    return _PZCreator->getLastQuestion().attributeValues.back();
}

void YOSDIInterpreter::taskTypeDefinitionFormDelegate(const std::string& taskType)
{
    assert(taskType == "diagnostics" || taskType == "planning");

    if( taskType == "planning" )
        return;

    _formsManager.reset(new YOSDIFormsManager(this, _mainWindow, taskType));

    _scenarioLoader.reset(new ScenarioLoader(taskType));
    executeProcedure( _scenarioLoader->getProcedure("Main") );
}

void YOSDIInterpreter::executeProcedure(const Procedure& procedure)
{
    for(const auto& command : procedure.commands)
    {
        switch(command.type)
        {
            case CommandType::CALL:
                executeProcedure(_scenarioLoader->getProcedure(command.params[0]));
                break;
            case CommandType::FORM:
                showForm(command.params[0]);
                break;
            case CommandType::CONNECT:
                addConnection(command.params[0], command.params[1]);
                break;
            case CommandType::TEXT:
                setText(command.params[0], command.params[1]);
                break;
            case CommandType::PARAM:
                setParam(command.params[0], command.params[1]);
                break;
        }
    }
}

void YOSDIInterpreter::showForm(const std::string& formName)
{
    _connections.clear();
    _currentWidget.reset( _formsManager->getForm(formName) );
    _mainWindow->setCentralWidget(_currentWidget.get());
}

void YOSDIInterpreter::addConnection(const std::string& event, const std::string& procedure)
{
    _connections[event] = procedure;
}

void YOSDIInterpreter::setText(const std::string& identifier, const std::string& text)
{
    YOSDIForm* form = dynamic_cast<YOSDIForm*>( _currentWidget.get() );
    if( form )
        form->setText(identifier, text);
}

void YOSDIInterpreter::setParam(const std::string& paramName, const std::string& paramValue)
{
    YOSDIForm* form = dynamic_cast<YOSDIForm*>( _currentWidget.get() );
    if( form )
        form->setParam(paramName, paramValue);
}
