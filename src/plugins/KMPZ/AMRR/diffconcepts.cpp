#include "diffconcepts.h"
#include "ui_diffconcepts.h"

#include "no_diff_warning.h"

DiffConcepts::DiffConcepts(QWidget *parent, IYOSDIInterpreter* interpreter)
    : YOSDIForm(parent, interpreter),
      ui(new Ui::DiffConcepts),
      parent(parent)
{
    ui->setupUi(this);
    current_branch = _interpreter->getLastBranch();
    for (auto condition: current_branch.first)
    {
        ui->pos_pole->addItem(QString(condition.c_str()));
    }
    if (current_branch.second.size() > 2)
    {
        set_triads();
        show_next_triad();
    } else {
        switch_to_duo();
        duo = true;
    }
}

void DiffConcepts::set_triads()
{
    for (size_t i = 0; i < current_branch.second.size(); ++i)
    {
        for (size_t j = 0; j < current_branch.second.size(); ++j)
        {
            if (i == j) continue;
            for (size_t k = j + 1; k < current_branch.second.size(); ++k)
            {
                if (i == k) continue;
                std::vector<std::string> triad;
                triad.push_back(current_branch.second[i]);
                triad.push_back(current_branch.second[j]);
                triad.push_back(current_branch.second[k]);
                triads.push_back(triad);
            }
        }
    }
}

void DiffConcepts::switch_to_duo(){
    ui->conclusionEdit->setPlainText("Выберите из списка или введите свой критерий, по которому первое заключение отличается от другого, или нажмите \"Далее\",  если такого критерия нет.");
    ui->concept1->setText(QString(current_branch.second[0].c_str()));
    ui->concept2->setText(QString(current_branch.second[1].c_str()));
    ui->concept3->setText("");
    ui->label_3->setVisible(false);
}

void DiffConcepts::show_next_triad()
{
    if (triads.size() > 0){
        ui->concept1->setText(QString(triads[0][0].c_str()));
        ui->concept2->setText(QString(triads[0][1].c_str()));
        ui->concept3->setText(QString(triads[0][2].c_str()));
        triads.erase(triads.begin());
    }
}

DiffConcepts::~DiffConcepts()
{
    delete ui;
}

void DiffConcepts::on_continue1_clicked()
{
    if (ui->pos_pole->currentText() != "")
    {
        if (ui->pos_pole->currentText().indexOf(":") == -1)
        {
            ui->pos_pole->addItem(ui->pos_pole->currentText());
        }
        if (std::find(used_conditions.begin(), used_conditions.end(), ui->pos_pole->currentText().toStdString()) == used_conditions.end())
        {
            used_conditions.push_back(ui->pos_pole->currentText().toStdString());
        }
        if (std::find(used_conclusions.begin(), used_conclusions.end(), ui->concept1->text().toStdString()) == used_conclusions.end())
        {
            used_conclusions.push_back(ui->concept1->text().toStdString());
        }
        if (std::find(used_conclusions.begin(), used_conclusions.end(), ui->concept2->text().toStdString()) == used_conclusions.end())
        {
            used_conclusions.push_back(ui->concept2->text().toStdString());
        }
        if (!duo && std::find(used_conclusions.begin(), used_conclusions.end(), ui->concept3->text().toStdString()) == used_conclusions.end())
        {
            used_conclusions.push_back(ui->concept3->text().toStdString());
        }
        while (triads.size() > 0 && ui->concept1->text().toStdString() == triads[0][0]) {
            triads.erase(triads.begin());
        }
        ui->pos_pole->setCurrentIndex(0);
    }
    if (triads.size() > 0){
        show_next_triad();
    } else {
        if (used_conditions.size() > 0)
        {
            _interpreter->switchForm(new RepertoryGrid(parent, _interpreter, used_conditions, used_conclusions ));
        } else {
            _interpreter->switchForm(new No_diff_warning(parent, _interpreter));
        }
    }
}
