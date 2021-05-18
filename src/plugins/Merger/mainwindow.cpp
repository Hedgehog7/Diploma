#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "knowledge_field_t.h"
#include <fstream>
#include "tester_t.h"

MergerWindow::MergerWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MergerWindow) {
  ui->setupUi(this);
  ui->fill_button->setVisible(true);
}

MergerWindow::~MergerWindow() {
  delete ui;
}

void MergerWindow::on_load_button_clicked()
{
  std::ifstream left_kf_in(ui->left_filename_value->text().toStdString());
  left_kf.load(left_kf_in);

  std::ifstream right_kf_in(ui->right_filename_value->text().toStdString());
  right_kf.load(right_kf_in);

  if (merger) {
    delete merger;
    merger = nullptr;
  }
  merger = new merger_t(left_kf, right_kf);
  log = "";
  ui->load_button->setDisabled(true);
  ui->merge_types_button->setEnabled(true);
}

void MergerWindow::on_fill_button_clicked()
{
  std::ofstream left_kf(ui->left_filename_value->text().toStdString());
  tester_t::fill_left_kf(left_kf);

  std::ofstream right_kf(ui->right_filename_value->text().toStdString());
  tester_t::fill_right_kf(right_kf);
}

void MergerWindow::on_merge_types_button_clicked()
{
  if (!merger->merge_types(log)) {
    std::ofstream log_out(ui->log_value->text().toStdString());
    log_out << log << std::endl;
    ui->load_button->setEnabled(true);
  } else {
    ui->merge_objects_button->setEnabled(true);
  }
  ui->merge_types_button->setDisabled(true);
}

void MergerWindow::on_save_button_clicked()
{
  std::ofstream log_out(ui->log_value->text().toStdString());
  log_out << log << std::endl;

  std::ofstream textout(ui->textout_value->text().toStdString());
  textout << merger->res_to_string() << std::endl;
  textout.close();

  std::ofstream xmlout(ui->xmlout_value->text().toStdString());
  xmlout << merger->res_to_xml() << std::endl;
  xmlout.close();

  std::ofstream innerout(ui->innerout_value->text().toStdString());
  innerout << merger->res_to_inner() << std::endl;
  innerout.close();

  ui->save_button->setDisabled(true);
}

void MergerWindow::on_merge_objects_button_clicked()
{
  merger->merge_objects(log);
  ui->merge_objects_button->setDisabled(true);
  ui->merge_events_button->setEnabled(true);
}

void MergerWindow::on_merge_events_button_clicked()
{
  merger->merge_events(log);
  ui->show_rtr_button->setEnabled(true);
  ui->show_dists_button->setEnabled(true);
  ui->merge_events_button->setDisabled(true);
  ui->merge_intervals_button->setEnabled(true);
}

void MergerWindow::on_merge_intervals_button_clicked()
{
  merger->merge_intervals(log);
  ui->merge_intervals_button->setDisabled(true);
  ui->merge_rules_button->setEnabled(true);
}

void MergerWindow::on_merge_rules_button_clicked()
{
  merger->merge_rules(log);
  ui->merge_rules_button->setDisabled(true);
  ui->save_button->setEnabled(true);
}

void MergerWindow::on_show_rtr_button_clicked()
{
  std::ofstream out(ui->rtr_value->text().toStdString());
  merger->show_rtr(out);
}

void MergerWindow::on_show_dists_button_clicked()
{
  std::ofstream out(ui->dists_value->text().toStdString());
  merger->show_dists(out);
}
