#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <string>
#include "model_t.h"

namespace Ui {
  class DataMiningWindow;
}

class DataMiningWindow : public QWidget
{
  Q_OBJECT

  public:
    explicit DataMiningWindow(QWidget *parent = 0);
    ~DataMiningWindow();
    std::string outFName;

  private slots:
    void on_open_db_button_clicked();

    void on_parse_names_button_clicked();

    void on_add_tree_button_clicked();

    void on_save_trees_button_clicked();

  private:
    Ui::DataMiningWindow *ui;
    model_t model;
};

#endif
