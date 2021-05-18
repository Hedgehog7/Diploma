#include "KBRule.h"

KBRule::KBRule()
    :m_name(QString("")),
     m_thenStatement(nullptr),
     m_ifNode(new KBNode)
{
    m_ifNode->m_data = QVariant("IF");
}

//QString KBRule::toString()
//{
//    QString res("If {} " + m_temporalRelation + " {} then " + m_objectName + "." + m_attributeName + " = " + m_value);
//    res.append(" with Belief[" + QString::number(m_belief) + "; " + QString::number(m_probabilty) + "] + accuracy: " + QString::number(m_accuracy));
//    return res;
//}
