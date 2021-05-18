#pragma once
#include <QString>

class KBStatementTemporal
{
public:
    KBStatementTemporal();
    KBStatementTemporal(const KBStatementTemporal& rhs);
    QString toString();

    QString m_arg1;
    QString m_temporalRelation;
    QString m_arg2;
};
