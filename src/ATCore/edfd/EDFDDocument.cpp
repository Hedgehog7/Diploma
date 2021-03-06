
#include "EDFDDocument.h"
#include "../utils/xml_helpers.h"
//#include <libxml/tree.h>
#include <algorithm>
#include <sstream>
#include <map>
#include <string>

using namespace std;

///////////////////////////////////////////////////////////////////

bool to_bool(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::istringstream is(str);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

inline const char * const BoolToString(bool b)
{
  return b ? "true" : "false";
}

EDFDDocument::EDFDDocument()
{

}

ADocument::Type EDFDDocument::type() const
{
	return ADocument::Type::EDFD;
}

std::vector<std::shared_ptr<DFDElement>> EDFDDocument::getElements() const
{
	return mElements;
}

const std::vector<std::shared_ptr<DFDElement>> & EDFDDocument::elements() const
{
	return mElements;
}

const std::vector<std::shared_ptr<DFDConnection>> & EDFDDocument::connections() const
{
	return mConnections;
}

void EDFDDocument::addElement(const std::shared_ptr<DFDElement> & new_element)
{
	mElements.push_back(new_element);
}

void EDFDDocument::addConnection(const std::shared_ptr<DFDConnection> & new_connection)
{
	mConnections.push_back(new_connection);
}

void EDFDDocument::deleteElement(const std::shared_ptr<DFDElement> & element)
{
	int el = find(mElements.begin(), mElements.end(), element) - mElements.begin();
	if (el <mElements.size())
		mElements.erase(mElements.begin() + el);
}

void EDFDDocument::deleteConnection(const std::shared_ptr<DFDConnection> & connection)
{
	int el = find(mConnections.begin(), mConnections.end(), connection) - mConnections.begin();
	if (el < mConnections.size())
		mConnections.erase(mConnections.begin() + el);
}

void EDFDDocument::serialize(_xmlNode * document_node) const
{
	int i = 1;
	xmlNodePtr doc_node1 = xmlNewChild(document_node, NULL, BAD_CAST "Elements", BAD_CAST "");
	for(auto e : mElements)
	{
		xmlNodePtr child_node = xmlNewChild(doc_node1, NULL, BAD_CAST "elem", BAD_CAST "");
		e->setId(i);
		e->serialize(child_node);
		i++;
	}

	xmlNodePtr doc_node2 = xmlNewChild(document_node, NULL, BAD_CAST "Connections", BAD_CAST "");

	for(auto c : mConnections)
	{
		int i_c = 1;
		xmlNodePtr child_node = xmlNewChild(doc_node2, NULL, BAD_CAST "con", BAD_CAST "");
		xmlNewProp (child_node, BAD_CAST "name" , BAD_CAST c->name().c_str());

		xmlNodePtr child_node_std = xmlNewChild(child_node, NULL, BAD_CAST "std", BAD_CAST "");

		xmlNewProp(child_node_std, BAD_CAST "name" , BAD_CAST "std");
		xmlNewProp(child_node_std, BAD_CAST "std" , BAD_CAST BoolToString(c->std()));
		if (c->std())
		{
			int i_cc = 1;
			for (auto cc : c->std_d())
			{
				std::string str = "std" + std::to_string(i_cc);
				xmlNewProp (child_node_std, BAD_CAST str.c_str(), BAD_CAST cc.c_str());
				i_cc++;
			}
			std::string str_num = std::to_string(i_cc - 1);
			xmlNewProp (child_node_std, BAD_CAST "std_number", BAD_CAST str_num.c_str());
		}

		xmlNodePtr child_node_dts = xmlNewChild(child_node, NULL, BAD_CAST "dts", BAD_CAST "");

		xmlNewProp(child_node_dts, BAD_CAST "name" , BAD_CAST "dts");
		xmlNewProp(child_node_dts, BAD_CAST "dts" , BAD_CAST BoolToString(c->dts()));
		if(c->dts())
		{
			int i_cc = 1;
			for (auto cc : c->dts_d())
			{
				std::string str = "dts" + std::to_string(i_cc);
				xmlNewProp (child_node_dts, BAD_CAST str.c_str(), BAD_CAST cc.c_str());
				i_cc++;
			}
			std::string str_num = std::to_string(i_cc - 1);
			xmlNewProp (child_node_dts, BAD_CAST "dts_number", BAD_CAST str_num.c_str());
		}

		for (auto e_c : mElements)
		{
			if (e_c->id() == c->source()->id())
			{
				stringstream n_str4;
				n_str4 << i_c;
				string s4(n_str4.str());
				xmlNewProp (child_node, BAD_CAST "source_id" , BAD_CAST s4.c_str());
			}
			if (e_c->id() == c->dest()->id())
			{
				stringstream n_str5;
				n_str5 << i_c;
				string s5(n_str5.str());
				xmlNewProp (child_node, BAD_CAST "dest_id" , BAD_CAST s5.c_str());
			}
			i_c++;
		}
	}
}

AError EDFDDocument::deserialize(_xmlNode * document_node)
{
	//Dictionary for elements
	map<int, shared_ptr<DFDElement>> element_dictionary;

	xmlNode *cur = child_for_path(document_node, "Elements");
	xml_for_each_child(cur, child)
	{
		int id = atoi(xml_prop(child, "id"));
		
		shared_ptr<DFDElement> new_el(DFDElement::createAndDeserialize(child));


		////////////////////////////////////////////////////
		bool det = to_bool(xml_prop(child, "detal"));
		new_el->mDetalization.used = det;
		if (det)
		{
			new_el->mDetalization.document_name = xml_prop(child, "detal_doc_name");
		}
		////////////////////////////////////////////////////

		mElements.push_back(new_el);

		element_dictionary[id] = new_el;
	}

	auto conn_nodes = child_for_path(document_node, "Connections");

	xml_for_each_child(conn_nodes, conn_node)
	{
		const char *_cname = xml_prop(conn_node, "name");

		//Get linked element indices
		int source_id = atoi(xml_prop(conn_node, "source_id")), dest_id = atoi(xml_prop(conn_node, "dest_id"));
		
		//Get links to real elements
		shared_ptr<DFDElement> src_elem = element_dictionary[source_id], dest_elem = element_dictionary[dest_id];

		//Create connection
		shared_ptr<DFDConnection> conn(new DFDConnection(this, _cname, src_elem, dest_elem));

		//Check for source-to-dest and dest-to-source datastreams
		xml_for_each_child(conn_node, d_node)
		{
			const char *_cname = xml_prop(d_node, "name");
			std::string sttr = _cname;
			if (sttr == "std")
			{
				bool std = to_bool(xml_prop(d_node, "std"));
				if (std) 
				{
					int std_number = atoi(xml_prop(d_node, "std_number"));
					for (int i = 1; i < std_number + 1; i++)
					{
						std::string str = "std" + std::to_string(i);
						conn->addSTD_data(xml_prop(d_node, str.c_str()));
					}
				}
			}
			else
			{
				bool dts = to_bool(xml_prop(d_node, "dts"));
				if (dts)
				{
					int dts_number = atoi(xml_prop(d_node, "dts_number"));
					for (int i = 1; i < dts_number + 1; i++)
					{
						std::string str = "dts" + std::to_string(i);
						conn->addDTS_data(xml_prop(d_node, str.c_str()));
					}
				}
			}
		}

		mConnections.push_back(conn);
	}
	return AError();
}

bool EDFDDocument::isDetalized() const
{
	for(auto e : mElements)
	{
		if(e->isDetalized())
			return true;
	}

	return false;
}

bool EDFDDocument::isDetalizedWith(EDFDDocument * detail_doc) const
{
	for(auto e : mElements)
	{
		if(e->isDetalized())
		{
			if(e->mDetalization.document == detail_doc)
				return true;
		}
	}

	return false;
}

AError EDFDDocument::buildDetalizationLinks(std::vector<EDFDDocument*> & edfd_docs)
{
	for(auto e : mElements)
	{
		if(e->isDetalized())
		{
			//e->buildDetalizationLink(edfd_docs);
			
			auto doc_it = find_if(edfd_docs.begin(), edfd_docs.end(), [=](EDFDDocument * doc){
				return doc->fileName() == e->mDetalization.document_name;
			});
	
			if(doc_it != edfd_docs.end())
			{
				e->mDetalization.document = *doc_it;
				//edfd_docs.erase(doc_it);
			}
			else
			{
				return AError(AT_ERROR_PROJECT_DATA, "Detalization diagramm document for \"" + e->name() + "\" object in \"" + fileName() + "\" document not found.");
			}
			
		}
	}

	return AError();

}

void EDFDDocument::mergeWith(EDFDDocument * detail_doc)
{
	//Find element, which is detalized with this document

	auto detailed_el = std::find_if(mElements.begin(), mElements.end(), [=](const std::shared_ptr<DFDElement> & el){return el->isDetalized() && el->mDetalization.document == detail_doc;});

	if(detailed_el != mElements.end())
	{
		//Get all links connected with the element in source document

		vector<shared_ptr<DFDConnection>> connected_links;
		for(auto l : mConnections)
		{
			if(l->connectedTo(*detailed_el))
			{
				connected_links.push_back(l);
			}
		}

		//Replace element with elements from detail doc
		mElements.erase(detailed_el);
		mElements.insert(mElements.end(), detail_doc->mElements.begin(), detail_doc->mElements.end());

		//Get connected to Ext links, other add to current docs
		vector<shared_ptr<DFDConnection>> ext_links;
		for(auto l : detail_doc->mConnections)
		{
			//If link is connected to Ext
			if(false)
			{
				ext_links.push_back(l);
			}
			else
				mConnections.push_back(l);
		}
		

		//Synchronize links
		//!!!!!
		for(auto l : connected_links)
			mConnections.erase(std::find(mConnections.begin(), mConnections.end(), l));

		//Merge elements with similar names
		for(int i = 0; i < mElements.size(); ++i)
		{
			for(int j = i+1; j < mElements.size(); ++j)
			{
				if(mElements[i]->isSameAs(mElements[j].get()))
				{
					mergeElements(i, j);
				}
			}
		}
	}
	else
	{
		AError::criticalErrorOccured(AError(AT_ERROR_UNKNOWN, "Failed to merge EDFD documents."));
	}
}

void EDFDDocument::mergeElements(int index1, int index2)
{
	//Relink connections
	for(auto c : mConnections)
	{
		if(c->connectedTo(mElements[index2]))
			c->relinkTo(mElements[index2], mElements[index1]);
	}

	//Remove 2nd element
	mElements.erase(mElements.begin() + index2);
}
