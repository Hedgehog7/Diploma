
#include "ATApplication.h"
#include <ATGUI/AEditor.h>
#include <ATPlanner/APlannerWidget.h>

#include <QtCore/QDir>
#include <QtCore/QDebug>
//#include <windows.h>
#include <string>

using namespace std;

ATApplication::ATApplication(int & argc, char ** argv)
	:QApplication(argc, argv), ACommandExecutor(), m_pPlanner(new ATPlanner(this)), m_pPlannerWidget(nullptr)
{
	searchAndLoadPlugins();

	m_pPlanner->setDelegate(this);
}

void ATApplication::searchAndLoadPlugins()
{
	QDir dir(QDir::currentPath() + "/plugins");
	dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList({"*.dl*", "*.so", "*.dylib"}));

    qDebug() << "Loading plugins from: " << dir.absolutePath();
    qDebug() << "Loading plugins from: " << dir.path();

	foreach(QFileInfo file, dir.entryInfoList())
    {
        qDebug() << "Found plugin: " << file.fileName();

		auto dle_path = dir.absoluteFilePath(file.fileName()).toStdString();

		AError res = loadPlugin(dle_path);
        if (!res.OK()){
            qDebug() << "Error while loading plugin: " << res.text();}
	}

	//buildCommandIndex();
}

AError ATApplication::executeCommand(const std::string & cmd_text, std::string & answer)
{
	static const string core_commands[] = {"build_plan"};

	if(cmd_text == core_commands[0])
	{
		auto res = m_pPlanner->rebuildPlan();
		return res;
	}
	else
	{
		return ACommandExecutor::executeCommand(cmd_text, answer);
	}
}

void ATApplication::planRebuilt(ATPlanner * planner, APlan * plan)
{
	m_pPlannerWidget->displayPlan(plan);
}

ATPlanner * ATApplication::planner() const
{
	return m_pPlanner;
}

void ATApplication::setPlannerWidget(APlannerWidget * wdg)
{
	m_pPlannerWidget = wdg;
}
