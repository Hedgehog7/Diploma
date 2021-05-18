#ifndef EDFDCOVERINTERNAL_H
#define EDFDCOVERINTERNAL_H

#include <ATCore/edfd/EDFDDocument.h>
#include "APlan.h"
#include "PlannerSDPRepresentation.h"
#include "edfd_cover.h"
#include <ATCore/sdp/SDP.h>

std::ostream& operator << (ostream& os, edfd_element::type_t t);
std::ostream& operator << (ostream& os, edfd_graph edfd);
std::ostream& operator << (std::ostream& os, const edfd_element& rhs);

edfd_element::type_t convertDFDElementType(DFDElement::Type t);
edfd_graph convertDocumentToGraph(const EDFDDocument* common_dfd);

template<typename V, typename E>
void print_explicit_graph(const explicit_graph<V, E>& graph, std::ostream& os, std::size_t indent)
{
    using graph_t = explicit_graph<V, E>;
    using vertex_t = typename graph_t::vertex_t;
    using edge_t = typename graph_t::edge_t;

    std::string str_indent(indent, ' ');

    for (const vertex_t& vert : graph.get_vertices())
    {
        os << vert;
        os << " -> ";
        os << "{\n";

        graph.forall_adj_verts(vert, [&](const vertex_t& adj_vert, const edge_t& edge)
        {
            os << str_indent << "(";
            os << str_indent << edge;
            os << str_indent << ") -> [";
            os << str_indent << adj_vert;
            os << str_indent << "]\n";
        });

        os << "}\n";
    }
}

#endif // EDFDCOVERINTERNAL_H
