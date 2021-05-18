#include "KBObject.h"

KBObject::KBObject()
    : m_name(QString(""))
{}


KBObject::KBObject(QString name)
    : m_name(name)
{
}

KBObject::~KBObject()
{
}

KBAttribute *KBObject::getAttribute(const QString name)
{
    for (auto it = m_attributes.begin(); it != m_attributes.end(); it++) {
        if (it->m_name == name) {
            return &(*it);
        }
    }
    return nullptr;
}

int KBObject::getAttributeIndex(const QString name)
{
    for (int i = 0; i < m_attributes.count(); i++) {
        if (m_attributes.at(i).m_name == name) {
            return i;
        }
    }
    return -1;
}

void KBObject::deleteAttribute(const QString name)
{
    m_attributes.removeAt(getAttributeIndex(name));
}

void KBObject::addAttribute(KBAttribute &attr)
{
    if (!getAttribute(attr.m_name)) {
        m_attributes.append(attr);
    }
}

QString KBObject::toString() const
{
	QString res("KBObject(name: ");
	res.append(m_name);
	res.append(" | attributes:\n");
	for (auto it = m_attributes.begin(); it != m_attributes.end(); it++) {
		res.append(it->toString());
	}
	res.append(")\n");
	return res;
}
