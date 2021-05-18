#include "RuleArg.h"

RuleArg::RuleArg()
    :m_name(QString(""))
{}

QString RuleArg::getName() const
{
    return m_name;
}
