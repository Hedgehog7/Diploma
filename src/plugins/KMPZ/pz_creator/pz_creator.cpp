#include "pz_creator.h"

#include "../dictionary/temporal_dictionary.h"
#include <QFile>
#include <iostream> 
#include <fstream>
#include <stdlib.h>
#include <vector>

#include <QString>
#include <QTextCodec>
#include <QXmlStreamReader>

#include <clocale>
#include <iostream>

namespace PZ {
    std::string get_path()
    {
        QDir dir;
        QString path = dir.currentPath();
        path.truncate(path.lastIndexOf('/'));
        path.truncate(path.lastIndexOf('/'));
        path.truncate(path.lastIndexOf('/'));
        return path.toStdString();
    }
}

PZCreator::PZCreator()
{    
    _outPath = QString((PZ::get_path() + "/resources/libTemp/output.xml").c_str());
    _bzPath = PZ::get_path() + "/resources/libTemp/bz.xml";
	_knowledgeField.add_object(_mainObjectName);
	auto object = _knowledgeField.get_object(_mainObjectName);

	std::string attributeName = "Атрибут" + std::to_string(_nAttributes++); //"Атрибут"
	_knowledgeField.add_attr(object, attributeName, "Диагноз"); //"Диагноз"
	_conclusionAttributeId = object->get_attr_id_by_name(attributeName);
	_knowledgeField.add_type("Диагноз");

	attributeName = "Атрибут" + std::to_string(_nAttributes++);
	_knowledgeField.add_attr(object, attributeName, "Время"); //"Время"
	_eventAttributeId = object->get_attr_id_by_name(attributeName);
	_knowledgeField.add_type("Время"); //"Время"
}

void PZCreator::addQuestionAndAnswer(const std::string& question, const std::string& answer)
{
	auto attributeId = addAttributeToMainObject(question); //"Симптом"

	_atn.init(false);
	_atn.loadData(answer);
	_atn.execute();

	vector<string> tempLexemes, tempObjects;
	tempLexemes.push_back(".");

	addTemporal(&tempLexemes, &tempObjects);

	_tempObjects[attributeId] = tempObjects;

	std::string symptom = getSymptom(tempLexemes, answer);

	_questionAnswerStack.push(question, answer, symptom, attributeId);

	_knowledgeField.add_type(question); //ru("Симптом").toStdString()
	_knowledgeField.add_type_value(question, symptom);
}

void PZCreator::addAnswer(const std::string& answer)
{
	_atn.init(false);
	_atn.loadData(answer);
	_atn.execute();

	vector<string> tempLexemes, tempObjects;
	tempLexemes.push_back(".");

	addTemporal(&tempLexemes, &tempObjects);

	_tempObjects[_questionAnswerStack.top().attributeId] = tempObjects;

	std::string symptom = getSymptom(tempLexemes, answer);

	_questionAnswerStack.add(answer, symptom);

	_knowledgeField.add_type_value(_questionAnswerStack.top().attributeType, symptom); //"Симптом"
}

std::string PZCreator::getSymptom(const vector<string>& tempLexemes, const std::string& answer)
{
	std::string symptom = QString::fromStdString(answer).toLower().toStdString();

	for (int i = 0; i < tempLexemes.size(); i++) {
		int n = tempLexemes[i].length();
		for (int j = symptom.find(tempLexemes[i]);
			j != string::npos; j = symptom.find(tempLexemes[i]))
			symptom.erase(j, n);
	}
	symptom = QString::fromStdString(symptom).trimmed().toStdString();
	return symptom;
}

void PZCreator::removeQuestion()
{
	std::cout << "PZ: " << _knowledgeField.to_string() << std::endl;

	_questionAnswerStack.pop();
}

std::string PZCreator::getBZ()
{
	return _knowledgeField.to_string();
}

std::string PZCreator::getBZXml()
{
	return _knowledgeField.to_xml();
}

void PZCreator::writeToFile()
{
	std::ofstream ofs;
	ofs.open(_bzPath, std::ofstream::out | std::ofstream::trunc);
	ofs << getBZXml();
	ofs.close();
}

void PZCreator::addConclusion(const std::string& conclusion)
{
	action_string_t* action = new action_string_t;
	action->set_object_id(_knowledgeField.get_object_id_by_name(_mainObjectName));
	action->set_attribute_id(_conclusionAttributeId);
	action->set_op("=");
	action->set_value(conclusion);

	_knowledgeField.add_type_value("Диагноз", conclusion); //"Диагноз"

	auto rule = _knowledgeField.add_rule("Правило" + std::to_string(_nRules++)); //"Правило"
	_knowledgeField.add_condition(rule, getCurrentCondition());
	_knowledgeField.add_action(rule, action);
}

const QuestionAnswerStack::Condition& PZCreator::getLastQuestion() const
{
	return _questionAnswerStack.top();
}

bool PZCreator::hasQuestions() const
{
	return !_questionAnswerStack.empty();
}

complex_condition_t* PZCreator::getCurrentCondition()
{
	auto objectId = _knowledgeField.get_object_id_by_name(_mainObjectName);

	complex_condition_t* complexCondition = new complex_condition_t;
	complexCondition->set_op("&");

	for (const auto& question : _questionAnswerStack.getConditions())
	{
		std::string tempObjectNames = "";
		for (const auto& tempObjectName : _tempObjects[question.attributeId])
		{
			tempObjectNames += " & " + tempObjectName;
		}

		condition_attr_value_const_t* condition = new condition_attr_value_const_t(objectId, question.attributeId, question.symptomValues.back()); //TODO  + tempObjectNames
		condition->set_op("=");
		complexCondition->add_condition(condition);
	}

	return complexCondition;
}


