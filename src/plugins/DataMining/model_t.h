#ifndef __MODEL_H__
#define __MODEL_H__

#include <QString>
#include <string>
#include "database_wrapper.h"
#include "tree_t.h"
#include <iostream>

using std::string;

class model_t {
  public:
    model_t();
    QString init(const string& database_name);
    QString set_names(const QString& table_name, const QString& id_column_name, const QString& time_column_name, const QString& properties, const QString& class_table_name, const QString& class_column_name);
    void add_tree();
    string serialize() const;
    void rulealize(std::ostream& text_out, std::ostream& inner_out, std::ostream& xml_out);
  private:
    database_wrapper_t dbw;
    vector<tree_t*> trees;
    knowledge_field_t kf;
};

#endif
