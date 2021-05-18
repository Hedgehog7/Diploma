#include "KBCreateRuleDialog.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>

KBCreateRuleDialog::KBCreateRuleDialog(KBRule *rule, KnowledgeBase *kb, QWidget *parent)
    :QDialog(parent),
     m_kb(kb),
     m_rule(rule),
     okBtn(new QPushButton("Ok")),
     ruleWidget(new RuleWidget(rule, kb, this))
{
    qDebug() << "Enter KBCreateRuleDialog constructor";
    buildUi();
}

bool KBCreateRuleDialog::isValid()
{
    return ruleWidget->isValid();
}

void KBCreateRuleDialog::beforeClose()
{
    this->accept();
}

void KBCreateRuleDialog::checkValid()
{
    okBtn->setEnabled(isValid());
}

void KBCreateRuleDialog::buildUi()
{
    auto layout = new QVBoxLayout;
    layout->addWidget(ruleWidget);
    layout->addWidget(okBtn);
    this->setLayout(layout);
    okBtn->setEnabled(isValid());
    connect(okBtn, &QPushButton::clicked, this, &KBCreateRuleDialog::beforeClose);
    connect(ruleWidget, &RuleWidget::ruleChanged, this, &KBCreateRuleDialog::checkValid);
}
