#ifndef KNOWLEDGEFIELD_H
#define KNOWLEDGEFIELD_H

#include <QtWidgets/QMainWindow>
#include <QList>
#include <QString>

class Type
{
public:
	QString name;
	QList<QString> values;
};

class Attribute
{
public:
	QString name;
	Type type;
};

class Object
{
public:
	QString name;
	QList<Attribute> attrs;
};

class Element
{
public:
	Object obj;
	Attribute attr;
	QString value;
};

class Rule
{
public:
	QList<Element> condition;
	QList<Element> action;
};

class KnowledgeField
{
public:
	QList<Type> types;
	QList<Object> objects;
	QList<Rule> rules;
	QString ru(const char * text);
	KnowledgeField();
	void createKnowledgeField();
	static KnowledgeField createCombinedProtocol(QList<KnowledgeField> knowledgeFieldList);
};

#endif
