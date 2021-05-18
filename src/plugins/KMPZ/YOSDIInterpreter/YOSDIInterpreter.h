#ifndef YOSDI_INTERPRETER_H
#define YOSDI_INTERPRETER_H

#include "IInterpreter.h"
#include "../pz_creator/pz_creator.h"
#include "../forms/common/task_type_definition_form.h"
#include "../forms/common/debug_pz.h"
#include "../forms/diagnostics/question_answer_form.h"
#include "YOSDI_forms_manager.h"
#include "scenario_loader.h"
#include <ATGUI/ATaskExecutionWindow.h>
#include <QWidget>

#include <assert.h>

#include <memory>
#include <map>

class YOSDIInterpreter : public IYOSDIInterpreter
{
public:
    YOSDIInterpreter(ATaskExecutionWindow* window);
    ~YOSDIInterpreter();

private:
    void event(const std::string& eventName) override;
    void addCondition(const std::string& attributeType, const std::string& attributeValue) override;
//    void addDifferentialCondition(const int& rule_number, const std::string& attributeValue) override;
    void addAnswer(const std::string& answer) override;
    void removeCondition() override;
    void removeBranchConclusions() override;
    void addRule() override;
    void addConclusion(const std::string& conclusion) override;
    void switchForm(YOSDIForm* form) override;
    const branch_t& getLastBranch() const override;
    const std::string& getLastQuestion() const override;
    const std::string& getLastAnswer() const override;

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
