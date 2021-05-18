#ifndef DEBUG_PZ_H
#define DEBUG_PZ_H

#include <QDialog>

namespace Ui {
class DebugPZ;
}

class DebugPZ : public QDialog
{
    Q_OBJECT

public:
    DebugPZ(QWidget* parent, const QString& bz);
    virtual ~DebugPZ();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DebugPZ* ui;
};

#endif // DEBUG_PZ_H
