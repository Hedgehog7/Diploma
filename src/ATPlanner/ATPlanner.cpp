#include "ATPlanner.h"
#include "APlannerWidget.h"
#include "AStartArchitectureGenerator.h"
#include "ADiagramProcessor.h"
#include <edfd_cover/APlan.h>
#include <ATProject/ADocumentRef.h>
#include <ATProject/AProject.h>
#include <ATProject/AProjectNode.h>
#include <ATGUI/APluginManager.h>
#include <ATGUI/APlugin.h>
#include <ATCore/architecture/AArchitectureDocument.h>
//#include <ATCore/ACommandExecutor.h>
#include <vector>
#include <iostream>

#include <edfd_cover/EDFDCover.h>
#include <ATCore/sdp/SDPDocument.h>

using namespace std;

ATPlanner::ATPlanner(APluginManager * plugin_mgr)
	:m_pCurrentPlan(nullptr), m_pPluginManager(plugin_mgr)
{

}

ATPlanner::~ATPlanner()
{

}

APlannerWidget * ATPlanner::createInfoWidget()
{
	return (m_pPlannerWidget = new APlannerWidget(this));
}

AError ATPlanner::rebuildPlan()
{
    m_pCurrentPlan = new APlan();

    DELEGATE()->planRebuilt(this, m_pCurrentPlan);
    m_pPlannerWidget->planRebuilt(this, m_pCurrentPlan);
    return AError();
}

void ATPlanner::loadProject(AProject * project)
{
	m_pProject = project;
}

AProject * ATPlanner::project() const
{
	return m_pProject;
}

AError ATPlanner::buildGeneralizedPlan()
{
	AError err;
	
	std::vector<EDFDDocument*> diagrams;
	m_pProject->foreach_doc("edfd", [&](ADocumentRef * ref) {
		auto new_diagram = new EDFDDocument;
		new_diagram->loadFromFile(ref->absolutePath().toStdString());
		diagrams.push_back(new_diagram);
	});

	ADiagramProcessor proc;
	EDFDDocument * common_dfd = proc.mergeHierarchy(diagrams);
	
	std::cout << "Levon starts from here" << std::endl;

    //TODO: think about better place to load SDPs, really they can be used somewhere else
    SDPDocument sdps;
    if (!(err = sdps.loadFromFile("kb/procedures.xml")).OK())
    {
        std::cout << err.text() << std::endl;
        return err;
    }

	if(!common_dfd)
		return AError(AT_ERROR_UNKNOWN, "Failed to build general EDFD diagram");

	EDFDCover cover;
    auto plan = cover.buildPlan(m_pProject, common_dfd, sdps.getSDPs());

	//Sample
    //auto kbs_ref = m_pProject->generateRef("kbs");
    //auto dsf_ref = m_pProject->generateRef("dsf");

    //auto task1 = new APlanTask;
    //{
    //	task1->name = "Acquire Knowledge";
    //	task1->stage = "Recquirments Analysys";
    //	task1->executorId = "KMPZ";
    //	task1->group = "Hospitalisation Planning";
    //	task1->docsOut.push_back(kbs_ref);
    //}
    //plan->addTask(task1);

    //auto task2 = new APlanTask;
    //{
    //	task2->name = "Configure Core";
    //	task2->stage = "Design";
    //	task2->group = "Hospitalisation Planning";
    //	task2->executorId = "configurator";
    //	task2->docsIn.push_back(kbs_ref);
    //}
    //plan->addTask(task2, { task1 });

    //auto task3 = new APlanTask;
    //{
    //	task3->name = "Create Dialogue Scenario";
    //	task3->stage = "Implementation";
    //	task3->group = "Monitoring";
    //	task3->executorId = "dsf_editor";
    //	task3->docsIn.push_back(kbs_ref);
    //	task3->docsOut.push_back(dsf_ref);
    //}
    //plan->addTask(task3, { task1 });

	//Show plan and make current
	setPlan(plan);

	return AError();
}

ADocumentProjectNode * ATPlanner::buildStartingArchitectureModel(AArchElementFactory * arch_factory)
{
/*	AError err;
	
	//Get preprocessed hierarchy
	auto common_dfd = m_pProject->commonEDFD(&err);

	if(!common_dfd)
		return nullptr;

	//Create new architecure document
	AArchitectureDocument * arch_doc = new AArchitectureDocument();

	//Build architecure model
	AStartArchitectureGenerator generator;
	generator.generate(arch_factory, common_dfd.get(), arch_doc);

	//Save document to project
	arch_doc->saveToFile(m_pProject->projectDir() + "/main.arch");
	auto doc_node = m_pProject->addDocument(arch_doc);
*/
	//Return saved document
	//return doc_node;
	return nullptr;
}

AError ATPlanner::buildDetailPlan()
{
	AError res;
/*
	//Check that generalized plan is built
	if(!m_pCurrentPlan)
		res = buildGeneralizedPlan();
	if(!res.OK())
		return res;

	//Get architecture document
	AError err;
	const ADocumentProjectNode* old_arch_doc = m_pProject->architectureDocument(&err);
	if(!err.OK())
		return err;

	auto doc_path = m_pProject->documentPath(old_arch_doc);
	shared_ptr<AArchitectureDocument> arch_doc(new AArchitectureDocument());
	arch_doc->loadFromFile(doc_path);

	//Find suitable adapter for planner
	auto adapters = m_pPluginManager->plugins(APlugin::Type::Adapter);
	if(adapters.size() == 0)
		return AError(AT_ERROR_UNKNOWN, "PDDL adapters not loaded");

	auto adapter = static_cast<AAdapterPlugin*>(adapters[0]->plugin());

	//Solve task with adapter, based on generalized plan and architecture
	APlan * new_plan = adapter->buildDetailPlan(m_pCurrentPlan, arch_doc.get());

	//Show plan and make current
	setPlan(new_plan);*/


	return AError();
}

int ATPlanner::executeTask(ASubTask * task)
{
	string res;
	//command_executor()->executeCommand(task->executionString(), res);

	return 0;
}

void ATPlanner::setPlan(APlan * new_plan)
{
	m_pCurrentPlan = new_plan;
	delegate()->planRebuilt(this, new_plan);
}
