#include "tree_node_t.h"
#include <cmath>

tree_node_t::tree_node_t(database_wrapper_t& database_wrapper) : dbw(database_wrapper)
{
}

void tree_node_t::set_childs(tree_node_t* l, tree_node_t* r) {
  this->l = l;
  this->r = r;
}

void tree_node_t::set_prop_id(const int prop_id) {
  this->prop_id = prop_id;
}

void tree_node_t::set_time_id(const int time_id) {
  this->time_id = time_id;
}

void tree_node_t::set_leaf(const bool leaf) {
  this->leaf = leaf;
}

void tree_node_t::set_prop_val(const int prop_val) {
  this->prop_val = prop_val;
}

long double tree_node_t::get_metric() {
  long double result = 0.0;
  map<string, int> mp;
  int total = ids.size();
  for (int id : ids)
      ++mp[dbw.get_class(id)];
  for (const std::pair<string, int>& it : mp) {
    long double prob = it.second;
    prob /= total;
    result -= prob * log2(prob);
    //result -= it.second * log2(it.second);
  }
  return result;
}

void tree_node_t::set_ids(const vector<int>& ids) {
  this->ids.clear();
  this->ids.reserve(ids.size());
  for (int it : ids)
    this->ids.push_back(it);
}

long double tree_node_t::get_max_prob() const {
  map<string, int> mp;
  for (int id : ids)
      ++mp[dbw.get_class(id)];
  int max_cnt = 0;
  for (const std::pair<string, int>& it : mp)
    max_cnt = std::max(max_cnt, it.second);
  return 1.0 * max_cnt / ids.size();
}

void tree_node_t::init_class() {
  map<string, int> mp;
  for (int id : ids)
      ++mp[dbw.get_class(id)];
  int best_cnt = -1;
  for (const std::pair<string, int>& it : mp)
    if (best_cnt < it.second) {
      best_cnt = it.second;
      node_class = it.first;
    }
}
