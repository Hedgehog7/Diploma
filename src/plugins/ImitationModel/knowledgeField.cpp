#include "knowledgeField.h"
#include "../../libs/KnowledgeField/knowledge_field_t.h"
#include <QString>
#include <QTextCodec>

//Для корректного отображения русского текста в Qt
QString KnowledgeField::ru(const char * text)
{
	return QTextCodec::codecForName("CP1251")->toUnicode(text);
}

KnowledgeField::KnowledgeField()
{
	QList<QString> values1;
	values1 << ru("Да") << ru("Нет") << ru("Скорее да, чем нет") << 
		ru("Скорее нет, чем да") << ru("Может быть");
	Type type1 = { ru("Тип1"), values1 };
	QList<Type> types;
	types << type1;
	this->types = types;

	Attribute attr1 = { ru("Атрибут1"), types.at(0) };
	Attribute attr2 = { ru("Атрибут21"), types.at(0) };
	Attribute attr3 = { ru("Атрибут22"), types.at(0) };

	QList<Attribute> attrs;

	attrs << attr1;
	Object obj1 = { ru("Объект1"), attrs };

	attrs.clear();
	attrs << attr2 << attr3;
	Object obj2 = { ru("Объект2"), attrs };

	QList<Object> objs;
	objs << obj1 << obj2;
	this->objects = objs;

	Element elem1 = { obj1, attr1, type1.values.at(0) };
	Element elem2 = { obj2, attr2, type1.values.at(0) };
	Element elem3 = { obj2, attr3, type1.values.at(0) };

	Element elem4 = { obj1, attr1, type1.values.at(1) };
	Element elem5 = { obj2, attr3, type1.values.at(1) };

	QList<Element> condition;
	QList<Element> action;

	condition << elem1 << elem2;
	action << elem3;
	Rule rule1 = { condition, action };

	condition.clear(); action.clear();
	condition << elem4;
	action << elem5;
	Rule rule2 = { condition, action };

	QList<Rule> rules;
	rules << rule1 << rule2;
	this->rules = rules;
}

void KnowledgeField::createKnowledgeField()
{
	QList<QString> values1;
	values1 << ru("Да") << ru("Нет") << ru("Скорее да, чем нет") <<
		ru("Скорее нет, чем да") << ru("Может быть");
	Type type1 = { ru("Тип1"), values1 };

	QList<Type> types;
	types << type1;
	this->types = types;

	Attribute attr1 = { ru("Атрибут3"), types.at(0) };
	Attribute attr2 = { ru("Атрибут21"), types.at(0) };
	Attribute attr3 = { ru("Атрибут23"), types.at(0) };

	QList<Attribute> attrs;

	attrs << attr1;
	Object obj1 = { ru("Объект3"), attrs };

	attrs.clear();
	attrs << attr2 << attr3;
	Object obj2 = { ru("Объект2"), attrs };

	QList<Object> objs;
	objs << obj1 << obj2;
	this->objects = objs;

	Element elem1 = { obj1, attr1, type1.values.at(0) };
	Element elem2 = { obj2, attr2, type1.values.at(0) };
	Element elem3 = { obj2, attr3, type1.values.at(0) };

	Element elem4 = { obj1, attr1, type1.values.at(1) };
	Element elem5 = { obj2, attr3, type1.values.at(1) };

	QList<Element> condition;
	QList<Element> action;

	condition << elem1 << elem2;
	action << elem3;
	Rule rule1 = { condition, action };

	condition.clear(); action.clear();
	condition << elem4;
	action << elem5;
	Rule rule2 = { condition, action };

	QList<Rule> rules;
	rules << rule1 << rule2;
	this->rules = rules;
}

