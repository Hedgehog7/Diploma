
#ifndef USCore_feature_tree_h
#define USCore_feature_tree_h

#include "check.h"
#include "utils/helpers.h"
#include <set>
#include <functional>
#include <algorithm>

template<bool B>
struct bool_identity {  };

template<typename T, typename C, unsigned N>
class feature_tree
{
private:
	using value_t = T;
	using cmp_t = C;
//	using value_is_ptr = std::is_pointer<value_t>::value;

        struct cmp_eq_t
	{
            cmp_eq_t(const cmp_t& comparator)
                :comparator(comparator)
            {}

            bool less(const value_t & lhs, const value_t & rhs, unsigned idx) const
            {
                return comparator.less(lhs, rhs, idx);
            }

            bool equal(const value_t & lhs, const value_t & rhs, unsigned idx) const
            {
                bool r = (!comparator.less(lhs, rhs, idx)) && (!comparator.less(rhs, lhs, idx));
                if (r)
                {
                    perform_check(comparator.comparable(lhs, rhs, idx));
                }
                return r;;
            }

            const cmp_t& comparator;
	};

	struct group_node_base_t;

	struct node_base_t
	{
		enum class type_t {group, leaf,/* root,*/ invalid};

                node_base_t(type_t _type, const cmp_t& comparator, group_node_base_t * _parent = nullptr)
                        :m_type(_type), parent(_parent), comparator(comparator)
		{
		}

		virtual ~node_base_t()
		{}

		virtual const value_t & representative() const = 0;
		virtual std::ostream & print(std::ostream & os, unsigned indent) const = 0;
		virtual size_t size() const = 0;

		virtual bool verify() const
		{
			return true;
		}

		virtual void for_each_element(std::function<void(const value_t &)> fun) const = 0;

		static std::ostream & print_indent(std::ostream & os, unsigned indent)
		{
			while (indent-- > 0)
				os << "..";
			return os;
		}

		const type_t m_type;
		group_node_base_t * parent;
                const cmp_t& comparator;
	};

	struct leaf_t : public node_base_t
	{
                leaf_t(const cmp_t& comparator, group_node_base_t * _parent = nullptr)
                :node_base_t(node_base_t::type_t::leaf, comparator, _parent)
		{
		}

		template<bool Type>
		void free_element(value_t & val, bool_identity<Type>) const {}

		void free_element(value_t & val, bool_identity<true>) const
		{
			delete val;
		}

		~leaf_t()
		{
			for (auto & el : elements)
				free_element(el, bool_identity<std::is_pointer<value_t>::value>());
		}

		

		virtual const value_t & representative() const override
		{
			return elements[0];
		}

		virtual std::ostream & print(std::ostream & os, unsigned indent) const override
		{
			node_base_t::print_indent(os, indent);
			return os << "L[" << print_range(elements.begin(), elements.end(), ';') << "](" << elements.size() << " els)" << std::endl;
		}

		bool empty() const
		{
			return elements.empty();
		}

		void erase(const value_t & el)
		{
			auto it = std::find(elements.begin(), elements.end(), el);
			perform_check(it != elements.end());
			elements.erase(it);
		}

		virtual size_t size() const override
		{
			return elements.size();
		}

		virtual void for_each_element(std::function<void(const value_t &)> fun) const override
		{
			for (auto & el : elements)
				fun(el);
		}

		bool can_store(const value_t & val)
		{
                        cmp_eq_t cmp(this->comparator);
			//Assert not checked feautures also coincide
			for (unsigned i = 0; i < N; ++i)
			{
				if (!cmp.equal(representative(), val, i))
					return false;
			}

			return true;
		}

		std::vector<value_t> elements;
	};

	struct group_node_base_t : public node_base_t
	{
                group_node_base_t(typename node_base_t::type_t _type, const cmp_t& comparator, group_node_base_t * _parent)
                        :node_base_t(_type, comparator, _parent)
		{}

		virtual node_base_t * get_child(const value_t & el) const = 0;
		virtual void add_child(node_base_t * new_child) = 0;
		virtual void remove_child(node_base_t * pchild) = 0;
		virtual void optimize(){};
	};

	struct group_node_t : public group_node_base_t
	{
                group_node_t(int idx, const cmp_t& comparator, /*const value_t & _repr,*/ group_node_base_t * _parent = nullptr)
                :group_node_base_t(node_base_t::type_t::group, comparator, _parent),/* repr(_repr),*/ featureIndex(idx)
		{
		}

		~group_node_t()
		{
			for (auto c : children)
				delete c;
		}

