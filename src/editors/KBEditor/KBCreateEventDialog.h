#pragma once
#include <QDialog>
#include <QLineEdit>
#include "KnowledgeBase.h"
#include "FormulaWidget.h"
#include "KBEvent.h"

class KBCreateEventDialog : public QDialog
{
    Q_OBJECT
public:
    KBCreateEventDialog(KnowledgeBase *kb, QWidget *parent = 0);
    KBCreateEventDialog(KBEvent* interval, KnowledgeBase *kb, QWidget *parent = 0);
    KBEvent* m_event;
    bool isValid();

private slots:
    void beforeClose();
    void onNameChanged(const QString name);
    void checkValid();

private:
    QLineEdit* nameEdit;
    FormulaWidget* formulaWidget;
    QPushButton* okBtn;

    void buildUi();
};
