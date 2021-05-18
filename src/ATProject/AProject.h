
#ifndef ATCore_AProject_h
#define ATCore_AProject_h

#include <ATCore/ANamedObject.h>
#include <ATCore/AError.h>
#include <ATCore/edfd/EDFDDocument.h>
#include <string>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <QString>
#include <memory>
#include <vector>
#include <functional>
#include "config.h"

class ARootProjectNode;
class ADocumentProjectNode;
class AProject;
class AProjectTemplate;
class ADocumentRef;

class AT_PROJECT_API AProject : public ANamedObject
{
public:
	AProject(/*const std::string & project_name = "", const std::string & project_dir = ""*/);
	ARootProjectNode * rootNode();
	
	/*
	Saves and load project and all it's files.
	*/
	void serialize(xmlNodePtr root_node) const;
	AError deserialize(xmlNodePtr root_node);

	/*
	Returns project dir. Useful for creating new files to
	get target dir.
	*/
	std::string projectDir() const;
	void setProjectDir(const std::string & dir);


	//const std::string & fileName() const;
	//void setFileName(std::string & f_name);

	/*
	Returns true if something has unsaved chanes.
	*/
	bool hasUnsavedChanges() const;

	/*
	Collects all documents with given extension into give container.
	*/
	void documentsWithExtension(std::vector<const ADocumentProjectNode*> & doc_nodes, const std::string & ext) const;

	void removeDocumentsWithExtension(const std::string & ext);

	/*
	Performs EDFD detalization, creates new document and returns it.
	*/
	//void buildCommonEDFD(AError * err = nullptr) const;

	//std::shared_ptr<EDFDDocument> commonEDFD(AError * err = nullptr);
	/*
	Adds document to project, creates document node
	in project root and returns created node.
	*/
	ADocumentProjectNode * addDocument(ADocument * doc);

	/*
	Searches for document node in the project tree.
	*/
	ADocumentProjectNode* findDocumentNode(const std::string & doc_name);

	//
	std::string documentPath(const ADocumentProjectNode * doc_node) const;

	const ADocumentProjectNode * architectureDocument(AError * err = nullptr) const;

	std::string projectFilePath() const;

	void save();
	void addDocumentRef(const QString & rel_path);
	void add(ADocumentRef * new_ref);
	ADocumentRef * generateRef(const QString & extension);
	ADocumentRef * generateRef(const std::string & extension);
	void foreach_doc(const QString & extension, std::function<void(ADocumentRef*)> fun);
	void applyTemplate(AProjectTemplate * templ);
public:
	ARootProjectNode * m_pRootNode;
	//AProjectNode * m_pProject;
	/*
	Path to dir where project main file is. Is absolute and
	is filled on project opening.
	*/
	QString dirPath, fileName;

	mutable std::shared_ptr<EDFDDocument> m_pDetailEDFD;

	std::vector<ADocumentRef*> documents;
	unsigned docCounter;
};

#endif
