#include "tree_t.h"
#include <random>
#include <cmath>
#include <stack>

using std::pair;
using std::endl;

tree_t::tree_t(database_wrapper_t& dbw) : dbw(dbw)
{
  std::mt19937 generator(time(NULL));
  size_t max_ids = dbw.get_ids_number();
  std::uniform_int_distribution<int> distr(0, max_ids - 1);
  while (ids.size() < max_ids)
    ids.push_back(distr(generator));

  set<int> buffer;

  max_ids = dbw.get_properties_number();
  distr = std::uniform_int_distribution<int>(0, max_ids - 1);
  while (properties.size() < ceil(sqrt(max_ids))) {
    int cur_id = distr(generator);
    if (buffer.find(cur_id) == buffer.end()) {
      buffer.insert(cur_id);
      properties.push_back(cur_id);
    }
  }

  buffer.clear();
  max_ids = dbw.get_times_number();
  distr = std::uniform_int_distribution<int>(0, max_ids - 1);
  while (times.size() < ceil(sqrt(max_ids))) {
    int cur_id = distr(generator);
    if (buffer.find(cur_id) == buffer.end()) {
      buffer.insert(cur_id);
      times.push_back(cur_id);
    }
  }

  std::cerr << "ID: ";
  for (auto it : ids)
    std::cerr << it << " ";
  std::cerr << endl;

  std::cerr << "PROPS: ";
  for (auto it : properties)
    std::cerr << dbw.get_property_name_by_id(it) << " ";
  std::cerr << endl;

  std::cerr << "TIMES: ";
  for (auto it : times)
    std::cerr << it << " ";
  std::cerr << endl;

  root = new tree_node_t(dbw);
  root->set_ids(ids);
  split(root);
}

void tree_t::split(tree_node_t* node, const int depth) {
  cerr << node_id << endl;
  ++node_id;
  if (depth >= max_depth)
    return node->init_class();
  if (node->get_ids_number() <= min_elems)
    return node->init_class();
  if (node->get_max_prob() >= max_prob)
    return node->init_class();

  long double current_entropy = node->get_metric();
  int best_property_id = -1, best_time_id = -1, best_obj_id = -1;
  long double best_entropy = current_entropy;
  tree_node_t* l_ptr = nullptr;
  tree_node_t* r_ptr = nullptr;

  int total = times.size() * properties.size();
  int cid = 0;
  vector<int> node_ids = node->get_ids();
  for (const int time_id : times) {
    for (const int prop_id : properties) {
      //value, id
      vector<pair<int, int>> current_objects;
      for (const int id : node_ids)
        current_objects.emplace_back(std::stoll(dbw.get_property(id, prop_id, time_id)), id);
      std::sort(current_objects.begin(), current_objects.end());

      vector<int> less, greater;
      for (const pair<int, int>& it : current_objects)
        less.push_back(it.second);

      while (less.size() > 1) {
        greater.push_back(less.back());
        less.pop_back();

        tree_node_t* lft = new tree_node_t(dbw);
        lft->set_ids(less);

        tree_node_t* rgt = new tree_node_t(dbw);
        rgt->set_ids(greater);

        long double current_value = 0.5 * (lft->get_metric() + rgt->get_metric());
        if (current_value < best_entropy + EPS) {
          best_property_id = prop_id;
          best_time_id = time_id;
          best_entropy = current_value;
          best_obj_id = greater.back();
          if (l_ptr)
            delete l_ptr;
          l_ptr = lft;
          if (r_ptr)
            delete r_ptr;
          r_ptr = rgt;
        }
      }
      ++cid;
      cerr << "Node Complete Percent " << 100.0 * cid / total << "%" << endl;
    }
  }

  if (l_ptr && r_ptr) {
    node->set_leaf(false);
    node->set_childs(l_ptr, r_ptr);
    node->set_prop_id(best_property_id);
    node->set_time_id(best_time_id);
    node->set_prop_val(std::stoll(dbw.get_property(best_obj_id, best_property_id, best_time_id)));

    split(l_ptr, depth + 1);
    split(r_ptr, depth + 1);
  } else {
    node->init_class();
    cerr << "Can't split " << node_id - 1 << ", max_prob = " << node->get_max_prob() << endl;
    cerr << "Number of elements = " << node->get_ids_number() << endl;
  }
}