		virtual void add_child(node_base_t * new_child) override
		{
			{
                                cmp_eq_t cmp(this->comparator);
				for (auto c : children)
					perform_check(!cmp.equal(c->representative(), new_child->representative(), this->featureIndex), "representatives are equal");

				if (new_child->m_type == node_base_t::type_t::group)
					perform_check(static_cast<group_node_t*>(new_child)->featureIndex == this->featureIndex + 1, "too different feature indices");
			}

			children.push_back(new_child);
			new_child->parent = this;
		}

		virtual void remove_child(node_base_t * pchild) override
		{
			auto it = std::find(children.begin(), children.end(), pchild);
			perform_check(it != children.end());
			pchild->parent = nullptr;
			children.erase(it);
		}

		virtual node_base_t * get_child(const value_t & el) const override
		{
                        cmp_eq_t cmp(this->comparator);

			for (auto pchild : children)
			{
				if (cmp.equal(pchild->representative(), el, featureIndex))
					return pchild;
			}
			return nullptr;
		}

		virtual void optimize() override
		{
			if (children.size() == 1)
			{
				//Replace it with single children
				auto pp = this->parent;
				auto lone_child = children[0];
				remove_child(lone_child);
				pp->remove_child(this);
				pp->add_child(lone_child);
				delete this;
			}
		}

		virtual const value_t & representative() const override
		{
			//return repr;
			return children[0]->representative();
		}

		virtual std::ostream & print(std::ostream & os, unsigned indent) const override
		{
			node_base_t::print_indent(os, indent) << "N" << featureIndex << '(';
			if (children.empty())
				os << "*";
			else
				os << representative();

			os << ')' << std::endl;

			for (auto pchild : children)
				pchild->print(os, indent + 1);
			return os;
		}

		virtual size_t size() const override
		{
			size_t res = 0;
			for (auto c : children)
				res += c->size();
			return res;
		}

		virtual bool verify() const override
		{
			//Check that children split elements correctly

			using child_data_t = std::vector < value_t > ;
			std::vector<child_data_t> child_elements;

                        cmp_eq_t cmp(this->comparator);

			for (auto c : children)
			{
				child_data_t child_data;
				c->for_each_element([&](const value_t & val){
					child_data.push_back(val);
				});

				std::sort(child_data.begin(), child_data.end(), [&](value_t lhs, value_t rhs){
					return cmp.less(lhs, rhs, this->featureIndex);
				});

				if (!cmp.equal(*child_data.begin(), *child_data.rbegin(), this->featureIndex))
				{
					return false;
				}

				child_elements.push_back(child_data);
			}

			//And then check recursive
			for (auto c : children)
			{
				if (!c->verify())
					return false;
			}

			return true;
		}

		virtual void for_each_element(std::function<void(const value_t &)> fun) const override
		{
			for (auto ch : children)
				ch->for_each_element(fun);
		}

		/*
		void try_optimize()
		{
			if (children.size() <= 1)
				return;

			cmp_eq_t cmp;

			//Try group
			unsigned distinguished_feature_idx = std::numeric_limits<unsigned>::max();
			for (unsigned i = this->featureIndex; i < N; ++i)
			{
				if (!cmp.equal(pleaf->representative(), new_el, i))
				{
					distinguished_feature_idx = i;
					break;
				}
			}

		}*/
		

		const int featureIndex;
		//value_t repr;

		std::vector<node_base_t*> children;
	};
	/*
	struct root_node_t : public group_node_base_t
	{
		root_node_t()
			:group_node_base_t(type_t::root, nullptr), child(nullptr)
		{
		}

		~root_node_t()
		{
			delete child;
		}

		virtual std::ostream & print(std::ostream & os, unsigned indent) const override
		{
			return child ? child->print(os, indent) : os;
		}

		virtual size_t size() const override
		{
			return child ? child->size() : 0ULL;
		}

		virtual const value_t & representative() const override
		{
			return child->representative();
		}

		virtual node_base_t * get_child(const value_t & el) const override
		{
			return child;
		}

		virtual void add_child(node_base_t * new_child) override
		{
			perform_check(child == nullptr);
			child = new_child;
			child->parent = this;
		}

		virtual void remove_child(node_base_t * pchild) override
		{
			perform_check(child == pchild);
			child = nullptr;
		}

		virtual bool verify() const override
		{
			return child ? child->verify() : true;
		}

		virtual void for_each_element(std::function<void(const value_t &)> fun) const override
		{
			if (child)
				child->for_each_element(fun);
		}

		node_base_t * child;
	};*/
public:
	feature_tree()
                :pRoot(new group_node_t(0, comparator))
	{}

