#include "knowledge_field_t.h"
#include <sstream>

int temporal_object_t::add_attr(const string& attr_name, const string& attr_type) {
	auto iter = name_to_id.find(attr_name);
	if (iter != name_to_id.end())
		return -1;
	attrs.emplace_back(attr_name, attr_type);
	name_to_id[attr_name] = attrs.size() - 1;
	return attrs.size() - 1;
}

int temporal_object_t::get_attr_id_by_name(const string& name) const {
	auto iter = name_to_id.find(name);
	if (iter != name_to_id.end())
		return iter->second;
	return -1;
}

string temporal_object_t::get_attr_name_by_id(int id) const {
	string res = "";
	if (id < (int)attrs.size()) {
		res += attrs[id].first;
	}
	return res;
}

string temporal_object_t::get_attr_type_by_id(int id) const {
	string res = "";
	if (id < (int)attrs.size()) {
		res += attrs[id].second;
	}
	return res;
}

int temporal_object_t::get_number_of_attrs() const {
	return attrs.size();
}

string temporal_object_t::to_string(const knowledge_field_t&) const {
	string res = "";
	res += "ИМЯ \"" + get_name() + "\"\n";
	res += "ГРУППА \"Объект\"\n";
	res += "АТРИБУТЫ\n";
	for (const auto& it : attrs)
		res += "  " + it.first + ": " + it.second + "\n";
	return res;
}

string condition_attr_value_const_t::to_string(const knowledge_field_t& kf, bool in_brackets, const string&, const string&) const {
	string res = "";
	if (in_brackets)
		res += "(";
	res += kf.get_full_attr_name_by_id(obj_id, attr_id);
	res += op;
	res += value;
	if (in_brackets)
		res += ")";
	return res;
}

string condition_attr_value_attr_value_t::to_string(const knowledge_field_t& kf, bool in_brackets, const string&, const string&) const {
	string res = "";
	if (in_brackets)
		res += "(";
	res += kf.get_full_attr_name_by_id(left_obj_id, left_attr_id);
	res += op;
	res += kf.get_full_attr_name_by_id(right_obj_id, right_attr_id);
	if (in_brackets)
		res += ")";
	return res;
}

string complex_condition_t::to_string(const knowledge_field_t& kf, bool in_brackets, const string& left_delimeter, const string& right_delimeter) const {
	string res = "";
	if (in_brackets)
		res += "(";
	for (size_t i = 0; i + 1 < data.size(); ++i)
		res += data[i]->to_string(kf, true, left_delimeter, right_delimeter) + left_delimeter + op + right_delimeter;
	res += data.back()->to_string(kf, true, left_delimeter, right_delimeter);
	if (in_brackets)
		res += ")";
	return res;
}

string temporal_interval_t::to_string(const knowledge_field_t &kf) const {
	string res = "";
	res += "ИМЯ \"" + get_name() + "\"\n";
	res += "ГРУППА \"Интервал\"\n";
	res += "АТРИБУТЫ\n";
	if (begin_condition)
		res += "  УслНач: " + begin_condition->to_string(kf, false) + "\n";
	if (end_condition)
		res += "  УслОконч: " + end_condition->to_string(kf, false) + "\n";
	if (duration_condition)
		res += "  Продолжительность: " + duration_condition->to_string(kf, false) + "\n";
	//res += "  Продолжительность: числовой\n";
	res += "  Количество возникновений: числовой\n";
	res += "  Активность: числовой\n";
	return res;
}

void temporal_interval_t::set_start_condition(condition_t* start) {
	if (begin_condition)
		delete begin_condition;
	begin_condition = start;
}

void temporal_interval_t::set_end_condition(condition_t* end) {
	if (end_condition)
		delete end_condition;
	end_condition = end;
}

void temporal_interval_t::set_duration_condition(condition_t* duration) {
	if (duration_condition)
		delete duration_condition;
	duration_condition = duration;
}

const temporal_entity_t* knowledge_field_t::add_object(const string& name) {
	if (mp_objects.find(name) != mp_objects.end())
		return nullptr;

	int cid = objects.size();
	objects.push_back(new temporal_object_t(cid));
	objects.back()->set_name(name);
	mp_objects[objects.back()->get_name()] = objects.back();

	return static_cast<temporal_entity_t*>(objects.back());
}

const temporal_entity_t* knowledge_field_t::add_interval(const string& name) {
	if (mp_intervals.find(name) != mp_intervals.end())
		return nullptr;

	int cid = intervals.size();
	intervals.push_back(new temporal_interval_t(cid));
	intervals.back()->set_name(name);
	mp_intervals[intervals.back()->get_name()] = intervals.back();

	return static_cast<temporal_entity_t*>(intervals.back());
}

const temporal_entity_t* knowledge_field_t::add_rule(const string& name) {
	if (mp_rules.find(name) != mp_rules.end())
		return nullptr;

	int cid = rules.size();
	rules.push_back(new temporal_rule_t(cid));
	rules.back()->set_name(name);
	mp_rules[rules.back()->get_name()] = rules.back();

	return static_cast<temporal_entity_t*>(rules.back());
}

const temporal_entity_t* knowledge_field_t::add_event(const string& name) {
	if (mp_events.find(name) != mp_events.end())
		return nullptr;

	int cid = events.size();
	events.push_back(new temporal_event_t(cid));
	events.back()->set_name(name);
	mp_events[events.back()->get_name()] = events.back();

	return static_cast<temporal_entity_t*>(events.back());
}

