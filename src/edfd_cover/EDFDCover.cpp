#include "EDFDCover.h"
#include "EDFDCoverInternal.h"
#include <core/state_space_solver.h>
#include "heuristic.h"
#include <algorithm>
#include <unordered_map>
#include <ATProject/ADocumentRef.h>
#include <ATProject/AProject.h>

ostream& operator << (ostream& os, edfd_element::type_t t)
{
    switch (t)
    {
    case edfd_element::type_t::entity:
        return os << "entity";
    case edfd_element::type_t::process:
        return os << "process";
    case edfd_element::type_t::nf_process:
        return os << "nf_process";
    case edfd_element::type_t::storage:
        return os << "storage";
    default:
        return os << "unknown";
    }
}

std::ostream& operator << (std::ostream& os, const edfd_element& rhs)
{
    return os << rhs.id << ", \"" << rhs.name << "\", " << rhs.type;
}

ostream& operator << (ostream& os, edfd_graph edfd)
{
    os << "EDFD graph:\n";
    print_explicit_graph(edfd, os, 2);
    return os;
}

edfd_element::type_t convertDFDElementType(DFDElement::Type t)
{
    switch (t)
    {
    case DFDElement::Type::Entity:
        return edfd_element::type_t::entity;
    case DFDElement::Type::Function:
        return edfd_element::type_t::process;
    case DFDElement::Type::NFFunction:
        return edfd_element::type_t::nf_process;
    case DFDElement::Type::Storage:
        return edfd_element::type_t::storage;
    default:
        return edfd_element::type_t::entity;
    }
}

edfd_graph convertDocumentToGraph(const EDFDDocument* common_dfd)
{
    //remember source element for each element from document
    unordered_map<DFDElement*, edfd_element> elementsMap;
    vector<edfd_element> elements; //for edfd_graph constructor

    for (auto src_el : common_dfd->elements())
    {
        edfd_element el;
        el.id = elements.size();
		auto tp = src_el->type();
        el.type = convertDFDElementType(tp);
        el.name = src_el->name();
        elements.push_back(el);
        elementsMap[src_el.get()] = el;
    }

    using connection_t = std::tuple<edfd_element, edfd_element, edfd_connection>;
    vector<connection_t> connections;
    for (auto src_con : common_dfd->connections())
    {
        auto from = src_con->source();
        auto to = src_con->dest();

        //add forward edges
        for (const std::string& edge : src_con->std_d())
        {
            //assume there are no new elements on the ends of connections
            //don't check if elements exist in map
            connections.emplace_back(elementsMap[from.get()], elementsMap[to.get()], edge);
        }

        //add backward edges
        for (const std::string& edge : src_con->dts_d())
        {
            //assume there are no new elements on the ends of connections
            //don't check if elements exist in map
            connections.emplace_back(elementsMap[to.get()], elementsMap[from.get()], edge);
        }
    }

    return edfd_graph(elements, connections);
}

//result state and map from used SDP index to SDP instance name
std::pair<edfd_cover_state, std::vector<std::string>> roughCover(const edfd_graph& src_graph, const vector<PlannerSDPRepresentation>& sdps)
{
    using edfd_t = transition_system<edfd_cover>;

    std::vector<edfd_graph> edfd_sdps;
    std::vector<std::string> result;

    //collect main parts of SDPs to find rough cover
    for (const PlannerSDPRepresentation& sdp : sdps)
        edfd_sdps.push_back(sdp.getMainPartEDFD());

    edfd_t ts(src_graph, edfd_sdps, vector<size_t>{}, false);
    state_space_solver<edfd_t> solver(ts, cout, edfd_t::state_t());

    edfd_t::state_t result_state;
    bool r = solver.solve<simple_heuristic>(false, "A*", result_state);
    if (!r)
        cout << "Solution not found!" << std::endl;

    //collect used SDPs
    for (size_t sdp_id : result_state.fragment_instances)
        for (const edfd_element& v : src_graph.get_vertices())
            if (v.type == edfd_element::type_t::nf_process)
                for (const auto& v_id_to_instance : result_state.cover)
                    if (result_state.fragment_instances[v_id_to_instance.second] == sdp_id &&
                            v_id_to_instance.first == v.id)
                        result.emplace_back(v.name);

    return make_pair(std::move(result_state), std::move(result));
}

//maps SDP instance index in used_sdps vector to used fragments of SDP instances
using SDP_to_fragments_t = std::unordered_multimap<std::size_t, std::size_t>;

//contains pointers to used SDPs after rough cover, their IDs and names
//those IDs are necessary because there can be several instances of one SDP
using used_sdps_t = std::vector<std::tuple<PlannerSDPRepresentation*, std::size_t, std::string>>;

