
#ifndef ATCore_xml_helpers_h
#define ATCore_xml_helpers_h

//struct _xmlNode;

#include "../config.h"
#include <string>
#include <functional>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlsave.h>



#define xml_for_each_child(root, iterator) for(xmlNode * iterator = root->children; iterator; iterator = iterator->next) if (iterator->type == XML_ELEMENT_NODE)
#define xml_prop(node, prop_name) (const char*)xmlGetProp(node, (xmlChar*)prop_name)

AT_CORE_API void write_xml_doc(const std::string & xml_name, std::function<void(_xmlNode*)> fun);
AT_CORE_API int read_xml_doc(const std::string & xml_name, std::function<void(_xmlNode*)> fun);
AT_CORE_API _xmlNode * child_for_path(_xmlNode * root_node, const std::string & path);

#endif
