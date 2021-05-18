#include "merger_t.h"

bool fact_t::operator <(const fact_t& it) const {
  return (object_id < it.object_id) ||
      (object_id == it.object_id && attribute_id < it.attribute_id) ||
      (object_id == it.object_id && attribute_id == it.attribute_id && value < it.value);
}

bool ee_fact_t::operator <(const ee_fact_t& it) const {
  return (left_event_id < it.left_event_id) ||
      (left_event_id == it.left_event_id && right_event_id < it.right_event_id) ||
      (left_event_id == it.left_event_id && right_event_id == it.right_event_id && op < it.op);
}

bool ei_fact_t::operator <(const ei_fact_t& it) const {
  return (event_id < it.event_id) ||
      (event_id == it.event_id && interval_id < it.interval_id) ||
      (event_id == it.event_id && interval_id == it.interval_id && op < it.op);
}

merger_t::merger_t(const knowledge_field_t& left, const knowledge_field_t& right) : left(left), right(right) {
  std::cout << this->left.to_string() << std::endl;
  std::cout << this->right.to_string() << std::endl;
  current_step = step::nothing;
  res = knowledge_field_t();
}

string merger_t::res_to_inner() const {
  return res.dump();
}

string merger_t::res_to_string() const {
  return res.to_string();
}

string merger_t::res_to_xml() const {
  return res.to_xml();
}

bool merger_t::merge_types(string& log) {
  map<string, set<string>> left_types;
  vector<string> typelist = left.get_all_types();
  for (const string& tname : typelist)
    left_types[tname] = left.get_type_values(tname);

  map<string, set<string>> right_types;
  typelist = right.get_all_types();
  for (const string& tname : typelist)
    right_types[tname] = right.get_type_values(tname);

  map<string, set<string>> res_types;

  for (const pair<string, set<string>>& it : left_types) {
    res_types[it.first] = set<string>();
    for (const string& value : it.second)
      res_types[it.first].insert(value);
  }

  for (const pair<string, set<string>>& it_right : right_types) {
    map<string, set<string>>::iterator res_iter;
    for (res_iter = res_types.begin(); res_iter != res_types.end(); ++res_iter)
      if (is_equal_names(it_right.first, res_iter->first))
        break;

    if (res_iter == res_types.end())
      continue;

    //right is subset of res
    bool is_subset = true;
    for (const string& value : it_right.second)
      if (res_iter->second.find(value) == res_iter->second.end())
        is_subset = false;

    if (is_subset)
      continue;

    is_subset = true;
    //res is subset of right
    for (const string& value : res_iter->second)
      if (it_right.second.find(value) == it_right.second.end())
        is_subset = false;

    if (!is_subset) {
      log += "Тип " + res_iter->first + " в первом фрагменте ПЗ и тип " + it_right.first + " во втором фрагменте ПЗ совпадают по имени, но объединение их множества значений не совпадает ни с одним из типов\nНеобходимо сменить именование типов или их область значений в фрагментах ПЗ\n";
      return false;
    }

    for (const string& value : it_right.second)
      res_iter->second.insert(value);
  }

  for (const pair<string, set<string>>& it : res_types) {
    res.add_type(it.first);
    for (const string& value : it.second)
      res.add_type_value(it.first, value);
  }

  return true;
}

const temporal_entity_t* merger_t::add_object(const temporal_object_t* it, const string& pref) {
  const temporal_entity_t* obj = res.add_object(it->get_name() + pref);
  for (int attr_id = 0; attr_id < it->get_number_of_attrs(); ++attr_id)
    res.add_attr(obj, it->get_attr_name_by_id(attr_id), it->get_attr_type_by_id(attr_id));
  return obj;
}

