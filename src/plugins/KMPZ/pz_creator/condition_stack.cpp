#include "condition_stack.h"

QuestionAnswerStack::QuestionAnswerStack()
{}

void QuestionAnswerStack::push(const std::string& question, const std::string& answer, const std::string& symptom, int attributeId)
{
	_conditions.emplace_back(question, answer, symptom, attributeId);
}

void QuestionAnswerStack::add(const std::string& answer, const std::string& symptom)
{
	_conditions.back().attributeValues.push_back(answer);
	_conditions.back().symptomValues.push_back(symptom);
}

const QuestionAnswerStack::Condition& QuestionAnswerStack::top() const
{
	return _conditions.back();
}

void QuestionAnswerStack::pop()
{
	_conditions.pop_back();
}

bool QuestionAnswerStack::empty() const
{
	return _conditions.empty();
}

const std::vector<QuestionAnswerStack::Condition>& QuestionAnswerStack::getConditions() const
{
	return _conditions;
}
