#ifndef APROJECTEXPLORER_H
#define APROJECTEXPLORER_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QTreeWidget>

class AGroupProjectNode;
class AQProjectTreeWidget;
class ARootProjectNode;
class AProject;
class AQProjectNode;
class ADocumentProjectNode;
class ADocumentRef;

class AProjectExplorer : public QWidget
{
	Q_OBJECT

public:
	AProjectExplorer(QWidget *parent = 0);
	~AProjectExplorer();
	void loadProjectTree(ARootProjectNode * root_node);
	void setProject(AProject * prj);
	void updateTree();
	private slots:
	void showContextMenu(const QPoint &pos);
	void onRenameItemClicked();
	void onRemoveItemClicked(QTreeWidgetItem * item);
	void changeItemName(QTreeWidgetItem *item, int column);
	void openNodeFileRequested(QTreeWidgetItem * item);
signals:
	void createNewFileRequested();
	void openDocumentRequested(ADocumentRef *);
private:
	bool editing = false;
	QTreeWidget * treeProject;
	AProject * pProject;
	QTreeWidgetItem * m_pCurrentSelection;
};

#endif // APROJECTEXPLORER_H
