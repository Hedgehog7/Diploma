#pragma once
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include "KBElementWidget.h"
#include "KnowledgeBase.h"

class KBStatementWidget : public KBElementWidget
{
    Q_OBJECT
public:
    explicit KBStatementWidget(KBStatement* statement, KnowledgeBase *kb, QWidget *parent = 0);
    KBStatement* m_statement;
    bool isValid();
private slots:
    void setObject(const QString name);
    void setAttribute(const QString name);
    void setValue(const QString value);
    void setBelief(int value);
    void setProbabilty(int value);
    void setAccuracy(int value);

private:
    KnowledgeBase* m_kb;

    QComboBox* objectCombo;
    QComboBox* attributeCombo;
    QLineEdit* valueEdit;
    QSpinBox* beliefSpin;
    QSpinBox* probabiltySpin;
    QSpinBox* accuracySpin;

    void fillObjectsCombo();
    void fillAttributeCombo();
    void buildUi();
};
