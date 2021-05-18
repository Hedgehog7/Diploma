#include "KBAttribute.h"

KBAttribute::KBAttribute()
    : m_name(QString("")),
      m_type(QString(""))
{}

KBAttribute::KBAttribute(QString name, QString type)
    : m_name(name),
      m_type(type)
{}

QString KBAttribute::toString() const {
    QString res("Attribute(Name: ");
    res.append(m_name);
    res.append(" | Type: ");
    res.append(m_type);
    res.append(")");
    return res;
}
