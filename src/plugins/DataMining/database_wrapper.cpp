#include "database_wrapper.h"

database_wrapper_t::database_wrapper_t() {
  sdb = QSqlDatabase::addDatabase("QSQLITE");
}

QString database_wrapper_t::init_database(const std::string& name) {
  QString result = "";
  if (sdb.isOpen())
    sdb.close();
  sdb.setDatabaseName(name.c_str());
  if (sdb.open()) {
    result.append("Success\n");
    result.append("Tables: ");
    for (auto i : sdb.tables()) {
      result.append(i);
      result.append(" ");
    }
    result.append("\n");
  } else {
    result.append("Failed");
  }
  return result;
}

QString database_wrapper_t::save_mapping(const QString& table_name, const QString& id_column_name, const QString& time_column_name, const QString& str_properties, const QString& class_table_name, const QString& class_column_name) {
  QString result = "";
  if (!sdb.isOpen()) {
    result.append("Database not opened");
    return result;
  }
  property_table = table_name.toStdString();
  id_column = id_column_name.toStdString();
  time_column = time_column_name.toStdString();
  class_table = class_table_name.toStdString();
  class_column = class_column_name.toStdString();

  properties.clear();
  properties_map.clear();

  string all = str_properties.toStdString(), buf;
  size_t pos = 0;
  while (pos < all.length()) {
    if (all[pos] == '\n') {
      properties_map[buf] = properties.size();
      properties.push_back(buf);
      buf = "";
    } else {
        buf += all[pos];
    }
    ++pos;
  }
  if (buf != "") {
    properties_map[buf] = properties.size();
    properties.push_back(buf);
  }

  result += "PROPERTIES :\n";
  for (const string& it : properties) {
    result.append(it.c_str());
    result.append(" : ");
    result.append(std::to_string(properties_map[it]).c_str());
    result.append('\n');
  }

  times.clear();
  times_map.clear();
  buf = "select distinct ";
  buf += time_column + " from " + property_table;
  QSqlQuery query(buf.c_str());
  while (query.next()) {
    string current = query.value(0).toString().toStdString();
    times_map[current] = times.size();
    times.push_back(current);
  }

  result += "TIMES :\n";
  for (const string& it : times) {
    result.append(it.c_str());
    result.append(" : ");
    result.append(std::to_string(times_map[it]).c_str());
    result.append('\n');
  }

  ids.clear();
  ids_map.clear();
  buf = "select distinct ";
  buf += id_column + " from " + property_table;
  query = QSqlQuery(buf.c_str());
  while (query.next()) {
    string current = query.value(0).toString().toStdString();
    ids_map[current] = ids.size();
    ids.push_back(current);
  }

  result += "IDS :\n";
  for (const string& it : ids) {
    result.append(it.c_str());
    result.append(" : ");
    result.append(std::to_string(ids_map[it]).c_str());
    result.append('\n');
  }

  return result;
}

string database_wrapper_t::get_property(const int object_id, const int property_id, const int time_id) {
  string buf = "select ";
  buf += properties[property_id] + " from " + property_table + " where ";
  buf += id_column + "=" + ids[object_id] + " and " + time_column + "=" + times[time_id];
  QSqlQuery query(buf.c_str());
  string result;
  while (query.next())
    result = query.value(0).toString().toStdString();
  return result;
}

string database_wrapper_t::get_class(const int object_id) {
  string buf = "select ";
  buf += class_column + " from " + class_table + " where ";
  buf += id_column + "=" + ids[object_id];
  QSqlQuery query(buf.c_str());
  string result;
  while (query.next())
    result = query.value(0).toString().toStdString();
  return result;
}

vector<string> database_wrapper_t::get_all_classes() {
  string buf = "select distinct " + class_column + " from " + class_table;
  QSqlQuery query(buf.c_str());
  vector<string> result;
  while (query.next())
    result.push_back(query.value(0).toString().toStdString());
  return result;
}
