#include "KBNode.h"

KBNode::KBNode()
    :m_type(""),
     m_parent(nullptr),
     m_data(QVariant("")),
     m_children(QList<KBNode>())
{
    static int id = 0;
    m_id = id++;
}

KBNode::~KBNode() {
}

void KBNode::die()
{
    m_children.clear();
    m_parent->removeChild(m_id);
}

int KBNode::getChildIndex(int id)
{
    for (int i = 0; i < m_children.count(); i++) {
        if (m_children.at(i).m_id == id) {
            return i;
        }
    }
    return -1;
}

void KBNode::removeChild(int id) {
    m_children.removeAt(getChildIndex(id));
}
