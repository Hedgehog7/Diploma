#include "ATEnvironment.h"

#include "AQNewProjectDialog.h"
#include "AQNewFileDialog.h"
#include "ATApplication.h"
#include "APluginsWidget.h"
#include "AConsoleWidget.h"
#include <ATProject/AProject.h>
#include <ATProject/AProjectNode.h>
#include <ATProject/ADocumentRef.h>
#include <ATCore/ADocument.h>
#include <ATCore/ADocument.h>
#include <ATGUI/AEditor.h>
#include <ATPlanner/APlannerWidget.h>
#include <ATProject/AProjectTemplate.h>
#include <ATGUI/ATaskExecutionWindow.h>
#include <QtCore/QFileInfo>
#include <QDir>
#include <QtWidgets/QMdiSubWindow>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QProcess>
#include <QDialog>
#include <iostream>

using namespace std;

class console_wrapper : public AErrorMessenger
{
public:
	console_wrapper(ATEnvironment * env)
		:pEnv(env)
	{}

	virtual void printMessage(const std::string & msg) override
	{
		//Ensure console visible
		pEnv->dockConsole->show();

		//Print text
		pEnv->wdgConsole->printMessage(msg);
	}

	ATEnvironment * pEnv;
};

ATEnvironment::ATEnvironment(ATApplication * app, QWidget *parent)
	: QMainWindow(parent), AProjectManager(), m_pApplication(app)
{
	ui.setupUi(this);

	auto cwrap = new console_wrapper(this);
	atOut = cwrap;

	dockConsole = new QDockWidget(tr("Console"), this);

	wdgConsole = new AConsoleWidget(dockConsole);
	dockConsole->setWidget(wdgConsole);
	dockConsole->setAllowedAreas(Qt::BottomDockWidgetArea);

	ui.menuView->addAction(dockConsole->toggleViewAction());

	addDockWidget(Qt::BottomDockWidgetArea, dockConsole);
	dockConsole->hide();
	//console_dock->toggleViewAction()->trigger();

	/*
	"File" menu.
	*/
	connect(ui.actionExit, &QAction::triggered, this, &ATEnvironment::close);
	connect(ui.actionNewProject, &QAction::triggered, this, &ATEnvironment::createNewProject);
	connect(ui.actionSave_Project, &QAction::triggered, [=]()
	{
		saveProject();
	}
	);
	connect(ui.actionOpenProject, &QAction::triggered, [=]()
	{
		this->openProject();
	});

	connect(ui.actionOpenFile, &QAction::triggered, [=]()
	{
		this->openFile();
	});


	//Create planner widget
	wdgPlanner = m_pApplication->planner()->createInfoWidget();
	ui.dockPlanner->setWidget((QWidget*)wdgPlanner);
	m_pApplication->setPlannerWidget(wdgPlanner);
	connect(wdgPlanner, &APlannerWidget::startArchDocumentCreated, this, &ATEnvironment::openNodeDocument);

	connect(wdgPlanner, &APlannerWidget::projectStructureChanged, [=]() {
		displayProject(project());
	});

	connect(wdgPlanner, &APlannerWidget::executeTaskRequested, [=](APlanTask * task) {
		executeTask(task);
	});

	/*
	Create switching menus in View menu.
	*/
	QList<QDockWidget *> dockwidgets = findChildren<QDockWidget *>();
	for (auto i : dockwidgets)
	{
		if (i->parentWidget() == this)
			ui.menuView->addAction(i->toggleViewAction());
	}

	//Setup project explorer links
	connect(ui.wdgProjectExplorer, &AProjectExplorer::createNewFileRequested, this, &ATEnvironment::createNewFile);
	connect(ui.wdgProjectExplorer, &AProjectExplorer::openDocumentRequested, this, [=](ADocumentRef * ref) {
		openDocument(ref);
	});

	//connect(ui.actionNewFile, &QAction::triggered, [=](){openFile("test");});

	//mEditorsFactory.insert(pair<AFile::Type, AAbstractEditorInitializer*>(AFile::Type::ExtendedDFD, new AEditorInitializer<EDFDEditor>()));

	wdgConsole->setDelegate(m_pApplication);

	connect(ui.actionPlugins, &QAction::triggered, [=]() {
		auto wdg = new APluginsWidget(m_pApplication);
		wdg->show();
	});

	connect(ui.actionBuildProject, &QAction::triggered, [=]() {
		this->buildProject();
	});

	connect(ui.actionSave_all, &QAction::triggered, [=]() {
		for (auto x : mOpenedFiles) {
			x.second->m_pEditorInstance->saveFileInitiated();
		}
	});

	connect(ui.actionBuildCleanProject, &QAction::triggered, [=]() {
		//this->buildClean(wdgConsole);
	});

	connect(ui.mdiArea, &QMdiArea::subWindowActivated, [=](QMdiSubWindow * wnd) {
		if (wnd)
		{
			ui.actionSaveFile->disconnect();
			auto at_mdi = dynamic_cast<ATMdiWindow*>(wnd);

			if (at_mdi)
			{
				connect(ui.actionSaveFile, &QAction::triggered, [=]() {
					at_mdi->m_pEditorInstance->saveFileInitiated();
				});
				ui.actionSaveFile->setEnabled(true);
				connect(ui.actionSaveAsFile, &QAction::triggered, [=]() {
					QString path = QFileDialog::getSaveFileName(this, tr("Select name/directory"), QDir::homePath());
					at_mdi->m_pEditorInstance->saveFile(path.toStdString());
				});
				ui.actionSaveAsFile->setEnabled(true);
			}
			else
				ui.actionSaveFile->setEnabled(false);
		}
		else
		{
			ui.actionSaveFile->setEnabled(false);
		}

	});



	/*auto arch_plugin = static_cast<AGUIEditorPlugin*>(m_pApplication->editorForExtension("arch"));
	auto ed = static_cast<AGUIEditorInstance*>(arch_plugin->createEditorInstance());
	ed->showMaximized();*/
}

