#ifndef TEMPORAL_FORM_H
#define TEMPORAL_FORM_H

#include <QDialog>
class IYOSDIInterpreter;

namespace Ui {
class TemporalForm;
}

class TemporalForm : public QDialog
{
    Q_OBJECT

public:
    TemporalForm(QWidget* parent, IYOSDIInterpreter* interpreter, const QString& question, const QString& answer);
    virtual ~TemporalForm();

private slots:
    void on_inputButton_clicked();

    void on_cancelButton_clicked();

private:
    void parseData();

private:
    IYOSDIInterpreter* const _interpreter;

    Ui::TemporalForm* ui;

    const QString _question;
    const QString _answer;
};

#endif // TEMPORAL_FORM_H
