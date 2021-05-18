#pragma once

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QList>

#include "Formula.h"
#include "KnowledgeBase.h"

class FormulaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FormulaWidget(KnowledgeBase *kb, QWidget *parent = 0);
    explicit FormulaWidget(Formula* formula, KnowledgeBase *kb, QWidget *parent = 0);
    bool isValid();

    QComboBox* objectCombo;
    QComboBox* attributeCombo;
    QComboBox* opCombo;
    QLineEdit* valueEdit;
    Formula* m_formula;

private:
    const static char* OPERATIONS_STRING;
    KnowledgeBase* m_kb;
    void buildUi();
    void fillObjectsCombo();
    void fillAttributeCombo();
    void fillOpCombo();
signals:
    void formulaChanged();
public slots:
    void setObject(QString objectname);
    void setAttribute(QString attribute);
    void setOp(QString op);
    void setValue(QString value);
};