void knowledge_field_t::add_attr(const temporal_entity_t* it, const string& attr_name, const string& attr_type) const {
	string name = it->get_name();
	{
		const temporal_object_t* ptr = dynamic_cast<const temporal_object_t*>(it);
		if (ptr) {
			auto iter = mp_objects.find(name);
			if (iter != mp_objects.end()) {
				iter->second->add_attr(attr_name, attr_type);
			}
		}
	}
}

void knowledge_field_t::set_start_condition(const temporal_entity_t* interval, condition_t* start) const {
	const temporal_interval_t* ptr = dynamic_cast<const temporal_interval_t*>(interval);
	if (ptr) {
		auto iter = mp_intervals.find(ptr->get_name());
		if (iter != mp_intervals.end()) {
			iter->second->set_start_condition(start);
		}
	}
}

void knowledge_field_t::set_end_condition(const temporal_entity_t* interval, condition_t* end) const {
	const temporal_interval_t* ptr = dynamic_cast<const temporal_interval_t*>(interval);
	if (ptr) {
		auto iter = mp_intervals.find(ptr->get_name());
		if (iter != mp_intervals.end()) {
			iter->second->set_end_condition(end);
		}
	}
}

void knowledge_field_t::set_duration_condition(const temporal_entity_t* interval, condition_t* duration) const {
	const temporal_interval_t* ptr = dynamic_cast<const temporal_interval_t*>(interval);
	if (ptr) {
		auto iter = mp_intervals.find(ptr->get_name());
		if (iter != mp_intervals.end()) {
			iter->second->set_duration_condition(duration);
		}
	}
}

string knowledge_field_t::get_full_attr_name_by_id(int obj_id, int attr_id) const {
	string res = "";
	if (obj_id < (int)objects.size()) {
		res += objects[obj_id]->get_name();
		res += '.';
		res += objects[obj_id]->get_attr_name_by_id(attr_id);
	}
	return res;
}

pair<string, string> knowledge_field_t::get_full_attr_name_by_id_as_pair(int obj_id, int attr_id) const {
	if (obj_id < (int)objects.size())
		return{ objects[obj_id]->get_name(), objects[obj_id]->get_attr_name_by_id(attr_id) };
	return{ "", "" };
}

int knowledge_field_t::get_object_id_by_name(const string& name) const {
	auto iter = mp_objects.find(name);
	if (iter != mp_objects.end())
		return iter->second->get_id();
	return -1;
}

int knowledge_field_t::get_interval_id_by_name(const string& name) const {
	auto iter = mp_intervals.find(name);
	if (iter != mp_intervals.end())
		return iter->second->get_id();
	return -1;
}

const temporal_object_t* knowledge_field_t::get_object(const string& name) const {
	auto iter = mp_objects.find(name);
	if (iter != mp_objects.end()) {
		return iter->second;
	}
	return nullptr;
}

const temporal_object_t* knowledge_field_t::get_object(const int id) const {
	if (id >= (int)objects.size())
		return nullptr;
	return objects[id];
}

const temporal_event_t* knowledge_field_t::get_event(const int id) const {
	if (id >= (int)events.size())
		return nullptr;
	return events[id];
}

const temporal_interval_t* knowledge_field_t::get_interval(const int id) const {
	if (id >= (int)intervals.size())
		return nullptr;
	return intervals[id];
}

const temporal_rule_t* knowledge_field_t::get_rule(const int id) const {
	if (id >= (int)rules.size())
		return nullptr;
	return rules[id];
}

string temporal_rule_t::to_string(const knowledge_field_t& kf) const {
	string res = "";
	res += "ИМЯ \"" + get_name() + "\"\n";
	res += "ТИП \"" + type + "\"\n";
	res += "ЕСЛИ\n";
	if (condition) {
		res += "  ";
		res += condition->to_string(kf, false, " ", "\n  ");
		res += "\n";
	}
	res += "ТО\n";
	if (action) {
		res += "  " + action->to_string(kf) + "\n";
	}
	return res;
}

string knowledge_field_t::get_interval_name_by_id(int id) const {
	if (id < (int)intervals.size()) {
		return intervals[id]->get_name();
	}
	return "";
}

int knowledge_field_t::get_event_id_by_name(const string& name) const {
	auto iter = mp_events.find(name);
	if (iter != mp_events.end())
		return iter->second->get_id();
	return -1;
}

string knowledge_field_t::get_event_name_by_id(int id) const {
	if (id < (int)events.size()) {
		return events[id]->get_name();
	}
	return "";
}

string condition_interval_duration_t::to_string(const knowledge_field_t& kf, bool in_brackets, const string&, const string&) const {
	string res = "";
	if (in_brackets)
		res += "(";
	res += "Интервал.";
	res += kf.get_interval_name_by_id(interval_id);
	res += ".Продолжителность ";
	res += op;
	res += " ";
	res += std::to_string(duration_val);
	if (in_brackets)
		res += ")";
	return res;
}

string knowledge_field_t::to_string() const {
	string res = "";
	res += types_to_string();
	res += objects_to_string();
	res += intervals_to_string();
	res += rules_to_string();
	res += events_to_string();
	return res;
}

string knowledge_field_t::types_to_string() const {
	string res = "";
	res += "ТИП ТИП" + std::to_string(number_type_id) + "\n";
	res += "ЧИСЛО\n";
	res += "ОТ " + std::to_string(number_from) + "\n";
	res += "ДО " + std::to_string(number_to) + "\n";
	res += "КОММЕНТАРИЙ number\n\n";

	for (const pair<string, set<string>>& it : types) {
		res += "ТИП ТИП" + std::to_string(type_ids.find(it.first)->second) + "\n";
		res += "СИМВОЛ\n";
		for (const string& value : it.second)
			res += "\"" + value + "\"\n";
		res += "КОММЕНТАРИЙ " + it.first + "\n\n";
	}

	return res;
}

