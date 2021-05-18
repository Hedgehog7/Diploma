#include "SDPDocument.h"
#include "../utils/xml_helpers.h"
//#include <libxml/tree.h>
#include <iostream>

void SDPDocument::serialize(_xmlNode* document_node) const
{
    std::ignore = document_node;
    //TODO: maybe add real serialization later, not needed for now because AT has not editor for it
}

AError SDPDocument::deserialize(_xmlNode* document_node)
{
    xmlNode* procedures = child_for_path(document_node, "procedures");
    if (procedures != nullptr)
        xml_for_each_child(procedures, procedure)
            _sdps.push_back(deserialize_sdp(procedure));

    return AError(AT_ERROR_OK);
}

ADocument::Type SDPDocument::type() const
{
    return Type::SDP;
}

const std::vector<SDP>&SDPDocument::getSDPs() const
{
    return _sdps;
}

SDP SDPDocument::deserialize_sdp(_xmlNode* procedure_node) const
{
    SDP result;

    result.set_name(xml_prop(procedure_node, "name"));

    xmlNode* nodes = child_for_path(procedure_node, "nodes");
    if (nodes != nullptr)
        xml_for_each_child(nodes, node)
            result.add_node(deserialize_sdp_node(node));

    xmlNode* flows = child_for_path(procedure_node, "flows");
    if (flows != nullptr)
        xml_for_each_child(flows, flow)
            result.add_flow(deserialize_sdp_flow(flow));

    xmlNode* fragments = child_for_path(procedure_node, "fragments");
    if (fragments != nullptr)
        xml_for_each_child(fragments, fragment)
            result.add_fragment(deserialize_sdp_fragment(fragment));

    xmlNode* network = child_for_path(procedure_node, "network");
    if (network != nullptr)
        xml_for_each_child(network, task)
            result.add_task(deserialize_sdp_task(task));

    return result;
}

SDP::Node SDPDocument::deserialize_sdp_node(_xmlNode* node_node) const
{
    SDP::Node result;

    result.id = atoi(xml_prop(node_node, "id"));
    result.name = xml_prop(node_node, "name");

    static const std::unordered_map<std::string, SDP::Node::Type> stringToType = {
        {"Entity", SDP::Node::Type::Entity},
        {"Storage", SDP::Node::Type::Storage},
        {"Function", SDP::Node::Type::Function},
        {"NFFunction", SDP::Node::Type::NFFunction},
    };

    auto type_str = xml_prop(node_node, "type");
    auto type_it = stringToType.find(type_str);
    if (type_it != stringToType.end())
    {
        result.type = type_it->second;
    }
    else
    {
        std::cout << "Warning: wrong type of SDP node " << type_str << std::endl;
        result.type = SDP::Node::Type::Entity;
    }

    return result;
}

SDP::Flow SDPDocument::deserialize_sdp_flow(_xmlNode* flow_node) const
{
    SDP::Flow result;

    result.src_id  = atoi(xml_prop(flow_node, "src_id"));
    result.dest_id = atoi(xml_prop(flow_node, "dest_id"));
    result.data = xml_prop(flow_node, "data");

    return result;
}

SDP::Fragment SDPDocument::deserialize_sdp_fragment(_xmlNode* fragment_node) const
{
    SDP::Fragment result;

    result.id = atoi(xml_prop(fragment_node, "id"));
    result.name = xml_prop(fragment_node, "name");
    result.primary = atoi(xml_prop(fragment_node, "primary")) > 0;

    xmlNode* nodes = child_for_path(fragment_node, "nodes");
    if (nodes != nullptr)
        xml_for_each_child(nodes, node_ref)
            result.node_ids.insert(atoi(xml_prop(node_ref, "id")));

    xmlNode* documents = child_for_path(fragment_node, "documents");
    if (documents != nullptr)
        xml_for_each_child(documents, document)
        {
            SDP::DocumentDescription document_descr;
            document_descr.id = atoi(xml_prop(document, "id"));
            document_descr.name = xml_prop(document, "name");
            document_descr.extension = xml_prop(document, "extension");
            result.documents.emplace(document_descr.id, document_descr);
        }

    return result;
}

SDP::Task SDPDocument::deserialize_sdp_task(_xmlNode* task_node) const
{
    SDP::Task result;

    result.id = atoi(xml_prop(task_node, "id"));
    result.fragment_id = atoi(xml_prop(task_node, "fragment_id"));
    result.name = xml_prop(task_node, "name");
    result.stage = xml_prop(task_node, "stage");
    result.executor = xml_prop(task_node, "executor");

    xmlNode* predcessors = child_for_path(task_node, "predcessors");
    if (predcessors != nullptr)
        xml_for_each_child(predcessors, predcessor_ref)
            result.predcessor_task_ids.insert(atoi(xml_prop(predcessor_ref, "id")));

    xmlNode* input_documents = child_for_path(task_node, "input");
    if (input_documents != nullptr)
        xml_for_each_child(input_documents, doc_ref)
            result.input_document_ids.insert(atoi(xml_prop(doc_ref, "id")));

    xmlNode* output_documents = child_for_path(task_node, "output");
    if (output_documents != nullptr)
        xml_for_each_child(output_documents, doc_ref)
            result.output_document_ids.insert(atoi(xml_prop(doc_ref, "id")));

    return result;
}
