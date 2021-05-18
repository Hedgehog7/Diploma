#ifndef __KNOWLEDGE_FIELD_T_H__
#define __KNOWLEDGE_FIELD_T_H__

#include <vector>
#include <string>
#include <map>
#include <set>
#include <iostream>

using std::vector;
using std::string;
using std::pair;
using std::map;
using std::set;

class knowledge_field_t;

class temporal_entity_t {
protected:
	string name = "";
	int id;

public:
	virtual ~temporal_entity_t() {
	}

	temporal_entity_t(int id) : id(id) {
	}

	inline void set_name(const string& name) {
		this->name = name;
	}

	inline string get_name() const {
		return name;
	}

	inline int get_id() const {
		return id;
	}

	virtual string to_string(const knowledge_field_t& kf) const = 0;
	virtual string dump() const = 0;
	virtual void dump(std::ostream& out) const = 0;
	virtual void load(std::istream& in) = 0;
	virtual string to_xml(const knowledge_field_t& kf) const = 0;
};

class temporal_object_t : public temporal_entity_t {
	vector<pair<string, string>> attrs;
	map<string, int> name_to_id;

public:
	virtual ~temporal_object_t() {
	}

	temporal_object_t(int id) : temporal_entity_t(id) {
	}

	int add_attr(const string& attr_name, const string& attr_type);

	int get_number_of_attrs() const;
	int get_attr_id_by_name(const string& name) const;
	string get_attr_name_by_id(int id) const;
	string get_attr_type_by_id(int id) const;
	virtual string to_string(const knowledge_field_t&) const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_xml(const knowledge_field_t& kf) const override;
};

class condition_t {
protected:
	string op;

public:
	virtual ~condition_t() {
	}

	inline void set_op(const string& op) {
		this->op = op;
	}

	inline string get_op() const {
		return op;
	}

	inline string get_text_operation() const {
		if (op == "<")
			return "lt";
		else if (op == "<=")
			return "le";
		else if (op == "=")
			return "eq";
		else if (op == ">=")
			return "ge";
		else if (op == ">")
			return "gt";
		else if (op == "&")
			return "and";
		else if (op == "|")
			return "or";
		else if (op == "!")
			return "not";
		else
			return op;
	}

	virtual string to_string(const knowledge_field_t& kf, bool in_brackets, const string& left_delimeter = " ", const string& right_delimeter = " ") const = 0;
	virtual string dump() const = 0;
	virtual void dump(std::ostream& out) const = 0;
	virtual void load(std::istream& in) = 0;
	virtual string to_interval_xml(const knowledge_field_t& kf) const = 0;
	virtual string to_rule_xml(const knowledge_field_t& kf) const = 0;
	virtual bool is_temporal() const = 0;
};

class condition_interval_duration_t : public condition_t {
private:
	int interval_id;
	int duration_val;

public:
	virtual ~condition_interval_duration_t() {
	}

	condition_interval_duration_t() {
	}

	condition_interval_duration_t(condition_interval_duration_t* ptr) : interval_id(ptr->get_interval_id()), duration_val(ptr->get_duration_val()) {
		set_op(ptr->get_op());
	}

	inline void set_duration_val(int val) {
		duration_val = val;
	}

	inline int get_duration_val() const {
		return duration_val;
	}

	inline void set_interval_id(int id) {
		interval_id = id;
	}

	inline int get_interval_id() const {
		return interval_id;
	}

	virtual string to_string(const knowledge_field_t& kf, bool in_brackets, const string& = " ", const string& = " ") const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_interval_xml(const knowledge_field_t&) const override;
	virtual string to_rule_xml(const knowledge_field_t&) const override;
	virtual bool is_temporal() const override;
};

class condition_sum_equal_const_t : public condition_t {
private:
	vector<int> obj_ids;
	vector<int> attr_ids;
	string value;

public:
	virtual ~condition_sum_equal_const_t() {
	}

	inline void add_item(int obj_id, int attr_id) {
		obj_ids.push_back(obj_id);
		attr_ids.push_back(attr_id);
	}

	inline void set_value(const string& it) {
		value = it;
	}

	inline string get_value() const {
		return value;
	}

	inline int get_term_count() const {
		return obj_ids.size();
	}

	inline pair<int, int> get_term(const int id) const {
		return{ obj_ids[id], attr_ids[id] };
	}

	virtual string to_string(const knowledge_field_t& kf, bool in_brackets, const string& left_delimeter = " ", const string& right_delimeter = " ") const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_interval_xml(const knowledge_field_t&) const override;
	virtual string to_rule_xml(const knowledge_field_t& kf) const override;
	virtual bool is_temporal() const override;
};

