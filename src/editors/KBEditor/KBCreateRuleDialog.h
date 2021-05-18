#pragma once
#include <QDialog>
#include <QLineEdit>
#include "KnowledgeBase.h"
#include "RuleWidget.h"
#include "KBEvent.h"
#include "KBRule.h"

class KBCreateRuleDialog : public QDialog
{
    Q_OBJECT
public:
//    KBCreateRuleDialog(KnowledgeBase *kb, QWidget *parent = 0);
    KBCreateRuleDialog(KBRule* rule, KnowledgeBase *kb, QWidget *parent = 0);
    KBRule* m_rule;
    bool isValid();
private slots:
    void beforeClose();
    void checkValid();
private:
    RuleWidget* ruleWidget;
    KnowledgeBase* m_kb;
    QPushButton* okBtn;
    void buildUi();
};
