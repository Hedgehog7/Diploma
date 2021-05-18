#pragma once

#include <QString>
#include <QVariant>
#include <QList>

class KBNode
{
public:
    KBNode();
    ~KBNode();
    void die();
    int getChildIndex(int id);
    void removeChild(int id);

    int m_id;
    QString m_type;
    QVariant m_data;
    QList<KBNode> m_children;
    KBNode* m_parent;
};
