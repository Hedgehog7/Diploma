#include "AProject.h"
#include "AProjectNode.h"
#include <ATCore/utils/xml_helpers.h>
#include <ATCore/check.h>
#include "AProjectTemplate.h"
#include "ADocumentRef.h"
#include <QFileInfo>
#include <iostream>

using namespace std;


//Writing and binding nodes with parent node and saving into output file
/*void writeNode (std::vector<AProjectNode*> _vector, xmlNodePtr _parent)
{
	while (_vector.empty() == false)
	{
		
		xmlNodePtr _child_node = xmlNewNode(NULL, BAD_CAST _vector.at(0)->name());
		xmlAddChild(_parent, _child_node);
		xmlNodeSetContent(_child_node, BAD_CAST "");

		writeNode(_vector.at(0)->getChild(), _child_node);

		_vector.erase(_vector.begin());
		
	}
}*/

AProject::AProject(/*const std::string & project_name, const std::string & project_dir*/)
	:ANamedObject("project"), m_pRootNode(0), docCounter(1)
{
	m_pRootNode = new ARootProjectNode("root");
}

ARootProjectNode * AProject::rootNode()
{
	return m_pRootNode;
}

std::string AProject::projectDir() const
{
	return "";//mProjectDir;
}

void AProject::setProjectDir(const std::string & dir)
{
//	mProjectDir = dir;
}
/*
const std::string & AProject::fileName() const
{
	return mProjectFileName;
}

void AProject::setFileName(std::string & f_name)
{
	mProjectFileName = f_name;
}
*/
void AProject::serialize(xmlNodePtr root_node) const
{
	//Project name
	xmlNewProp(root_node, BAD_CAST "name" , BAD_CAST name().c_str());

	//Serialize project tree and save all changed referenced files
	xmlNode * project_tree_node = xmlNewChild(root_node, NULL, BAD_CAST "project_root_node", BAD_CAST "");
	m_pRootNode->serialize(project_tree_node);

	//Save current detailed EDFD
	if(m_pDetailEDFD)
	{
		xmlNode * detailed_edfd = xmlNewChild(root_node, NULL, BAD_CAST "detailed_edfd", BAD_CAST "");
		xmlNewProp(detailed_edfd, BAD_CAST "filename" , BAD_CAST "main.edfd");
		m_pDetailEDFD->saveToFile(projectDir() + "/main.edfd");
	}

	xmlNode * docs_node = xmlNewChild(root_node, NULL, BAD_CAST "documents", BAD_CAST "");
	for(auto ref : documents)
	{
		xmlNode * doc_node = xmlNewChild(docs_node, NULL, BAD_CAST "document_ref", BAD_CAST "");
		xmlSetProp(doc_node, BAD_CAST "path", BAD_CAST ref->relativePath.toStdString().c_str());
	}
}

AError AProject::deserialize(xmlNodePtr root_node)
{
	auto cproject_name = xml_prop(root_node, "name");
	setName(string(cproject_name));

	xmlNode * project_tree_node = child_for_path(root_node, "project_root_node");
	m_pRootNode->deserialize(project_tree_node);

	xmlNode * detailed_edfd = child_for_path(root_node, "detailed_edfd");
	if(detailed_edfd)
	{
		m_pDetailEDFD.reset(new EDFDDocument());
		auto cfilename = xml_prop(detailed_edfd, "filename");

		m_pDetailEDFD->loadFromFile(projectDir() + "/" + string(cfilename));
	}

	xmlNode * docs_node = child_for_path(root_node, "documents");
	xml_for_each_child(docs_node, doc_node)	{
		std::string doc_rel_path((char*)xmlGetProp(doc_node, BAD_CAST "path"));
		addDocumentRef(QString::fromStdString(doc_rel_path));
	}

	return AError();
}

bool AProject::hasUnsavedChanges() const
{
	return true;
}

void AProject::documentsWithExtension(std::vector<const ADocumentProjectNode*> & doc_nodes, const std::string & ext) const
{
	m_pRootNode->getDocumentNodesWithExtension(doc_nodes, ext);
}

void AProject::removeDocumentsWithExtension(const std::string & ext)
{
	m_pRootNode->removeDocumentsWithExtension(ext);
}

