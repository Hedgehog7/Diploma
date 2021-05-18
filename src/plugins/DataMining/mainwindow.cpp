#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>

DataMiningWindow::DataMiningWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DataMiningWindow) {
  ui->setupUi(this);
}

DataMiningWindow::~DataMiningWindow() {
  delete ui;
}

void DataMiningWindow::on_open_db_button_clicked() {
  std::string database_name = ui->database_name_value->text().toStdString();
  if (database_name == "") {
    ui->log_value->append("Empty database name");
  } else {
    ui->log_value->append("Open database with name ");
    ui->log_value->append(database_name.c_str());
    ui->log_value->append(model.init(database_name));
    ui->open_db_button->setEnabled(false);
    ui->parse_names_button->setEnabled(true);
  }
}

void DataMiningWindow::on_parse_names_button_clicked() {
  QString table_name, id_column_name, time_column_name, properties, class_table_name, class_column_name;
  table_name = ui->table_name_value->text();
  id_column_name = ui->id_column_name_value->text();
  time_column_name = ui->time_column_name_value->text();
  properties = ui->properties_text->toPlainText();
  class_table_name = ui->class_table_name_value->text();
  class_column_name = ui->class_column_name_value->text();

  if (table_name.size() == 0) {
    ui->log_value->append("Table name Failed");
    return;
  }
  if (id_column_name.size() == 0) {
    ui->log_value->append("Id column name Failed");
    return;
  }
  if (time_column_name.size() == 0) {
    ui->log_value->append("Time column name Failed");
    return;
  }
  if (properties.size() == 0) {
    ui->log_value->append("Properties Failed");
    return;
  }
  if (class_table_name.size() == 0) {
    ui->log_value->append("Class table name Failed");
    return;
  }
  if (class_column_name.size() == 0) {
    ui->log_value->append("Class column name Failed");
    return;
  }
  ui->log_value->append("Success");
  ui->log_value->append(model.set_names(table_name, id_column_name, time_column_name, properties, class_table_name, class_column_name));
  ui->parse_names_button->setEnabled(false);
  ui->add_tree_button->setEnabled(true);
  ui->save_trees_button->setEnabled(true);
}

void DataMiningWindow::on_add_tree_button_clicked() {
  string buf = ui->add_tree_value->text().toStdString();
  for (char c : buf) {
    if (!isdigit(c)) {
      ui->log_value->append("Incorrect Number");
      return;
    }
  }
  long long cnt = stoll(buf);
  while (cnt--)
    model.add_tree();
  ui->log_value->append("BUILDED");
}

void DataMiningWindow::on_save_trees_button_clicked() {
  std::ofstream sout(ui->serialize_name_value->text().toStdString());
  sout << model.serialize();
  sout.close();
  ui->log_value->append("JSON SERIALIZATION DONE");
  std::ofstream textout(ui->rules_name_value->text().toStdString());
  std::ofstream innerout(ui->inner_name_value->text().toStdString());
  std::ofstream xmlout(outFName);
  model.rulealize(textout, innerout, xmlout);
  textout.close();
  innerout.close();
  xmlout.close();
  ui->log_value->append("TEXT SERIALIZATION DONE");
}