void merger_t::merge_objects(string& log) {
  //предполагается, что для каждого объекта в одном фрагменте ПЗ есть только один синонимичный объект во втором фрагменте ПЗ
  //в одном фрагменте ПЗ нет синонимичных объектов
  set<int> used;
  for (int left_id = 0; left_id < left.get_number_of_objects(); ++left_id) {
    const temporal_object_t* l = left.get_object(left_id);
    int right_id = 0;
    for (; right_id < right.get_number_of_objects(); ++right_id)
      if (is_equal_names(l->get_name(), right.get_object(right_id)->get_name()))
        break;

    if (right_id == right.get_number_of_objects()) {
      left_object_id_in_res[left_id] = add_object(l)->get_id();
      continue;
    }

    used.insert(right_id);
    set<pair<string, string>> left_attrs, right_attrs;
    for (int attr_id = 0; attr_id < l->get_number_of_attrs(); ++attr_id)
      left_attrs.emplace(l->get_attr_name_by_id(attr_id), l->get_attr_type_by_id(attr_id));

    const temporal_object_t* r = right.get_object(right_id);
    for (int attr_id = 0; attr_id < r->get_number_of_attrs(); ++attr_id)
      right_attrs.emplace(r->get_attr_name_by_id(attr_id), r->get_attr_type_by_id(attr_id));

    //left object is subset of right
    bool is_subset = true;
    for (const pair<string, string>& it : left_attrs)
      if (right_attrs.find(it) == right_attrs.end())
        is_subset = false;
    if (is_subset) {
      int id = add_object(r)->get_id();
      left_object_id_in_res[left_id] = id;
      right_object_id_in_res[right_id] = id;
      continue;
    }

    //right object is subset of left
    is_subset = true;
    for (const pair<string, string>& it : right_attrs)
      if (left_attrs.find(it) == left_attrs.end())
        is_subset = false;
    if (is_subset) {
      int id = add_object(l)->get_id();
      left_object_id_in_res[left_id] = id;
      right_object_id_in_res[right_id] = id;
      continue;
    }

    left_object_id_in_res[left_id] = add_object(l, "_ПЗ_1")->get_id();
    right_object_id_in_res[right_id] = add_object(r, "_ПЗ_2")->get_id();

    log += "Объекты " + l->get_name() + "_ПЗ_1 и " + r->get_name() + "_ПЗ_2 необходимо объединить или переименовать вручную. Они имели схожие названия, но различную структуру\n";
  }

  for (int right_id = 0; right_id < right.get_number_of_objects(); ++right_id) {
    if (used.find(right_id) != used.end())
      continue;
    right_object_id_in_res[right_id] = add_object(right.get_object(right_id))->get_id();
  }
}

