#pragma once

#include <QString>

class KBStatement
{
public:
    KBStatement();
    KBStatement(const KBStatement& rhs);
    QString statementString();
    QString withString();

    QString m_objectName;
    QString m_attributeName;
    QString m_value;
    std::size_t m_belief;
    std::size_t m_probabilty;
    std::size_t m_accuracy;
};
