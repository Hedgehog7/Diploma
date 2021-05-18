#ifndef __TREE_NODE_T_H__
#define __TREE_NODE_T_H__

#include "database_wrapper.h"
#include <vector>

using std::vector;

class tree_node_t {
public:
  tree_node_t(database_wrapper_t& database_wrapper);
  void set_childs(tree_node_t* l, tree_node_t* r);
  void set_prop_id(const int prop_id);
  void set_time_id(const int time_id);
  void set_prop_val(const int prop_val);
  void set_leaf(const bool leaf);
  void set_ids(const vector<int>& ids);

  tree_node_t* get_left() const {return l;}
  tree_node_t* get_right() const {return r;}
  int get_prop_id() const {return prop_id;}
  int get_time_id() const {return time_id;}
  int get_prop_val() const {return prop_val;}
  int get_ids_number() const {return ids.size();}
  bool is_leaf() const {return leaf;}
  vector<int> get_ids() const {return ids;}
  long double get_max_prob() const;

  long double get_metric();

  void init_class();
  string get_class() const {return node_class;}
private:
  database_wrapper_t& dbw;

  vector<int> ids;
  tree_node_t* l = nullptr;
  tree_node_t* r = nullptr;
  int prop_id;
  int prop_val;
  int time_id;
  bool leaf = true;
  string node_class = "";
};

#endif