//! \param current_state is already rough covered state, will be precise covered
//! \returns multimap with association from SDP in used_sdps to fragment ids used in cover
SDP_to_fragments_t preciseCover(const edfd_graph& src_graph, edfd_cover_state& current_state, const used_sdps_t& used_sdps)
{
    using edfd_t = transition_system<edfd_cover>;

    SDP_to_fragments_t result;

    std::vector<edfd_graph> edfd_sdp_fragments;
    std::vector<size_t> main_parts_for_fragments;
    std::vector<std::pair<std::size_t, std::size_t>> fragment_ids; //for association

    //collect optional parts of SDPs to find precise cover
    for (std::size_t i = 0; i < used_sdps.size(); ++i)
    {
        const used_sdps_t::value_type& sdp_instance = used_sdps[i];
        //mark main part as used already
        result.emplace(i, get<0>(sdp_instance)->getMainPart().second);

        for (const PlannerSDPRepresentation::fragment_edfd_t& frag_edfd : get<0>(sdp_instance)->getOptionalParts())
        {
            edfd_sdp_fragments.push_back(frag_edfd.first);
            main_parts_for_fragments.push_back(i);
            fragment_ids.emplace_back(i, frag_edfd.second);
        }
    }

    edfd_t ts(src_graph, edfd_sdp_fragments, main_parts_for_fragments, true);
    state_space_solver<edfd_t> solver(ts, cout, current_state); //start from rough covered state

    bool r = solver.solve<simple_heuristic>(false, "A*", current_state); //save result back to current_state variable
    if (!r)
        cout << "Solution not found!" << std::endl;

    //collect used fragments, skip first values because they point to main parts of SDPs we already used in rough cover
    auto begin = current_state.fragment_instances.begin();
    std::advance(begin, used_sdps.size());
    for (auto it = begin; it != current_state.fragment_instances.end(); ++it)
        result.insert(fragment_ids[*it]);

    return result;
}

APlan* fillPlan(AProject* proj, const SDP_to_fragments_t& used_fragments, const used_sdps_t& used_sdps)
{
    APlan* result = new APlan();

    for (const typename used_sdps_t::value_type& sdp_with_id : used_sdps)
    {
        std::unordered_map<std::size_t, APlanTask*> plan_tasks;
        std::unordered_map<std::size_t, ADocumentRef*> docs;

        const PlannerSDPRepresentation* planner_sdp = get<0>(sdp_with_id);
        const SDP& sdp = planner_sdp->getOriginalSDP();
        const string sdp_instance_name = get<2>(sdp_with_id) + "(" + sdp.name() + ")" + (get<1>(sdp_with_id) > 0 ? "[" + std::to_string(get<1>(sdp_with_id)) + "]" : "");
        for (const typename SDP_to_fragments_t::value_type& it : used_fragments)
        {
            //all dependencies between tasks should be managed within one group
            if (used_sdps[it.first] == sdp_with_id)
            {
                std::cout << "Used fragment " << it.second << " of SDP " << sdp_instance_name << endl;

                //first step, create all task instances
                const SDP::Fragment& frag = sdp.find_fragment(it.second);
                for (const auto& doc_desc : frag.documents)
                {
                    ADocumentRef* doc_ref = proj->generateRef(doc_desc.second.extension);
                    docs.emplace(doc_desc.first, doc_ref);
                }

                sdp.for_each_task_in_fragment(it.second, [&](const SDP::Task& task)
                {
                    auto plan_task = new APlanTask();
                    plan_task->name = task.name;
                    plan_task->stage = task.stage;
                    plan_task->executorId = task.executor;
                    plan_task->group = sdp_instance_name;

                    for (std::size_t id : task.input_document_ids)
                    {
                        auto doc_it = docs.find(id);
                        perform_check(doc_it != docs.end());
                        plan_task->docsIn.push_back(doc_it->second);
                    }
                    for (std::size_t id : task.output_document_ids)
                    {
                        auto doc_it = docs.find(id);
                        perform_check(doc_it != docs.end());
                        plan_task->docsOut.push_back(doc_it->second);
                    }

                    perform_check(plan_tasks.emplace(task.id, plan_task).second);

                    //add task to plan without specifying predecessors
                    result->addTask(plan_task);
                });
            }
        }

        for (auto it : used_fragments)
        {
            if (used_sdps[it.first] == sdp_with_id)
            {
                //second step, collect predecessors and add tasks to plan
                sdp.for_each_task_in_fragment(it.second, [&plan_tasks, result](const SDP::Task& task){
                    auto plan_task = plan_tasks.find(task.id)->second; //assume task 100% found

                    std::vector<APlanTask*> predecessors;
                    for (std::size_t id : task.predcessor_task_ids)
                    {
                        auto f_it = plan_tasks.find(id);

                        //maybe predecessor is not used, don't worry about it
                        if (f_it != plan_tasks.end())
                            predecessors.push_back(f_it->second);
                    }

                    //specify predecessors for task
                    result->addTask(plan_task, predecessors);
                });
            }
        }
    }

    return result;
}

APlan* EDFDCover::buildPlan(AProject* proj, const EDFDDocument* common_dfd, const vector<SDP>& sdps)
{
    edfd_graph src_graph = convertDocumentToGraph(common_dfd);
    vector<PlannerSDPRepresentation> planner_sdps;

    for (const SDP& sdp : sdps)
        planner_sdps.emplace_back(sdp);

    std::vector<std::string> used_sdps_names;
    edfd_cover_state rough_covered_state;
    std::tie(rough_covered_state, used_sdps_names) = roughCover(src_graph, planner_sdps);

    used_sdps_t used_sdps;
    //count number of instances for each used SDP and save instance IDs
    std::unordered_map<std::size_t, std::size_t> used_sdps_instance_ids;
    for (std::size_t i = 0; i < rough_covered_state.fragment_instances.size(); ++i)
        used_sdps.emplace_back(&planner_sdps[rough_covered_state.fragment_instances[i]], used_sdps_instance_ids[rough_covered_state.fragment_instances[i]]++, used_sdps_names[i]);

    auto used_fragments = preciseCover(src_graph, rough_covered_state, used_sdps);
    return fillPlan(proj, used_fragments, used_sdps);
}
