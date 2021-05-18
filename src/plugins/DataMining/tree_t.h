#ifndef __TREE_T_H__
#define __TREE_T_H__

#include "database_wrapper.h"
#include "tree_node_t.h"
#include <vector>
#include <iostream>
#include "knowledge_field_t.h"
#include <stack>

using std::vector;
using std::string;
using std::cerr;

class tree_t {
  public:
    tree_t(database_wrapper_t& dbw);
    void show_stats(std::ostream& out = cerr) const;
    string serialize() const;
    void rulealize(knowledge_field_t& kf, const temporal_entity_t* main_object, const temporal_entity_t* timer, const temporal_entity_t* counter, int start_time, int last_time) const;
  private:
    const long double EPS = 1e-7;
    int node_id = 0;

    int max_depth = 3;
    long double max_prob = 0.95;
    int min_elems = 5;

    database_wrapper_t& dbw;

    vector<int> ids;
    vector<int> properties;
    vector<int> times;

    tree_node_t* root;

    void split(tree_node_t* node, const int depth = 0);
    void show_stats(tree_node_t* node, std::ostream& out = cerr) const;
    string serialize(tree_node_t* node) const;
    void add_rules(tree_node_t* node, knowledge_field_t& kf, vector<event_interval_condition_t*> up_conditions, int start_time, int last_time) const;
};

#endif
