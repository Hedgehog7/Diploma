#ifndef SDP_H
#define SDP_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <functional>
#include "../config.h"

class AT_CORE_API SDP
{
public:
    struct Node
    {
        enum class Type
        {
            Entity,
            Storage,
            Function,
            NFFunction
        };

        std::size_t id;
        std::string name;
        Type type;
    };

    struct Flow
    {
        std::string data;
        std::size_t src_id, dest_id;
    };

    struct DocumentDescription
    {
        std::size_t id;
        std::string name;
        std::string extension;
    };

    struct Fragment
    {
        std::size_t id;
        std::string name;
        bool primary;
        std::unordered_set<std::size_t> node_ids;
        std::unordered_set<std::size_t> task_ids;
        std::unordered_map<std::size_t, DocumentDescription> documents;
    };

    struct Task
    {
        std::size_t id;
        std::string name;
        std::size_t fragment_id;
        std::string stage;
        std::string executor;
        std::unordered_set<std::size_t> predcessor_task_ids;
        std::unordered_set<std::size_t> input_document_ids, output_document_ids;
    };

    void set_name(const std::string name);
    const std::string& name() const;

    void add_node(const Node& n);
    void add_node(std::size_t id, const std::string& name, Node::Type type);

    //when add a flow, nodes with src_id and dest_id must be in sdp already
    void add_flow(const Flow& f);
    void add_flow(const std::string data, std::size_t src_id, std::size_t dest_id);

    void add_fragment(const Fragment& f);

    void add_task(const Task& t);

    using NodesWithFlow = std::tuple<const Node&, const Node&, const Flow&>;
    void for_each_node_in_fragment(std::size_t fragment_id, std::function<void(const Node&)> f) const;
    void for_each_flow_in_fragment(std::size_t fragment_id, std::function<void(const Flow&)> f) const;
    void for_each_flow_in_fragment(std::size_t fragment_id, std::function<void(NodesWithFlow)> f) const;
    void for_each_task_in_fragment(std::size_t fragment_id, std::function<void(const Task&)> f) const;

    void for_each_node(std::function<void(const Node&)> f) const;
    void for_each_flow(std::function<void(const Flow&)> f) const;
    void for_each_fragment(std::function<void(const Fragment&)> f) const;

    const Node& find_node(std::size_t id) const;
    const Task& find_task(std::size_t id) const;
    const Fragment& find_fragment(std::size_t id) const;
    const DocumentDescription& find_document(std::size_t fragment_id, std::size_t document_id) const;
private:
    std::string _name;
    std::unordered_map<std::size_t, Node> _nodes;
    std::unordered_map<std::size_t, Fragment> _fragments;
    std::vector<std::shared_ptr<Flow>> _flows;
    std::unordered_map<std::size_t, Task> _tasks;
    //maps node id to flows with src in it
    std::unordered_multimap<std::size_t, Flow*> _flowsFromNodes;
};

#endif // SDP_H
