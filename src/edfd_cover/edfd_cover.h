
#ifndef EDFDCover_edfd_cover_h
#define EDFDCover_edfd_cover_h

#include <core/algorithm/math.h>
#include <core/io/streamer.h>
#include <core/utils/helpers.h>
#include <core/hash.h>
#include <core/compressed_stream.h>
#include <array>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <random>
#include <queue>
#include <map>
#include <set>
#include "core/algorithm/graph.h"

//#define STATE_COMPRESSION 1

using namespace std;

struct edfd_element
{
    using id_t = int;

    enum class type_t
    {
        entity,
        process,
        nf_process, //special type,
        storage
    };

    id_t id;
    string name;
    type_t type;

    friend bool operator == (const edfd_element& lhs, const edfd_element& rhs)
    {
        return lhs.id == rhs.id && lhs.name == rhs.name && lhs.type == rhs.type;
    }

    //! comparison for generating transitions
    //! checking types and names for now
    friend bool compare(const edfd_element& lhs, const edfd_element& rhs)
    {
        //if this is nf-operation, don't check names
        return lhs.type == rhs.type && ((lhs.type == type_t::nf_process) || (lhs.type == type_t::storage) ? true : lhs.name == rhs.name);
        //return lhs.type == rhs.type && lhs.name == rhs.name;
    }
};

typedef string edfd_connection; //connection name is the only parameter of a connection

namespace std{
    template<>
    struct hash <edfd_element>
    {
        std::size_t operator()(const edfd_element& vert) const
        {
            return id_hasher(vert.id) + name_hasher(vert.name) + type_hasher(static_cast<unsigned>(vert.type));
        }

        std::hash<edfd_element::id_t> id_hasher;
        std::hash<string> name_hasher;
        std::hash<unsigned> type_hasher;
        //std::hash<edfd_element::type_t> type_hasher;
    };
}

typedef explicit_graph<edfd_element, edfd_connection> edfd_graph;

static std::pair<edfd_element, bool> find_element_in_edfd(const edfd_graph& graph, edfd_element::id_t id)
{
    const auto& verts = graph.get_vertices();
    auto it = std::find_if(verts.begin(), verts.end(), [id](const edfd_element& vert) { return vert.id == id; });
    if (it != verts.end())
        return {*it, true};
    else
        return {edfd_element(), false};
}

struct edfd_cover_state
{
    vector<size_t> fragment_instances; //map SDP fragment instance id(index in vector) to SDP fragment type(index in sdps vector in edfd_cover class)
    unordered_map<edfd_graph::vertex_t::id_t, size_t> cover; //what SDP instance is used to cover i-th edfd_element in source graph
    size_t main_instances_count; //how many fragments from fragment_instances (to count from the beginning) are main parts of SDPs

    edfd_cover_state() :
        main_instances_count(0)
    {}

    bool isCovered(edfd_graph::vertex_t::id_t vertexId) const
    {
        return cover.find(vertexId) != cover.end();
    }

    bool isMainPart(size_t fragment_instance_id) const
    {
        return fragment_instance_id < main_instances_count;
    }

    bool operator == (const edfd_cover_state& rhs) const
    {
        return fragment_instances == rhs.fragment_instances &&
            cover == rhs.cover &&
            main_instances_count == rhs.main_instances_count;
    }
};

namespace std
{
    template<>
    struct hash <edfd_cover_state>
    {
        std::size_t operator()(const edfd_cover_state& state) const
        {
            size_t result = 0;
            for (auto& vert : state.cover)
                result += id_hasher(vert.first) + sdp_instance_hasher(vert.second);
            for (auto& sdp_instance : state.fragment_instances)
                result += sdp_instance_hasher(sdp_instance);
            return result;
        }

        std::hash<edfd_element::id_t> id_hasher;
        std::hash<size_t> sdp_instance_hasher;
    };
}

struct edfd_cover_transition
{
    std::set<edfd_element::id_t> cover_difference; //what vertices are covered by this transition
    size_t new_sdp_instance; //value to append to edfd_cover_state::sdp_instances vector
    size_t min_distance_from_main_part; //used for calculating transition cost, closer fragments should have more priority, for precise cover

    friend bool operator < (const edfd_cover_transition& lhs, const edfd_cover_transition& rhs)
    {
        if (lhs.new_sdp_instance < rhs.new_sdp_instance ||
            lhs.new_sdp_instance == rhs.new_sdp_instance && lhs.cover_difference.size() < rhs.cover_difference.size())
            return true;
        else if (lhs.new_sdp_instance == rhs.new_sdp_instance && lhs.cover_difference.size() == rhs.cover_difference.size())
            for (auto it1 = lhs.cover_difference.cbegin(), it2 = rhs.cover_difference.cbegin(); it1 != lhs.cover_difference.cend(); ++it1, ++it2)
            {
                if (*it1 < *it2)
                    return true;
            }
        return false;
    }

