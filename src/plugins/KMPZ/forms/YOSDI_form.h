#ifndef YOSDI_FORM_H
#define YOSDI_FORM_H

#include <QWidget>

class IYOSDIInterpreter;

class YOSDIForm : public QWidget
{
    Q_OBJECT

public:
    YOSDIForm(QWidget* parent, IYOSDIInterpreter* interpreter)
        : QWidget(parent)
        , _interpreter(interpreter)
    {}

    virtual void setText(const std::string&, const std::string&) {}
    virtual void setParam(const std::string&, const std::string&) {}

protected:
    IYOSDIInterpreter* _interpreter;
};

#endif // YOSDI_FORM_H