string knowledge_field_t::objects_to_string() const {
	string res = "";
	for (const temporal_object_t* it : objects) {
		res += it->to_string(*this);
		res += '\n';
	}
	return res;
}

string knowledge_field_t::intervals_to_string() const {
	string res = "";
	for (const temporal_interval_t* it : intervals) {
		res += it->to_string(*this);
		res += '\n';
	}
	return res;
}

string knowledge_field_t::rules_to_string() const {
	string res = "";
	for (const temporal_rule_t* it : rules) {
		res += it->to_string(*this);
		res += '\n';
	}
	return res;
}

string knowledge_field_t::events_to_string() const {
	string res = "";
	for (const temporal_event_t* it : events) {
		res += it->to_string(*this);
		res += '\n';
	}
	return res;
}

void knowledge_field_t::add_condition(const temporal_entity_t* entity, condition_t* condition) const {
	{
		const temporal_rule_t* ptr = dynamic_cast<const temporal_rule_t*>(entity);
		if (ptr) {
			auto iter = mp_rules.find(ptr->get_name());
			if (iter != mp_rules.end()) {
				iter->second->add_condition(condition);
			}
			return;
		}
	}
	{
		const temporal_event_t* ptr = dynamic_cast<const temporal_event_t*>(entity);
		if (ptr) {
			auto iter = mp_events.find(ptr->get_name());
			if (iter != mp_events.end()) {
				iter->second->add_condition(condition);
			}
			return;
		}
	}
	std::cerr << "Some shit" << std::endl;
}

void knowledge_field_t::add_action(const temporal_entity_t *rule, action_t *action) const {
	const temporal_rule_t* ptr = dynamic_cast<const temporal_rule_t*>(rule);
	if (ptr) {
		auto iter = mp_rules.find(ptr->get_name());
		if (iter != mp_rules.end()) {
			iter->second->add_action(action);
		}
	}
}

string action_int_t::to_string(const knowledge_field_t& kf) const {
	string res = "";
	res += kf.get_full_attr_name_by_id(obj_id, attr_id);
	res += " " + op + " ";
	res += std::to_string(value);
	return res;
}

string action_string_t::to_string(const knowledge_field_t &kf) const {
	string res = "";
	res += kf.get_full_attr_name_by_id(obj_id, attr_id);
	res += " " + op + " ";
	res += "\"" + value + "\"";
	return res;
}

string condition_sum_equal_const_t::to_string(const knowledge_field_t& kf, bool in_brackets, const string&, const string&) const {
	string res = "";
	if (in_brackets)
		res += "(";
	for (vector<int>::size_type i = 0; i + 1 < obj_ids.size(); ++i) {
		res += kf.get_full_attr_name_by_id(obj_ids[i], attr_ids[i]);
		res += " + ";
	}
	res += kf.get_full_attr_name_by_id(obj_ids.back(), attr_ids.back());
	res += " = ";
	res += value;
	if (in_brackets)
		res += ")";
	return res;
}

string true_condition_t::to_string(const knowledge_field_t&, bool in_brackets, const string&, const string&) const {
	return in_brackets ? "(1=1)" : "1=1";
}

string complex_action_t::to_string(const knowledge_field_t &kf) const {
	string res = "";
	for (vector<action_t*>::size_type i = 0; i + 1 < actions.size(); ++i)
		res += actions[i]->to_string(kf) + " " + op + "\n  ";
	res += actions.back()->to_string(kf);
	return res;
}

string temporal_object_t::dump() const {
	string res = "temporal_object_t ";

	res += std::to_string(id) + " ";
	res += name + " ";
	res += std::to_string(attrs.size()) + " ";
	for (vector<pair<string, string>>::size_type i = 0; i + 1 < attrs.size(); ++i)
		res += attrs[i].first + " " + attrs[i].second + " ";
	res += attrs.back().first + " " + attrs.back().second;

	return res;
}

void temporal_object_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void temporal_object_t::load(std::istream& in) {
	in >> id;
	in >> name;
	int cnt = 0;
	in >> cnt;
	for (int i = 0; i < cnt; ++i) {
		string name, type;
		in >> name >> type;
		name_to_id[name] = attrs.size();
		attrs.emplace_back(name, type);
	}
}

string condition_interval_duration_t::dump() const {
	string res = "condition_interval_duration_t ";

	res += op + " ";
	res += std::to_string(interval_id) + " ";
	res += std::to_string(duration_val);

	return res;
}

void condition_interval_duration_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void condition_interval_duration_t::load(std::istream& in) {
	in >> op;
	in >> interval_id;
	in >> duration_val;
}

string condition_sum_equal_const_t::dump() const {
	string res = "condition_sum_equal_const_t ";

	res += std::to_string(obj_ids.size()) + " ";
	for (vector<int>::size_type i = 0; i < obj_ids.size(); ++i)
		res += std::to_string(obj_ids[i]) + " " + std::to_string(attr_ids[i]) + " ";
	res += value;

	return res;
}

void condition_sum_equal_const_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void condition_sum_equal_const_t::load(std::istream& in) {
	int cnt;
	in >> cnt;
	for (int i = 0; i < cnt; ++i) {
		int obj_id, attr_id;
		in >> obj_id >> attr_id;
		obj_ids.emplace_back(obj_id);
		attr_ids.emplace_back(attr_id);
	}
	in >> value;
}

string condition_attr_value_const_t::dump() const {
	string res = "condition_attr_value_const_t ";

	res += op + " ";
	res += std::to_string(obj_id) + " ";
	res += std::to_string(attr_id) + " ";
	res += value;

	return res;
}

