#include "AProjectExplorer.h"
#include <ATProject/AProjectNode.h>
#include <ATCore/ADocument.h>
#include <ATProject/AProject.h>
#include <ATCore/check.h>
#include <ATProject/ADocumentRef.h>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QFile>

Q_DECLARE_METATYPE(ADocumentRef*)

//============================AProjectExplorer================================
AProjectExplorer::AProjectExplorer(QWidget *parent)
	: QWidget(parent), pProject(nullptr)
{
	//ui.setupUi(this);
	treeProject = new QTreeWidget(this);
	treeProject->setColumnCount(1);

	//Setup context menu
	treeProject->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(treeProject, &QTreeWidget::customContextMenuRequested, this, &AProjectExplorer::showContextMenu);

	connect(treeProject, &QTreeWidget::itemChanged, this, &AProjectExplorer::changeItemName);
	connect(treeProject, &QTreeWidget::itemDoubleClicked, this, &AProjectExplorer::openNodeFileRequested);

	auto lyt = new QVBoxLayout;
	lyt->addWidget(treeProject);
	setLayout(lyt);


	//connect(treeProject, &AQProjectTreeWidget::createNewFileRequested, this, &AProjectExplorer::createNewFileRequested);
	//connect(treeProject, &AQProjectTreeWidget::openNodeFileRequested, this, &AProjectExplorer::openNodeFileRequested);
}

AProjectExplorer::~AProjectExplorer()
{
	delete treeProject;
}

void AProjectExplorer::loadProjectTree(ARootProjectNode * root_node)
{
	//treeProject->loadProjectTree(root_node);
}

void AProjectExplorer::setProject(AProject * prj)
{
	pProject = prj;
	updateTree();
}

void AProjectExplorer::updateTree()
{
	treeProject->clear();

	if (!pProject)
		return;

	auto root_item = new QTreeWidgetItem(treeProject);
	root_item->setText(0, tr("Project '%1'").arg(pProject->fileName));

	auto docs_item = new QTreeWidgetItem(root_item);
	docs_item->setText(0, tr("documents"));

	for (auto ref : pProject->documents)
	{
		auto doc_item = new QTreeWidgetItem(docs_item);
		doc_item->setText(0, ref->relativePath);
		docs_item->addChild(doc_item);

		doc_item->setData(0, Qt::UserRole, qVariantFromValue(ref));

	}

	treeProject->expandAll();
}

void AProjectExplorer::openNodeFileRequested(QTreeWidgetItem *item)
{
	auto doc_ref = item->data(0, Qt::UserRole).value<ADocumentRef*>();
	if (doc_ref)
	{
		emit openDocumentRequested(doc_ref);
	}
}

void AProjectExplorer::showContextMenu(const QPoint &pos)
{
	//Determine selection
	QTreeWidgetItem* selected_item = treeProject->itemAt(pos);
	
	QMenu contextMenu("Context menu", this);
	if (selected_item)
	{
		if (selected_item->data(0, Qt::UserRole).value<ADocumentRef*>() != NULL)
		{
			QAction * actionOpen = new QAction(QIcon(":/SurveyViewer/resources/icons/project_tree/open_file.ico"), "Open", this);
			contextMenu.addAction(actionOpen);
			connect(actionOpen, &QAction::triggered, [=]() {this->openNodeFileRequested(selected_item); });

			QAction * actionRemove = new QAction(QIcon(":/SurveyViewer/resources/icons/project_tree/remove.ico"), "Remove", this);
			contextMenu.addAction(actionRemove);
			connect(actionRemove, &QAction::triggered, [=]() {this->onRemoveItemClicked(selected_item); });

			QAction * actionRename = new QAction(QIcon(":/SurveyViewer/resources/icons/project_tree/rename.ico"), "Rename", this);
			contextMenu.addAction(actionRename);
			connect(actionRename, &QAction::triggered, this, &AProjectExplorer::onRenameItemClicked);
		}
		else
		{
			QMenu * menuAdd = new QMenu("Add", this);
			QAction * actionAddNewFile = new QAction(QIcon(":/SurveyViewer/resources/icons/project_tree/file.ico"), "New Item...", menuAdd);
			menuAdd->addAction(actionAddNewFile);
			connect(actionAddNewFile, &QAction::triggered, [=]() {this->createNewFileRequested(); });
			
			contextMenu.addMenu(menuAdd);
		}


	}
	m_pCurrentSelection = selected_item;
	contextMenu.exec(treeProject->viewport()->mapToGlobal(pos));
}

void AProjectExplorer::onRenameItemClicked()
{
	treeProject->openPersistentEditor(m_pCurrentSelection);
	editing = true;
}

void AProjectExplorer::changeItemName(QTreeWidgetItem *item, int column)
{
	if (editing) {
		auto new_name = item->text(0);
		auto doc_ref = item->data(0, Qt::UserRole).value<ADocumentRef*>();

		QFile::rename(doc_ref->absolutePath(), pProject->dirPath + "/" + new_name);
		doc_ref->relativePath = new_name;
		pProject->save();

		treeProject->closePersistentEditor(m_pCurrentSelection);
		editing = false;
	}
}

void AProjectExplorer::onRemoveItemClicked(QTreeWidgetItem *item)
{
	auto res = QMessageBox::warning(this, "Confirm", QString("Are you sure you want to remove %1?").arg(m_pCurrentSelection->data(0, Qt::UserRole).value<ADocumentRef*>()->relativePath), QMessageBox::Ok | QMessageBox::Cancel);

	if (res == QMessageBox::Ok)
	{
		auto doc_ref = item->data(0, Qt::UserRole).value<ADocumentRef*>();
		QFile::remove(doc_ref->absolutePath());
		int n = (std::find(pProject->documents.begin(), pProject->documents.end(), doc_ref) - pProject->documents.begin());
		pProject->documents.erase(pProject->documents.begin() + n);
		pProject->save();
		delete item;
	}
}
