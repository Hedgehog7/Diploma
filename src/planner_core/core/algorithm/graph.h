#ifndef UltraCore_graph_h
#define UltraCore_graph_h

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "../io/streamer.h"
#include <list>
#include <functional>
#include <cassert>
#include <algorithm>

template<typename V>
class simple_graph
{
    typedef V vertex_t;
public:
    void serialize_state(void * dst, const vertex_t & vertex) const
    {
        *((vertex_t*)dst) = vertex;
    }

    void deserialize_state(const void * src, vertex_t & vertex) const
    {
        vertex = *((const vertex_t*)src);
    }

    int serialized_state_size() const
    {
        return sizeof(vertex_t);
    }
};

/*
Describes graph explicitely set by the
vertices and edges.
*/
template<typename V, typename E>
class explicit_graph : public simple_graph<V>
{
public:
    using vertex_t = V;
    using edge_t = E;
    using vertex_streamer_t = base_type_streamer<vertex_t>;

    struct adjacent_vertex_t
    {
        edge_t edge;
        vertex_t vertex;

        friend bool operator ==(const adjacent_vertex_t& lhs, const adjacent_vertex_t& rhs)
        {
            return lhs.edge == rhs.edge &&
                    lhs.vertex == rhs.vertex;
        }
    };

    explicit_graph()
    {}

    explicit_graph(const std::initializer_list<vertex_t> & verts, const std::initializer_list<std::pair<vertex_t, vertex_t>> & adjacent_verts)
    {
        for (auto & v : verts)
            m_vertices.insert(v);

        for (auto & av : adjacent_verts)
        {
            adjacent_vertex_t forward_edge;
            forward_edge.vertex = av.second;
            m_adjacentLists[av.first].push_back(forward_edge);

            adjacent_vertex_t backward_edge;
            backward_edge.vertex = av.first;
            m_adjacentLists[av.second].push_back(backward_edge);
        }
    }

    explicit_graph(const std::initializer_list<vertex_t> & verts, const std::initializer_list<std::tuple<vertex_t, vertex_t, edge_t>> & edges)
    {
        init_with_edge_content(verts, edges);
    }

    explicit_graph(const std::vector<vertex_t> & verts, const std::vector<std::tuple<vertex_t, vertex_t, edge_t>> & edges)
    {
        init_with_edge_content(verts, edges);
    }

    template<typename F>
    void forall_adj_verts(const vertex_t & vertex, F fun) const
    {
        auto adj_it = m_adjacentLists.find(vertex);
#if _DEBUG
        assert(m_vertices.find(vertex) != m_vertices.end());
        //assert(adj_it != m_adjacentLists.end());
        //commented out this assert because it's normal when there are no adjacent vertices for given vertex
#endif
        if (adj_it != m_adjacentLists.end())
            for(auto & edge : adj_it->second)
                fun(edge.vertex, edge.edge);
    }

    //! looks like 'forall_adj_verts' but edges go to initial vertex
    template<typename F>
    void forall_adj_verts_reversed(const vertex_t& vertex, F fun) const
    {
        for (const auto& l : m_adjacentLists)
            for (const adjacent_vertex_t& vert : l.second)
                if (vert.vertex == vertex)
                    fun(l.first, vert.edge);
    }

    template<typename F>
    void forall_adj_verts_both_directions(const vertex_t& vertex, F fun) const
    {
        forall_adj_verts(vertex, fun);
        forall_adj_verts_reversed(vertex, fun);
    }

    void add_vertex(const vertex_t & vert)
    {
        m_vertices.insert(vert);
    }

    void add_edge(const vertex_t & src, const vertex_t & dst, const edge_t & edg)
    {
#if _DEBUG
        assert(m_vertices.find(src) != m_vertices.end());
        assert(m_vertices.find(dst) != m_vertices.end());
#endif
        adjacent_vertex_t new_edge;
        new_edge.edge = edg;
        new_edge.vertex = dst;

        auto it = m_adjacentLists.find(src);
        if (it == m_adjacentLists.end())
            m_adjacentLists.insert(make_pair(src, std::list<adjacent_vertex_t>(1, new_edge)));
        else
        {
            it->second.push_back(new_edge);
        }
    }

    std::list<adjacent_vertex_t> & adjacent_list(const vertex_t & vertex) const
    {
        auto it = m_adjacentLists.find(vertex);
        return it->second;
    }