condition_t* merger_t::copy_condition(const condition_t* it, map<int, int>& object_id_in_res, map<int, int>& event_id_in_res, map<int, int>& interval_id_in_res, const knowledge_field_t& kf) {
  {
    const condition_interval_duration_t* old = dynamic_cast<const condition_interval_duration_t*>(it);
    if (old != nullptr) {
      condition_interval_duration_t* cond = new condition_interval_duration_t();
      cond->set_op(old->get_op());
      cond->set_duration_val(old->get_duration_val());
      cond->set_interval_id(event_id_in_res[old->get_interval_id()]);
      return cond;
    }
  }
  {
    const condition_sum_equal_const_t* old = dynamic_cast<const condition_sum_equal_const_t*>(it);
    if (old != nullptr) {
      condition_sum_equal_const_t* cond = new condition_sum_equal_const_t();
      cond->set_op(old->get_op());
      cond->set_value(old->get_value());
      for (int id = 0; id < old->get_term_count(); ++id) {
        pair<int, int> old_term = old->get_term(id);
        int obj_id = object_id_in_res[old_term.first];
        string attr_name = kf.get_object(old_term.first)->get_attr_name_by_id(old_term.second);
        int attr_id = res.get_object(obj_id)->get_attr_id_by_name(attr_name);
        cond->add_item(obj_id, attr_id);
      }
      return cond;
    }
  }
  {
    const condition_attr_value_const_t* old = dynamic_cast<const condition_attr_value_const_t*>(it);
    if (old != nullptr) {
      condition_attr_value_const_t* cond = new condition_attr_value_const_t();
      cond->set_op(old->get_op());
      cond->set_value(old->get_value());

      int obj_id = object_id_in_res[old->get_obj_id()];
      string attr_name = kf.get_object(old->get_obj_id())->get_attr_name_by_id(old->get_attr_id());
      int attr_id = res.get_object(obj_id)->get_attr_id_by_name(attr_name);
      cond->set_obj_id(obj_id);
      cond->set_attr_id(attr_id);
      return cond;
    }
  }
  {
    const condition_attr_value_attr_value_t* old = dynamic_cast<const condition_attr_value_attr_value_t*>(it);
    if (old != nullptr) {
      condition_attr_value_attr_value_t* cond = new condition_attr_value_attr_value_t();
      cond->set_op(old->get_op());

      int obj_id = object_id_in_res[old->get_left_obj_id()];
      string attr_name = kf.get_object(old->get_left_obj_id())->get_attr_name_by_id(old->get_left_attr_id());
      int attr_id = res.get_object(obj_id)->get_attr_id_by_name(attr_name);
      cond->set_left_obj_id(obj_id);
      cond->set_left_attr_id(attr_id);

      obj_id = object_id_in_res[old->get_right_obj_id()];
      attr_name = kf.get_object(old->get_right_obj_id())->get_attr_name_by_id(old->get_right_attr_id());
      attr_id = res.get_object(obj_id)->get_attr_id_by_name(attr_name);
      cond->set_right_obj_id(obj_id);
      cond->set_right_attr_id(attr_id);
      return cond;
    }
  }
  {
    const event_interval_condition_t* old = dynamic_cast<const event_interval_condition_t*>(it);
    if (old != nullptr) {
      event_interval_condition_t* cond = new event_interval_condition_t();
      cond->set_op(old->get_op());

      cond->set_event_id(event_id_in_res[old->get_event_id()]);
      cond->set_interval_id(interval_id_in_res[old->get_interval_id()]);
      return cond;
    }
  }
  {
    const event_event_condition_t* old = dynamic_cast<const event_event_condition_t*>(it);
    if (old != nullptr) {
      event_event_condition_t* cond = new event_event_condition_t();
      cond->set_op(old->get_op());

      cond->set_left_event_id(event_id_in_res[old->get_left_event_id()]);
      cond->set_right_event_id(event_id_in_res[old->get_right_event_id()]);
      return cond;
    }
  }
  {
    const complex_condition_t* old = dynamic_cast<const complex_condition_t*>(it);
    if (old != nullptr) {
      complex_condition_t* cond = new complex_condition_t();
      cond->set_op(old->get_op());
      int cnt = old->get_number_of_conditions();
      for (int id = 0; id < cnt; ++id)
        cond->add_condition(copy_condition(old->get_condition(id), object_id_in_res, event_id_in_res, interval_id_in_res, kf));
      return cond;
    }
  }
  {
    const true_condition_t* old = dynamic_cast<const true_condition_t*>(it);
    if (old != nullptr) {
      return new true_condition_t();
    }
  }
  return nullptr;
}

action_t* merger_t::copy_action(const action_t* it, map<int, int>& object_id_in_res, const knowledge_field_t& kf) {
  {
    const complex_action_t* old = dynamic_cast<const complex_action_t*>(it);
    if (old != nullptr) {
      complex_action_t* action = new complex_action_t();
      int cnt = old->get_number_of_actions();
      for (int id = 0; id < cnt; ++id)
        action->add_action(copy_action(old->get_action(id), object_id_in_res, kf));
      return action;
    }
  }
  {
    const action_int_t* old = dynamic_cast<const action_int_t*>(it);
    if (old != nullptr) {
      action_int_t* action = new action_int_t();
      action->set_value(old->get_value());

      int obj_id = object_id_in_res[old->get_obj_id()];
      string attr_name = kf.get_object(old->get_obj_id())->get_attr_name_by_id(old->get_attr_id());
      int attr_id = res.get_object(obj_id)->get_attr_id_by_name(attr_name);

      action->set_object_id(obj_id);
      action->set_attribute_id(attr_id);
      action->set_op(old->get_op());
      return action;
    }
  }
  {
    const action_string_t* old = dynamic_cast<const action_string_t*>(it);
    if (old != nullptr) {
      action_string_t* action = new action_string_t();
      action->set_value(old->get_value());

      int obj_id = object_id_in_res[old->get_obj_id()];
      string attr_name = kf.get_object(old->get_obj_id())->get_attr_name_by_id(old->get_attr_id());
      int attr_id = res.get_object(obj_id)->get_attr_id_by_name(attr_name);

      action->set_object_id(obj_id);
      action->set_attribute_id(attr_id);
      action->set_op(old->get_op());
      return action;
    }
  }
  return nullptr;
}

