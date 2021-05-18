#ifndef REPERTORYGRID_H
#define REPERTORYGRID_H

#include "../forms/YOSDI_form.h"
#include "../YOSDIInterpreter/IInterpreter.h"
#include "confirm.h"


namespace Ui {
class RepertoryGrid;
}

class RepertoryGrid : public YOSDIForm
{
    Q_OBJECT

public:
    explicit RepertoryGrid(QWidget *parent, IYOSDIInterpreter* interpreter, std::vector<std::string>& used_conditions, std::vector<std::string>& used_conclusions);
    ~RepertoryGrid();

private slots:
    void on_OKButton_clicked();

private:
    Ui::RepertoryGrid *ui;
    QWidget* parent;
};

#endif // REPERTORYGRID_H
