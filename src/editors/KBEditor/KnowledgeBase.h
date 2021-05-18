#pragma once

#include <QList>

#include "KBObject.h"
#include "KBInterval.h"
#include "KBEvent.h"
#include "KBRule.h"
class KnowledgeBase
{
public:
    KnowledgeBase();

    QList<KBObject> objects;
    QList<KBInterval> intervals;
    QList<KBEvent> events;
    QList<KBRule> rules;

    KBObject* getObject(const QString name);
    KBInterval* getInterval(const QString name);
    KBEvent* getEvent(const QString name);
    KBRule* getRule(const QString name);

    int getObjectIndex(const QString name);
    int getIntervalIndex(const QString name);
    int getEventIndex(const QString name);
    int getRuleIndex(const QString name);


    void addObject(KBObject& obj);
    void addInterval(KBInterval& interval);
    void addEvent(KBEvent& event);
    void addRule(KBRule& rule);

    void deleteObject(const QString name);
    void deleteInterval(const QString name);
    void deleteEvent(const QString name);
    void deleteRule(const QString name);

    QString objectsToString() const;
    QString intervalsToString();
    QString eventsToString();
    QString rulesToString();
};
