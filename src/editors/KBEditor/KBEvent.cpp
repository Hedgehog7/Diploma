#include "KBEvent.h"

KBEvent::KBEvent()
{

}

QString KBEvent::toString()
{
    QString res("Event(Name: ");
    res.append(m_name);
    res.append("; Formula: ");
    res.append(m_formula.toString());
    res.append(")");
    return res;
}