void merger_t::merge_events(string& log) {
  for (int id = 0; id < left.get_number_of_events(); ++id) {
    const temporal_event_t* event = left.get_event(id);
    const temporal_entity_t* new_event = res.add_event(event->get_name() + "_ПЗ_1");
    condition_t* new_condition = copy_condition(event->get_condition(), left_object_id_in_res, left_event_id_in_res, left_interval_id_in_res, left);
    res.add_condition(new_event, new_condition);
    left_event_id_in_res[id] = new_event->get_id();
  }
  for (int id = 0; id < right.get_number_of_events(); ++id) {
    const temporal_event_t* event = right.get_event(id);
    const temporal_entity_t* new_event = res.add_event(event->get_name() + "_ПЗ_2");
    condition_t* new_condition = copy_condition(event->get_condition(), right_object_id_in_res, right_event_id_in_res, right_interval_id_in_res, right);
    res.add_condition(new_event, new_condition);
    right_event_id_in_res[id] = new_event->get_id();
  }

  table.clear();
  facts.clear();
  dur_facts.clear();
  ee_facts.clear();
  ei_facts.clear();
  for (int id = 0; id < res.get_number_of_events(); ++id)
    add_fact(res.get_event(id)->get_condition(), id, res.get_number_of_events(), 1, 0);

  dist_table.clear();
  dist_table.resize(res.get_number_of_events(), vector<vector<double>>(res.get_number_of_events(), vector<double>(1, 0)));

  for (int eid1 = 0; eid1 < res.get_number_of_events(); ++eid1) {
    for (int eid2 = eid1; eid2 < res.get_number_of_events(); ++eid2) {
      const int N = table.size();
      int eq = 0;
      for (int fid = 0; fid < N; ++fid)
        eq += (table[fid][eid1][0] == table[fid][eid2][0]);
      dist_table[eid1][eid2][0] = dist_table[eid2][eid1][0] = 1.0 * eq / N;
      //константа = количество фактов, в которых могут отличаться события, чтобы всё равно остаться достаточно близкими
      if (eq + 1 >= N && eid1 != eid2)
        log += "События " + res.get_event_name_by_id(eid1) + " и " + res.get_event_name_by_id(eid2) + " похожи. Необходимо обработать в ручном режиме\n";
    }
  }
  current_step = step::events;
}