void condition_attr_value_const_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void condition_attr_value_const_t::load(std::istream &in) {
	in >> op;
	in >> obj_id;
	in >> attr_id;
	in >> value;
}

string condition_attr_value_attr_value_t::dump() const {
	string res = "condition_attr_value_attr_value_t ";
	res += op + " ";
	res += std::to_string(left_obj_id) + " ";
	res += std::to_string(left_attr_id) + " ";
	res += std::to_string(right_obj_id) + " ";
	res += std::to_string(right_attr_id) + " ";

	return res;
}

void condition_attr_value_attr_value_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void condition_attr_value_attr_value_t::load(std::istream& in) {
	in >> op;
	in >> left_obj_id;
	in >> left_attr_id;
	in >> right_obj_id;
	in >> right_attr_id;
}

string complex_condition_t::dump() const {
	string res = "complex_condition_t ";

	res += op + " ";
	res += std::to_string(data.size()) + " ";
	for (condition_t* ptr : data)
		res += ptr->dump() + " ";

	return res;
}

void complex_condition_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void complex_condition_t::load(std::istream& in) {
	in >> op;
	int cnt;
	in >> cnt;
	for (int i = 0; i < cnt; ++i)
		data.push_back(knowledge_field_t::load_condition(in));
}

string true_condition_t::dump() const {
	string res = "true_condition_t ";

	return res;
}

void true_condition_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void true_condition_t::load(std::istream&) {
}

string temporal_interval_t::dump() const {
	string res = "temporal_interval_t ";

	res += std::to_string(id) + " ";
	res += name + " ";
	res += begin_condition->dump() + " ";
	res += end_condition->dump();

	return res;
}

void temporal_interval_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void temporal_interval_t::load(std::istream &in) {
	in >> id;
	in >> name;
	begin_condition = knowledge_field_t::load_condition(in);
	end_condition = knowledge_field_t::load_condition(in);
}

string complex_action_t::dump() const {
	string res = "complex_action_t ";

	res += std::to_string(actions.size()) + " ";
	for (vector<action_t*>::size_type i = 0; i + 1 < actions.size(); ++i)
		res += actions[i]->dump() + " ";
	res += actions.back()->dump();

	return res;
}

void complex_action_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void complex_action_t::load(std::istream& in) {
	int cnt;
	in >> cnt;
	for (int i = 0; i < cnt; ++i)
		actions.push_back(knowledge_field_t::load_action(in));
}

string action_int_t::dump() const {
	string res = "action_int_t ";

	res += std::to_string(obj_id) + " ";
	res += std::to_string(attr_id) + " ";
	res += op + " ";
	res += std::to_string(value);

	return res;
}

void action_int_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void action_int_t::load(std::istream& in) {
	in >> obj_id;
	in >> attr_id;
	in >> op;
	in >> value;
}

string action_string_t::dump() const {
	string res = "action_string_t ";

	res += std::to_string(obj_id) + " ";
	res += std::to_string(attr_id) + " ";
	res += op + " ";
	res += value;

	return res;
}

void action_string_t::dump(std::ostream &out) const {
	out << dump() << std::endl;
}

void action_string_t::load(std::istream& in) {
	in >> obj_id;
	in >> attr_id;
	in >> op;
	in >> value;
}

string temporal_rule_t::dump() const {
	string res = "temporal_rule_t ";

	res += std::to_string(id) + " ";
	res += name + " ";
	res += condition->dump() + " ";
	res += action->dump() + " ";

	return res;
}

void temporal_rule_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void temporal_rule_t::load(std::istream &in) {
	in >> id;
	in >> name;
	condition = knowledge_field_t::load_condition(in);
	action = knowledge_field_t::load_action(in);
}

string knowledge_field_t::dump() const {
	string res = "";

	res += dump_objects() + " ";
	res += dump_intervals() + " ";
	res += dump_rules() + " ";
	res += dump_events() + " ";
	res += dump_types();

	return res;
}

void knowledge_field_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

string knowledge_field_t::dump_objects() const {
	string res = "";

	for (vector<temporal_object_t*>::size_type i = 0; i + 1 < objects.size(); ++i)
		res += objects[i]->dump() + " ";
	if (!objects.empty())
		res += objects.back()->dump();

	return res;
}

void knowledge_field_t::dump_objects(std::ostream& out) const {
	out << dump_objects() << std::endl;
}

string knowledge_field_t::dump_intervals() const {
	string res = "";

	for (vector<temporal_interval_t*>::size_type i = 0; i + 1 < intervals.size(); ++i)
		res += intervals[i]->dump() + " ";
	if (!intervals.empty())
		res += intervals.back()->dump();

	return res;
}

void knowledge_field_t::dump_intervals(std::ostream& out) const {
	out << dump_intervals() << std::endl;
}

string knowledge_field_t::dump_rules() const {
	string res = "";

	for (vector<temporal_rule_t*>::size_type i = 0; i + 1 < rules.size(); ++i)
		res += rules[i]->dump() + " ";
	if (!rules.empty())
		res += rules.back()->dump();

	return res;
}

void knowledge_field_t::dump_rules(std::ostream& out) const {
	out << dump_rules() << std::endl;
}

string knowledge_field_t::dump_events() const {
	string res = "";

	for (vector<temporal_event_t*>::size_type i = 0; i + 1 < events.size(); ++i)
		res += events[i]->dump() + " ";
	if (!events.empty())
		res += events.back()->dump();

	return res;
}

void knowledge_field_t::dump_events(std::ostream& out) const {
	out << dump_events() << std::endl;
}

