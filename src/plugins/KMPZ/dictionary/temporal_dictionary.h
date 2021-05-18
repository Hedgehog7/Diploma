#ifndef TEMPORAL_DICTIONARY_H
#define TEMPORAL_DICTIONARY_H

#include <string>
#include <vector>
#include <regex>
#include <map>
#include <functional>

#include <QString>

class TemporalDictionary
{
public:
    class Result
    {
    public:
        const std::wstring& getStart() const { return _start; }
        const std::wstring& getEnd() const { return _end; }
        const std::wstring& getDuration() const { return _duration; }
        const std::wstring& getEvent() const { return _event; }

        void setStart(const std::wstring& start) { _hasStart = true; _start = start; }
        void setEnd(const std::wstring& end) { _hasEnd = true; _end = end; }
        void setDuration(const std::wstring& duration) { _hasDuration = true; _duration = duration; }
        void setEvent(const std::wstring& event) { _hasEvent = true; _event = event; }

        bool hasStart() const { return _hasStart; }
        bool hasEnd() const { return _hasEnd; }
        bool hasDuration() const { return _hasDuration; }
        bool hasEvent() const { return _hasEvent; }

    private:
            std::wstring _start;
            std::wstring _end;
            std::wstring _duration;
            std::wstring _event;
            bool _hasStart = false;
            bool _hasEnd = false;
            bool _hasDuration = false;
            bool _hasEvent= false;
    };

private:
    enum class LeksemAction {START, END, DURATION, EVENT};

    struct Leksem
    {
        std::wstring pattern;
        LeksemAction action;
        QString function;
    };

public:
    bool load(const std::string& fileName);
    std::vector<Result> getResult(const std::vector<std::wstring>& search);

private:
    void initSetFunctions();
    LeksemAction actionFromString(const std::string& str) const;
    void setCondition(const Leksem& leksem, std::wstring matched, std::vector<Result>& result);

private:
    std::map<std::wstring, std::vector<Leksem>> _data;

    std::function<void(const std::wstring& res, std::vector<Result>&)> setStart;
    std::function<void(const std::wstring& res, std::vector<Result>&)> setEnd;
    std::function<void(const std::wstring& res, std::vector<Result>&)> setDuration;
    std::function<void(const std::wstring& res, std::vector<Result>&)> setEvent;
};

#endif // CONDITION_STACK_H