void merger_t::merge_intervals(string& log) {
  for (int id = 0; id < left.get_number_of_intervals(); ++id) {
    const temporal_interval_t* interval = left.get_interval(id);
    const temporal_entity_t* new_interval = res.add_interval(interval->get_name() + "_ПЗ_1");
    condition_t* begin_condition = copy_condition(interval->get_begin_condition(), left_object_id_in_res, left_event_id_in_res, left_interval_id_in_res, left);
    condition_t* end_condition = copy_condition(interval->get_end_condition(), left_object_id_in_res, left_event_id_in_res, left_interval_id_in_res, left);
    res.set_start_condition(new_interval, begin_condition);
    res.set_end_condition(new_interval, end_condition);
    left_interval_id_in_res[id] = new_interval->get_id();
  }
  for (int id = 0; id < right.get_number_of_intervals(); ++id) {
    const temporal_interval_t* interval = right.get_interval(id);
    const temporal_entity_t* new_interval = res.add_interval(interval->get_name() + "_ПЗ_2");
    condition_t* begin_condition = copy_condition(interval->get_begin_condition(), right_object_id_in_res, right_event_id_in_res, right_interval_id_in_res, right);
    condition_t* end_condition = copy_condition(interval->get_end_condition(), right_object_id_in_res, right_event_id_in_res, right_interval_id_in_res, right);
    res.set_start_condition(new_interval, begin_condition);
    res.set_end_condition(new_interval, end_condition);
    right_interval_id_in_res[id] = new_interval->get_id();
  }

  table.clear();
  facts.clear();
  dur_facts.clear();
  ee_facts.clear();
  ei_facts.clear();
  for (int id = 0; id < res.get_number_of_intervals(); ++id) {
    add_fact(res.get_interval(id)->get_begin_condition(), id, res.get_number_of_intervals(), 2, 0);
    add_fact(res.get_interval(id)->get_end_condition(), id, res.get_number_of_intervals(), 2, 1);
  }

  dist_table.clear();
  dist_table.resize(res.get_number_of_intervals(), vector<vector<double>>(res.get_number_of_intervals(), vector<double>(2, 0)));

  for (int iid1 = 0; iid1 < res.get_number_of_intervals(); ++iid1) {
    for (int iid2 = iid1; iid2 < res.get_number_of_intervals(); ++iid2) {
      const int N = table.size();

      int beg_eq = 0, end_eq = 0;
      for (int fid = 0; fid < N; ++fid) {
        beg_eq += (table[fid][iid1][0] == table[fid][iid2][0]);
        end_eq += (table[fid][iid1][1] == table[fid][iid2][1]);
      }
      dist_table[iid1][iid2][0] = dist_table[iid2][iid1][0] = 1.0 * beg_eq / N;
      dist_table[iid1][iid2][1] = dist_table[iid2][iid1][1] = 1.0 * end_eq / N;
      //константа = количество фактов, в которых могут отличаться интервалы, чтобы всё равно остаться достаточно близкими
      if ((beg_eq + 1 >= N || end_eq + 1 >= N) && iid1 != iid2)
        log += "Интервалы " + res.get_interval_name_by_id(iid1) + " и " + res.get_interval_name_by_id(iid2) + " похожи. Необходимо обработать в ручном режиме\n";
    }
  }

  current_step = step::intervals;
}

void merger_t::merge_rules(string& log) {
  for (int id = 0; id < left.get_number_of_rules(); ++id) {
    const temporal_rule_t* rule = left.get_rule(id);
    const temporal_entity_t* new_rule = res.add_rule(rule->get_name() + "_ПЗ_1");
    condition_t* condition = copy_condition(rule->get_condition(), left_object_id_in_res, left_event_id_in_res, left_interval_id_in_res, left);
    action_t* action = copy_action(rule->get_action(), left_object_id_in_res, left);
    res.add_condition(new_rule, condition);
    res.add_action(new_rule, action);
  }
  for (int id = 0; id < right.get_number_of_rules(); ++id) {
    const temporal_rule_t* rule = right.get_rule(id);
    const temporal_entity_t* new_rule = res.add_rule(rule->get_name() + "_ПЗ_2");
    condition_t* condition = copy_condition(rule->get_condition(), right_object_id_in_res, right_event_id_in_res, right_interval_id_in_res, right);
    action_t* action = copy_action(rule->get_action(), right_object_id_in_res, right);
    res.add_condition(new_rule, condition);
    res.add_action(new_rule, action);
  }

  table.clear();
  facts.clear();
  dur_facts.clear();
  ee_facts.clear();
  ei_facts.clear();
  for (int id = 0; id < res.get_number_of_rules(); ++id) {
    add_fact(res.get_rule(id)->get_condition(), id, res.get_number_of_intervals(), 2, 0);
    add_fact(res.get_rule(id)->get_action(), id, res.get_number_of_intervals(), 2, 1);
  }

  dist_table.clear();
  dist_table.resize(res.get_number_of_rules(), vector<vector<double>>(res.get_number_of_rules(), vector<double>(2, 0)));

  for (int rid1 = 0; rid1 < res.get_number_of_rules(); ++rid1) {
    for (int rid2 = rid1; rid2 < res.get_number_of_rules(); ++rid2) {
      const int N = table.size();

      int cond_eq = 0, act_eq = 0;
      for (int fid = 0; fid < N; ++fid) {
        cond_eq += (table[fid][rid1][0] == table[fid][rid2][0]);
        act_eq += (table[fid][rid1][1] == table[fid][rid2][1]);
      }
      dist_table[rid1][rid2][0] = dist_table[rid2][rid1][0] = 1.0 * cond_eq / N;
      dist_table[rid1][rid2][1] = dist_table[rid2][rid1][1] = 1.0 * act_eq / N;
      //константа = количество фактов, в которых могут отличаться правила, чтобы всё равно остаться достаточно близкими
      if ((cond_eq + 1 >= N || act_eq + 1 >= N) && rid1 != rid2)
        log += "Правила " + res.get_rule(rid1)->get_name() + " и " + res.get_rule(rid2)->get_name() + " похожи. Необходимо обработать в ручном режиме\n";
    }
  }

  current_step = step::rules;
}

