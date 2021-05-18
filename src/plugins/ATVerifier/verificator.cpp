#include "verificator.h"
#include "ui_verificator.h"
#include<QFile>
#include<QFileDialog>
#include<string>

verificator::verificator(QMainWindow *parent) :
    QMainWindow(parent),
    bUi(new Ui::verificator)
{
    ui->setupUi(this);
    ui->textBrowser_6->setVisible(false);
}

verificator::~verificator()
{
    //delete ui;
}


void verificator::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString("Открыть файл"), QString("C://"), QString("txt (*.txt)"));
    QFile file(fileName);

            if (file.open(QIODevice::ReadOnly))
            {
                QString lastline = "";
                while(!file.atEnd()){
                    lastline = file.readLine();
                }
                file.close();
                int space = lastline.indexOf(' ');
                int first = 0;
                for (int i = 0; i < space; ++i){
                    first = first*10 + int(lastline.toStdString().c_str()[i]);
                }
                int second = 0;
                for (int i = space + 1; i < lastline.length(); ++i){
                    second = second*10 + int(lastline.toStdString().c_str()[i]);
                }
                ui->textBrowser_3->setText(QString(first));
                ui->textBrowser_4->setText(QString(second));
            }
            ui->textBrowser_6->setVisible(true);
}


