#include "KBStatement.h"

KBStatement::KBStatement()
    :m_objectName(QString("")),
     m_attributeName(QString("")),
     m_value(QString("")),
     m_belief(50),
     m_probabilty(50),
     m_accuracy(50)
{}

KBStatement::KBStatement(const KBStatement& rhs)
{
     m_objectName = rhs.m_objectName;
     m_attributeName = rhs.m_attributeName;
     m_value = rhs.m_value;
     m_belief = rhs.m_belief;
     m_probabilty = rhs.m_probabilty;
     m_accuracy = rhs.m_accuracy;
}

QString KBStatement::statementString() {
    return QString(m_objectName + "." + m_attributeName + " = " + m_value);
}

QString KBStatement::withString() {
    return QString("Belief = " + QString::number(m_belief) + " Probabilty = " + QString::number(m_probabilty) + " Accuracy = " + QString::number(m_accuracy));
}
