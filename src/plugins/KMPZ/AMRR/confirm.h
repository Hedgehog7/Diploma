#ifndef CONFIRM_H
#define CONFIRM_H

#include "../forms/YOSDI_form.h"
#include "../YOSDIInterpreter/IInterpreter.h"

namespace Ui {
class Confirm;
}

class Confirm : public YOSDIForm
{
    Q_OBJECT

public:
    explicit Confirm(QWidget* parent, IYOSDIInterpreter* interpreter, std::vector<std::pair<std::string, std::pair<std::string, int>>>& mark_list);
    ~Confirm() override;

private slots:
    void on_acceptButton_clicked();

    void on_skipButton_clicked();

private:
    Ui::Confirm *ui;
    QWidget* parent;
    std::vector<std::pair<std::string, std::pair<std::string, int>>> mark_list;
};

#endif // CONFIRM_H