/*
void AProject::buildCommonEDFD(AError * err) const
{
	//Get all EDFD document nodes
	vector<const ADocumentProjectNode*> edfd_nodes;
	documentsWithExtension(edfd_nodes, "edfd");

	//Load documents into RAM
	vector<shared_ptr<EDFDDocument>> edfd_docs;
	for(auto doc_node : edfd_nodes)
	{
		shared_ptr<EDFDDocument> new_doc(new EDFDDocument());
		//new_doc->loadFromFile(mProjectDir + "/" + doc_node->name());
		edfd_docs.push_back(new_doc);
	}

	//Build detalization links
	for(auto & doc : edfd_docs)
	{
		if(doc->isDetalized())
		{
			AError res = doc->buildDetalizationLinks(edfd_docs);
			if(!res.OK())
			{
				AError::criticalErrorOccured(res);
				return;
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
			return;
		}
	}

	//Recursively merge tree children
	root_node->merge();
	m_pDetailEDFD = root_node->doc;

	//Delete tree structure
	delete root_node;

	if(err)
		*err = AError();

	//For building IDs!
	m_pDetailEDFD->saveToFile(projectDir() + "/main.cache.edfd");
}

std::shared_ptr<EDFDDocument> AProject::commonEDFD(AError * err)
{
	if(!m_pDetailEDFD)
		buildCommonEDFD(err);

	return m_pDetailEDFD;
}*/

ADocumentProjectNode * AProject::addDocument(ADocument * doc)
{
	ADocumentProjectNode * new_node = new ADocumentProjectNode(doc);
	m_pRootNode->addChild(new_node);
	return new_node;
}

ADocumentProjectNode* AProject::findDocumentNode(const std::string & doc_name)
{
	return m_pRootNode->findDocumentNode(doc_name);
}

std::string AProject::documentPath(const ADocumentProjectNode * doc_node) const
{
	return "";//mProjectDir + "/" + doc_node->name();
}

const ADocumentProjectNode * AProject::architectureDocument(AError * err) const
{
	vector<const ADocumentProjectNode*> archs;
	documentsWithExtension(archs, "arch");

	if(archs.size() == 0)
	{
		if(err)
			*err = AError(AT_ERROR_PROJECT_DATA, "Architecture docs not found.");
		return nullptr;
	}
	else if(archs.size() > 1)
	{
		if(err)
		*err = AError(AT_ERROR_PROJECT_DATA, "There must be only 1 architecture document.");
		return nullptr;
	}

	return archs[0];
}

std::string AProject::projectFilePath() const
{
	auto res = dirPath + "/" + fileName;
	return res.toStdString();
}

void AProject::save()
{
	write_xml_doc(projectFilePath(), [=](_xmlNode* root_node){
		serialize(root_node);
	});
}

void AProject::addDocumentRef(const QString & rel_path)
{
	auto new_ref = new ADocumentRef(this);
	new_ref->relativePath = rel_path;
	add(new_ref);
}

void AProject::add(ADocumentRef * new_ref)
{
	QFileInfo fi(dirPath + "/" + new_ref->relativePath);
	perform_check(fi.exists());
	documents.push_back(new_ref);
}

ADocumentRef * AProject::generateRef(const std::string & extension)
{
	return generateRef(QString::fromStdString(extension));
}

ADocumentRef * AProject::generateRef(const QString & extension)
{
	QString rel_path;
	QFileInfo fi;
	
	do
	{
		rel_path = QString("document_%1.%2").arg(docCounter++).arg(extension);
		fi = QFileInfo(dirPath + "/" + rel_path);
	} while (fi.exists());

	auto new_ref = new ADocumentRef(this);
	new_ref->relativePath = rel_path;
	return new_ref;
}

void AProject::foreach_doc(const QString & extension, std::function<void(ADocumentRef*)> fun)
{
	for (auto ref : documents)
	{
		if (ref->extension() == extension)
			fun(ref);
	}
}

void AProject::applyTemplate(AProjectTemplate * templ)
{
	for(auto & doc_rec : templ->docs)
	{
		auto src_doc = templ->dirPath + "/" + doc_rec,
			dst_doc = this->dirPath + "/" + doc_rec;

		std::cout << "Adding default document" << src_doc.toStdString() << " => " << dst_doc.toStdString() << std::endl;
		QFile::copy(src_doc, dst_doc);

		addDocumentRef(doc_rec);
	}
}