KnowledgeField KnowledgeField::createCombinedProtocol(QList<KnowledgeField> knowledgeFieldList)
{
	KnowledgeField knowledgeField = knowledgeFieldList[0];
	for (int kfnum = 1; kfnum < knowledgeFieldList.length(); kfnum++)
	{
		knowledgeField.objects << knowledgeFieldList[kfnum].objects;
		knowledgeField.rules << knowledgeFieldList[kfnum].rules;
		knowledgeField.types << knowledgeFieldList[kfnum].types;
	}

	for (int i1 = 0; i1 < knowledgeField.objects.count(); i1++)
	{
		for (int i2 = 0; i2 < knowledgeField.objects.count(); i2++)
		{
			if (i1 != i2 && knowledgeField.objects.at(i1).name == knowledgeField.objects.at(i2).name)
			{
				Object newobj = { knowledgeField.objects.at(i1).name,
					knowledgeField.objects.at(i1).attrs + knowledgeField.objects.at(i2).attrs };
				knowledgeField.objects.replace(i1, newobj);
				knowledgeField.objects.removeAt(i2);
			}
		}
	}

	for (int i = 0; i < knowledgeField.objects.count(); i++)
	{
		for (int j1 = 0; j1 < knowledgeField.objects.at(i).attrs.count(); j1++)
		{
			for (int j2 = 0; j2 < knowledgeField.objects.at(i).attrs.count(); j2++)
			{
				if (j1 != j2 && knowledgeField.objects.at(i).attrs.at(j1).name ==
					knowledgeField.objects.at(i).attrs.at(j2).name)
				{
					Type newtype = { knowledgeField.objects.at(i).attrs.at(j1).type.name, knowledgeField.objects.at(i).attrs.at(j1).type.values + knowledgeField.objects.at(i).attrs.at(j2).type.values };
					Attribute newattr = { knowledgeField.objects.at(i).attrs.at(j1).name, newtype };
					QList<Attribute> newattrs;
					newattrs << knowledgeField.objects.at(i).attrs;
					newattrs.replace(j1, newattr);
					newattrs.removeAt(j2);
					Object newobj = { knowledgeField.objects.at(i).name, newattrs };
					knowledgeField.objects.replace(i, newobj);
				}
			}
		}
	}

	for (int i = 0; i < knowledgeField.objects.count(); i++)
	{
		for (int j = 0; j < knowledgeField.objects.at(i).attrs.count(); j++)
		{
			for (int k1 = 0; k1 < knowledgeField.objects.at(i).attrs.at(j).type.values.count(); k1++)
			{
				for (int k2 = 0; k2 < knowledgeField.objects.at(i).attrs.at(j).type.values.count(); k2++)
				{
					if (k1 != k2 && knowledgeField.objects.at(i).attrs.at(j).type.values.at(k1) ==
						knowledgeField.objects.at(i).attrs.at(j).type.values.at(k2))
					{
						QList<QString> newvalues;
						newvalues << knowledgeField.objects.at(i).attrs.at(j).type.values;
						newvalues.removeAt(k2);
						Type newtype = { knowledgeField.objects.at(i).attrs.at(j).type.name, newvalues };
						Attribute newattr = { knowledgeField.objects.at(i).attrs.at(j).name, newtype };
						QList<Attribute> newattrs;
						newattrs << knowledgeField.objects.at(i).attrs;
						newattrs.replace(j, newattr);
						Object newobj = { knowledgeField.objects.at(i).name, newattrs };
						knowledgeField.objects.replace(i, newobj);
					}
				}
			}
		}
	}

	knowledgeField.types.clear();
	for (int i = 0; i < knowledgeField.objects.count(); i++)
	{
		for (int j = 0; j < knowledgeField.objects.at(i).attrs.count(); j++)
		{
			bool flag = true;
			for (int k = 0; k < knowledgeField.types.length(); k++)
			{
				if (knowledgeField.types.at(k).name == knowledgeField.objects.at(i).attrs.at(j).type.name)
				{
					flag = false;
					break;
				}
			}
			if (flag) knowledgeField.types << knowledgeField.objects.at(i).attrs.at(j).type;
		}
	}
	return knowledgeField;
}

