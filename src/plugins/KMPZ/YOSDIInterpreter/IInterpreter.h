#ifndef I_YOSDI_INTERPRETER_H
#define I_YOSDI_INTERPRETER_H

#include <string>
#include <QList>
#include <QPair>
#include <QString>

class YOSDIForm;

typedef std::pair<std::vector<std::string>, std::vector<std::string>> branch_t;

class IYOSDIInterpreter
{
public:
    virtual void event(const std::string& eventName) = 0;

    virtual void addCondition(const std::string& attributeType, const std::string& attributeValue) = 0;
//    virtual void addDifferentialCondition(const int& rule_number, const std::string& attributeValue) = 0;
    virtual void addAnswer(const std::string& answer) = 0;
    virtual void removeCondition() = 0;
    virtual void removeBranchConclusions() = 0;
    virtual void addRule() = 0;
    virtual void addConclusion(const std::string& conclusion) = 0;
    virtual void switchForm(YOSDIForm* form) = 0;
    virtual const branch_t& getLastBranch() const = 0;

    virtual const std::string& getLastQuestion() const = 0;
    virtual const std::string& getLastAnswer() const= 0;
};

#endif // I_YOSDI_INTERPRETER_H
