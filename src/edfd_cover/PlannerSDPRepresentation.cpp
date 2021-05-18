#include "PlannerSDPRepresentation.h"

#include <ATCore/sdp/SDP.h>

static edfd_element::type_t SDPNodeTypeToEDFDElementType(SDP::Node::Type type)
{
    switch (type)
    {
    case SDP::Node::Type::Entity:
        return edfd_element::type_t::entity;
    case SDP::Node::Type::Function:
        return edfd_element::type_t::process;
    case SDP::Node::Type::NFFunction:
        return edfd_element::type_t::nf_process;
    case SDP::Node::Type::Storage:
        return edfd_element::type_t::storage;
    default:
        std::cout << "Unsupported type of SDP element, assume it is Entity" << std::endl;
        return edfd_element::type_t::entity;
    }
}

static edfd_element SDPNodeToGraphElement(const SDP::Node& node)
{
    edfd_element result;
    result.type = SDPNodeTypeToEDFDElementType(node.type);
    result.id = node.id;
    result.name = node.name;
    return result;
}

static edfd_connection SDPFlowToGraphConnection(const SDP::Flow& flow)
{
    return flow.data;
}

void PlannerSDPRepresentation::convertSDPToGraphs(const SDP& sdp)
{
    using connection_t = std::tuple<edfd_element, edfd_element, edfd_connection>;
    sdp.for_each_fragment([&](const SDP::Fragment& fragment)
    {
        vector<edfd_element> elements; //for edfd_graph constructor
        vector<connection_t> connections;

        //some nodes can have no flows within one fragment (e.g. if a fragment consists of the only node)
        sdp.for_each_node_in_fragment(fragment.id, [&](const SDP::Node& node)
        {
            elements.push_back(SDPNodeToGraphElement(node));
        });

        sdp.for_each_flow_in_fragment(fragment.id, [&](SDP::NodesWithFlow src_dest_flow)
        {
            auto src = SDPNodeToGraphElement(std::get<0>(src_dest_flow));
            auto dest = SDPNodeToGraphElement(std::get<1>(src_dest_flow));

            //don't worry about duplicates now, we'll remove them later
            elements.push_back(src);
            elements.push_back(dest);

            connections.emplace_back(
                src,
                dest,
                SDPFlowToGraphConnection(std::get<2>(src_dest_flow))
            );
        });

        //remove duplicates, sort vector then remove them
        std::sort(elements.begin(), elements.end(), [](const edfd_element& lhs, const edfd_element& rhs)
        {
            return lhs.id < rhs.id;
        });
        auto it = std::unique(elements.begin(), elements.end());
        elements.resize(std::distance(elements.begin(), it));

        //if two fragments are marked as primary, assume there is only the first one
        if (fragment.primary && mainPart.first.empty())
            mainPart = {edfd_graph(elements, connections), fragment.id};
        else
            optionalParts.emplace_back(edfd_graph(elements, connections), fragment.id);
    });
}

PlannerSDPRepresentation::PlannerSDPRepresentation(const SDP& sdp) :
    originalSDP(sdp)
{
    convertSDPToGraphs(sdp);
}

const edfd_graph& PlannerSDPRepresentation::getMainPartEDFD() const
{
    return mainPart.first;
}

const PlannerSDPRepresentation::fragment_edfd_t PlannerSDPRepresentation::getMainPart() const
{
    return mainPart;
}

const std::vector<PlannerSDPRepresentation::fragment_edfd_t>& PlannerSDPRepresentation::getOptionalParts() const
{
    return optionalParts;
}

const SDP& PlannerSDPRepresentation::getOriginalSDP() const
{
    return originalSDP;
}
