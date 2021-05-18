
#include "APlan.h"
#include <ATCore/utils/helpers.h>
//#include <QString>

AGeneralTask::AGeneralTask(int dfd_element_id, DFDElement::Type dfd_type, std::string _name)
    :ANamedObject(_name), mDfdType(dfd_type), mDFDElementId(dfd_element_id)
{

}

int AGeneralTask::dfdElementId() const
{
    return mDFDElementId;
}

void AGeneralTask::addSubTask(ASubTask * sub_task)
{
    mSubTasks.push_back(sub_task);
}

const DFDElement::Type AGeneralTask::dfdType() const
{
    return mDfdType;
}

//================ASubTask=====================
ASubTask::ASubTask(AGeneralTask * parent_task, std::string _name)
    :ANamedObject(_name), m_pParent(parent_task), mDone(false)
{

}

AGeneralTask * ASubTask::parent() const
{
    return m_pParent;
}

void ASubTask::setExecutionString(const std::string & execution_string)
{
    mExecutionString = execution_string;
}

const std::string & ASubTask::executionString() const
{
    return mExecutionString;
}

bool ASubTask::done() const
{
    return mDone;
}

//================APlan======================
APlanTask::APlanTask()
    :closed(false), inProgress(false)
{}

void APlan::addTask(AGeneralTask * _task)
{
    mTasks.push_back(_task);
}

void APlan::addTask(APlanTask * _task, const std::vector<APlanTask*>& prevs)
{
    if (std::find(tasks.begin(), tasks.end(), _task) == tasks.end())
        tasks.push_back(_task);

    if (!predcess.has_vertex(_task))
        predcess.add_vertex(_task);

    for (auto prev : prevs)
        predcess.add_edge(_task, prev, true);
}

/*
const std::vector<AGeneralTask*> & APlan::tasks() const
{
    return mTasks;
}

std::vector<AGeneralTask*> & APlan::tasks()
{
    return mTasks;
}
*/
const std::vector<DisplayTaskGroup> & APlan::displayGroups() const
{
    return mDisplayGroups;
}

void APlan::addDisplayGroup(const DisplayTaskGroup & dg)
{
    mDisplayGroups.push_back(dg);
}

AHierarchyPlanPresentation * APlan::hierarchyPresentation() const
{
    auto pres = new AHierarchyPlanPresentation(predcess);

    AHierarchyPlanPresentation::hierarchy_comparator hcmp(reversed_topological_sort(predcess));

    //for (auto task : tasks)
    //	pres->addTask(task);

    auto _tasks = tasks;
    //std::random_shuffle(_tasks.begin(), _tasks.end());
    std::sort(_tasks.begin(), _tasks.end());
    auto last_it = std::unique(_tasks.begin(), _tasks.end());
    _tasks.erase(last_it, _tasks.end());

    //Make unique general tasks
    std::set<std::wstring> gtasks;
    for (auto t : _tasks)
        gtasks.insert(utf8_to_wstring(t->group));

    for (auto & gtask_name : gtasks)
    {
        auto new_gt = new AHierarchyPlanPresentation::general_group;
        new_gt->name = gtask_name;

        pres->generalTasks.push_back(new_gt);

        auto g_subtasks = _tasks;
        last_it = std::remove_if(g_subtasks.begin(), g_subtasks.end(), [&](const APlanTask * el) {
            return utf8_to_wstring(el->group) != gtask_name;
        });
        g_subtasks.erase(last_it, g_subtasks.end());

        //Fill stages

        std::set<std::wstring> stage_names;
        for(auto gt : g_subtasks)
            stage_names.insert(utf8_to_wstring(gt->stage));

        for (auto & st : stage_names)
        {
            auto new_stage = new AHierarchyPlanPresentation::stage_group;
            new_stage->name = st;

            new_gt->stages.push_back(new_stage);

            new_stage->tasks = g_subtasks;
            last_it = std::remove_if(new_stage->tasks.begin(), new_stage->tasks.end(), [&](const APlanTask * el) {
                return utf8_to_wstring(el->stage) != st;
            });
            new_stage->tasks.erase(last_it, new_stage->tasks.end());

            //Sort tasks
            std::sort(new_stage->tasks.begin(), new_stage->tasks.end(), [&](APlanTask * lhs, APlanTask * rhs) {
                return hcmp.less(lhs, rhs, 2);
            });
        }

        //Sort stages inside group
        std::sort(new_gt->stages.begin(), new_gt->stages.end(), [&](AHierarchyPlanPresentation::stage_group * lhs, AHierarchyPlanPresentation::stage_group * rhs) {
            return hcmp.less(lhs->tasks[0], rhs->tasks[0], 1);
        });
    }

    //Sort groups

    return pres;
}

AHierarchyPlanPresentation::AHierarchyPlanPresentation(const AHierarchyPlanPresentation::graph_t& predcess) :
    tree(hierarchy_comparator(reversed_topological_sort(predcess)))
{

}

void AHierarchyPlanPresentation::addTask(APlanTask* task)
{
    tree.insert(task);
}

bool APlan::isAvailable(APlanTask * task) const
{
    if (task->closed)
        return false;

    bool r = true;
    predcess.forall_adj_verts(task, [&](APlanTask * prev, bool) {
        r = r && prev->closed;
    });
    return r;
}

bool AHierarchyPlanPresentation::hierarchy_comparator::less(APlanTask* lhs, APlanTask* rhs, unsigned idx) const
{
    switch (idx)
    {
    case 0:
        return lhs->group < rhs->group;
    case 1:
    {
        //return lhs->stage < rhs->stage;
        //check if a stage of lhs preceeds a stage of rhs in topological sort
        bool r = check_precedence(_stages_sorted, lhs->stage, rhs->stage);

        //std::wcout << QString::fromStdString(lhs->stage).toStdWString() << (r ? " < " : " > ") << QString::fromStdString(rhs->stage).toStdWString() << std::endl;
        return r;
    }
    case 2:
        //return lhs->name < rhs->name;
        //check if lhs preceeds rhs in topological sort
        return check_precedence(_top_sort, lhs, rhs);
    default:
        throw std::runtime_error("Invalid feature");
    }
}
