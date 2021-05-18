
#include "AProjectManager.h"
#include <ATProject/AProject.h>
#include <ATCore/utils/xml_helpers.h>
#include <ATCore/utils/helpers.h>
#include <QApplication>
#include <iostream>

using namespace std;

AProjectManager::AProjectManager()
	:m_pProject(nullptr)
{

}

AProject * AProjectManager::project() const
{
	return m_pProject;
}

int AProjectManager::closeProject()
{
	delete m_pProject;
	m_pProject = nullptr;

	return 0;
}

AProject * AProjectManager::createProject(/*const std::string & project_path*/)
{
	if(closeProject())
		return nullptr;

	auto prj = new AProject;

	
	setCurrentProject(prj);
	//string dir_path, fname;
	//split_file_path(project_path, dir_path, fname);

	//m_pProject = new AProject(fname, project_path);
	//saveProject();

	return prj;
}

AError AProjectManager::saveProject() const
{
	if(!project())
		return AError(AT_ERROR_NO_ACTIVE_PROJECT);

/*	//Create XML-structure of project file
	xmlDocPtr doc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
    xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST "at_project");
	xmlNodeSetContent(root_node, BAD_CAST "");
	xmlDocSetRootElement(doc, root_node);

	//Serialize project tree
	project()->serialize(root_node);

	//Write it
	xmlSetDocCompressMode(doc, 0);*/
	project()->save();

	/*string project_file_path = project()->projectDir() + "/" + project()->fileName();
	xmlSaveFormatFile(project_file_path.c_str(), doc, 1);
    xmlFreeDoc(doc);*/

	return AError();
}

AProject * AProjectManager::openProject(const std::string & path)
{
	if(closeProject())
		return nullptr;

	AProject * prj = nullptr;
	int r = read_xml_doc(path, [&](xmlNode* node_ptr) {
		prj = new AProject();
		string dir_path, fname;
		split_file_path(path, dir_path, fname);
		prj->fileName = QString::fromStdString(fname);
		prj->dirPath = QString::fromStdString(dir_path);

		prj->deserialize(node_ptr);
	});

	/*xmlDocPtr doc = xmlParseFile(path.c_str());
	if(!doc)
		return nullptr;

	xmlNodePtr cur = xmlDocGetRootElement(doc);

	

	AProject * prj = new AProject();

	
	prj->setProjectDir(dir_path);

	prj->deserialize(cur);

	xmlFreeDoc(doc);
	xmlCleanupParser();*/

	setCurrentProject(prj);

	return prj;
}


void AProjectManager::buildClean(AErrorMessenger & os) const
{
	std::string bin_dir = m_pProject->projectDir() + "/bin";
	std::string exec_cmd = "rd \"" + bin_dir + "\" /s /q";
	cout << "Cleaning: " << exec_cmd << std::endl;
	system(exec_cmd.c_str());
}

void AProjectManager::setCurrentProject(AProject * p_project)
{
	m_pProject = p_project;
}