class condition_attr_value_const_t : public condition_t {
private:
	int obj_id;
	int attr_id;
	string value;

public:
	virtual ~condition_attr_value_const_t() {
	}

	condition_attr_value_const_t() {
	}

	condition_attr_value_const_t(int obj_id, int attr_id, const string& value) : obj_id(obj_id), attr_id(attr_id), value(value) {
	}

	inline void set_value(const string& value) {
		this->value = value;
	}

	inline string get_value() const {
		return value;
	}

	inline void set_obj_id(int id) {
		obj_id = id;
	}

	inline void set_attr_id(int id) {
		attr_id = id;
	}

	inline int get_obj_id() const {
		return obj_id;
	}

	inline int get_attr_id() const {
		return attr_id;
	}

	virtual string to_string(const knowledge_field_t& kf, bool in_brackets, const string& left_delimeter = " ", const string& right_delimeter = " ") const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_interval_xml(const knowledge_field_t& kf) const override;
	virtual string to_rule_xml(const knowledge_field_t& kf) const override;
	virtual bool is_temporal() const override;
};

class condition_attr_value_attr_value_t : public condition_t {
private:
	int left_obj_id;
	int left_attr_id;
	int right_obj_id;
	int right_attr_id;

public:
	virtual ~condition_attr_value_attr_value_t() {
	}

	condition_attr_value_attr_value_t() {
	}

	condition_attr_value_attr_value_t(int left_obj_id, int left_attr_id, int right_obj_id, int right_attr_id) : left_obj_id(left_obj_id), left_attr_id(left_attr_id), right_obj_id(right_obj_id), right_attr_id(right_attr_id) {
	}

	inline void set_left_obj_id(int id) {
		left_obj_id = id;
	}

	inline void set_left_attr_id(int id) {
		left_attr_id = id;
	}

	inline void set_right_obj_id(int id) {
		right_obj_id = id;
	}

	inline void set_right_attr_id(int id) {
		right_attr_id = id;
	}

	inline int get_left_obj_id() const {
		return left_obj_id;
	}

	inline int get_left_attr_id() const {
		return left_attr_id;
	}

	inline int get_right_obj_id() const {
		return right_obj_id;
	}

	inline int get_right_attr_id() const {
		return right_attr_id;
	}

	virtual string to_string(const knowledge_field_t& kf, bool in_brackets, const string& left_delimeter = " ", const string& right_delimeter = " ") const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_interval_xml(const knowledge_field_t& kf) const override;
	virtual string to_rule_xml(const knowledge_field_t& kf) const override;
	virtual bool is_temporal() const override;
};

class event_interval_condition_t : public condition_t {
private:
	int event_id;
	int interval_id;

public:
	virtual ~event_interval_condition_t() {
	}

	event_interval_condition_t() {
	}

	event_interval_condition_t(int event_id, int interval_id) : event_id(event_id), interval_id(interval_id) {
	}

	event_interval_condition_t(event_interval_condition_t* ptr) : event_id(ptr->event_id), interval_id(ptr->interval_id) {
		set_op(ptr->get_op());
	}

	inline void set_event_id(int id) {
		event_id = id;
	}

	inline void set_interval_id(int id) {
		interval_id = id;
	}

	inline int get_event_id() const {
		return event_id;
	}

	inline int get_interval_id() const {
		return interval_id;
	}

	virtual string to_string(const knowledge_field_t& kf, bool in_brackets, const string& left_delimeter = " ", const string& right_delimeter = " ") const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_interval_xml(const knowledge_field_t&) const override;
	virtual string to_rule_xml(const knowledge_field_t& kf) const override;
	virtual bool is_temporal() const override;
};

class event_event_condition_t : public condition_t {
private:
	int left_event_id;
	int right_event_id;

public:
	virtual ~event_event_condition_t() {
	}

	event_event_condition_t() {
	}

	event_event_condition_t(int left_event_id, int right_event_id) : left_event_id(left_event_id), right_event_id(right_event_id) {
	}

	event_event_condition_t(event_event_condition_t* ptr) : left_event_id(ptr->left_event_id), right_event_id(ptr->right_event_id) {
		set_op(ptr->get_op());
	}

	inline void set_left_event_id(int id) {
		left_event_id = id;
	}

	inline void set_right_event_id(int id) {
		right_event_id = id;
	}

	inline int get_left_event_id() const {
		return left_event_id;
	}

	inline int get_right_event_id() const {
		return right_event_id;
	}

	virtual string to_string(const knowledge_field_t& kf, bool in_brackets, const string& left_delimeter = " ", const string& right_delimeter = " ") const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_interval_xml(const knowledge_field_t&) const override;
	virtual string to_rule_xml(const knowledge_field_t& kf) const override;
	virtual bool is_temporal() const override;
};

