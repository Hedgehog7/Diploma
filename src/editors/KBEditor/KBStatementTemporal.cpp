#include "KBStatementTemporal.h"

KBStatementTemporal::KBStatementTemporal()
    :m_arg1(QString("")),
     m_temporalRelation(QString("")),
     m_arg2(QString(""))
{}

KBStatementTemporal::KBStatementTemporal(const KBStatementTemporal &rhs)
{
    m_arg1 = rhs.m_arg1;
    m_temporalRelation = rhs.m_temporalRelation;
    m_arg2 = rhs.m_arg2;
}

QString KBStatementTemporal::toString()
{
    return QString(m_arg1 + " " + m_temporalRelation + " " + m_arg2);
}
