#ifndef YOSDI_INTERPRETER_H
#define YOSDI_INTERPRETER_H

#include "IInterpreter.h"

#include <memory>
#include <map>

class ATaskExecutionWindow;
class QWidget;
class Procedure;
class ScenarioLoader;
class YOSDIFormsManager;
class PZCreator;

class YOSDIInterpreter : public IYOSDIInterpreter
{
public:
    YOSDIInterpreter(ATaskExecutionWindow* window);
    ~YOSDIInterpreter();

private:
    virtual void event(const std::string& eventName) override;
    virtual void addCondition(const std::string& attributeType, const std::string& attributeValue) override;
    virtual void addAnswer(const std::string& answer) override;
    virtual void removeCondition() override;
    virtual void addRule() override;
    virtual void addConclusion(const std::string& conclusion) override;
    virtual const std::string& getLastQuestion() override;
    virtual const std::string& getLastAnswer() override;

    void executeProcedure(const Procedure& procedure);
    void showForm(const std::string& formName);
    void addConnection(const std::string& event, const std::string& procedure);
    void setText(const std::string& identifier, const std::string& text);
    void setParam(const std::string& paramName, const std::string& paramValue);

    void taskTypeDefinitionFormDelegate(const std::string& taskType);

private:
    ATaskExecutionWindow* const _mainWindow;
    std::unique_ptr<QWidget> _currentWidget;
    std::unique_ptr<YOSDIFormsManager> _formsManager;
    std::unique_ptr<ScenarioLoader> _scenarioLoader;
    std::unique_ptr<PZCreator> _PZCreator;

    std::map<std::string, std::string> _connections;

    bool _isFinished = false;
};

#endif // YOSDI_INTERPRETER_H
