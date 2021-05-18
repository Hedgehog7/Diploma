#ifndef __DATABASE_WRAPPER_H__
#define __DATABASE_WRAPPER_H__

#include <QString>
#include <QtSql>
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class database_wrapper_t {
  public:
    database_wrapper_t();

    QString init_database(const std::string& name);
    QString save_mapping(const QString& table_name, const QString& id_column_name, const QString& time_column_name, const QString& properties, const QString& class_table_name, const QString& class_column_name);

    inline int get_times_number() const {return times.size();}
    inline int get_properties_number() const {return properties.size();}
    inline int get_ids_number() const {return ids.size();}
    string get_property(const int object_id, const int property_id, const int time_id);
    string get_class(const int object_id);

    string get_property_name_by_id(int id) const {return properties.at(id);}
    string get_time_name_by_id(int id) const {return times.at(id);}

    const vector<string>& get_properties() const {return properties;}
    vector<string> get_all_classes();

  private:
    QSqlDatabase sdb;

    string property_table;
    string id_column;
    string time_column;
    vector<string> properties;
    map<string, int> properties_map;
    string class_table;
    vector<string> times;
    map<string, int> times_map;
    vector<string> ids;
    map<string, int> ids_map;

    string class_column;
};

#endif
