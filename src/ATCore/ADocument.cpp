
#include "ADocument.h"
#include "utils/xml_helpers.h"
#include "utils/helpers.h"
#include <string>

using namespace std;

const std::string & ADocument::fileName() const
{
	return mFileName;
}

AError ADocument::save(std::string path) {
	return saveToFile(path);
}

AError ADocument::save()
{
	return saveToFile(mDirectory + "/" + mFileName);
}

AError ADocument::saveToFile(const std::string & file_name)
{
	//Save path
	split_file_path(file_name, mDirectory, mFileName);
	return AError();
}

AError ADocument::loadFromFile(const std::string & file_name)
{
	//Save path if OK
	split_file_path(file_name, mDirectory, mFileName);
	return AError();
}

AError AXMLDocument::saveToFile(const std::string & file_path)
{
	_Base::saveToFile(file_path);

	write_xml_doc(file_path, [=](_xmlNode* root_node){
		serialize(root_node);
	});

	//Save data
/*	xmlDocPtr doc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "document");

    xmlNodeSetContent(root_node, BAD_CAST "");
	xmlDocSetRootElement(doc, root_node);

	serialize(root_node);

	xmlSetDocCompressMode(doc, 0);
	xmlSaveFormatFile(file_path.c_str(), doc, 1);
	xmlFreeDoc(doc);*/

	return AError();
}

AError AXMLDocument::loadFromFile(const std::string & file_path)
{
	_Base::loadFromFile(file_path);

	//Load data
	xmlDoc * doc = xmlReadFile(file_path.c_str(), 0, 0);
	
	if(!doc)
		return AError(AT_ERROR_INVALID_FILE);

	xmlNode * root_element = xmlDocGetRootElement(doc);
	
	if(!root_element)
	{
		xmlFreeDoc(doc);
		xmlCleanupParser();
		return AError(AT_ERROR_INVALID_XML);
	}

	auto res = deserialize(root_element);

	xmlFreeDoc(doc);
	xmlCleanupParser();

	return res;
}


ADocument::Type ADocument::type() const
{
	return ADocument::Type::NotSystem;
}
