#ifndef verification_H
#define verification_H

#include <QDialog>

namespace Ui {
class verification;
}

class verification : public QDialog
{
    Q_OBJECT

public:
    explicit verification(QWidget *parent = nullptr);
    ~verification();

private slots:
    void on_pushButton_clicked();

private:
    Ui::verification *ui;
};

#endif // verification_H