void merger_t::add_fact(const action_t* it, int id, int size, int cols, int col) {
  {
    const complex_action_t* action = dynamic_cast<const complex_action_t*>(it);
    if (action != nullptr) {
      int cnt = action->get_number_of_actions();
      for (int id = 0; id < cnt; ++id)
        add_fact(action->get_action(id), id, size, cols, col);
      return;
    }
  }
  {
    const action_int_t* action = dynamic_cast<const action_int_t*>(it);
    if (action != nullptr) {
      int obj_id = action->get_obj_id();
      int attr_id = action->get_attr_id();
      string value = std::to_string(action->get_value());
      fact_t tmp(obj_id, attr_id, value);

      auto iter = facts.find(tmp);
      if (iter == facts.end()) {
        facts[tmp] = table.size();
        table.push_back(vector<vector<int>>(size, vector<int>(cols, 0)));
      }

      int fact_id = facts[tmp];
      table[fact_id][id][col] = 1;
      return;
    }
  }
  {
    const action_string_t* action = dynamic_cast<const action_string_t*>(it);
    if (action != nullptr) {
      int obj_id = action->get_obj_id();
      int attr_id = action->get_attr_id();
      string value = action->get_value();
      fact_t tmp(obj_id, attr_id, value);

      auto iter = facts.find(tmp);
      if (iter == facts.end()) {
        facts[tmp] = table.size();
        table.push_back(vector<vector<int>>(size, vector<int>(cols, 0)));
      }

      int fact_id = facts[tmp];
      table[fact_id][id][col] = 1;
      return;
    }
  }
}

