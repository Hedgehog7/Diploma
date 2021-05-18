#pragma once

#include <QString>
#include "KBStatement.h"
#include "KBNode.h"

class KBRule
{
public:
    KBRule();

    QString m_name;
    KBNode* m_ifNode;
    KBStatement* m_thenStatement;
};
