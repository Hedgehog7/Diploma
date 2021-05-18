#include "SDP.h"

void SDP::set_name(const std::string name)
{
    _name = name;
}

const std::string& SDP::name() const
{
    return _name;
}

void SDP::add_node(const SDP::Node& n)
{
    _nodes.emplace(n.id, n);
}

void SDP::add_node(std::size_t id, const std::string& name, SDP::Node::Type type)
{
    add_node(Node{id, name, type});
}

void SDP::add_flow(const SDP::Flow& f)
{
    _flows.emplace_back(new Flow(f));
    auto src_node_it  = _nodes.find(f.src_id);
    auto dest_node_it  = _nodes.find(f.dest_id);

    if (src_node_it == _nodes.end())
        throw std::invalid_argument("Source node of data flow is invalid, node id = " +  std::to_string(f.src_id));
    if (dest_node_it == _nodes.end())
        throw std::invalid_argument("Destination node of data flow is invalid, node id = " +  std::to_string(f.dest_id));

    _flowsFromNodes.insert({f.src_id, _flows.back().get()});
}

void SDP::add_flow(const std::string data, std::size_t src_id, std::size_t dest_id)
{
    add_flow(Flow{data, src_id, dest_id});
}

void SDP::add_fragment(const SDP::Fragment& f)
{
    _fragments.emplace(f.id, f);
}

void SDP::add_task(const SDP::Task& t)
{
    auto fragment_it = _fragments.find(t.fragment_id);

    if (fragment_it == _fragments.end())
        throw std::invalid_argument("Wrong fragment id " + std::to_string(t.fragment_id));

    _tasks.emplace(t.id, t);
    fragment_it->second.task_ids.insert(t.id);
}

void SDP::for_each_node_in_fragment(std::size_t fragment_id, std::function<void (const SDP::Node&)> f) const
{
    const Fragment& fragment = find_fragment(fragment_id);

    for (std::size_t node_id : fragment.node_ids)
        f(find_node(node_id));
}

void SDP::for_each_flow_in_fragment(std::size_t fragment_id, std::function<void (const SDP::Flow&)> f) const
{
    for_each_flow_in_fragment(fragment_id, [&f](const NodesWithFlow& t)
    {
        f(std::get<2>(t)); //pass only flow itself without src and dest nodes
    });
}

void SDP::for_each_flow_in_fragment(std::size_t fragment_id, std::function<void (NodesWithFlow)> f) const
{
    const Fragment& fragment = find_fragment(fragment_id);

    for (std::size_t src_id : fragment.node_ids)
    {
        const Node& src = find_node(src_id);
        auto flows_range = _flowsFromNodes.equal_range(src.id);

        for (auto flow_it = flows_range.first; flow_it != flows_range.second; ++flow_it)
        {
            const Node& dest = find_node(flow_it->second->dest_id);
            auto dest_it_in_fragment = fragment.node_ids.find(dest.id);

            //assume a flow belongs to a fragment if both source and dest nodes are belong to this fragment
            if (dest_it_in_fragment != fragment.node_ids.end())
                f(std::tie(src, dest, *flow_it->second));
        }
    }
}

void SDP::for_each_task_in_fragment(std::size_t fragment_id, std::function<void (const SDP::Task&)> f) const
{
    const Fragment& fragment = find_fragment(fragment_id);

    for (std::size_t task_id : fragment.task_ids)
        f(find_task(task_id));
}

void SDP::for_each_node(std::function<void(const Node&)> f) const
{
    for (auto p : _nodes)
        f(p.second);
}

void SDP::for_each_flow(std::function<void(const Flow&)> f) const
{
    for (auto flow : _flows)
        f(*flow);
}

void SDP::for_each_fragment(std::function<void (const SDP::Fragment&)> f) const
{
    for (auto p : _fragments)
        f(p.second);
}

const SDP::Node& SDP::find_node(std::size_t id) const
{
    auto node_it = _nodes.find(id);
    if (node_it == _nodes.end())
        throw std::invalid_argument("Wrong node id=" + std::to_string(id));
    else
        return node_it->second;
}

const SDP::Task&SDP::find_task(std::size_t id) const
{
    auto task_it = _tasks.find(id);
    if (task_it == _tasks.end())
        throw std::invalid_argument("Wrong task id=" + std::to_string(id));
    else
        return task_it->second;
}

const SDP::Fragment& SDP::find_fragment(std::size_t id) const
{
    auto fragment_it = _fragments.find(id);
    if (fragment_it == _fragments.end())
        throw std::invalid_argument("Wrong fragment id " + std::to_string(id));
    else
        return fragment_it->second;
}

const SDP::DocumentDescription& SDP::find_document(std::size_t fragment_id, std::size_t document_id) const
{
    const Fragment& frag = find_fragment(fragment_id);

    auto doc_it = frag.documents.find(document_id);
    if (doc_it == frag.documents.end())
        throw std::invalid_argument("Wrong document id " + std::to_string(document_id));
    else
        return doc_it->second;

}
