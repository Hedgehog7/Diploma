#pragma once

#include <QDialog>
#include <QLineEdit>

#include "KnowledgeBase.h"
#include "KBInterval.h"
#include "FormulaWidget.h"

class KBCreateIntervalDialog : public QDialog
{
    Q_OBJECT

public:
    KBCreateIntervalDialog(KnowledgeBase *kb, QWidget *parent = 0);
    KBCreateIntervalDialog(KBInterval* interval, KnowledgeBase *kb, QWidget *parent = 0);
    KBInterval* m_interval;
    bool isValid();
private slots:
    void beforeClose();
    void onNameChanged(const QString name);
    void checkValid();
private:
    QLineEdit* nameEdit;
    FormulaWidget* openWidget;
    FormulaWidget* closeWidget;
    QPushButton* okBtn;

    void buildUi();
};