string knowledge_field_t::dump_types() const {
	string res = "types ";
	res += std::to_string(type_ids.size()) + " ";
	for (const auto& it : type_ids) {
		const string& type_name = it.first;
		res += type_name + " ";
		res += std::to_string(it.second) + " ";
		auto iter = types.find(type_name);
		res += std::to_string(iter->second.size()) + " ";
		for (const string& value : iter->second)
			res += value + " ";
	}
	return res;
}

void knowledge_field_t::dump_types(std::ostream& out) const {
	out << dump_types() << std::endl;
}

void knowledge_field_t::load_types(std::istream& in) {
	int cnt;
	in >> cnt;
	for (int i = 0; i < cnt; ++i) {
		string type_name;
		in >> type_name;
		int id;
		in >> id;
		type_ids[type_name] = id;
		int cnt_values;
		in >> cnt_values;
		for (int j = 0; j < cnt_values; ++j) {
			string value;
			in >> value;
			types[type_name].insert(value);
		}
	}
}

condition_t* knowledge_field_t::load_condition(std::istream& in) {
	string type;
	in >> type;
	if (type == "condition_interval_duration_t") {
		condition_interval_duration_t* condition = new condition_interval_duration_t;
		condition->load(in);
		return condition;
	}
	else if (type == "condition_sum_equal_const_t") {
		condition_sum_equal_const_t* condition = new condition_sum_equal_const_t;
		condition->load(in);
		return condition;
	}
	else if (type == "condition_attr_value_const_t") {
		condition_attr_value_const_t* condition = new condition_attr_value_const_t;
		condition->load(in);
		return condition;
	}
	else if (type == "complex_condition_t") {
		complex_condition_t* condition = new complex_condition_t;
		condition->load(in);
		return condition;
	}
	else if (type == "true_condition_t") {
		true_condition_t* condition = new true_condition_t;
		condition->load(in);
		return condition;
	}
	else if (type == "event_interval_condition_t") {
		event_interval_condition_t* condition = new event_interval_condition_t;
		condition->load(in);
		return condition;
	}
	else if (type == "event_event_condition_t") {
		event_event_condition_t* condition = new event_event_condition_t;
		condition->load(in);
		return condition;
	}
	else if (type == "condition_attr_value_attr_value_t") {
		condition_attr_value_attr_value_t* condition = new condition_attr_value_attr_value_t;
		condition->load(in);
		return condition;
	}
	else {
		std::cerr << "No such condition: " << type << std::endl;
	}
	return nullptr;
}

action_t* knowledge_field_t::load_action(std::istream& in) {
	string type;
	in >> type;

	if (type == "complex_action_t") {
		complex_action_t* action = new complex_action_t;
		action->load(in);
		return action;
	}
	else if (type == "action_int_t") {
		action_int_t* action = new action_int_t;
		action->load(in);
		return action;
	}
	else if (type == "action_string_t") {
		action_string_t* action = new action_string_t;
		action->load(in);
		return action;
	}
	else {
		std::cerr << "No such action: " << type << std::endl;
	}

	return nullptr;
}

void knowledge_field_t::load(std::istream& in) {
	string type;

	while (in >> type) {
		if (type == "temporal_object_t") {
			temporal_object_t* object = new temporal_object_t(0);
			object->load(in);
			objects.push_back(object);
			mp_objects[object->get_name()] = object;
		}
		else if (type == "temporal_interval_t") {
			temporal_interval_t* interval = new temporal_interval_t(0);
			interval->load(in);
			intervals.push_back(interval);
			mp_intervals[interval->get_name()] = interval;
		}
		else if (type == "temporal_rule_t") {
			temporal_rule_t* rule = new temporal_rule_t(0);
			rule->load(in);
			rules.push_back(rule);
			mp_rules[rule->get_name()] = rule;
		}
		else if (type == "temporal_event_t") {
			temporal_event_t* event = new temporal_event_t(0);
			event->load(in);
			events.push_back(event);
			mp_events[event->get_name()] = event;
		}
		else if (type == "types") {
			load_types(in);
		}
		else {
			std::cerr << "No such entity: " << type << std::endl;
		}
	}
}

void knowledge_field_t::load(const string& s) {
	std::istringstream in(s);
	load(in);
}

const vector<temporal_object_t*>& knowledge_field_t::getObjects() const
{
	return objects;
}
const vector<temporal_interval_t*>& knowledge_field_t::getIntervals() const
{
	return intervals;
}
const vector<temporal_rule_t*>& knowledge_field_t::getRules() const
{
	return rules;
}

string temporal_event_t::to_string(const knowledge_field_t& kf) const {
	string res = "";
	res += "ИМЯ \"" + get_name() + "\"\n";
	res += "ГРУППА \"Событие\"\n";
	res += "АТРИБУТЫ\n";
	res += "  УслВозн: " + condition->to_string(kf, false) + "\n";
	res += "  Количество Возникновений: числовой\n";
	res += "  Активность: числовой\n";
	return res;
}

string temporal_event_t::dump() const {
	string res = "temporal_event_t ";

	res += std::to_string(id) + " ";
	res += name + " ";
	res += condition->dump() + " ";

	return res;
}

void temporal_event_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void temporal_event_t::load(std::istream& in) {
	in >> id;
	in >> name;
	condition = knowledge_field_t::load_condition(in);
}

string event_interval_condition_t::to_string(const knowledge_field_t& kf, bool in_brackets, const string&, const string&) const {
	string res = "";
	if (in_brackets)
		res += "(";
	res += kf.get_event_name_by_id(event_id);
	res += " " + op + " ";
	res += kf.get_interval_name_by_id(interval_id);
	if (in_brackets)
		res += ")";
	return res;
}

