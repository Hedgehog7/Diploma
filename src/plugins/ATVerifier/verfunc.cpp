#include"verfunc.h"

const QVector <QString> TEMPMARKERS = {"вчера", "сегодня", "завтра", "дня", "днем", "вечер"};


QPair<QVector<QString>, QVector<QString>>& __make_pair_from_line(QString& line){
    QPair<QVector<QString>, QVector<QString>>* result = new QPair<QVector<QString>, QVector<QString>>;
    QString thenline = line.mid(line.indexOf("ТО ")), ifline = line;
    line.reserve(0);
    ifline.remove(line.indexOf("ТО "), thenline.length());
    ifline.remove(0, 5);
    ifline.remove(line.length() - 1, 1);
    thenline.remove(0, 3);

    QVector<QString> ifarg, thenarg;
    while (ifline.indexOf(" И ") > 0 || ifline.indexOf(" ИЛИ ") > 0)
    {
        QString arg = ifline.mid(0, (ifline.indexOf(" И ") < ifline.indexOf(" ИЛИ "))?ifline.indexOf(" И "):ifline.indexOf(" ИЛИ "));
        int number = 0;
            while (number < ifarg.length() && arg > ifarg[number]) ++number;
        ifarg.insert(number, arg);
    }
    while (thenline.indexOf(" И ") > 0 || thenline.indexOf(" ИЛИ ") > 0)
    {
        QString arg = thenline.mid(0, (thenline.indexOf(" И ") < thenline.indexOf(" ИЛИ "))?thenline.indexOf(" И "):thenline.indexOf(" ИЛИ "));
        int number = 0;
            while (number < thenarg.length() && arg > thenarg[number]) ++number;
        thenarg.insert(number, arg);
    }
    result->first = ifarg;
    result->second = thenarg;
    return *result;
}


int find_temporal_markers(QFile &file)
{
    int result = 0;
    if (file.open(QIODevice::ReadOnly))
    {
        QString line;
        while (!file.atEnd())
        {
            line = file.readLine();\
            for (QString mark : TEMPMARKERS)
            {
                if (line.indexOf(mark) >= 0)
                {
                    result += 0;
                }
            }
        }
    }
    file.close();
    return result;
}

int find_same_rules(QFile &file)
{
    int result = 0;
    QVector <QPair<QVector<QString>, QVector<QString>>> rules;//ifarg and thenarg
    if (file.open(QIODevice::ReadOnly))
    {
        QString line;
        while (!file.atEnd())
        {
            line = file.readLine();
            QPair<QVector<QString>, QVector<QString>> arg = __make_pair_from_line(line);
            bool unique = true;
            for (QPair<QVector<QString>, QVector<QString>> rule : rules)
            {
                if (rule == arg)
                {
                    ++result;
                    unique = false;
                }
            }
            if (unique)
            {
                rules.append(arg);
            }
        }
    }
    file.close();
    return result;
}

int find_void_conditions(QFile &file)
{
    int result = 0;
    if (file.open(QIODevice::ReadOnly))
    {
        QString line;
        while (!file.atEnd())
        {
            line = file.readLine();
            QPair<QVector<QString>, QVector<QString>> arg = __make_pair_from_line(line);
            if (arg.first.isEmpty())
            {
                ++result;
            }
        }
    }
    return result;
}

int find_void_conclusions(QFile &file)
{
    int result = 0;
    if (file.open(QIODevice::ReadOnly))
    {
        QString line;
        while (!file.atEnd())
        {
            line = file.readLine();
            QPair<QVector<QString>, QVector<QString>> arg = __make_pair_from_line(line);
            if (arg.second.isEmpty())
            {
                ++result;
            }
        }
    }
    return result;
}

