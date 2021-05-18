#ifndef CONDITION_STACK_H
#define CONDITION_STACK_H

#include <string>
#include <vector>

class QuestionAnswerStack
{
public:
	struct Condition
	{
		std::string attributeType;
		std::vector<std::string> attributeValues;
		std::vector<std::string> symptomValues;
		int attributeId;

		Condition(const std::string& _attributeType, const std::string& _attributeValue
			, const std::string& _symptomValue, int _attributeId)
			: attributeType(_attributeType)
			, attributeValues({ _attributeValue })
			, symptomValues({ _symptomValue })
			, attributeId(_attributeId)
		{}
	};

public:
	QuestionAnswerStack();

	void push(const std::string& question, const std::string& answer, const std::string& symptom, int attributeId);
	void add(const std::string& answer, const std::string& symptom);
	const Condition& top() const;
	void pop();
	bool empty() const;

	const std::vector<Condition>& getConditions() const;

private:
	std::vector<Condition> _conditions;
};

#endif // CONDITION_STACK_H