string event_interval_condition_t::dump() const {
	string res = "event_interval_condition_t ";

	res += std::to_string(event_id) + " ";
	res += op + " ";
	res += std::to_string(interval_id) + " ";

	return res;
}

void event_interval_condition_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void event_interval_condition_t::load(std::istream& in) {
	in >> event_id;
	in >> op;
	in >> interval_id;
}

string event_event_condition_t::to_string(const knowledge_field_t& kf, bool in_brackets, const string&, const string&) const {
	string res = "";
	if (in_brackets)
		res += "(";
	res += kf.get_event_name_by_id(left_event_id);
	res += " " + op + " ";
	res += kf.get_event_name_by_id(right_event_id);
	if (in_brackets)
		res += ")";
	return res;
}

string event_event_condition_t::dump() const {
	string res = "event_event_condition_t ";

	res += std::to_string(left_event_id) + " ";
	res += op + " ";
	res += std::to_string(right_event_id) + " ";

	return res;
}

void event_event_condition_t::dump(std::ostream& out) const {
	out << dump() << std::endl;
}

void event_event_condition_t::load(std::istream& in) {
	in >> left_event_id;
	in >> op;
	in >> right_event_id;
}

string knowledge_field_t::to_xml() const {
	string res = "";
	res += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<knowledge-base>\n";
	res += types_to_xml();
	res += "<classes>\n";
	res += objects_to_xml();
	res += rules_to_xml();
	res += "</class>\n";
	res += "</classes>\n";

	res += "\n\n\n";
	res += "<IntervalsAndEvents>\n";
	res += intervals_to_xml();
	res += events_to_xml();
	res += "</IntervalsAndEvents>\n";

	res += "</knowledge-base>";
	return res;
}

string knowledge_field_t::types_to_xml() const {
	string res = "";
	res += "<types>\n";
	res += "<type id=\"ТИП" + std::to_string(number_type_id) + "\" meta=\"number\" desc=\"number\">\n";
	res += "<from>" + std::to_string(number_from) + "</from>\n";
	res += "<to>" + std::to_string(number_to) + "</to>\n";
	res += "</type>\n";
	for (const auto& it : type_ids) {
		const string& type_name = it.first;
		res += "<type id=\"ТИП" + std::to_string(it.second) + "\" meta=\"string\" desc=\"string\">\n";
		auto iter = types.find(type_name);

		if (iter != types.end())
			for (const string& value : iter->second)
				res += "<value>" + value + "</value>\n";
		res += "</type>\n";
	}
	res += "</types>\n";
	return res;
}

string knowledge_field_t::objects_to_xml() const {
	string res = "";
	for (temporal_object_t* ptr : objects)
		res += ptr->to_xml(*this);
	res += "<class id=\"world\" desc=\"Класс верхнего уровня, включающий в себя экземпляры других классов и общие правила\">\n";
	res += "<properties>\n";
	for (temporal_object_t* ptr : objects)
		res += "<property id=\"" + ptr->get_name() + "\" type=\"" + ptr->get_name() + "\" source=\"none\" create=\"true\" desc=\"" + ptr->get_name() + "\"/>\n";
	res += "</properties>\n";
	return res;
}

string knowledge_field_t::intervals_to_xml() const {
	string res = "";
	res += "<Intervals>\n";
	for (temporal_interval_t* ptr : intervals)
		res += ptr->to_xml(*this);
	res += "</Intervals>\n";
	return res;
}

string knowledge_field_t::events_to_xml() const {
	string res = "";
	res += "<Events>\n";
	for (temporal_event_t* ptr : events)
		res += ptr->to_xml(*this);
	res += "</Events>\n";
	return res;
}

string knowledge_field_t::rules_to_xml() const {
	string res = "";
	res += "<rules>\n";
	for (temporal_rule_t* ptr : rules)
		if (ptr->is_temporal())
			res += ptr->to_xml(*this);
	for (temporal_rule_t* ptr : rules)
		if (!ptr->is_temporal())
			res += ptr->to_xml(*this);
	res += "</rules>\n";
	return res;
}

string temporal_object_t::to_xml(const knowledge_field_t& kf) const {
	string res = "";
	res += "<class id=\"" + get_name() + "\" desc=\"" + get_name() + "\">\n";
	res += "<properties>\n";
	for (const pair<string, string>& attr : attrs)

		res += "<property id=\"" + attr.first + "\" type=\"ТИП" + std::to_string(kf.get_type_id(attr.second)) + "\" desc=\"" + attr.first + "\" source=\"inferred\" />\n";
	res += "</properties>\n";
	res += "</class>\n";
	return res;
}

string temporal_interval_t::to_xml(const knowledge_field_t& kf) const {
	string res = "";
	res += "<Interval Name=\"" + get_name() + "\">\n";
	res += "<Open>\n";
	res += begin_condition->to_interval_xml(kf);
	res += "</Open>\n";
	res += "<Close>\n";
	res += end_condition->to_interval_xml(kf);
	res += "</Close>\n";
	res += "</Interval>\n";
	return res;
}

string temporal_event_t::to_xml(const knowledge_field_t& kf) const {
	string res = "";
	res += "<Event Name=\"" + get_name() + "\">\n";
	res += "<Formula>\n";
	res += condition->to_interval_xml(kf);
	res += "</Formula>\n";
	res += "</Event>\n";
	return res;
}

