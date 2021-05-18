#include "model_t.h"
#include <fstream>

model_t::model_t() {
}

QString model_t::init(const string& database_name) {
  return dbw.init_database(database_name);
}

QString model_t::set_names(const QString& table_name, const QString& id_column_name, const QString& time_column_name, const QString& properties, const QString& class_table_name, const QString& class_column_name) {
  trees.clear();
  return dbw.save_mapping(table_name, id_column_name, time_column_name, properties, class_table_name, class_column_name);
}

void model_t::add_tree() {
  trees.push_back(new tree_t(dbw));
  trees.back()->show_stats();
}

string model_t::serialize() const {
  string result = "[\n";
  if (trees.size() > 0) {
    for (size_t i = 0; i < trees.size() - 1; ++i)
      result += "  " + trees[i]->serialize() + ",\n";
    result += "  " + trees.back()->serialize() + "\n";
  }
  result += "]";
  return result;
}

void model_t::rulealize(std::ostream& text_out, std::ostream& inner_out, std::ostream& xml_out) {
  kf = knowledge_field_t();

  const temporal_entity_t* main_object = kf.add_object("Главный_объект");
  if (!main_object)
    return;
  for (const string& it : dbw.get_properties()) {
    kf.add_attr(main_object, it, "числовой");
  }

  const temporal_entity_t* timer = kf.add_object("Таймер");
  kf.add_attr(timer, "такт", "числовой");

  int last_time = stoll(dbw.get_time_name_by_id(dbw.get_times_number() - 1)) + 1;

  for (int i = 0; i < last_time; ++i) {
    const temporal_entity_t* event = kf.add_event("Текущий_такт_" + std::to_string(i));

    condition_attr_value_const_t* condition = new condition_attr_value_const_t(timer->get_id(), static_cast<const temporal_object_t*>(timer)->get_attr_id_by_name("такт"), std::to_string(i));
    condition->set_op("=");

    kf.add_condition(event, condition);
  }

  const temporal_entity_t* counter = kf.add_object("Счётчик");
  vector<string> all_classes = dbw.get_all_classes();
  for (const string& it : all_classes) {
    kf.add_attr(counter, string("Голоса_за_") + it, "числовой");
  }
  kf.add_attr(counter, "Результат", "символьный");
  kf.add_type("символьный");
  vector<string> classes = dbw.get_all_classes();
  for (const string& class_name : classes)
    kf.add_type_value("символьный", class_name);
  kf.add_type_value("символьный", "Пусто");

  for (int i = 0; i < (int) trees.size(); ++i)
    for (int start_time = 0; start_time < last_time; ++start_time)
      trees[i]->rulealize(kf, main_object, timer, counter, start_time, last_time);

  for (const string& class_name : classes) {
    string name = "Правило_" + class_name;
    const temporal_entity_t* rule = kf.add_rule(name);

    complex_condition_t* condition = new complex_condition_t;
    condition->set_op("&");

    condition_sum_equal_const_t* sum_condition = new condition_sum_equal_const_t;
    for (const string& cname : classes)
      sum_condition->add_item(counter->get_id(), static_cast<const temporal_object_t*>(counter)->get_attr_id_by_name("Голоса_за_" + cname));
    sum_condition->set_value(std::to_string(trees.size()));

    condition->add_condition(sum_condition);
    for (const string& other_class : classes) {
      if (other_class == class_name)
        continue;
      int obj_id = counter->get_id();
      int left_attr_id = static_cast<const temporal_object_t*>(counter)->get_attr_id_by_name("Голоса_за_" + class_name);
      int right_attr_id = static_cast<const temporal_object_t*>(counter)->get_attr_id_by_name("Голоса_за_" + other_class);
      condition_attr_value_attr_value_t* ge_condition = new condition_attr_value_attr_value_t(obj_id, left_attr_id, obj_id, right_attr_id);
      ge_condition->set_op(">=");
      condition->add_condition(ge_condition);
    }

    action_string_t* elections_action = new action_string_t;
    elections_action->set_op("=");
    elections_action->set_object_id(counter->get_id());
    elections_action->set_attribute_id(static_cast<const temporal_object_t*>(counter)->get_attr_id_by_name("Результат"));
    elections_action->set_value(class_name);

    complex_action_t* action = new complex_action_t;
    action->set_op("&");
    action->add_action(elections_action);

    kf.add_condition(rule, condition);
    kf.add_action(rule, action);
  }

  text_out << kf.to_string() << std::endl;
  inner_out << kf.dump() << std::endl;
  xml_out << kf.to_xml() << std::endl;
}
