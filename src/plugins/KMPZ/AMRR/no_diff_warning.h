#ifndef NO_DIFF_WARNING_H
#define NO_DIFF_WARNING_H

#include "../forms/YOSDI_form.h"
#include "../YOSDIInterpreter/IInterpreter.h"
#include "diffconcepts.h"

namespace Ui {
class No_diff_warning;
}

class No_diff_warning : public YOSDIForm
{
    Q_OBJECT

public:
    explicit No_diff_warning(QWidget *parent, IYOSDIInterpreter* interpreter);
    ~No_diff_warning();

private slots:

    void on_returnButton_clicked();

    void on_endButton_clicked();

private:
    Ui::No_diff_warning *ui;
    QWidget* parent;
};

#endif // NO_DIFF_WARNING_H