string temporal_rule_t::to_xml(const knowledge_field_t& kf) const {
	string res = "";
	res += "<rule id=\"" + (is_temporal() ? string("t") : string("b")) + std::to_string(get_id()) + "\" meta=\"simple\" desc=\"" + (is_temporal() ? string("Т") : string("В")) + "-правило\">\n";
	res += "<condition>\n";
	res += condition->to_rule_xml(kf);
	res += "</condition>\n";
	res += "<action>\n";
	res += action->to_xml(kf);
	res += "</action>\n";
	res += "</rule>\n";
	return res;
}

string condition_interval_duration_t::to_interval_xml(const knowledge_field_t&) const {
	return "ERROR! THIS MUST NOT BE CALLED! NO INTERVAL DURATION CONDITION IN INTERVAL START/END CONDITION!";
}

string condition_interval_duration_t::to_rule_xml(const knowledge_field_t&) const {
	return "ERROR! NOT IMPLEMENTED IN TEMPORAL REASONER! NO EXAMPLE OF XML!";
}

string condition_sum_equal_const_t::to_interval_xml(const knowledge_field_t&) const {
	return "ERROR! THIS MUST NOT BE CALLED! NO PLUS OPERATION IN INTERVAL START/END CONDITION!";
}

string condition_sum_equal_const_t::to_rule_xml(const knowledge_field_t& kf) const {
	string res = "";
	pair<string, string> obj_desc;
	obj_desc = kf.get_full_attr_name_by_id_as_pair(obj_ids.back(), attr_ids.back());
	res += "<ref id=\"" + obj_desc.first + "\">\n";
	res += "<ref id=\"" + obj_desc.second + "\"/>\n";
	res += "</ref>\n";
	for (int i = obj_ids.size() - 2; i >= 0; --i) {
		string buffer = "";
		buffer += "<sub>\n";
		buffer += "<value>\n0\n</value>\n";
		buffer += res;
		buffer += "</sub>\n";

		res = buffer;
		buffer = "";

		buffer = "<sub>\n";
		obj_desc = kf.get_full_attr_name_by_id_as_pair(obj_ids[i], attr_ids[i]);
		buffer += "<ref id=\"" + obj_desc.first + "\">\n";
		buffer += "<ref id=\"" + obj_desc.second + "\"/>\n";
		buffer += "</ref>\n";
		buffer += res;
		buffer += "</sub>\n";

		res = buffer;
	}
	res = "<eq>\n" + res;
	res += "<value>\n";
	res += value + "\n" + "\n";
	res += "</value>\n";
	res += "</eq>\n";
	return res;
}

string condition_attr_value_const_t::to_interval_xml(const knowledge_field_t& kf) const {
	string res = "";
	res += "<EqOp Value=\"" + get_text_operation() + "\">\n";
	res += "<Attribute Value=\"" + kf.get_full_attr_name_by_id(obj_id, attr_id) + "\"/>\n";
	res += "<String Value=\"" + value + "\"/>\n";
	res += "</EqOp>\n";
	return res;
}

string condition_attr_value_attr_value_t::to_interval_xml(const knowledge_field_t& kf) const {
	string res = "";
	res += "<EqOp Value=\"" + get_text_operation() + "\">\n";
	res += "<Attribute Value=\"" + kf.get_full_attr_name_by_id(left_obj_id, left_attr_id) + "\"/>\n";
	res += "<Attribute Value=\"" + kf.get_full_attr_name_by_id(right_obj_id, right_attr_id) + "\"/>\n";
	res += "</EqOp>\n";
	return res;
}

string condition_attr_value_const_t::to_rule_xml(const knowledge_field_t& kf) const {
	string res = "";
	res += "<" + get_text_operation() + ">\n";
	pair<string, string> obj_desc = kf.get_full_attr_name_by_id_as_pair(obj_id, attr_id);
	res += "<ref id=\"" + obj_desc.first + "\">\n";
	res += "<ref id=\"" + obj_desc.second + "\"/>\n";
	res += "</ref>\n";
	res += "<value>" + value + "</value>\n";
	res += "</" + get_text_operation() + ">\n";
	return res;
}

string condition_attr_value_attr_value_t::to_rule_xml(const knowledge_field_t& kf) const {
	string res = "";
	res += "<" + get_text_operation() + ">\n";
	pair<string, string> obj_desc = kf.get_full_attr_name_by_id_as_pair(left_obj_id, left_attr_id);
	res += "<ref id=\"" + obj_desc.first + "\">\n";
	res += "<ref id=\"" + obj_desc.second + "\"/>\n";
	res += "</ref>\n";
	obj_desc = kf.get_full_attr_name_by_id_as_pair(right_obj_id, right_attr_id);
	res += "<ref id=\"" + obj_desc.first + "\">\n";
	res += "<ref id=\"" + obj_desc.second + "\"/>\n";
	res += "</ref>\n";
	res += "</" + get_text_operation() + ">\n";
	return res;
}

string event_interval_condition_t::to_interval_xml(const knowledge_field_t&) const {
	return "ERROR! THIS MUST NOT BE CALLED! NO TEMPORAL CONDITION IN IN INTERVAL START/END CONDITION!";
}

string event_interval_condition_t::to_rule_xml(const knowledge_field_t& kf) const {
	string res = "";
	res += "<EvIntRel Value=\"" + get_text_operation() + "\">\n";
	res += "<Event Name=\"" + kf.get_event_name_by_id(event_id) + "\"/>\n";
	res += "<Interval Name=\"" + kf.get_interval_name_by_id(interval_id) + "\"/>\n";
	res += "</EvIntRel>\n";
	return res;
}

string event_event_condition_t::to_interval_xml(const knowledge_field_t&) const {
	return "ERROR! THIS MUST NOT BE CALLED! NO TEMPORAL CONDITION IN IN INTERVAL START/END CONDITION!";
}