        feature_tree(cmp_t comparator)
                : comparator(comparator), pRoot(new group_node_t(0, comparator))
        {}

	~feature_tree()
	{
		delete pRoot;
	}

	void insert(const value_t & new_el)
	{
		//cout << "Inserting " << new_el << std::endl;
		//Find appropriate leaf

		std::vector<bool> checked_features(N, false);
		node_base_t * last_node = node_for_element(new_el, checked_features);

		if (last_node->m_type == node_base_t::type_t::leaf)
		{
			auto pleaf = static_cast<leaf_t*>(last_node);
			pleaf->elements.push_back(new_el);

/*			//Check if new element can be distinguished from already existing
			cmp_eq_t cmp;
			unsigned distinguished_feature_idx = std::numeric_limits<unsigned>::max();
			for (unsigned i = N; i > 0; --i)
			{
				if (!checked_features[i - 1])
				{
					if (!cmp.equal(pleaf->representative(), new_el, i - 1))
					{
						distinguished_feature_idx = i - 1;
						break;
					}
				}
			}

			if (distinguished_feature_idx == std::numeric_limits<unsigned>::max())	//If no - just insert
			{
				pleaf->elements.push_back(new_el);
			}
			else  //If yes
			{
				pleaf->elements.push_back(new_el);

				//Find first group withh index less than adding (higher group)

				node_base_t * prev_node = pleaf;
				auto p_higher_group = static_cast<group_node_base_t*>(pleaf->parent);

				while ((p_higher_group != pRoot) && (static_cast<group_node_t*>(p_higher_group)->featureIndex > distinguished_feature_idx))
				{
					prev_node = p_higher_group;
					p_higher_group = static_cast<group_node_base_t*>(p_higher_group->parent);
				}

				//Split the branch
				p_higher_group->remove_child(prev_node);

				auto new_gr = new group_node_t(distinguished_feature_idx, new_el, p_higher_group);
				p_higher_group->add_child(new_gr);

				std::vector<node_base_t*> branches;
				split_branch(prev_node, branches, distinguished_feature_idx);

				if (std::find(branches.begin(), branches.end(), prev_node) == branches.end())
					delete prev_node;

				for (auto new_branch : branches)
					new_gr->add_child(new_branch);
			}*/
		}
		else
		{
                        auto new_leaf = new leaf_t(comparator, static_cast<group_node_base_t*>(last_node));
			new_leaf->elements.push_back(new_el);

			auto gr_node = static_cast<group_node_t*>(last_node);
			
			bool added = false;

                        cmp_eq_t cmp(comparator);

			//Check current node restriction
			leaf_t * coinciding_child = nullptr;
			for (auto child : gr_node->children)
			{
				if (cmp.equal(child->representative(), new_el, gr_node->featureIndex))
				{
					perform_check(child->m_type == node_base_t::type_t::leaf);
					coinciding_child = static_cast<leaf_t*>(child);
					break;
				}
			}

			if (coinciding_child)	//Given element can not be added to the current node level
			{
				gr_node->remove_child(coinciding_child);

				
				/*group_node_t * cur_group = gr_node;

				do
				{
					++cur_feature_idx;
					auto new_group = new group_node_t(cur_feature_idx, coinciding_child->representative());
					cur_group->add_child(new_group);
					cur_group = new_group;
				} while (cmp.equal(coinciding_child->representative(), new_el, cur_feature_idx));*/

				unsigned cur_feature_idx = gr_node->featureIndex + 1;
				while (cmp.equal(coinciding_child->representative(), new_el, cur_feature_idx))
					++cur_feature_idx;

                                auto bottom_group = new group_node_t(cur_feature_idx, comparator);
				bottom_group->add_child(coinciding_child);
				bottom_group->add_child(new_leaf);

				group_node_t * cur_group = bottom_group;
				for (unsigned i = cur_feature_idx - 1; i > gr_node->featureIndex; --i)
				{
                                        auto new_group = new group_node_t(i, comparator);
					new_group->add_child(cur_group);
					cur_group = new_group;
				}

				gr_node->add_child(cur_group);
					

				added = true;
			}
			/*else
			{
				
				if (gr_node->children > 2)
				{
				unsigned conincided_feature_idx = gr_node->featureIndex;

				
				for (; conincided_feature_idx < N; ++conincided_feature_idx)
				{
				for (auto child : gr_node->children)
				{
				if (cmp.equal(gr_node->representative(), new_el, conincided_feature_idx))
				{
				perform_check(child->m_type == node_base_t::type_t::leaf);

				added = true;
				break;
				}
				}
				}
				}
			}*/
			
			if (!added)
				gr_node->add_child(new_leaf);

			

			//gr_node->try_optimize();
		}
		
		//cout << "Insertion done: " << std::endl << *this << std::endl;
	}

