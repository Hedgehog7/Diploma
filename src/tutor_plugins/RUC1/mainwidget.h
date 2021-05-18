#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    std::vector<std::string> outFName;
    std::vector<std::string> inFName;

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H
