#pragma once

#include <QString>
#include "Formula.h"

class KBInterval
{
public:
    KBInterval();

    QString toString();

    QString m_name;
    Formula m_close;
    Formula m_open;
};