    friend bool operator == (const edfd_cover_transition& lhs, const edfd_cover_transition& rhs)
    {
        return lhs.cover_difference == rhs.cover_difference && lhs.new_sdp_instance == rhs.new_sdp_instance; //tmp implementation just for compile test
    }

    friend ostream& operator << (ostream& os, const edfd_cover_transition& rhs)
    {
        os << "Cover vertices (";
        for (auto& vert_id : rhs.cover_difference)
            os << vert_id << ' ';
        os << ')';
        return os << " with SDP fragment " << rhs.new_sdp_instance;
    }
};

namespace std
{
    template<>
    struct hash<edfd_cover_transition>
    {
        std::size_t operator()(const edfd_cover_transition& tr) const
        {
            auto result = sdp_instance_hasher(tr.new_sdp_instance);
            for (const auto& id : tr.cover_difference)
                result += id_hasher(id);
            return result;
        }

        std::hash<decltype(edfd_cover_transition::new_sdp_instance)> sdp_instance_hasher;
        std::hash<edfd_element::id_t> id_hasher;
    };
}

class edfd_cover
{
public:
    typedef edfd_cover_state state_t;
    typedef edfd_cover_transition transition_t;
    using cover_data_t = pair<edfd_element::id_t, size_t>;

    class state_streamer_t : public streamer_base
    {
    public:
        state_streamer_t(const edfd_cover & problem)
            : streamer_base(problem.size() * (sizeof(cover_data_t) + sizeof(size_t)) + sizeof(size_t)),
            problem(problem)
            //for each edfd element we store id, cover (as size_t index of sdp instance) and (maybe) one sdp instance
            //assuming maximum count of sdp instances is equal to source graph size
        {
        }

        void serialize(void * dst, const state_t & state) const
        {
            auto ptr = reinterpret_cast<cover_data_t*>(dst);

            for (auto& vert : problem.src_graph.get_vertices())
            {
                auto it = state.cover.find(vert.id); //looking for cover of each vertex
                if (it == state.cover.end()) //if this vertex hasn't been covered
                    *ptr = cover_data_t(vert.id, std::numeric_limits<size_t>::max()); //write max size_t instead of -1
                else
                    *ptr = *it; //write SDP instance number
                ++ptr;
            }

            auto ptr_sdp = reinterpret_cast<size_t*>(ptr);
            for (size_t i = 0; i < problem.size(); ++i, ++ptr_sdp)
            {
                if (state.fragment_instances.size() > i)
                    *ptr_sdp = state.fragment_instances[i];
                else
                    *ptr_sdp = std::numeric_limits<size_t>::max(); //instead of -1
            }

            auto ptr_main_parts = ptr_sdp;
            *ptr_main_parts = state.main_instances_count;
        }

        void deserialize(const void * src, state_t & state) const
        {
            state.cover.clear();
            state.fragment_instances.clear();
            state.main_instances_count = 0;

            auto ptr = reinterpret_cast<const cover_data_t*>(src);
            for (size_t i = 0; i < problem.size(); ++i, ++ptr)
            {
                cover_data_t cover_data(*ptr);

                if (cover_data.second != std::numeric_limits<size_t>::max())
                    state.cover.insert(cover_data);
            }

            auto ptr_sdp = reinterpret_cast<const size_t*>(ptr);
            for (size_t i = 0; i < problem.size(); ++i, ++ptr_sdp)
            {
                size_t sdp (*ptr_sdp);

                if (sdp != std::numeric_limits<size_t>::max())
                    state.fragment_instances.push_back(sdp);
            }

            auto ptr_main_parts = ptr_sdp;
            state.main_instances_count = *ptr_main_parts;
        }
    private:
        const edfd_cover& problem;
    };

    edfd_cover() :
        preciseCover(false)
    {
    }

    edfd_cover(size_t problem_size) :
        preciseCover(false)
    {
        std::ignore = problem_size;
        //dummy, just for compilation
    }

    edfd_cover(const edfd_graph & src_graph, const vector<edfd_graph>& sdp_fragments, const vector<size_t>& main_parts_for_fragments,bool preciseCover) :
        src_graph(src_graph),
        sdp_fragments(sdp_fragments),
        main_parts_for_fragments(main_parts_for_fragments),
        preciseCover(preciseCover),
        r_matrix(src_graph)
    {
    }

    static size_t deserialize_problem_size(std::istream & is)
    {
        size_t size;
        is >> size;
        return size;
    }

