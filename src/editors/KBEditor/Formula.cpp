#include "Formula.h"
#include <QDebug>
Formula::Formula()
    : m_op(QString("")),
      m_object(KBObject("")),
      m_attribute(KBAttribute("", "")),
      m_value(QString(""))
{
    qDebug() << "Formula default constructor";
}

Formula::Formula(QString op, KBObject object, KBAttribute attribute, QString value)
    : m_op(op),
      m_object(object),
      m_attribute(attribute),
      m_value(value)
{
    qDebug() << "Formula param constructor";
}

Formula::~Formula()
{}

QString Formula::toString()
{
    QString res("");
    res.append(m_object.m_name);
    res.append(".");
    res.append(m_attribute.m_name);
    res.append(" ");
    res.append(m_op);
    res.append(" ");
    res.append(m_value);
    res.append("");
    return res;
}
