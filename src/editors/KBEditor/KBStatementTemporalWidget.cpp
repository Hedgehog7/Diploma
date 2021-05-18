#include "KBStatementTemporalWidget.h"
#include <QDebug>
#include <QHBoxLayout>
KBStatementTemporalWidget::KBStatementTemporalWidget(KBStatementTemporal *statement, KnowledgeBase *kb, QWidget *parent)
    :KBElementWidget(parent),
     m_kb(kb),
     m_statement(statement),
     arg1Combo(new QComboBox),
     arg2Combo(new QComboBox),
     temporalRelCombo(new QComboBox)
{
    qDebug() << "Enter KBStatementTemporalWidget constructor";
    buildUi();
}

void KBStatementTemporalWidget::buildUi()
{
    auto mainLayout = new QHBoxLayout;
    mainLayout->addWidget(arg1Combo);
    mainLayout->addWidget(temporalRelCombo);
    mainLayout->addWidget(arg2Combo);
    this->setLayout(mainLayout);

    fillArgsCombo();
    fillTemporalRelCombo();

    connect(arg1Combo, &QComboBox::currentTextChanged, this, &KBStatementTemporalWidget::setArg1);
    connect(arg2Combo, &QComboBox::currentTextChanged, this, &KBStatementTemporalWidget::setArg2);
    connect(temporalRelCombo, &QComboBox::currentTextChanged, this, &KBStatementTemporalWidget::setTemporalRel);
}

bool KBStatementTemporalWidget::isValid()
{
    return m_statement->m_arg1 != "" && m_statement->m_temporalRelation != "" && m_statement->m_arg2 != "";
}

void KBStatementTemporalWidget::fillArgsCombo() {
    arg1Combo->clear();
    arg2Combo->clear();
    QStringList sl;
    for (auto it = m_kb->intervals.begin(); it != m_kb->intervals.end(); it++) {
        sl << it->m_name;
    }
    for (auto it = m_kb->events.begin(); it != m_kb->events.end(); it++) {
        sl << it->m_name;
    }
    arg1Combo->addItems(sl);
    arg2Combo->addItems(sl);

    if (m_statement->m_arg1 != "") {
        arg1Combo->setCurrentIndex(arg1Combo->findText(m_statement->m_arg1));
    }
    if (m_statement->m_arg2 != "") {
        arg2Combo->setCurrentIndex(arg2Combo->findText(m_statement->m_arg2));
    }
    setArg1(arg1Combo->currentText());
    setArg2(arg2Combo->currentText());
}

void KBStatementTemporalWidget::fillTemporalRelCombo(){
    QStringList sl;
    sl << "b" << "bi" << "m" << "mi" << "o" << "oi" << "d" << "di" << "s" << "si" << "f" << "fi" << "E";
    temporalRelCombo->addItems(sl);
    if (m_statement->m_temporalRelation != "") {
        temporalRelCombo->setCurrentIndex(temporalRelCombo->findText(m_statement->m_temporalRelation));
    }
    setTemporalRel(temporalRelCombo->currentText());

}

void KBStatementTemporalWidget::setArg1(const QString name)
{
    m_statement->m_arg1 = name;
    emit changed();
}

void KBStatementTemporalWidget::setArg2(const QString name)
{
    m_statement->m_arg2 = name;
    emit changed();
}

void KBStatementTemporalWidget::setTemporalRel(const QString name)
{
    m_statement->m_temporalRelation = name;
    emit changed();
}