    void serialize_state(std::ostream & os, const state_t & state) const
    {
        os << size() << std::endl; //serializing size here
        interpet_state(os, state);
    }

    void deserialize_state(std::istream & is, state_t & state) const
    {
        state.cover.clear();
        state.fragment_instances.clear();
        //not deserializing size here

        //read cover
        for (size_t i = 0; i < size(); ++i)
        {
            edfd_element::id_t id;
            int sdp_instance;
            is >> id >> sdp_instance;

            if (sdp_instance != -1)
                state.cover[id] = sdp_instance;
        }

        //read sdp instances
        for (size_t i = 0; i < size(); ++i)
        {
            int sdp;
            is >> sdp;

            if (sdp != -1)
                state.fragment_instances.push_back(sdp);
        }

        is >> state.main_instances_count;
    }

    void apply(state_t & state, transition_t cover_diff) const
    {
        state.fragment_instances.push_back(cover_diff.new_sdp_instance); //add new sdp fragment instance
        if (!preciseCover)
            ++state.main_instances_count; //in rough cover problem each new fragment is main
        for (auto id : cover_diff.cover_difference)
            state.cover[id] = state.fragment_instances.size() - 1; //mark vertices covered by this new sdp fragment instance
    }

    template<typename F>
    void forall_available_transitions(const state_t & base_state, F fun) const
    {
        auto available_transitions = generate_transitions_from_state(base_state);
        for (const transition_t& transition : available_transitions)
            fun(transition);
    }

    bool is_solved(const state_t & cur_state) const
    {
        //for (auto& vert : src_graph.get_vertices())
        //{
        //	auto it = cur_state.cover.find(vert.id); //looking for cover of each vertex
        //	if (it == cur_state.cover.end()) //if this vertex hasn't been covered
        //		return false; //problem is not yet solved
        //}
        //return true; //if we have found a cover for each vertex, the problem is solved

        //just check whether we cannot cover more vertices from this state
        //rough cover cannot cover all graph
        return generate_transitions_from_state(cur_state).size() == 0;
    }

    float transition_cost(const state_t&, transition_t transition) const
    {
        return preciseCover ? transition.min_distance_from_main_part : 1.0f;
    }

    size_t size() const
    {
        return src_graph.get_vertices().size();
    }

    std::ostream & interpret_transition(std::ostream & os, const state_t & state, const transition_t & transition) const
    {
        std::ignore = state;
        return os << transition;
    }

    std::ostream & interpet_state(std::ostream & os, const state_t & state) const
    {
        for (auto& vert : src_graph.get_vertices())
        {
            os << vert.id << ' ';
            auto it = state.cover.find(vert.id); //looking for cover of each vertex
            if (it == state.cover.end()) //if this vertex hasn't been covered
                os << -1; //write -1
            else
                os << it->second; //write SDP instance number
            os << '\n';
        }
        for (size_t i = 0; i < size(); ++i)
        {
            if (state.fragment_instances.size() > i)
                os << state.fragment_instances[i] << ' ';
            else
                os << -1 << ' ';
        }

        os << state.main_instances_count;
        return os;
    }

private:
    const edfd_graph src_graph; //source graph
    const vector<edfd_graph> sdp_fragments; //available set of SDP fragments to cover source graph
    const vector<size_t> main_parts_for_fragments; //maps each id of fragment from 'sdp_fragments' to corresponding main part instance id (for precise cover only)
    const bool preciseCover; //whether problem is being solved on precise cover stage
    const reachability_matrix_t<edfd_element, edfd_connection> r_matrix; //for fast checking

