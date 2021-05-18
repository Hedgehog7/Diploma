#pragma once

#include <QComboBox>

#include "KBStatementTemporal.h"
#include "KBElementWidget.h"
#include "KnowledgeBase.h"

class KBStatementTemporalWidget : public KBElementWidget
{
    Q_OBJECT
public:
    explicit KBStatementTemporalWidget(KBStatementTemporal* statement, KnowledgeBase *kb, QWidget *parent = 0);
    KBStatementTemporal* m_statement;
    bool isValid();
public slots:
    void setArg1(const QString name);
    void setArg2(const QString name);
    void setTemporalRel(const QString name);
private:
    KnowledgeBase* m_kb;
    QComboBox* arg1Combo;
    QComboBox* arg2Combo;
    QComboBox* temporalRelCombo;

    void fillArgsCombo();
    void fillTemporalRelCombo();
    void buildUi();
};
