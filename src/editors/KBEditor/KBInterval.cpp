#include "KBInterval.h"

#include <QString>

KBInterval::KBInterval()
{

}

QString KBInterval::toString()
{
    QString res("Interval(Name: ");
    res.append(m_name);
    res.append("; Open: ");
    res.append(m_open.toString());
    res.append("; Close: ");
    res.append(m_close.toString());
    res.append(")");
    return res;
}