    void generate_transitions_from_state_connected_with_main_part(const state_t & base_state, size_t main_part_instance_id, unordered_set<transition_t>& result) const
    {
        //first step - find vertices covered with given main part
        std::vector<size_t> main_part_vertex_indices; //indices for reachability matrix
        if (preciseCover)
        {
            for (auto vert_id_to_frag_id : base_state.cover)
                if (base_state.isMainPart(vert_id_to_frag_id.second) &&
                        vert_id_to_frag_id.second == main_part_instance_id)
                    main_part_vertex_indices.push_back(r_matrix.find_vertex_id(find_element_in_edfd(src_graph, vert_id_to_frag_id.first).first).first);
        }

        auto get_path_length = [this, &main_part_vertex_indices] (const edfd_element& v)
        {
            size_t l = std::numeric_limits<size_t>::max();
            for (size_t main_part_vertex_id : main_part_vertex_indices)
                l =  std::min(std::min(l, r_matrix.path_length(main_part_vertex_id, v)), r_matrix.path_length(r_matrix.find_vertex_id(v).first, main_part_vertex_id));
            return l;
        };

        auto is_any_path = [get_path_length] (const edfd_element& v)
        {
            return get_path_length(v) < reachability_matrix_t<edfd_element, edfd_connection>::Infinity;
        };

        //for each sdp from available set try to find sdp-like subgraph in current state source graph
        const auto& source_vertices = src_graph.get_vertices();
        for (size_t i = 0; i < sdp_fragments.size(); ++i)
        {
            if (preciseCover &&
                    main_parts_for_fragments[i] != main_part_instance_id)
                continue; //we need only optional parts of given SDP

            const edfd_graph& fragment = sdp_fragments[i];
            const auto& fragment_vertices = fragment.get_vertices();
            for (const edfd_element& sdp_vertex : fragment_vertices)
            {
                for (const edfd_element& source_vertex : source_vertices)
                {
                    if (!base_state.isCovered(source_vertex.id) &&
                        compare(source_vertex, sdp_vertex) &&
                        (!preciseCover || is_any_path(source_vertex)))
                    {
                        transition_t transition;
                        transition.new_sdp_instance = i;
                        transition.min_distance_from_main_part = std::numeric_limits<size_t>::max();

                        queue<edfd_element> elements_to_check;
                        unordered_map<edfd_element, edfd_element> used; //maps sdp vertices to src_graph vertices
                        unordered_set<edfd_element> sdp_visited; //sdp vertices already visited
                        unordered_set<edfd_element> source_used; //source vertices already used
                        used[sdp_vertex] = source_vertex;
                        elements_to_check.push(sdp_vertex);
                        sdp_visited.insert(sdp_vertex); //don't try to use this vertex again
                        source_used.insert(source_vertex); //don't try to use this vertex again
                        bool fail = false;
                        while (!elements_to_check.empty() && !fail)
                        {
                            auto curVertInSdp = elements_to_check.front();
                            auto curVertInSrcGraph = used[curVertInSdp];
                            elements_to_check.pop();

                            //std::cout << "Start walk from SDP vertex " << curVertInSdp.id << " - " << curVertInSdp.name << std::endl;
                            fragment.forall_adj_verts_both_directions(curVertInSdp, [&](const edfd_element& vert, const edfd_connection& edge)
                            {
                                std::ignore = edge;
                                //std::cout << "In SDP vertex " << vert.id << " - " << vert.name << std::endl;
                                if (sdp_visited.find(vert) != sdp_visited.end()) //we have already visited this vertex
                                    return;
                                sdp_visited.insert(vert);

                                bool found = false;
                                //std::cout << "\tStart walk from source vertex " << curVertInSrcGraph.id << " - " << curVertInSrcGraph.name << std::endl;
                                src_graph.forall_adj_verts_both_directions(curVertInSrcGraph, [&](const edfd_element& src_vert, const edfd_connection& src_edge)
                                {
                                    std::ignore = src_edge;
                                    if (used.find(vert) == used.end())
                                    {
                                        //std::cout << "\tIn source vertex " << src_vert.id << " - " << src_vert.name << std::endl;
                                        if (source_used.find(src_vert) == source_used.end() &&
                                            !base_state.isCovered(source_vertex.id) &&
                                            compare(vert, src_vert))
                                        {
                                            used[vert] = src_vert;
                                            source_used.insert(src_vert);
                                            elements_to_check.push(vert);
                                            //std::cout << "\t\tUsing source vertex" << src_vert.id << " as SDP vertex " << vert.id << std::endl;
                                            found = true;
                                        }
                                    }
                                });
                                if (!found)
                                    fail = true;
                            });
                        }
                        if (sdp_visited.size() != fragment_vertices.size()) // we haven't check all sdp vertices
                            fail = true;
                        if (!fail)
                        {
                            for (const auto& vert : used)
                            {
                                transition.cover_difference.insert(vert.second.id);
                                if (preciseCover)
                                    transition.min_distance_from_main_part = std::min(transition.min_distance_from_main_part, get_path_length(vert.second));
                            }
                            result.insert(transition);
                        }
                    }
                }
            }
        }
    }

    unordered_set<transition_t> generate_transitions_from_state(const state_t & base_state) const
    {
        unordered_set<transition_t> result; //using set to prevent equivalent transitions in result
        if (preciseCover)
        {
            //collect transitions for every SDP instance
            for (size_t main_part_index = 0; base_state.isMainPart(main_part_index); ++main_part_index)
                generate_transitions_from_state_connected_with_main_part(base_state, main_part_index, result);
        }
        else
        {
            //no matter what main part id is passed to this method if rough cover is performed
            generate_transitions_from_state_connected_with_main_part(base_state, 0, result);
        }
        return result;
    }

};

#endif
