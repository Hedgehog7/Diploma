#include "temporal_form.h"
#include "ui_temporal_form.h"

#include "../../YOSDIInterpreter/IInterpreter.h"
#include "../../dictionary/temporal_dictionary.h"

#include <QStandardItem>

#include <iostream>

TemporalForm::TemporalForm(QWidget* parent, IYOSDIInterpreter* interpreter, const QString& question, const QString& answer)
    : QDialog(parent)
    , _interpreter(interpreter)
    , ui(new Ui::TemporalForm)
    , _question(question)
    , _answer(answer)
{
    ui->setupUi(this);

    ui->questionEdit->setText(question);
    ui->answerEdit->setText(answer);

    parseData();
}

TemporalForm::~TemporalForm()
{
    delete ui;
}

void TemporalForm::on_inputButton_clicked()
{
    this->close();
}

void TemporalForm::on_cancelButton_clicked()
{
    this->close();
}

void TemporalForm::parseData()
{
    TemporalDictionary dict;
    dict.load("dictionaries/specialDictionaryOfTemporalLeksems.dict");

    std::vector<std::wstring> search;
    search.push_back(_question.toStdWString());
    search.push_back(_answer.toStdWString());
    auto res = dict.getResult(search);

    QStandardItemModel *model = new QStandardItemModel;//новая модель списка
    model->setColumnCount(3);

    for( auto r : res )
    {
        if( r.hasEvent() )
        {
            QList<QStandardItem*> column;
            column.append(new QStandardItem(QString("СОБЫТИЕ")));
            column.append(new QStandardItem(QString("Произошло:")));
            column.append(new QStandardItem(QString::fromStdWString(r.getEvent())));
            model->appendRow(column);
        }
        else
        {
            QString firstColumn("ИНТЕРВАЛ");

            if( r.hasStart() )
            {
                QList<QStandardItem*> column;
                column.append(new QStandardItem(firstColumn));
                column.append(new QStandardItem(QString("Начало:")));
                column.append(new QStandardItem(QString::fromStdWString(r.getStart())));
                model->appendRow(column);

                firstColumn = "";
            }
            if( r.hasEnd() )
            {
                QList<QStandardItem*> column;
                column.append(new QStandardItem(firstColumn));
                column.append(new QStandardItem(QString("Конец:")));
                column.append(new QStandardItem(QString::fromStdWString(r.getEnd())));
                model->appendRow(column);

                firstColumn = "";
            }
            if( r.hasDuration() )
            {
                QList<QStandardItem*> column;
                column.append(new QStandardItem(firstColumn));
                column.append(new QStandardItem(QString("Продолжительность:")));
                column.append(new QStandardItem(QString::fromStdWString(r.getDuration())));
                model->appendRow(column);

                firstColumn = "";
            }
        }
    }

    ui->tableView->setModel(model);
}
