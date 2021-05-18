#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <iostream>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_pushButton_clicked()
{
        std::cout << "Meh" << std::endl;
}