void merger_t::add_fact(const condition_t* it, int id, int size, int cols, int col) {
  {
    const condition_interval_duration_t* condition = dynamic_cast<const condition_interval_duration_t*>(it);
    if (condition != nullptr) {
      dur_fact_t tmp(condition->get_interval_id(), condition->get_duration_val());
      auto iter = dur_facts.find(tmp);

      if (iter == dur_facts.end()) {
        dur_facts[tmp] = table.size();
        table.push_back(vector<vector<int>>(size, vector<int>(cols, 0)));
      }

      int fact_id = dur_facts[tmp];
      table[fact_id][id][col] = 1;
      return;
    }
  }
  {
    const condition_sum_equal_const_t* condition = dynamic_cast<const condition_sum_equal_const_t*>(it);
    if (condition != nullptr) {
      string value = condition->get_value();
      for (int tid = 0; tid < condition->get_term_count(); ++tid) {
        int obj_id = condition->get_term(tid).first;
        int attr_id = condition->get_term(tid).second;

        fact_t tmp(obj_id, attr_id, value);

        auto iter = facts.find(tmp);
        if (iter == facts.end()) {
          facts[tmp] = table.size();
          table.push_back(vector<vector<int>>(size, vector<int>(cols, 0)));
        }

        int fact_id = facts[tmp];
        table[fact_id][id][col] = 1;
      }
      return;
    }
  }
  {
    const condition_attr_value_const_t* condition = dynamic_cast<const condition_attr_value_const_t*>(it);
    if (condition != nullptr) {
      string value = condition->get_value();
      int obj_id = condition->get_obj_id();
      int attr_id = condition->get_attr_id();
      fact_t tmp(obj_id, attr_id, value);

      auto iter = facts.find(tmp);
      if (iter == facts.end()) {
        facts[tmp] = table.size();
        table.push_back(vector<vector<int>>(size, vector<int>(cols, 0)));
      }

      int fact_id = facts[tmp];
      table[fact_id][id][col] = 1;
      return;
    }
  }
  {
    const condition_attr_value_attr_value_t* condition = dynamic_cast<const condition_attr_value_attr_value_t*>(it);
    if (condition != nullptr) {
      int obj_id = condition->get_left_obj_id();
      int attr_id = condition->get_left_attr_id();
      fact_t tmp(obj_id, attr_id, "");

      auto iter = facts.find(tmp);
      if (iter == facts.end()) {
        facts[tmp] = table.size();
        table.push_back(vector<vector<int>>(size, vector<int>(cols, 0)));
      }

      int fact_id = facts[tmp];
      table[fact_id][id][col] = 1;

      obj_id = condition->get_right_obj_id();
      attr_id = condition->get_right_attr_id();
      tmp = fact_t(obj_id, attr_id, "");

      iter = facts.find(tmp);
      if (iter == facts.end()) {
        facts[tmp] = table.size();
        table.push_back(vector<vector<int>>(size, vector<int>(cols, 0)));
      }

      fact_id = facts[tmp];
      table[fact_id][id][col] = 1;

      return;
    }
  }
  {
    const event_interval_condition_t* condition = dynamic_cast<const event_interval_condition_t*>(it);
    if (condition != nullptr) {
      int event_id = condition->get_event_id();
      int interval_id = condition->get_interval_id();
      string op = condition->get_op();

      ei_fact_t tmp(event_id, interval_id, op);

      auto iter = ei_facts.find(tmp);
      if (iter == ei_facts.end()) {
        ei_facts[tmp] = table.size();
        table.push_back(vector<vector<int>>(size, vector<int>(cols, 0)));
      }

      int fact_id = ei_facts[tmp];
      table[fact_id][id][col] = 1;

      return;
    }
  }
  {
    const event_event_condition_t* condition = dynamic_cast<const event_event_condition_t*>(it);
    if (condition != nullptr) {
      int left_event_id = condition->get_left_event_id();
      int right_event_id = condition->get_right_event_id();
      string op = condition->get_op();

      ee_fact_t tmp(left_event_id, right_event_id, op);

      auto iter = ee_facts.find(tmp);
      if (iter == ee_facts.end()) {
        ee_facts[tmp] = table.size();
        table.push_back(vector<vector<int>>(size, vector<int>(cols, 0)));
      }

      int fact_id = ee_facts[tmp];
      table[fact_id][id][col] = 1;

      return;
    }
  }
  {
    const complex_condition_t* condition = dynamic_cast<const complex_condition_t*>(it);
    if (condition != nullptr) {
      int cnt = condition->get_number_of_conditions();
      for (int tid = 0; tid < cnt; ++tid)
        add_fact(condition->get_condition(tid), id, size, cols, col);
      return;
    }
  }
  {
    const true_condition_t* condition = dynamic_cast<const true_condition_t*>(it);
    if (condition != nullptr) {
      return;
    }
  }
}

