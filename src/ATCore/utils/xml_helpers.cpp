
#include "xml_helpers.h"
#include "helpers.h"
#include <fstream>
#include <cstring>

_xmlNode * child_for_path(_xmlNode * root_node, const std::string & path)
{
	if (path.empty())
		return nullptr;

	std::string s_path(path);
	size_t slash_pos = s_path.find("/");

	std::string topNodeName;
	bool isBottomNode = true;
	if (slash_pos != std::string::npos) //If found slash
	{
		isBottomNode = false;
		topNodeName = s_path.substr(0, slash_pos);
	}
	else
		topNodeName = s_path;

	for (xmlNode * cur_node = root_node->children; cur_node; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE)
		{
			if (!strcmp((const char*)cur_node->name, topNodeName.c_str()))
			{
				if (isBottomNode)
					return cur_node;
				else
				{
					return child_for_path(cur_node, s_path.substr(slash_pos + 1).c_str());
				}
			}
		}
	}

	return 0;
}


void xml_memory_write(std::function<void(_xmlNode*)> fun, std::string & data)
{
	const char *encoding = "UTF-8";
	xmlKeepBlanksDefault(0);

	xmlDocPtr doc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlNodeSetContent(root_node, BAD_CAST "");

	fun(root_node);

	xmlDocSetRootElement(doc, root_node);
	xmlSetDocCompressMode(doc, 0);

	xmlChar *xmlbuff;
	int buffersize;
	xmlDocDumpFormatMemoryEnc(doc, &xmlbuff, &buffersize, encoding, 1);

	data.resize(buffersize);
	memcpy((char*)data.data(), xmlbuff, buffersize);

	xmlFree(xmlbuff);
	xmlFreeDoc(doc);
}

void write_xml_doc(const std::string & xml_name, std::function<void(_xmlNode*)> fun)
{
	std::string data;
	xml_memory_write(fun, data);

	std::ofstream file;
	open_ofstream(file, xml_name);
	file.write(data.c_str(), data.size());
}

int read_xml_doc(const std::string & xml_name, std::function<void(_xmlNode*)> fun)
{
	std::string xml_data;
	bool r = read_file_content(xml_name, xml_data);
	if (!r)
		return 1;

	//xmlDoc *doc = xmlReadFile(xml_name.c_str(), 0, 0);
	xmlDoc *doc = xmlReadMemory(xml_data.data(), xml_data.size(), 0, 0, 0);

	if (!doc)
		return 1;

	xmlNode *root_element = xmlDocGetRootElement(doc);

	fun(root_element);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return 0;
}