	void erase(const value_t & el)
	{
		//Find leaf with the element
		std::vector<bool> checked_features(N, false);
		node_base_t * last_node = node_for_element(el, checked_features);

		perform_check(last_node != nullptr);
		perform_check(last_node->m_type == node_base_t::type_t::leaf);

		//Remove element from the leaf
		leaf_t * pleaf = static_cast<leaf_t*>(last_node);
		pleaf->erase(el);

		//If leaf is empty - delete it
		if (pleaf->empty())
		{
			auto cur_node = static_cast<group_node_t*>(pleaf->parent);
			cur_node->remove_child(pleaf);
			delete pleaf;

			//And optimize parent
			//parent->optimize();

			while (cur_node->children.empty() && cur_node->parent)
			{
				auto pp = static_cast<group_node_t*>(cur_node->parent);
				pp->remove_child(cur_node);
				delete cur_node;
				cur_node = pp;
			}

			if (cur_node->children.size() == 1)
			{
				//Possible linear branch discovered, get to its bottom first
				bool is_linear_branch = true;

				while (is_linear_branch && (cur_node->children[0]->m_type == node_base_t::type_t::group))
				{
					if (cur_node->children.size() != 1)
					{
						is_linear_branch = false;
						break;
					}

					cur_node = static_cast<group_node_t*>(cur_node->children[0]);
				}

				if (is_linear_branch)
				{
					cmp_eq_t cmp;

					while ((cur_node->children.size() == 1) && cur_node->parent)
					{
						//Replace it with single children
						auto pp = static_cast<group_node_t*>(cur_node->parent);
						auto lone_child = cur_node->children[0];

						//Check that given child will be distinguished by parent feature from other children
						bool distinguishes_from_others = true;
						for (auto c : pp->children)
						{
							if (c != cur_node)
								distinguishes_from_others = distinguishes_from_others && (!cmp.equal(c->representative(), cur_node->representative(), cur_node->featureIndex));
						}

						if (distinguishes_from_others)
						{
							cur_node->remove_child(lone_child);
							pp->remove_child(cur_node);
							pp->add_child(lone_child);
							delete cur_node;
							cur_node = pp;
						}
						else
							break;
					}
				}
			}
		}
		else
		{
			auto parent = static_cast<group_node_t*>(pleaf->parent);
//			parent->repr = pleaf->representative();
		}
	}

	size_t size() const
	{
		return pRoot->size();
	}

	bool verify() const
	{
		return true;
	}

	leaf_t * find_leaf(const value_t & new_el) const
	{
		std::vector<bool> checked_features(N, false);
		node_base_t * last_node = node_for_element(new_el, checked_features);

		if (last_node->m_type == node_base_t::type_t::leaf)
		{
			cmp_eq_t cmp;
			for (unsigned i = 0; i < N; ++i)
			{
				if ((!checked_features[i]) && (!cmp.equal(new_el, last_node->representative(), i)))
					return nullptr;
			}
			return static_cast<leaf_t*>(last_node);
		}
		else
			return nullptr;

	}

	friend std::ostream & operator<<(std::ostream & os, const feature_tree & ftree)
	{
		return ftree.pRoot->print(os, 0);
	}

	struct level_iterator
	{
		level_iterator(unsigned level_id = 0, node_base_t * p_node = nullptr)
			:levelId(level_id), pNode(p_node)
		{
			perform_check(levelId < N);
		}

		template<typename F>
		void for_each_branch(F fun) const
		{
			perform_check(levelId < N-1);
			if (pNode)
			{
				switch (pNode->m_type)
				{
				case node_base_t::type_t::leaf:
					fun(level_iterator(levelId + 1, pNode));
					break;
				case node_base_t::type_t::group:
				{
					auto pgr = static_cast<group_node_t*>(pNode);
					/*if (pgr->featureIndex == levelId)
					{*/
						for (auto c : pgr->children)
						{
							fun(level_iterator(levelId + 1, c));
						}
					/*}
					else
						fun(level_iterator(levelId + 1, pNode));
					*/
					break;
				}
				default:
					throw std::runtime_error("Invalid node");
				}
			}
		}

		template<typename F>
		void for_each_element(F fun) const
		{
			//perform_check(levelId == N - 1);
			pNode->for_each_element(fun);
		}

		const value_t & representative() const
		{
			return pNode->representative();
		}

		/*bool empty() const
		{

		}*/
	private:
		unsigned levelId;
		node_base_t * pNode;
	};

