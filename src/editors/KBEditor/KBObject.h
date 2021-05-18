#pragma once

#include <QString>
#include <QList>
#include "KBAttribute.h"

class KBObject
{
public:
    KBObject();
    KBObject(QString name);
	~KBObject();
	
    void addAttribute(KBAttribute &attr);
    KBAttribute *getAttribute(const QString name);
    int getAttributeIndex(const QString name);
    void deleteAttribute(const QString name);

    QString toString() const;

	QString m_name;
    QList<KBAttribute> m_attributes;
};
