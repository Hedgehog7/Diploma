#include "KnowledgeBase.h"

KnowledgeBase::KnowledgeBase()
{
}

KBObject* KnowledgeBase::getObject(const QString name)
{
    for (auto it = objects.begin(); it != objects.end(); it++) {
        if (it->m_name == name) {
            return &(*it);
        }
    }
    return nullptr;
}

KBInterval* KnowledgeBase::getInterval(const QString name)
{
    for (auto it = intervals.begin(); it != intervals.end(); it++) {
        if (it->m_name == name) {
            return &(*it);
        }
    }
    return nullptr;
}

KBEvent *KnowledgeBase::getEvent(const QString name)
{
    for (auto it = events.begin(); it != events.end(); it++) {
        if (it->m_name == name) {
            return &(*it);
        }
    }
    return nullptr;
}

KBRule *KnowledgeBase::getRule(const QString name)
{
    for (auto it = rules.begin(); it != rules.end(); it++) {
        if (it->m_name == name) {
            return &(*it);
        }
    }
    return nullptr;
}

int KnowledgeBase::getObjectIndex(const QString name)
{
    for (int i = 0; i < objects.count(); i++) {
        if (objects.at(i).m_name == name) {
            return i;
        }
    }
    return -1;
}

int KnowledgeBase::getIntervalIndex(const QString name)
{
    for (int i = 0; i < intervals.count(); i++) {
        if (intervals.at(i).m_name == name) {
            return i;
        }
    }
    return -1;
}

int KnowledgeBase::getEventIndex(const QString name)
{
    for (int i = 0; i < events.count(); i++) {
        if (events.at(i).m_name == name) {
            return i;
        }
    }
    return -1;
}

int KnowledgeBase::getRuleIndex(const QString name)
{
    for (int i = 0; i < rules.count(); i++) {
        if (rules.at(i).m_name == name) {
            return i;
        }
    }
    return -1;
}

void KnowledgeBase::addObject(KBObject& obj) {
    if (!getObject(obj.m_name)) {
        objects.append(obj);
    }
}

void KnowledgeBase::addInterval(KBInterval &interval)
{
    if (!getInterval(interval.m_name)) {
        intervals.append(interval);
    }
}

void KnowledgeBase::addEvent(KBEvent &event)
{
    if (!getEvent(event.m_name)) {
        events.append(event);
    }
}

void KnowledgeBase::addRule(KBRule &rule)
{
    if (!getRule(rule.m_name)) {
        rules.append(rule);
    }
}

void KnowledgeBase::deleteObject(const QString name)
{
    objects.removeAt(getObjectIndex(name));
}

void KnowledgeBase::deleteInterval(const QString name)
{
    intervals.removeAt(getIntervalIndex(name));
}

void KnowledgeBase::deleteEvent(const QString name)
{
    events.removeAt(getEventIndex(name));
}

void KnowledgeBase::deleteRule(const QString name)
{
    rules.removeAt(getRuleIndex(name));
}

QString KnowledgeBase::objectsToString() const
{
    QString res("Objects: \n");
    for (auto it = objects.begin(); it != objects.end(); it++) {
        res.append(it->toString());
    }
    return res;
}

QString KnowledgeBase::intervalsToString()
{
    QString res("Intervals: \n");
    for (auto it = intervals.begin(); it != intervals.end(); it++) {
        res.append(it->toString());
    }
    return res;
}

QString KnowledgeBase::eventsToString()
{
    QString res("Events: \n");
    for (auto it = events.begin(); it != events.end(); it++) {
        res.append(it->toString());
    }
    return res;
}

QString KnowledgeBase::rulesToString()
{
    QString res("Rules: \n");
    for (auto it = rules.begin(); it != rules.end(); it++) {
        res.append(it->m_name);
    }
    return res;

}