void tree_t::show_stats(std::ostream& out) const {
  show_stats(root, out);
}

void tree_t::show_stats(tree_node_t* node, std::ostream& out) const {
  if (node->is_leaf()) {
    out << "MAX_PROB in node " << node << " is " << node->get_max_prob() << endl;
  } else {
    show_stats(node->get_left(), out);
    show_stats(node->get_right(), out);
  }
}

string tree_t::serialize(tree_node_t* node) const {
  string result;
  if (node->is_leaf())
    result = "{is_leaf: true, node_class: " +node->get_class() + "}";
  else
    result = string("{property: ") + dbw.get_property_name_by_id(node->get_prop_id()) + ", " +
             "time: " + dbw.get_time_name_by_id(node->get_time_id()) + ", " +
             "val: " + std::to_string(node->get_prop_val()) + ", " +
             "left: " + serialize(node->get_left()) + ", " +
             "right: " + serialize(node->get_right()) + "}";
  return result;
}

string tree_t::serialize() const {
  return serialize(root);
}

void tree_t::add_rules(tree_node_t* node, knowledge_field_t& kf, vector<event_interval_condition_t*> up_conditions, int start_time, int last_time) const {
  string current_time_name = "Текущий_такт_" + std::to_string((start_time + last_time - 1) % last_time);
  string next_time_name = "Текущий_такт_" + std::to_string(start_time % last_time);
  if (node->is_leaf()) {
    string name = string("Правило_") + std::to_string((uint64_t) node) + "_" + std::to_string(start_time);
    const temporal_entity_t* rule = kf.add_rule(name);

    complex_condition_t* condition = new complex_condition_t;
    condition->set_op("&");
    for (condition_t* ptr : up_conditions) {
      event_interval_condition_t* cid_ptr = dynamic_cast<event_interval_condition_t*>(ptr);
      if (cid_ptr) {
        condition_t* nested_condition = new event_interval_condition_t(cid_ptr);
        condition->add_condition(nested_condition);
      }
    }
    event_event_condition_t* current_time_is_last_condition = new event_event_condition_t(kf.get_event_id_by_name(current_time_name), kf.get_event_id_by_name(next_time_name));
    current_time_is_last_condition->set_op("a");
    condition->add_condition(current_time_is_last_condition);

    action_int_t* result_action = new action_int_t;
    const temporal_object_t* counter = kf.get_object("Счётчик");
    result_action->set_object_id(counter->get_id());
    result_action->set_attribute_id(counter->get_attr_id_by_name(string("Голоса_за_") + node->get_class()));
    result_action->set_op("+=");
    result_action->set_value(1);

    kf.add_condition(rule, condition);
    kf.add_action(rule, result_action);
  } else {
    string name = string("Отрицательный_интервал_") + std::to_string((uint64_t) node) + "_" + std::to_string(start_time);
    event_interval_condition_t* less_condition = new event_interval_condition_t(kf.get_event_id_by_name(current_time_name), kf.get_interval_id_by_name(name));
    less_condition->set_op("f");

    name = string("Положительный_интервал_") + std::to_string((uint64_t) node) + "_" + std::to_string(start_time);
    event_interval_condition_t* greater_or_equal_condition = new event_interval_condition_t(kf.get_event_id_by_name(current_time_name), kf.get_interval_id_by_name(name));
    greater_or_equal_condition->set_op("f");

    up_conditions.push_back(less_condition);
    add_rules(node->get_left(), kf, up_conditions, start_time, last_time);
    up_conditions.pop_back();
    up_conditions.push_back(greater_or_equal_condition);
    add_rules(node->get_right(), kf, up_conditions, start_time, last_time);

    delete less_condition;
    delete greater_or_equal_condition;
  }
}

