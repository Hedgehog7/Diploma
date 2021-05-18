#ifndef I_YOSDI_INTERPRETER_H
#define I_YOSDI_INTERPRETER_H

#include <string>

class IYOSDIInterpreter
{
public:
    virtual void event(const std::string& eventName) = 0;

    virtual void addCondition(const std::string& attributeType, const std::string& attributeValue) = 0;
    virtual void addAnswer(const std::string& answer) = 0;
    virtual void removeCondition() = 0;
    virtual void addRule() = 0;
    virtual void addConclusion(const std::string& conclusion) = 0;

    virtual const std::string& getLastQuestion() = 0;
    virtual const std::string& getLastAnswer() = 0;
};

#endif // I_YOSDI_INTERPRETER_H
