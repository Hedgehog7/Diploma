#pragma once

#include <QString>
#include "Formula.h"

class KBEvent
{
public:
    KBEvent();
    QString toString();

    QString m_name;
    Formula m_formula;

};