void tree_t::rulealize(knowledge_field_t& kf, const temporal_entity_t* main_object, const temporal_entity_t* timer, const temporal_entity_t*, int start_time, int last_time) const {
  std::stack<tree_node_t*> st;
  st.push(root);
  while (!st.empty()) {
    tree_node_t* cur = st.top();
    st.pop();
    if (cur->is_leaf())
      continue;

    string positive_name = "Положительный_интервал_" + std::to_string((uint64_t) cur) + "_" + std::to_string(start_time);
    const temporal_entity_t* positive = kf.add_interval(positive_name);

    int obj_id = main_object->get_id();
    int attr_id = dynamic_cast<const temporal_object_t*>(main_object)->get_attr_id_by_name(dbw.get_property_name_by_id(cur->get_prop_id()));
    string value = std::to_string(cur->get_prop_val());
    condition_t* left = new condition_attr_value_const_t(obj_id, attr_id, value);
    left->set_op(">=");

    obj_id = timer->get_id();
    attr_id = dynamic_cast<const temporal_object_t*>(timer)->get_attr_id_by_name("такт");
    value = std::to_string((stoi(dbw.get_time_name_by_id(cur->get_time_id())) + start_time) % last_time);
    condition_t* right = new condition_attr_value_const_t(obj_id, attr_id, value);
    right->set_op("=");

    complex_condition_t* start_condition = new complex_condition_t();
    start_condition->set_op("&");
    start_condition->add_condition(left);
    start_condition->add_condition(right);

    kf.set_start_condition(positive, start_condition);

    obj_id = timer->get_id();
    attr_id = dynamic_cast<const temporal_object_t*>(timer)->get_attr_id_by_name("такт");
    value = std::to_string((start_time + last_time - 1) % last_time);
    condition_t* end_condition = new condition_attr_value_const_t(obj_id, attr_id, value);
    end_condition->set_op("=");

    kf.set_end_condition(positive, end_condition);

    string negative_name = "Отрицательный_интервал_" + std::to_string((uint64_t) cur) + "_" + std::to_string(start_time);
    const temporal_entity_t* negative = kf.add_interval(negative_name);

    obj_id = main_object->get_id();
    attr_id = dynamic_cast<const temporal_object_t*>(main_object)->get_attr_id_by_name(dbw.get_property_name_by_id(cur->get_prop_id()));
    value = std::to_string(cur->get_prop_val());
    left = new condition_attr_value_const_t(obj_id, attr_id, value);
    left->set_op("<");

    obj_id = timer->get_id();
    attr_id = dynamic_cast<const temporal_object_t*>(timer)->get_attr_id_by_name("такт");
    value = std::to_string((stoi(dbw.get_time_name_by_id(cur->get_time_id())) + start_time) % last_time);
    right = new condition_attr_value_const_t(obj_id, attr_id, value);
    right->set_op("=");

    start_condition = new complex_condition_t();
    start_condition->set_op("&");
    start_condition->add_condition(left);
    start_condition->add_condition(right);

    kf.set_start_condition(negative, start_condition);

    obj_id = timer->get_id();
    attr_id = dynamic_cast<const temporal_object_t*>(timer)->get_attr_id_by_name("такт");
    value = std::to_string((start_time + last_time - 1) % last_time);
    end_condition = new condition_attr_value_const_t(obj_id, attr_id, value);
    end_condition->set_op("=");

    kf.set_end_condition(negative, end_condition);

    if (cur->get_left())
      st.push(cur->get_left());
    if (cur->get_right())
      st.push(cur->get_right());
  }

  vector<event_interval_condition_t*> up_conditions;
  add_rules(root, kf, up_conditions, start_time, last_time);
}