    const std::unordered_set<vertex_t> & get_vertices() const
    {
        return m_vertices;
    }

    bool operator ==(const explicit_graph& rhs) const
    {
        //simple memberwise check
        return m_vertices == rhs.m_vertices &&
                m_adjacentLists == rhs.m_adjacentLists;
    }

    bool empty() const
    {
        return m_vertices.empty();
    }

    bool has_vertex(const vertex_t& v) const
    {
        return m_vertices.find(v) != m_vertices.end();
    }

private:
    std::unordered_set<vertex_t> m_vertices;
    std::unordered_map<vertex_t, std::list<adjacent_vertex_t>> m_adjacentLists;

    void init_with_edge_content(const std::vector<vertex_t> & verts, const std::vector<std::tuple<vertex_t, vertex_t, edge_t>> & edges)
    {
        for (auto & v : verts)
            m_vertices.insert(v);

        for (auto & edge : edges)
        {
            adjacent_vertex_t forward_edge;
            forward_edge.vertex = std::get<1>(edge);
            forward_edge.edge = std::get<2>(edge);
            m_adjacentLists[std::get<0>(edge)].push_back(forward_edge);
        }
    }

};

/*
Describes the graph, where vertices are not
known, before they were discovered.
*/
template<typename V>
class implicit_graph : public simple_graph<V>
{
public:
    typedef V vertex_t;
    using vertex_streamer_t = base_type_streamer<vertex_t>;

    template<typename AdjFun>
    implicit_graph(AdjFun adj_fun)
        :m_adjGetter(adj_fun)
    {
    }

    template<typename F>
    void forall_adj_verts(const vertex_t & vertex, F fun) const
    {
        for(auto & vert : m_adjGetter(vertex))
            fun(vert, 1);
    }

private:
    std::function<std::list<vertex_t>(vertex_t)> m_adjGetter;
};

template<typename TS>
class transition_system_graph
{
public:
    typedef TS transition_system_t;
    using vertex_t = typename transition_system_t::state_t;
    using vertex_streamer_t = typename transition_system_t::state_streamer_t;

    transition_system_graph(const transition_system_t & ts)
        :m_system(ts)
    {
    }

    template<typename F>
    void forall_adj_verts(const vertex_t & vertex, F fun) const
    {
        m_system.forall_generated_states(vertex, fun);
    }

    const transition_system_t & transition_system() const
    {
        return m_system;
    }

    /*int serialized_state_size() const
    {
        return m_system.serialized_state_size();
    }

    void serialize_state(void * dst, const vertex_t & vertex) const
    {
        m_system.serialize_state(dst, vertex);
    }

    void deserialize_state(const void * src, vertex_t & vertex) const
    {
        m_system.deserialize_state(src, vertex);
    }*/
private:
    const transition_system_t & m_system;
};

template <typename V, typename E>
class reachability_matrix_t
{
public:
    using graph_t = explicit_graph<V, E>;
    using vertex_t = typename graph_t::vertex_t;
    using edge_t = typename graph_t::edge_t;

    enum
    {
        Infinity = std::numeric_limits<size_t>::max() / 2
    };

    reachability_matrix_t()
    {
    }

    reachability_matrix_t(const graph_t& graph)
    {
        _initial_fill(graph);
        _Floyd_Warshall();
    }

    template<typename lhs_t, typename rhs_t>
    bool is_path(const lhs_t& lhs, const rhs_t& rhs) const
    {
        return path_length(lhs, rhs) < Infinity;
    }

    size_t path_length(const vertex_t& lhs, const vertex_t& rhs) const
    {
        auto lhs_id_p = find_vertex_id(lhs);
        if (!lhs_id_p.second)
            return Infinity;

        return path_length(lhs_id_p.first, rhs);
    }

    size_t path_length(size_t lhs_id, const vertex_t& rhs) const
    {
        auto rhs_id_p = find_vertex_id(rhs);
        if (!rhs_id_p.second)
            return Infinity;

        return path_length(lhs_id, rhs_id_p.first);
    }

    size_t path_length(size_t lhs_id, size_t rhs_id) const
    {
        return _mat[lhs_id][rhs_id];
    }