void PZCreator::addTemporal(vector<string> *tempLexemes, vector<string> *tempObjects)
{
	QFile file(_outPath);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString xml = "";
		while (!file.atEnd()) {
			xml += file.readLine();
		}
		//setlocale(LC_ALL, "rus");

		QXmlStreamReader reader(xml);
		while (!reader.atEnd() && !reader.hasError()) {
			reader.readNext();
			if (reader.name() == "Point") {
				std::string tempObjectName = "Событие" + std::to_string(++_nEvents); //"Событие"

				const temporal_entity_t* event = _knowledgeField.add_event(tempObjectName);
				tempObjects->push_back(tempObjectName);

				std::string strEvent = reader.attributes()[0].value().toString().toLower().toStdString();
				tempLexemes->push_back(strEvent);

				_knowledgeField.add_type_value("Время", strEvent); //"Время"

				condition_attr_value_const_t* condition = new condition_attr_value_const_t(mainObjectId(), _eventAttributeId, strEvent);
				condition->set_op("=");

				_knowledgeField.add_condition(event, condition);

				reader.readNext();
			}
			else if (reader.name() == "Period") { //TODO периодические правила
				std::string tempObjectName = "Событие" + std::to_string(++_nEvents); //"Событие"

				const temporal_entity_t* event = _knowledgeField.add_event(tempObjectName);
				tempObjects->push_back(tempObjectName);

				std::string strEvent = reader.attributes()[0].value().toString().toLower().toStdString();
				tempLexemes->push_back(strEvent);

				_knowledgeField.add_type_value("Время", strEvent); // "Время"

				condition_attr_value_const_t* condition = new condition_attr_value_const_t(mainObjectId(), _eventAttributeId, strEvent);
				condition->set_op("=");

				_knowledgeField.add_condition(event, condition);

				reader.readNext();
			}
			else if (reader.name() == "Interval") {

				std::string tempObjectName = "Интервал" + std::to_string(++_nIntervals); //"Интервал"

				const temporal_entity_t* interval = _knowledgeField.add_interval(tempObjectName);
				tempObjects->push_back(tempObjectName);

				reader.readNext();
				reader.readNext();

				while (reader.name().toString() == "Start" || reader.name().toString() == "End"
					|| reader.name().toString() == "Duration") {

					if (reader.name() == "Start") {

						std::string strStart = reader.attributes()[0].value().toString().toLower().toStdString();
						tempLexemes->push_back(strStart);

						_knowledgeField.add_type_value("Время", strStart); //"Время"

						condition_attr_value_const_t* condition = new condition_attr_value_const_t(mainObjectId(), _eventAttributeId, strStart);
						condition->set_op("=");

						_knowledgeField.set_start_condition(interval, condition);

						reader.readNext();
					}
					else if (reader.name() == "End") {

						std::string strEnd = reader.attributes()[0].value().toString().toLower().toStdString();
						tempLexemes->push_back(strEnd);

						_knowledgeField.add_type_value("Время", strEnd); //"Время"

						condition_attr_value_const_t* condition = new condition_attr_value_const_t(mainObjectId(), _eventAttributeId, strEnd);
						condition->set_op("=");

						_knowledgeField.set_end_condition(interval, condition);

						reader.readNext();
					}
					else if (reader.name() == "Duration") {

						std::string strDuration = reader.attributes()[0].value().toString().toLower().toStdString();
						tempLexemes->push_back(strDuration);

						_knowledgeField.add_type_value("Время", strDuration); //"Время"

						condition_attr_value_const_t* condition = new condition_attr_value_const_t(mainObjectId(), _eventAttributeId, strDuration);
						condition->set_op("=");

						_knowledgeField.set_duration_condition(interval, condition);

						reader.readNext();
					}

					reader.readNext();
				}
			}
		}
	}

	file.close();
}

int PZCreator::addAttributeToMainObject(const std::string& attrType)
{
	auto attributeName = "Атрибут" + std::to_string(_nAttributes++); //"Атрибут"
	auto object = _knowledgeField.get_object(_mainObjectName);
	_knowledgeField.add_attr(object, attributeName, attrType);
	auto attributeId = object->get_attr_id_by_name(attributeName);

	return attributeId;
}

int PZCreator::mainObjectId() const
{
	return _knowledgeField.get_object_id_by_name(_mainObjectName);
}

void PZCreator::addRule()
{
	if (_conclusions.empty())
		return;

	// TODO
	std::cout << "ПРАВИЛО:\nЕСЛИ\n";
	for (auto& condition : _questionAnswerStack.getConditions())
		std::cout << "    Пациент." << condition.attributeType << " = " << condition.attributeValues.back() << "\n";

	std::cout << "ТО\n";
	for (auto& conclusion : _conclusions)
		std::cout << "    " << conclusion << "\n";

	std::cout << std::endl;


	_conclusions.clear();
}

//Для корректного отображения русского текста в Qt
QString PZCreator::ru(const char * text)
{
	return QTextCodec::codecForName("CP1251")->toUnicode(text);
}
