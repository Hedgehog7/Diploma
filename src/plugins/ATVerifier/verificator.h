#ifndef verificator_H
#define verificator_H

#include <QDialog>
#include <qmainwindow.h>

namespace Ui {
class verificator;
}

class verificator : public QMainWindow
{
    Q_OBJECT

public:
    explicit verificator(QMainWindow *parent = nullptr);
    ~verificator();

private slots:
    void on_pushButton_clicked();

private:
    Ui::verificator *ui;
};


#endif // verificator_H

