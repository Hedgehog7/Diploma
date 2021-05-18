#ifndef __MERGER_T_H__
#define __MERGER_T_H__

#include "knowledge_field_t.h"

struct fact_t {
  int object_id, attribute_id;
  string value;

  fact_t(int object_id, int attribute_id, const string& value) : object_id(object_id), attribute_id(attribute_id), value(value) {
  }

  bool operator <(const fact_t& it) const;
};

using dur_fact_t = pair<int, int>;

struct ee_fact_t {
  int left_event_id, right_event_id;
  string op;

  ee_fact_t(int left_event_id, int right_event_id, const string& op) : left_event_id(left_event_id), right_event_id(right_event_id), op(op) {
  }

  bool operator <(const ee_fact_t& it) const;
};

struct ei_fact_t {
  int event_id, interval_id;
  string op;

  ei_fact_t(int event_id, int interval_id, const string& op) : event_id(event_id), interval_id(interval_id), op(op) {
  }

  bool operator <(const ei_fact_t& it) const;
};

class merger_t {
  private:
    enum class step {
      nothing,
      events,
      intervals,
      rules
    };

    const knowledge_field_t& left;
    const knowledge_field_t& right;
    knowledge_field_t res;

    map<int, int> left_object_id_in_res, right_object_id_in_res;
    map<int, int> left_event_id_in_res, right_event_id_in_res;
    map<int, int> left_interval_id_in_res, right_interval_id_in_res;

    step current_step;
    map<fact_t, int> facts;
    //interval_dur
    map<dur_fact_t, int> dur_facts;
    //event-event
    map<ee_fact_t, int> ee_facts;
    //event-interval
    map<ei_fact_t, int> ei_facts;
    vector<vector<vector<int>>> table;
    vector<vector<int>> table_second_col;
    vector<vector<vector<double>>> dist_table;

    inline bool is_equal_names(const string& a, const string& b) const {
      return a == b;
    }

    const temporal_entity_t* add_object(const temporal_object_t* it, const string& pref = "");
    condition_t* copy_condition(const condition_t* it, map<int, int>& object_id_in_res, map<int, int>& event_id_in_res, map<int, int>& interval_id_in_res, const knowledge_field_t& kf);
    action_t* copy_action(const action_t* it, map<int, int>& objects_id_in_res, const knowledge_field_t& kf);

    void add_fact(const condition_t* it, int id, int size, int cols, int col);
    void add_fact(const action_t* it, int id, int size, int cols, int col);

  public:
    merger_t(const knowledge_field_t& left, const knowledge_field_t& right);

    bool merge_types(string& log);
    void merge_objects(string& log);
    void merge_events(string& log);
    void merge_intervals(string& log);
    void merge_rules(string& log);

    string res_to_string() const;
    string res_to_xml() const;
    string res_to_inner() const;

    void show_rtr(std::ostream& out) const;
    void show_dists(std::ostream& out) const;
};

#endif
