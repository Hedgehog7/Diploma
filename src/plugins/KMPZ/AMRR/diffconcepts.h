#ifndef DIFFCONCEPTS_H
#define DIFFCONCEPTS_H

#include "../forms/YOSDI_form.h"
#include "../YOSDIInterpreter/IInterpreter.h"
#include "repertorygrid.h"



namespace Ui {
class DiffConcepts;
}

class DiffConcepts : public YOSDIForm
{
    Q_OBJECT

public:
    explicit DiffConcepts(QWidget *parent, IYOSDIInterpreter* interpreter);
    ~DiffConcepts();
    void set_triads();
    void show_next_triad();
    void switch_to_duo();

private slots:
    void on_continue1_clicked();

private:
    Ui::DiffConcepts *ui;
    QWidget* parent;
    std::vector<std::vector<std::string>> triads;
    branch_t current_branch;
    bool duo = false;
    std::vector<std::string> used_conclusions;
    std::vector<std::string> used_conditions;
};

#endif // DIFFCONCEPTS_H