class complex_condition_t : public condition_t {
private:
	vector<condition_t*> data;

public:
	virtual ~complex_condition_t() {
		for (condition_t* ptr : data)
			delete ptr;
	}

	inline void add_condition(condition_t* newbie) {
		data.push_back(newbie);
	}

	inline int get_number_of_conditions() const {
		return data.size();
	}

	const condition_t* get_condition(int id) const {
		return data[id];
	}

	virtual string to_string(const knowledge_field_t& kf, bool in_brackets, const string& left_delimeter = " ", const string& right_delimeter = " ") const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_interval_xml(const knowledge_field_t& kf) const override;
	virtual string to_rule_xml(const knowledge_field_t& kf) const override;
	virtual bool is_temporal() const override;
};

class true_condition_t : public condition_t {
public:
	virtual ~true_condition_t() {
	}

	virtual string to_string(const knowledge_field_t& kf, bool in_brackets, const string& left_delimeter = " ", const string& right_delimeter = " ") const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream &in) override;
	virtual string to_interval_xml(const knowledge_field_t&) const override;
	virtual string to_rule_xml(const knowledge_field_t& kf) const override;
	virtual bool is_temporal() const override;
};

class temporal_interval_t : public temporal_entity_t {
private:
	condition_t* begin_condition = nullptr;
	condition_t* end_condition = nullptr;
	condition_t* duration_condition = nullptr;

public:
	virtual ~temporal_interval_t() {
		if (begin_condition)
			delete begin_condition;
		if (end_condition)
			delete end_condition;
	}

	temporal_interval_t(int id) : temporal_entity_t(id) {
	}

	const condition_t* get_begin_condition() const {
		return begin_condition;
	}

	const condition_t* get_end_condition() const {
		return end_condition;
	}

	virtual string to_string(const knowledge_field_t &kf) const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream &in) override;
	virtual string to_xml(const knowledge_field_t& kf) const override;
	void set_start_condition(condition_t* start);
	void set_end_condition(condition_t* end);
	void set_duration_condition(condition_t* end);
};

class action_t {
protected:
	int obj_id, attr_id;
	string op;

public:
	virtual ~action_t() {
	}

	inline void set_op(const string& op) {
		this->op = op;
	}

	inline void set_object_id(int obj_id) {
		this->obj_id = obj_id;
	}

	inline void set_attribute_id(int attr_id) {
		this->attr_id = attr_id;
	}

	inline int get_obj_id() const {
		return obj_id;
	}

	inline int get_attr_id() const {
		return attr_id;
	}

	inline string get_op() const {
		return op;
	}

	virtual string to_string(const knowledge_field_t& kf) const = 0;
	virtual string dump() const = 0;
	virtual void dump(std::ostream& out) const = 0;
	virtual void load(std::istream& in) = 0;
	virtual string to_xml(const knowledge_field_t& kf) const = 0;
};

class complex_action_t : public action_t {
private:
	vector<action_t*> actions;

public:
	virtual ~complex_action_t() {
		for (action_t* ptr : actions)
			delete ptr;
	}

	inline void add_action(action_t* action) {
		actions.push_back(action);
	}

	inline int get_number_of_actions() const {
		return actions.size();
	}

	inline const action_t* get_action(int id) const {
		return actions[id];
	}

	virtual string to_string(const knowledge_field_t &kf) const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_xml(const knowledge_field_t& kf) const override;
};

class action_int_t : public action_t {
private:
	int value;

public:
	virtual ~action_int_t() {
	}

	inline void set_value(int value) {
		this->value = value;
	}

	inline int get_value() const {
		return value;
	}

	virtual string to_string(const knowledge_field_t& kf) const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_xml(const knowledge_field_t& kf) const override;
};

class action_string_t : public action_t {
private:
	string value;

public:
	virtual ~action_string_t() {
	}

	inline void set_value(const string& value) {
		this->value = value;
	}

	inline string get_value() const {
		return value;
	}

	virtual string to_string(const knowledge_field_t &kf) const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_xml(const knowledge_field_t& kf) const override;
};

class temporal_rule_t : public temporal_entity_t {
private:
	string type = "Обычное";
	condition_t* condition = nullptr;
	action_t* action = nullptr;

public:
	virtual ~temporal_rule_t() {
		if (condition)
			delete condition;
		if (action)
			delete action;
	}

	temporal_rule_t(int id) : temporal_entity_t(id) {
	}

	inline const condition_t* get_condition() const {
		return condition;
	}

	inline const action_t* get_action() const {
		return action;
	}

	inline void add_condition(condition_t* ptr) {
		if (condition)
			delete condition;
		condition = ptr;
	}

	inline void add_action(action_t* ptr) {
		if (action)
			delete action;
		action = ptr;
	}