    std::pair<size_t, bool> find_vertex_id(const vertex_t& v) const
    {
        auto l_it = _vertices_to_indices.find(v);
        if (l_it == _vertices_to_indices.end())
            return {0, false};
        else
            return {l_it->second, true};
    }

private:
    std::vector<vertex_t> _indices_to_vertices;
    std::unordered_map<vertex_t, size_t> _vertices_to_indices;
    std::vector<std::vector<size_t>> _mat;

    //! create adjacency matrix
    void _initial_fill(const graph_t& graph)
    {
        size_t cnt = 0;
        for (const vertex_t& v : graph.get_vertices())
        {
            _indices_to_vertices.push_back(v);
            _vertices_to_indices.emplace(v, cnt);
            ++cnt;
        }

        for (const vertex_t& v : graph.get_vertices())
        {
            std::vector<size_t> adj_list(cnt, Infinity);
            adj_list[_vertices_to_indices[v]] = 0;
            graph.forall_adj_verts(v, [this, &adj_list](const vertex_t& adj_vert, const edge_t&)
            {
                adj_list[_vertices_to_indices[adj_vert]] = 1;
            });
            _mat.push_back(std::move(adj_list));
        }
    }

    //! Floyd-Warshall algorithm for finding shortest paths or reachability matrix
    void _Floyd_Warshall()
    {
        const size_t cnt = _vertices_to_indices.size();
        for (size_t k = 0; k < cnt; ++k)
            for (size_t i = 0; i < cnt; ++i)
                for (size_t j = 0; j < cnt; ++j)
                    _mat[i][j] = std::min(_mat[i][j], _mat[i][k] + _mat[k][j]);
    }
};

template<typename V, typename E>
reachability_matrix_t<V, E> reachability_matrix(const explicit_graph<V, E>& graph)
{
    return reachability_matrix_t<V, E>(graph);
}

template<typename V, typename E>
class topological_sort_t
{
public:
    using graph_t = explicit_graph<V, E>;
    using vertex_t = typename graph_t::vertex_t;
    using edge_t = typename graph_t::edge_t;
    using answer_t = std::vector<vertex_t>;
    using answer_iterator_t = typename answer_t::const_iterator;
    using answer_reverse_iterator_t = typename answer_t::const_reverse_iterator;

    topological_sort_t(const graph_t& graph, bool reverse_order = false)
    {
        _sort(graph);
        if (!reverse_order)
            std::reverse(_answer.begin(), _answer.end());
    }

    const graph_t& graph() const;

    answer_iterator_t begin() const
    {
        return _answer.cbegin();
    }

    answer_iterator_t end() const
    {
        return _answer.cend();
    }

    answer_reverse_iterator_t rbegin() const
    {
        return _answer.crbegin();
    }

    answer_reverse_iterator_t rend() const
    {
        return _answer.crend();
    }

private:
    std::vector<vertex_t> _answer;

    enum class vertex_color_t : char
    {
        unused = 0,
        grey,
        black
    };

    void _dfs(const graph_t& graph, std::unordered_map<vertex_t, vertex_color_t>& used, const vertex_t& v)
    {
        used[v] = vertex_color_t::grey;
        graph.forall_adj_verts(v, [&](const vertex_t& adj_v, const edge_t&)
        {
            auto f_it = used.find(adj_v);
            if (f_it == used.end() || f_it->second == vertex_color_t::unused)
                _dfs(graph, used, adj_v);
            else if (f_it->second == vertex_color_t::grey)
                throw std::logic_error("Given graph cannot be topologically sorted, graph should be acyclic");
        });
        used[v] = vertex_color_t::black;
        _answer.push_back(v);
    }

    void _sort(const graph_t& graph)
    {
        std::unordered_map<vertex_t, vertex_color_t> used;
        _answer.clear();
        for (const vertex_t& v : graph.get_vertices())
        {
            if (used[v] == vertex_color_t::unused)
                _dfs(graph, used, v);
        }
    }
};

template<typename V, typename E>
topological_sort_t<V, E> topological_sort(const explicit_graph<V, E>& graph)
{
    return topological_sort_t<V, E>(graph);
}

template<typename V, typename E>
topological_sort_t<V, E> reversed_topological_sort(const explicit_graph<V, E>& graph)
{
    return topological_sort_t<V, E>(graph, true);
}

#endif
