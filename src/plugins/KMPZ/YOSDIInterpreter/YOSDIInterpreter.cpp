#include "YOSDIInterpreter.h"


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
        QFile file;
        QTextStream stream;

        file.setFileName("html_prepare.html");
        if (!file.open(QIODevice::WriteOnly))
        {

        }
        else
        {
            stream.setDevice(&file);
            stream << this->_PZCreator->protocol->toHTML();
            file.close();
        }

        _currentWidget.reset(nullptr);

        DebugPZ* pz = new DebugPZ(nullptr, QString::fromStdString(_PZCreator->getBZ()));
        pz->show();

        //çàïèñü ÏÇ â xml ôàéë
        _PZCreator->writeToFile();

        //äîáàâèòü âåðèôèêàòîð


        return;
    }
    auto connection = _connections.find(eventName);
    if( connection != _connections.end() ){
        executeProcedure(_scenarioLoader->getProcedure(connection->second));
    }
}

void YOSDIInterpreter::addCondition(const std::string& attributeType, const std::string& attributeValue)
{
    _PZCreator->addQuestionAndAnswer(attributeType, attributeValue);
}

//void addDifferentialCondition(const int& rule_number, const std::string& attributeValue)
//{

//}

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

void YOSDIInterpreter::removeBranchConclusions(){
    _PZCreator->removeBranchConclusions();
}

void YOSDIInterpreter::addConclusion(const std::string& conclusion)
{
    _PZCreator->addConclusion(conclusion);
}

const branch_t& YOSDIInterpreter::getLastBranch() const
{
    return _PZCreator->getLastBranch();
}

const std::string& YOSDIInterpreter::getLastQuestion() const
{
    return _PZCreator->getLastQuestion().attributeType;
}

const std::string& YOSDIInterpreter::getLastAnswer() const
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

void YOSDIInterpreter::switchForm(YOSDIForm *form){
    _currentWidget.reset(form);
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