ATEnvironment::~ATEnvironment()
{

}

void ATEnvironment::displayProject(AProject * _project)
{
	if (_project)
		ui.wdgProjectExplorer->loadProjectTree(_project->rootNode());

	QString file_name = project() ? QFileInfo(QString::fromStdString(project()->name())).fileName() : "No File";
	setWindowTitle(QString("%1 - ATEnvironment").arg(file_name));

	m_pApplication->planner()->loadProject(_project);
}

int ATEnvironment::closeProject()
{
	int res = 0;

	if (project())
	{
		if (project()->hasUnsavedChanges())
		{
			int ret = QMessageBox::warning(this, tr("Save chanhes"),
				tr("The project has been modified.\n"
					"Do you want to save your changes?"),
				QMessageBox::Save | QMessageBox::Discard
				| QMessageBox::Cancel,
				QMessageBox::Save);


			if (ret == QMessageBox::Cancel)
				return 1;
			else if (ret == QMessageBox::Save)
			{
				saveProject();
			}
		}
		res = AProjectManager::closeProject();
	}


	return res;
}

void ATEnvironment::createNewProject()
{
	auto wizard = new ANewProjectWizard;
	wizard->setWindowTitle(tr("New Project"));

	if (wizard->exec() == QDialog::DialogCode::Accepted)
	{
		std::cout << "Creating project " << wizard->selectedTemplate->name.toStdString() << "," << wizard->projectDir.toStdString() << "," << wizard->projectName.toStdString() << std::endl;

		auto new_prj = createProject();
		if (new_prj)
		{
			QDir dir(wizard->projectDir);
			dir.mkpath(wizard->projectName);

			new_prj->dirPath = wizard->projectDir + "/" + wizard->projectName;
			new_prj->fileName = wizard->projectName + ".atprj";
			new_prj->save();

			new_prj->applyTemplate(wizard->selectedTemplate);

			//auto edfd_doc = new EDFDDocument;
			//edfd_doc->saveToFile((new_prj->dirPath + "/main.edfd").toStdString());
			//delete edfd_doc;

			//new_prj->addDocumentRef("main.edfd");
			new_prj->save();
			setCurrentProject(new_prj);
		}
	}

	/*AQNewProjectDialog prj_dlg(this);
	if(prj_dlg.exec() == QDialog::Accepted)
	{
	if(closeProject() == 0)
	{
	//Create dir for new project
	QDir base_dir(prj_dlg.projectDir());
	base_dir.mkdir(prj_dlg.projectName());

	QString project_dir = prj_dlg.projectDir() + "/" + prj_dlg.projectName();

	auto res = createProject(project_dir.toStdString());
	displayProject(res);
	}
	}*/
}

void ATEnvironment::createNewFile()
{
	AQNewFileDialog dlg(m_pApplication);
	if (dlg.exec() != QDialog::Accepted)
	{
		return;
	}

	//auto editor_plugin = dlg.selectedTypeEditor();
	//ADocument * doc = editor_plugin->createFile(project()->projectDir(), dlg.fileName().toStdString());

	QString name = m_pProject->dirPath + "/" + dlg.fileName() + ".edfd";
	QFile file(name);
	if (!file.open(QIODevice::WriteOnly))
		return;
	auto new_ref = new ADocumentRef(project());
	new_ref->relativePath = dlg.fileName() + ".edfd";
	m_pProject->add(new_ref);

	ui.wdgProjectExplorer->updateTree();

	m_pProject->save();
	openDocument(new_ref);

}

