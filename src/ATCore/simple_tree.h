
#ifndef USCore_simple_tree_h
#define USCore_simple_tree_h

#include <vector>
#include <map>
#include "check.h"
//#include "utils/xml_helpers.h"

template<typename T>
class simple_tree
{
public:
	using value_t = T;

	struct node_t
	{
		~node_t()
		{
			for (auto c : children)
				delete c;
		}

		node_t * create_child()
		{
			auto child = new node_t;
			children.push_back(child);
			return child;
		}

		/*void serialize(_xmlNode * xml_ptr) const
		{
			xmlNodeSetName(xml_ptr, BAD_CAST key.c_str());
			//xmlNodeSetContent(xml_ptr, BAD_CAST value.c_str());

			for (auto & pair : attributes)
			{
				xmlSetProp(xml_ptr, BAD_CAST pair.first.c_str(), BAD_CAST pair.second.c_str());
			}

			for (auto child : children)
			{
				xmlNodePtr xml_child = xmlNewChild(xml_ptr, NULL, BAD_CAST "", BAD_CAST "");
				child->serialize(xml_child);
			}
		}

		void deserialize(_xmlNode * xml_ptr)
		{
			key = std::string((const char*)xml_ptr->name);
			
			//xmlGetConten
			//if (xml_ptr->content)
			//	value = std::string((const char*)xml_ptr->content);

            xml_foreach_child(xml_ptr, [=](_xmlNode *iter){
				auto child = create_child();
				child->deserialize(iter);
                return true;
            });

			
			for (xmlAttr* attribute = xml_ptr->properties; attribute && attribute->name && attribute->children; attribute = attribute->next)
			{
				xmlChar* value = xmlNodeListGetString(xml_ptr->doc, attribute->children, 1);
				//printf("Atributo %s: %s\n", attribute->name, value);

				attributes[std::string((const char*)attribute->name)] = std::string((const char*)value);
				xmlFree(value);
				
			}
		}*/

		bool is_equal(const node_t * other) const
		{
			bool res = (attributes == other->attributes) &&
				(value == other->value) &&
				(key == other->key);
			
			if (children.size() == other->children.size())
			{
				for (int i = 0; i < children.size(); ++i)
				{
					res = res && children[i]->is_equal(other->children[i]);
				}
			}
			else
				res = false;
			
			return res;
		}

		node_t * clone() const
		{
			node_t * new_node = new node_t;
			new_node->key = key;
			new_node->value = value;
			new_node->attributes = attributes;

			for (auto child : children)
			{
				new_node->children.push_back(child->clone());
			}

			return new_node;
		}

		node_t * child(const std::string & _key)
		{
			auto it = std::find_if(children.begin(), children.end(), [&](node_t * el){
				return el->key == _key;
			});
			if (it == children.end())
				return nullptr;
			else
				return *it;
		}

		void swap_children(node_t * lhs, node_t * rhs)
		{
			auto lit = std::find(children.begin(), children.end(), lhs),
				rit = std::find(children.begin(), children.end(), rhs);

			std::swap(*lit, *rit);
		}

		node_t * find_parent(node_t * node)
		{
			auto it = std::find(children.begin(), children.end(), node);
			if (it != children.end())
				return this;
			else
			{
				for (auto child : children)
				{
					auto ch_p = child->find_parent(node);
					if (ch_p)
						return ch_p;
				}

				return nullptr;
			}
		}

		template<typename F>
		void for_each(F fun)
		{
			for (auto child : children)
				fun(child);
		}

		value_t value, key;
		std::vector<node_t*> children;
		std::map<value_t, value_t> attributes;
	};

	simple_tree()
		:pRoot(new node_t)
	{}

	~simple_tree()
	{
		delete pRoot;
	}

	node_t * root() const
	{
		return pRoot;
	}

	bool is_equal(const simple_tree & other) const
	{
		return pRoot->is_equal(other.pRoot);
	}
	
	node_t * find_parent(node_t * pnode)
	{
		return pRoot->find_parent(pnode);
	}

	void remove_node(node_t * pnode)
	{
		perform_check(pnode != pRoot);
		auto parent = find_parent(pnode);

		auto it = std::find(parent->children.begin(), parent->children.end(), pnode);
		parent->children.erase(it);
		delete pnode;
	}

	void replace_node(node_t * old_node, node_t * new_node)
	{
		auto parent = find_parent(old_node);
		auto it = std::find(parent->children.begin(), parent->children.end(), old_node);
		delete old_node;

		*it = new_node;
	}
private:
	node_t * pRoot;
};

#endif
