#ifndef YOSDI_FORMS_MANANGER_H
#define YOSDI_FORMS_MANANGER_H

#include <string>

class YOSDIForm;
class IYOSDIInterpreter;
class QWidget;

class YOSDIFormsManager
{
public:
    YOSDIFormsManager(IYOSDIInterpreter* interpreter, QWidget* widgetsParent, const std::string& taskType);
    ~YOSDIFormsManager();

    YOSDIForm* getForm(const std::string& formName);

private:
    YOSDIForm* getDiagnosticsForm(const std::string& formName);

private:
    enum class TaskType
    {
        UNKNOWN,
        DIAGNOSTICS
    };

private:
    IYOSDIInterpreter* const _interpreter;
    QWidget* const _widgetsParent;
    TaskType _type = TaskType::UNKNOWN;
};

#endif // YOSDI_INTERPRETER_H