void ATEnvironment::openProject(const std::string & path)
{
	auto res = AProjectManager::openProject(path);
	if (res)
	{
		//displayProject(res);
	}
	else
		QMessageBox::warning(this, tr("Error"), tr("Unable to open given project."), QMessageBox::Ok);
}

void ATEnvironment::openProject()
{
	QString file_name = QFileDialog::getOpenFileName(this, tr("Open project"),
		"D:",
		tr("AT Project Files (*.atprj);; All Files (*)"));
	if (!file_name.isEmpty())
		openProject(file_name.toStdString());
}

void ATEnvironment::openFile()
{
	QString file_name = QFileDialog::getOpenFileName(this, tr("Open file"),
		"D:",
		tr("All Files (*)"));
	if (file_name != "")
		openFile(file_name.toStdString());
}

void ATEnvironment::openFile(const std::string & file_path)
{
	//Check, maybe document is already opened
	auto oi = mOpenedFiles.find(file_path);
	if (oi != mOpenedFiles.end())
	{
		ui.mdiArea->setActiveSubWindow(oi->second);
	}
	else
	{
		ui.mdiArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		//Find suitable editor for file
		QFileInfo fi(QString::fromStdString(file_path));
		QString ext = fi.suffix();

		auto ed_plug = m_pApplication->editorForExtension(ext.toStdString());
		if (!ed_plug)
		{
			QMessageBox::warning(this, tr("Error"), tr("No suitable editors loaded into environment."), QMessageBox::Ok);
			return;
		}

		auto gui_plug = static_cast<AGUIEditorPlugin*>(ed_plug);
		auto ed_wdg = static_cast<AGUIEditorInstance*>(gui_plug->createEditorInstance());
		ed_wdg->setDelegate(this);

		auto mdi_sub_wind = new ATMdiWindow(ed_wdg);

		ui.mdiArea->addSubWindow(mdi_sub_wind);
		ui.mdiArea->setActiveSubWindow(mdi_sub_wind);

		ed_wdg->openFile(file_path);
		mdi_sub_wind->setWindowTitle(ed_wdg->windowTitle());

		connect(mdi_sub_wind, &ATMdiWindow::editorClosed, [=]() {
			closeMdiWindow(mdi_sub_wind, file_path);
		});

		mOpenedFiles.insert(pair<string, ATMdiWindow*>(file_path, mdi_sub_wind));

		//Connect events
		/*connect(mdi_sub_wind, &ATMdiWindow::aboutToActivate, [=]()
		{
		linkFileActions(ed_wdg);
		});*/

		mdi_sub_wind->show();
	}
}

void ATEnvironment::openNodeDocument(ADocumentProjectNode * doc_node)
{
	openFile(project()->documentPath(doc_node));
}

void ATEnvironment::openDocument(const std::string & document_name)
{
	//Find node with document in the project
	auto doc_node = project()->findDocumentNode(document_name);

	if (doc_node)
	{
		//Open it
		openNodeDocument(doc_node);
	}
	else
	{
		AError::criticalErrorOccured(AError(AT_ERROR_FILE_NOT_FOUND, "Document \"" + document_name + "\" was not found in the project."));
	}
}

void ATEnvironment::closeMdiWindow(ATMdiWindow * mdi_wnd, const std::string & fpath)
{
	int ret = QMessageBox::warning(this, tr("Save chanhes"),
		tr("The file has been modified.\n"
			"Do you want to save your changes?"),
		QMessageBox::Save | QMessageBox::Discard
		/*| QMessageBox::Cancel*/,
		QMessageBox::Save);
	//if (ret == QMessageBox::Cancel) return;
	if (ret == QMessageBox::Save)
		mdi_wnd->m_pEditorInstance->saveFileInitiated();
	
	ui.mdiArea->removeSubWindow(mdi_wnd);

	auto oi = mOpenedFiles.find(fpath);
	mOpenedFiles.erase(oi);
	mdi_wnd->deleteLater();
}

void ATEnvironment::documentChanged(const std::shared_ptr<ADocument> & doc)
{
	//	if(doc->type() == ADocument::Type::EDFD)
	//		m_pApplication->planner()->buildGeneralizedPlan();
}

void ATEnvironment::documentOpenRequested(const std::string & document_name)
{
	openDocument(document_name);
}
/*
void ATEnvironment::linkFileActions(AGUIEditorInstance * editor_widget)
{
//Link file actions

if(editor_widget)
{
ui.actionSaveAsFile->setEnabled(true);
ui.actionSaveFile->setEnabled(true);

connect(ui.actionSaveFile, &QAction::triggered, editor_widget, &AGUIEditorInstance::saveFileInitiated);
}
else
{
ui.actionSaveAsFile->setEnabled(false);
ui.actionSaveFile->setEnabled(false);
}
}*/

