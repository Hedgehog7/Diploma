
#ifndef ATPlanner_APlan_H
#define ATPlanner_APlan_H

#include "config.h"
#include <ATCore/ANamedObject.h>
#include <ATCore/edfd/DFDElement.h>
#include <planner_core/core/algorithm/graph.h>
#include <vector>
#include <string>
#include <ATCore/feature_tree.h>
#include <ATCore/simple_tree.h>
#include <iostream>
#include "../global_conf.h"

class ASubTask;
class ADocumentRef;

/*
Describes general task, linked with some EDFD element.
*/

#ifdef EDFD_COVER_EXPORT
	#define EDFD_COVER_API AT_EXPORT
#else
	#define EDFD_COVER_API AT_IMPORT
#endif

class EDFD_COVER_API AGeneralTask : public ANamedObject
{
public:
	AGeneralTask(int dfd_element_id, DFDElement::Type dfd_type, std::string _name);
	int dfdElementId() const;
	void addSubTask(ASubTask * sub_task);
	const DFDElement::Type dfdType() const;
private:
	int mDFDElementId;
	DFDElement::Type mDfdType;
	std::vector<ASubTask*> mSubTasks;
};

class EDFD_COVER_API ASubTask : public ANamedObject
{
public:
	ASubTask(AGeneralTask * parent_task, std::string _name);
	AGeneralTask * parent() const;
	void setExecutionString(const std::string & execution_string);
	const std::string & executionString() const;

	bool done() const;
private:
	AGeneralTask * m_pParent;
	std::string mExecutionString;
	bool mDone;
};

struct DisplayTaskGroup
{
	AGeneralTask * general_task;
	std::vector<ASubTask*> sub_tasks;
};

class EDFD_COVER_API APlanTask
{
public:
	APlanTask();

	std::string name;
	std::string stage, group;
	std::string executorId;
	std::vector<ADocumentRef*> docsIn, docsOut;
	bool closed;
	bool inProgress;
};

class EDFD_COVER_API AHierarchyPlanPresentation
{
public:
    struct EDFD_COVER_API hierarchy_comparator
	{
        using top_sort_t = topological_sort_t<APlanTask*, bool>;

        template<typename ContT, typename ValueT>
        static inline bool check_precedence(const ContT& container, const ValueT& lhs, const ValueT& rhs)
        {
            return std::find(container.begin(), container.end(), lhs) < std::find(container.begin(), container.end(), rhs);
        }

        bool comparable(APlanTask* lhs, APlanTask* rhs, unsigned idx) const
		{
			return true;
		}

		bool less(APlanTask* lhs, APlanTask* rhs, unsigned idx) const;

        hierarchy_comparator(const top_sort_t& top_sort) :
            _top_sort(top_sort)
        {
            std::unordered_set<std::string> used_stages;
            for (APlanTask* task : _top_sort)
            {
                if (used_stages.find(task->stage) == used_stages.end())
                {
                    used_stages.insert(task->stage);
                    _stages_sorted.push_back(task->stage);
                }
            }
        }

        top_sort_t _top_sort;
        std::vector<std::string> _stages_sorted;
    };
//public:
	using tree_t = feature_tree<APlanTask*, hierarchy_comparator, 3>;
    using graph_t = explicit_graph<APlanTask*, bool>;

    AHierarchyPlanPresentation(const graph_t& predcess);
	void addTask(APlanTask* task);

    tree_t tree;

	simple_tree<APlanTask*> plainTree;

	struct stage_group
	{
		std::wstring name;
		std::vector<APlanTask*> tasks;
	};

	struct general_group
	{
		std::wstring name;
		std::vector<stage_group*> stages;
	};

	std::vector<general_group*> generalTasks;
};

class EDFD_COVER_API APlan
{
public:
	void addTask(AGeneralTask * _task);
    void addTask(APlanTask * _task, const std::vector<APlanTask*>& prevs = {});
	//const std::vector<AGeneralTask*> & tasks() const;
	//std::vector<AGeneralTask*> & tasks();
	const std::vector<DisplayTaskGroup> & displayGroups() const;
	void addDisplayGroup(const DisplayTaskGroup & dg);
	AHierarchyPlanPresentation * hierarchyPresentation() const;
	bool isAvailable(APlanTask * task) const;
//private:
	std::vector<AGeneralTask*> mTasks;
	
	std::vector<DisplayTaskGroup> mDisplayGroups;

	std::vector<APlanTask*> tasks;
	explicit_graph<APlanTask*, bool> predcess;
};


#endif
