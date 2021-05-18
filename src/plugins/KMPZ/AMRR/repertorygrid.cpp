#include "repertorygrid.h"
#include "ui_repertorygrid.h"


RepertoryGrid::RepertoryGrid(QWidget *parent, IYOSDIInterpreter* interpreter, std::vector<std::string>& used_conditions, std::vector<std::string>& used_conclusions)
    : YOSDIForm(parent, interpreter),
      ui(new Ui::RepertoryGrid),
      parent(parent)
{
    ui->setupUi(this);
    size_t count_columns = used_conclusions.size();
    size_t count_rows = used_conditions.size();

    this->ui->left_construct->setRowCount(count_rows);
    this->ui->repertory_grid->setRowCount(count_rows);
    this->ui->repertory_grid->setColumnCount(count_columns);
    this->ui->repertory_grid->setVerticalScrollBar(this->ui->left_construct->verticalScrollBar());
    QStringList hl;
    for (size_t i = 0; i < count_columns; ++i)
        hl.append(QString(used_conclusions[i].c_str()));
    for (size_t i = 0; i < count_rows; ++i)
    {
        ui->left_construct->setItem(i, 0, new QTableWidgetItem(QString(used_conditions[i].c_str()), 0));
    }
    this->ui->repertory_grid->setHorizontalHeaderLabels(hl);
    this->ui->repertory_grid->horizontalHeader()->setVisible(true);
    for (size_t i = 0; i < count_rows; ++i)
        for (size_t j = 0; j < count_columns; ++j)
            this->ui->repertory_grid->setItem(i, j, new QTableWidgetItem(QString(""), 0));
}

RepertoryGrid::~RepertoryGrid()
{
    delete ui;
}

void RepertoryGrid::on_OKButton_clicked()
{
    std::string* result = new std::string("");
    std::vector<std::pair<std::string, std::pair<std::string, int>>> mark_list;
    for (int i = 0; i < ui->repertory_grid->columnCount(); ++i)
    {
        std::string premise = ui->repertory_grid->horizontalHeaderItem(i)->text().toStdString();
        std::pair<std::string, int> mark;
        mark.first = "";
        mark.second = 0;
        for (int j = 0; j < ui->repertory_grid->rowCount(); ++j)
        {
            QString text_value = ui->repertory_grid->item(j, i)->text();
            int item_value = ui->repertory_grid->item(j, i)->text().toInt();
            if (item_value == 0 && text_value != "0")
            {
                item_value = -1;
            }
            if (item_value >= 0 && item_value <= 100 && mark.second < item_value)
            {
                mark.first = ui->left_construct->item(j, 0)->text().toStdString();
                mark.second = item_value;
                mark_list.push_back(std::pair<std::string, std::pair<std::string, int>>(premise, mark));
            }
        }
    }
    _interpreter->switchForm(new Confirm(parent, _interpreter, mark_list));
}
