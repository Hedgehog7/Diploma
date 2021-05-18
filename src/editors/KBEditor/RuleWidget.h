#pragma once

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include "KBRule.h"
#include "KnowledgeBase.h"

class RuleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuleWidget(KBRule* rule, KnowledgeBase* kb, QWidget *parent = 0);
    KBRule* m_rule;

    bool isValid();
signals:
    void ruleChanged();
public slots:
    void setArg1(const QString name);
    void setArg2(const QString name);
    void setTemporalRel(const QString name);
    void setName(const QString name);
private:
    KnowledgeBase* m_kb;
    QLineEdit* nameEdit;
    QComboBox* arg1Combo;
    QComboBox* arg2Combo;
    QComboBox* temporalRelCombo;

    void buildUi();
    void fillArgsCombo();
    void fillAttributeCombo();
    void fillTemporalRelCombo();
    void fillObjectsCombo();
};
