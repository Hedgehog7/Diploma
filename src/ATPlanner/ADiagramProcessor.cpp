
#include "ADiagramProcessor.h"
#include <ATCore/edfd/EDFDDocument.h>

struct DFDHierarchyNode
{
	DFDHierarchyNode(EDFDDocument * _doc)
		:doc(_doc)
	{}

	~DFDHierarchyNode()
	{
		for(auto c : children)
			delete c;
	}

	bool detailWith(EDFDDocument * _doc)
	{
		if(doc->isDetalizedWith(_doc))
		{
			DFDHierarchyNode * new_child = new DFDHierarchyNode(_doc);
			children.push_back(new_child);
			return true;
		}
		else
		{
			for(auto c : children)
			{
				if(c->detailWith(_doc))
				{
					return true;
				}
			}

			return false;
		}
	}

	void merge()
	{
		for(auto c : children)
		{
			c->merge();

			//Merge documents
			doc->mergeWith(c->doc);
		}
	}

	EDFDDocument * doc;
	std::vector<DFDHierarchyNode*> children;
};

EDFDDocument * ADiagramProcessor::mergeHierarchy(const std::vector<EDFDDocument*> & diagrams)
{
	auto edfd_docs = diagrams;
	//Build detalization links
	for(auto & doc : edfd_docs)
	{
		if(doc->isDetalized())
		{
			AError res = doc->buildDetalizationLinks(edfd_docs);
			if(!res.OK())
			{
				AError::criticalErrorOccured(res);
				return nullptr;
			}
		}
	}

	//Build detalization tree
	
	DFDHierarchyNode * root_node = new DFDHierarchyNode(*edfd_docs.begin());
	edfd_docs.erase(edfd_docs.begin());

	while(!edfd_docs.empty())
	{
		bool something_added = false;
		auto it = edfd_docs.begin();

		while(it != edfd_docs.end())
		{
			//Check that current document can be detalized with root node
			if((*it)->isDetalizedWith(root_node->doc))
			{
				//Create top-level node
				DFDHierarchyNode * new_root = new DFDHierarchyNode(*it);
				
				//Add current root to it's children
				new_root->children.push_back(root_node);

				root_node = new_root;

				//Remove doc from src list
				
				something_added = true;
			}
			else
			{
				something_added = root_node->detailWith(*it);
			}

			if(something_added)
			{
				edfd_docs.erase(it);
				break;
			}
			else
				++it;
		}

		if(!something_added)
		{
			AError::criticalErrorOccured(AError(AT_ERROR_PROJECT_DATA, "Invalid EDFD Hierarchy structure, cannon build detailed diagramm. Maybe bore than one EDFD diagramm are top-level, must be only one."));
			return nullptr;
		}
	}

	//Recursively merge tree children
	root_node->merge();
	auto res = root_node->doc;
	//m_pDetailEDFD = 

	//Delete tree structure
	delete root_node;
	return res;
}