string event_event_condition_t::to_rule_xml(const knowledge_field_t& kf) const {
	string res = "";
	res += "<EvRel Value\"" + get_text_operation() + "\">\n";
	res += "<Event Name=\"" + kf.get_event_name_by_id(left_event_id) + "\"/>\n";
	res += "<Event Name=\"" + kf.get_event_name_by_id(right_event_id) + "\"/>\n";
	res += "</EvRel>\n";
	return res;
}

string complex_condition_t::to_interval_xml(const knowledge_field_t& kf) const {
	string res = "";
	if (get_text_operation() == "and")
		res += "<LogOp Value=\"AND\">\n";
	else
		res += "<" + get_text_operation() + ">\n";
	for (condition_t* ptr : data)
		res += ptr->to_interval_xml(kf);
	if (get_text_operation() == "and")
		res += "</LogOp>\n";
	else
		res += "</" + get_text_operation() + ">\n";
	return res;
}

string complex_condition_t::to_rule_xml(const knowledge_field_t& kf) const {
	string res = "";
	if (data.size() > 1)
		res += "<" + get_text_operation() + ">\n";
	for (condition_t* ptr : data)
		res += ptr->to_rule_xml(kf);
	if (data.size() > 1)
		res += "</" + get_text_operation() + ">\n";
	return res;
}

string true_condition_t::to_interval_xml(const knowledge_field_t&) const {
	return "ERROR! THIS MUST NOT BE CALLED! NO TRUE CONDITION IN INTERVAL START/END CONDITION!";
}

string true_condition_t::to_rule_xml(const knowledge_field_t&) const {
	string res = "";
	res += "<eq>\n";
	res += "<value>1</value>\n";
	res += "<value>1</value>\n";
	res += "</eq>\n";
	return res;
}

string complex_action_t::to_xml(const knowledge_field_t& kf) const {
	string res = "";
	for (action_t* ptr : actions)
		res += ptr->to_xml(kf);
	return res;
}

string action_int_t::to_xml(const knowledge_field_t& kf) const {
	string res = "";
	res += "<assign>\n";
	pair<string, string> obj_desc = kf.get_full_attr_name_by_id_as_pair(obj_id, attr_id);
	res += "<ref id=\"" + obj_desc.first + "\">\n";
	res += "<ref id=\"" + obj_desc.second + "\"/>\n";
	res += "</ref>\n";
	if (op == "=") {
		res += "<value>" + std::to_string(value) + "</value>\n";
	}
	else if (op == "+=") {
		res += "<sub>\n";
		res += "<ref id=\"" + obj_desc.first + "\">\n";
		res += "<ref id=\"" + obj_desc.second + "\"/>\n";
		res += "</ref>\n";
		res += "<value>\n";
		if (value >= 0)
			res += "-";
		res += std::to_string(value) + "\n";
		res += "</value>\n";
		res += "</sub>\n";
	}
	res += "<with belief=\"50\" probability=\"100\" accuracy=\"0\"/>\n";
	res += "</assign>\n";
	return res;
}

string action_string_t::to_xml(const knowledge_field_t& kf) const {
	string res = "";
	res += "<assign>\n";
	pair<string, string> obj_desc = kf.get_full_attr_name_by_id_as_pair(obj_id, attr_id);
	res += "<ref id=\"" + obj_desc.first + "\">\n";
	res += "<ref id=\"" + obj_desc.second + "\"/>\n";
	res += "</ref>";
	res += "<value>" + value + "</value>\n";
	res += "<with belief=\"50\" probability=\"100\" accuracy=\"0\"/>\n";
	res += "</assign>\n";
	return res;
}

bool condition_interval_duration_t::is_temporal() const {
	return false;
}

bool condition_sum_equal_const_t::is_temporal() const {
	return false;
}

bool condition_attr_value_const_t::is_temporal() const {
	return false;
}

bool condition_attr_value_attr_value_t::is_temporal() const {
	return false;
}

bool event_interval_condition_t::is_temporal() const {
	return true;
}

bool event_event_condition_t::is_temporal() const {
	return true;
}

bool complex_condition_t::is_temporal() const {
	bool res = false;
	for (condition_t* ptr : data)
		res |= ptr->is_temporal();
	return res;
}

bool true_condition_t::is_temporal() const {
	return false;
}

void knowledge_field_t::add_type(const string& type_name) {
	if (type_ids.find(type_name) == type_ids.end()) {
		int id = type_ids.size() + 2;
		type_ids[type_name] = id;
	}
}

void knowledge_field_t::add_type_value(const string& type_name, const string& value) {
	types[type_name].insert(value);
}

int knowledge_field_t::get_type_id(const string& type_name) const {
	if (type_name == "числовой")
		return number_type_id;
	auto iter = type_ids.find(type_name);
	if (iter == type_ids.end())
		return 0;
	return iter->second;
}

vector<string> knowledge_field_t::get_all_types() const {
	vector<string> res;
	for (auto iter = types.begin(); iter != types.end(); ++iter)
		res.emplace_back(iter->first);
	return res;
}

set<string> knowledge_field_t::get_type_values(const string& tname) const {
	set<string> res;
	auto iter = types.find(tname);
	if (iter != types.end()) {
		for (const string& value : iter->second)
			res.insert(value);
	}
	return res;
}

int knowledge_field_t::get_number_of_objects() const {
	return objects.size();
}

int knowledge_field_t::get_number_of_events() const {
	return events.size();
}

int knowledge_field_t::get_number_of_intervals() const {
	return intervals.size();
}

int knowledge_field_t::get_number_of_rules() const {
	return rules.size();
}

const condition_t* temporal_event_t::get_condition() const {
	return condition;
}
