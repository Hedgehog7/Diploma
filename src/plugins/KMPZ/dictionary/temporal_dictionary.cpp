#include "temporal_dictionary.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
//#include <QtScript>

#include <iostream>
bool TemporalDictionary::load(const std::string& fileName)
{
    initSetFunctions();

	QString settings;
    QFile file;
    file.setFileName(fileName.c_str());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    settings = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(settings.toUtf8());

    for( const auto& val: jsonDoc.array())
    {
        QJsonObject obj = val.toObject();

        auto indicator = obj["temporal_entity_id"].toString().toStdWString();
        auto leksemPattern = obj["temporal_leksem"].toString().toStdWString();
        auto action = actionFromString(obj["action"].toString().toStdString());
        QString function = "none";
        if( obj.contains("function") )
            function = obj["function"].toString();

        Leksem leksem = { leksemPattern, action, function };
        _data[indicator].push_back(leksem);
    }

    return true;
}

void TemporalDictionary::initSetFunctions()
{
    setStart = [](const std::wstring& res, std::vector<Result>& result)
    {
        if( result.empty() || result.back().hasStart() || result.back().hasEvent() )
            result.push_back(Result());

        result.back().setStart(res);
    };

    setEnd = [](const std::wstring& res, std::vector<Result>& result)
    {
        if( result.empty() || result.back().hasEnd() || result.back().hasEvent() )
            result.push_back(Result());

        result.back().setEnd(res);
    };

    setDuration = [](const std::wstring& res, std::vector<Result>& result)
    {
        if( result.empty() || result.back().hasDuration() || result.back().hasEvent() )
            result.push_back(Result());

        result.back().setDuration(res);
    };

    setEvent = [](const std::wstring& res, std::vector<Result>& result)
    {
        result.push_back(Result());
        result.back().setEvent(res);
    };
}

TemporalDictionary::LeksemAction TemporalDictionary::actionFromString(const std::string& str) const
{
    if( str == "setStart" )
        return LeksemAction::START;

    if( str == "setEnd" )
        return LeksemAction::END;

    if( str == "setDuration" )
        return LeksemAction::DURATION;

    if( str == "setEvent" )
        return LeksemAction::EVENT;
}

void TemporalDictionary::setCondition(const Leksem& leksem, std::wstring matched, std::vector<Result>& result)
{
    /*if( leksem.function != "none" )
    {
        QScriptEngine engine;

        QScriptValue val(&engine, QString::fromStdWString(matched));
        engine.globalObject().setProperty("leksem", val);

        matched = engine.evaluate(leksem.function).toString().toStdWString();
    }*/

    if( leksem.action == LeksemAction::START )
        setStart(matched, result);
    else if( leksem.action == LeksemAction::END )
        setEnd(matched, result);
    else if( leksem.action == LeksemAction::DURATION )
        setDuration(matched, result);
    else if( leksem.action == LeksemAction::EVENT )
        setEvent(matched, result);
}

std::vector<TemporalDictionary::Result> TemporalDictionary::getResult(const std::vector<std::wstring>& search)
{
    std::vector<Result> result;
    int metric = 0;

    for( const auto& source : search )
    {
        for( auto& indicator : _data )
        {
            ++metric;

            std::wsmatch indicator_ws;
            std::wregex indicator_wrx(indicator.first);
            if( std::regex_search(source.begin(), source.end(), indicator_ws, indicator_wrx) )
            {
                for( auto& leksem : indicator.second)
                {
                    ++metric;

                    std::wsmatch leksem_ws;
                    std::wregex leksem_wrx(leksem.pattern);
                    if( std::regex_search(source.begin(), source.end(), leksem_ws, leksem_wrx) )
                        setCondition(leksem, leksem_ws.str(), result);
                }
            }
        }
    }
    std::cout << "METRIC: " << metric << std::endl;

    return result;
}