	virtual string to_string(const knowledge_field_t& kf) const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_xml(const knowledge_field_t& kf) const override;
	inline bool is_temporal() const { return (condition ? condition->is_temporal() : false); }
};

class temporal_event_t : public temporal_entity_t {
private:
	condition_t* condition = nullptr;

public:
	temporal_event_t(int id) : temporal_entity_t(id) {
	}

	inline void add_condition(condition_t* ptr) {
		if (condition)
			delete condition;
		condition = ptr;
	}

	const condition_t* get_condition() const;

	virtual string to_string(const knowledge_field_t& kf) const override;
	virtual string dump() const override;
	virtual void dump(std::ostream& out) const override;
	virtual void load(std::istream& in) override;
	virtual string to_xml(const knowledge_field_t& kf) const override;
};

class knowledge_field_t {
private:
	vector<temporal_object_t*> objects;
	vector<temporal_interval_t*> intervals;
	vector<temporal_rule_t*> rules;
	vector<temporal_event_t*> events;

	map<string, temporal_object_t*> mp_objects;
	map<string, temporal_interval_t*> mp_intervals;
	map<string, temporal_rule_t*> mp_rules;
	map<string, temporal_event_t*> mp_events;

	map<string, int> type_ids;
	map<string, set<string>> types;

	int number_type_id = 1;
	int number_from = 0;
	int number_to = 100000;

public:
	knowledge_field_t() {
	}

	~knowledge_field_t() {
		for (temporal_object_t* ptr : objects)
			delete ptr;
		for (temporal_interval_t* ptr : intervals)
			delete ptr;
		for (temporal_rule_t* ptr : rules)
			delete ptr;
		for (temporal_event_t* ptr : events)
			delete ptr;
	}

	const temporal_entity_t* add_object(const string& name);
	const temporal_entity_t* add_interval(const string& name);
	const temporal_entity_t* add_rule(const string& name);
	const temporal_entity_t* add_event(const string& name);
	void add_type(const string& type_name);
	void add_type_value(const string& type_name, const string& value);
	int get_type_id(const string& type_name) const;
	void add_attr(const temporal_entity_t* it, const string& attr_name, const string& attr_type) const;
	void set_start_condition(const temporal_entity_t* interval, condition_t* start) const;
	void set_end_condition(const temporal_entity_t* interval, condition_t* end) const;
	void set_duration_condition(const temporal_entity_t * interval, condition_t * duration) const;
	void add_condition(const temporal_entity_t* entity, condition_t* condition) const;
	void add_action(const temporal_entity_t* rule, action_t* action) const;
	string get_full_attr_name_by_id(int obj_id, int attr_id) const;
	pair<string, string> get_full_attr_name_by_id_as_pair(int obj_id, int attr_id) const;
	int get_object_id_by_name(const string& name) const;
	int get_interval_id_by_name(const string& name) const;
	string get_interval_name_by_id(int id) const;
	int get_event_id_by_name(const string& name) const;
	string get_event_name_by_id(int id) const;
	const temporal_object_t* get_object(const string& name) const;
	const temporal_object_t* get_object(const int id) const;
	const temporal_event_t* get_event(const int id) const;
	const temporal_interval_t* get_interval(const int id) const;
	const temporal_rule_t* get_rule(const int id) const;

	const vector<temporal_object_t*>& getObjects() const;
	const vector<temporal_interval_t*>& getIntervals() const;
	const vector<temporal_rule_t*>& getRules() const;

	string to_string() const;
	string objects_to_string() const;
	string intervals_to_string() const;
	string rules_to_string() const;
	string events_to_string() const;
	string types_to_string() const;

	string dump() const;
	string dump_objects() const;
	string dump_intervals() const;
	string dump_rules() const;
	string dump_events() const;
	string dump_types() const;

	void dump(std::ostream& out) const;
	void dump_objects(std::ostream& out) const;
	void dump_intervals(std::ostream& out) const;
	void dump_rules(std::ostream& out) const;
	void dump_events(std::ostream& out) const;
	void dump_types(std::ostream& out) const;

	void load(const string& s);
	void load(std::istream& in);

	void load_types(std::istream& in);

	string to_xml() const;
	string objects_to_xml() const;
	string intervals_to_xml() const;
	string events_to_xml() const;
	string rules_to_xml() const;
	string types_to_xml() const;

	static condition_t* load_condition(std::istream& in);
	static action_t* load_action(std::istream& in);

	vector<string> get_all_types() const;
	set<string> get_type_values(const string& tname) const;

	int get_number_of_objects() const;
	int get_number_of_events() const;
	int get_number_of_intervals() const;
	int get_number_of_rules() const;
};

#endif