void ATEnvironment::setCurrentProject(AProject * p_project)
{
	AProjectManager::setCurrentProject(p_project);
	ui.wdgProjectExplorer->setProject(p_project);

	m_pApplication->planner()->loadProject(p_project);
	updateWindowTitle();
}

void ATEnvironment::updateWindowTitle()
{
	QString base_title = tr("AT Workbench");
	if (m_pProject)
	{
		setWindowTitle(QString("%1 - %2/%3").arg(base_title).arg(m_pProject->dirPath).arg(m_pProject->fileName));
	}
	else
		setWindowTitle(base_title);
}

void ATEnvironment::openDocument(ADocumentRef* ref)
{
	std::cout << "XXX" << std::endl;
	auto abs_path = m_pProject->dirPath + "/" + ref->relativePath;
	openFile(abs_path.toStdString());
}

void ATEnvironment::executeTask(APlanTask * task)
{
	std::cout << "Executing task " << task->name << std::endl;

	auto plug = m_pApplication->plugin(task->executorId);

	if (!plug)
	{
		int ret = QMessageBox::warning(this, tr("ATWorkbench"),
			tr("Plugin with identifier %1 not found, task can not be executed.").arg(QString::fromStdString(task->executorId)),
			QMessageBox::Ok);
		return;
	}

	auto new_tab = new QMdiSubWindow(ui.mdiArea);
	new_tab->setWindowTitle(tr("Executing task %1").arg(QString::fromStdString(task->name)));
	auto task_wnd = new ATaskExecutionWindow(task, 0);
	new_tab->setWidget(task_wnd);
	//new_tab->layout()->addWidget(task_wnd);

	ui.mdiArea->addSubWindow(new_tab);

	new_tab->show();
	ui.mdiArea->setActiveSubWindow(new_tab);

	connect(task_wnd, &ATaskExecutionWindow::discardTask, [=]() {
		ui.mdiArea->removeSubWindow(new_tab);
		new_tab->deleteLater();
	});

	connect(task_wnd, &ATaskExecutionWindow::commitTask, [=]() {
		task->closed = true;
		ui.mdiArea->removeSubWindow(new_tab);
		new_tab->deleteLater();

		for (auto doc : task->docsOut)
			m_pProject->add(doc);

		ui.wdgProjectExplorer->updateTree();

		wdgPlanner->updatePlan();
	});

    int r = plug->executeTask(task_wnd, task->name, task->docsIn, task->docsOut, "login");

	if (r == 1)
	{
		QMessageBox::warning(this, tr("ATWorkbench"),
			tr("Sorry, plugin does not support automation"),
			QMessageBox::Ok);
		task_wnd->discardTask();
	}
}

#if AT_PLATFORM_WIN
#define LINKER_EXT ".exe"
#else
#define LINKER_EXT ""
#endif

void ATEnvironment::buildProject()
{
	*atOut << "Build started: Project: " << m_pProject->name();

	auto proj_path = m_pProject->projectFilePath();
	*atOut << "Project path: " << proj_path;


	auto base_dir = QDir::currentPath(); //QApplication::applicationDirPath();
	auto linker_path = QString("%1/ATLinker%2").arg(base_dir).arg(LINKER_EXT);

	auto run_cmd = QString("%1 %2").arg(linker_path).arg(QString::fromStdString(proj_path));
	*atOut << "Starting linker process: " << run_cmd.toStdString();

	auto linker_proc = new QProcess();

	//	connect(linker_proc, &QProcess::errorOccurred, [=](QProcess::ProcessError error) {
	//		*atOut << "Failed to start linker, error=" << std::to_string(error);
	//	});
	//

	connect(linker_proc, &QProcess::readyRead, [=]() {
		auto data = linker_proc->readAllStandardOutput();
	});

	/*	std::string exec_cmd = "ATLinker.exe " + m_pProject->projectDir() + ' ' + m_pProject->projectDir() + '/' + m_pProject->fileName();

	FILE* pipe = _popen(exec_cmd.c_str(), "r");
	if (!pipe)
	{
	os << "Unable to launch linker";
	return;
	}

	char buffer[128];
	std::string result = "";
	while (!feof(pipe))
	{
	if (fgets(buffer, 128, pipe) != NULL)
	result += buffer;
	}
	_pclose(pipe);

	os << result;*/

	connect(linker_proc, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus) {
		*atOut << "Build finished.";
	});

	linker_proc->start(run_cmd);
}
