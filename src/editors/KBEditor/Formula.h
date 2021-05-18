#pragma once

#include <QString>
#include "KBObject.h"
#include "KBAttribute.h"

class Formula
{
public:
    Formula();
    Formula(QString op, KBObject object, KBAttribute attribute, QString value);
    ~Formula();
    QString toString();

    QString m_op;
    KBObject m_object;
    KBAttribute m_attribute;
    QString m_value;
};
