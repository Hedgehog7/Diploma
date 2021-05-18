#pragma once

#include <QString>

class KBAttribute
{
public:
    KBAttribute();
    KBAttribute(QString name, QString type);
    QString toString() const;

    QString m_name;
    QString m_type;
};
