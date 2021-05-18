#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <iostream>
#include <QFile>

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
        ui->outFName->setText(QString::fromStdString((outFName[0])));
        QFile outFile(ui->outFName->text());
        if (outFile.exists()){}
        else {
            outFile.open(QIODevice::WriteOnly);
            outFile.write("Nice");
            outFile.close();
        }
}