	level_iterator top_level_iterator() const
	{
		return level_iterator(0, pRoot);
	}

	template<typename F>
	void for_each(F fun)
	{
		top_level_iterator().for_each_element(fun);
	}
private:
	node_base_t * node_for_element(const value_t & val, std::vector<bool> & checked_features) const
	{
		//if (!pRoot->child)
		//	return pRoot;
		
		node_base_t * cur_node = pRoot;// ->child;
		while (cur_node->m_type != node_base_t::type_t::leaf)
		{
			auto gr_node = static_cast<group_node_t*>(cur_node);
			if (gr_node->featureIndex != -1)
				checked_features[gr_node->featureIndex] = true;
			auto next_child = gr_node->get_child(val);
			if (!next_child)
				return gr_node;
			else
				cur_node = next_child;
		}

		if (static_cast<leaf_t*>(cur_node)->can_store(val))
			return cur_node;
		else
			return cur_node->parent;
	}

        struct std_less_t
	{
                std_less_t(unsigned f_idx, const cmp_t& comparator)
                        :featureIndex(f_idx), comparator(comparator)
		{}

		bool operator()(const value_t & lhs, const value_t & rhs) const
		{
                        return comparator.less(lhs, rhs, featureIndex);
		}

		unsigned featureIndex;
                const cmp_t& comparator;
	};

	void split_branch(node_base_t * branch, std::vector<node_base_t*> & branches, unsigned feature_id) const
	{
		switch (branch->m_type)
		{
			case node_base_t::type_t::leaf:
			{
                                std::multiset<value_t, std_less_t> groups({}, std_less_t(feature_id, comparator));
				auto old_leaf = static_cast<leaf_t*>(branch);

				groups.insert(old_leaf->elements.begin(), old_leaf->elements.end());
				if (groups.size() > 1)
				{
					//delete branch;

					for (auto it = groups.begin(); it != groups.end(); it = groups.upper_bound(*it))
					{
						auto new_leaf = new leaf_t;
						for (auto iit = it; iit != groups.upper_bound(*it); ++iit)
							new_leaf->elements.push_back(*iit);
						branches.push_back(new_leaf);
					}
				}
				else
				{
					static_cast<group_node_base_t*>(branch->parent)->remove_child(branch);
					branches.push_back(branch);
				}
				break;
			}
			case node_base_t::type_t::group:
			{
				using splitted_child_t = std::vector < node_base_t* > ;
				std::vector<splitted_child_t> groups;

				auto p_group = static_cast<group_node_t*>(branch);

				auto group_children = p_group->children;
				for (auto c : group_children)
				{
					splitted_child_t group;
					split_branch(c, group, feature_id);
					groups.push_back(group);
				}

				//Rebuild to vertical groups
				struct sorted_el
				{
					unsigned group_id, element_id;
				};

				std::vector<sorted_el> sorted_els;

				for (unsigned i = 0; i < groups.size(); ++i)
				{
					for (unsigned j = 0; j < groups[i].size(); ++j)
					{
						sorted_el new_el;
						new_el.group_id = i;
						new_el.element_id = j;
						sorted_els.push_back(new_el);
					}
				}

                                std_less_t std_cmp(feature_id, comparator);
				std::sort(sorted_els.begin(), sorted_els.end(), [&](const sorted_el & lhs, const sorted_el & rhs){
					return std_cmp(groups[lhs.group_id][lhs.element_id]->representative(), groups[rhs.group_id][rhs.element_id]->representative());
				});

				auto first = sorted_els.begin();

				while (first != sorted_els.end())
				{
					auto last_group_it = first + 1;
                                        while ((last_group_it != sorted_els.end()) && (!comparator.less(groups[first->group_id][first->element_id]->representative(), groups[last_group_it->group_id][last_group_it->element_id]->representative(), feature_id)))
						++last_group_it;

					const unsigned el_count = std::distance(first, last_group_it);
					//cout << "Group has " << el_count << " elements" << std::endl;
					if (el_count == 1)
					{
						auto p_el = groups[first->group_id][first->element_id];
						branches.push_back(p_el);
					}
					else
					{
						auto new_gr = new group_node_t(p_group->featureIndex);	//groups[first->group_id][first->element_id]->representative()
						for (auto it = first; it != last_group_it; ++it)
							new_gr->add_child(groups[it->group_id][it->element_id]);
						branches.push_back(new_gr);
					}

					first = last_group_it;
				}

				//delete p_group;

				break;
			}
			default:
				throw std::runtime_error("Not implemented");
		}

	}
private:
        cmp_t comparator;
        //root_node_t * pRoot;
	group_node_t * pRoot;
};
#endif