void merger_t::show_rtr(std::ostream& out) const {
  vector<string> vs(table.size(), "");
  string header = "";
  header += ",";
  if (current_step == step::events) {
    for (int id = 0; id < res.get_number_of_events(); ++id) {
      header += res.get_event_name_by_id(id) + ",";
    }
  } else if (current_step == step::intervals) {
    for (int id = 0; id < res.get_number_of_intervals(); ++id) {
      header += res.get_interval_name_by_id(id) + ",";
    }
  } else if (current_step == step::rules) {
    for (int id = 0; id < res.get_number_of_rules(); ++id) {
      header += res.get_rule(id)->get_name() + ",";
    }
  }

  for (const auto& it : facts) {
    int id = it.second;
    vs[id] += res.get_full_attr_name_by_id(it.first.object_id, it.first.attribute_id) + " " + it.first.value + ",";
    for (size_t col_id = 0; col_id < table[id].size(); ++col_id) {
      vs[id] += "[ ";
      for (size_t subcol = 0; subcol < table[id][col_id].size(); ++subcol)
        vs[id] += std::to_string(table[id][col_id][subcol]) + " ";
      vs[id] += "],";
    }
  }

  for (const auto& it : dur_facts) {
    int id = it.second;
    vs[id] += res.get_interval_name_by_id(it.first.first) + " длится " + std::to_string(it.first.second) + ",";
    for (size_t col_id = 0; col_id < table[id].size(); ++col_id) {
      vs[id] += "[ ";
      for (size_t subcol = 0; subcol < table[id][col_id].size(); ++subcol)
        vs[id] += std::to_string(table[id][col_id][subcol]) + " ";
      vs[id] += "],";
    }
  }

  for (const auto& it : ee_facts) {
    int id = it.second;
    vs[id] += res.get_event(it.first.left_event_id)->get_name() + " " + it.first.op + " " + res.get_event(it.first.right_event_id)->get_name() + ",";
    for (size_t col_id = 0; col_id < table[id].size(); ++col_id) {
      vs[id] += "[ ";
      for (size_t subcol = 0; subcol < table[id][col_id].size(); ++subcol)
        vs[id] += std::to_string(table[id][col_id][subcol]) + " ";
      vs[id] += "],";
    }
  }

  for (const auto& it : ei_facts) {
    int id = it.second;
    vs[id] += res.get_event(it.first.event_id)->get_name() + " " + it.first.op + " " + res.get_interval(it.first.interval_id)->get_name() + ",";
    for (size_t col_id = 0; col_id < table[id].size(); ++col_id) {
      vs[id] += "[ ";
      for (size_t subcol = 0; subcol < table[id][col_id].size(); ++subcol)
        vs[id] += std::to_string(table[id][col_id][subcol]) + " ";
      vs[id] += "],";
    }
  }

  out << header << std::endl;
  for (const string& it : vs)
    out << it << std::endl;
}

void merger_t::show_dists(std::ostream &out) const {
  vector<string> vs(table.size(), "");
  string header = "";
  header += ",";
  if (current_step == step::events) {
    for (int id = 0; id < res.get_number_of_events(); ++id) {
      header += res.get_event_name_by_id(id) + ",";
    }
    for (int row_id = 0; row_id < res.get_number_of_events(); ++row_id) {
      vs[row_id] += res.get_event_name_by_id(row_id) + ",";
      for (int col_id = 0; col_id < res.get_number_of_events(); ++col_id) {
        vs[row_id] += "[ ";
        for (size_t subcol = 0; subcol < dist_table[row_id][col_id].size(); ++subcol)
          vs[row_id] += std::to_string(dist_table[row_id][col_id][subcol]) + " ";
        vs[row_id] += "],";
      }
    }
  } else if (current_step == step::intervals) {
    for (int id = 0; id < res.get_number_of_intervals(); ++id) {
      header += res.get_interval_name_by_id(id) + ",";
    }
    for (int row_id = 0; row_id < res.get_number_of_intervals(); ++row_id) {
      vs[row_id] += res.get_interval_name_by_id(row_id) + ",";
      for (int col_id = 0; col_id < res.get_number_of_intervals(); ++col_id) {
        vs[row_id] += "[ ";
        for (size_t subcol = 0; subcol < dist_table[row_id][col_id].size(); ++subcol)
          vs[row_id] += std::to_string(dist_table[row_id][col_id][subcol]) + " ";
        vs[row_id] += "],";
      }
    }
  } else if (current_step == step::rules) {
    for (int id = 0; id < res.get_number_of_rules(); ++id) {
      header += res.get_rule(id)->get_name() + ",";
    }
    for (int row_id = 0; row_id < res.get_number_of_rules(); ++row_id) {
      vs[row_id] += res.get_rule(row_id)->get_name() + ",";
      for (int col_id = 0; col_id < res.get_number_of_rules(); ++col_id) {
        vs[row_id] += "[ ";
        for (size_t subcol = 0; subcol < dist_table[row_id][col_id].size(); ++subcol)
          vs[row_id] += std::to_string(dist_table[row_id][col_id][subcol]) + " ";
        vs[row_id] += "],";
      }
    }
  }

  out << header << std::endl;
  for (const string& it : vs)
    out << it << std::endl;
}
