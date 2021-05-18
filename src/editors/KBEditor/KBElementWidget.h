#pragma once
#include <QWidget>

class KBElementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KBElementWidget(QWidget *parent = 0);
    virtual bool isValid();
signals:
    void changed();
private:
    virtual void buildUi();
};
