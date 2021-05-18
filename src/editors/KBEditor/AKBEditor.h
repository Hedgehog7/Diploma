#ifndef AKBEDITOR_H
#define AKBEDITOR_H

#include <QtWidgets/QWidget>
#include <ATGUI/AEditor.h>
//#include "ui_AKBEditor.h"
#include <QList>
#include "kbeditor_global.h"
#include <ATCore/documents/AKBDocument.h>

#include <QTreeWidgetItem>
#include "KBObject.h"
#include "KBCreateObjectDialog.h"
#include "KnowledgeBase.h"
#include "KBCreateIntervalDialog.h"
#include "KBCreateEventDialog.h"
#include "KBCreateRuleDialog.h"
#include "KBStatementDialog.h"
#include "KBStatementWidget.h"
#include "KBStatementTemporalDialog.h"

class QTreeWidget;
class QWidget;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QLineEdit;
class QComboBox;
class QDialog;

class KBEDITOR_EXPORT AKBEditor : public AGUIEditorInstance
{
	Q_OBJECT

public:
	AKBEditor(AGUIEditorPlugin * _plug, QWidget *parent = 0);
	~AKBEditor();
	virtual void showDocument() override;

	static int ruleIterator;
    void addStatementToTree(KBStatement** pp_statement, QTreeWidgetItem* parentItem);
    void addNodeToTree(KBNode* node, QTreeWidgetItem* parentItem);


private slots:
	void resizeButton1();
    void addRuleToTree();
//    void addRuleToTreeClose();
	void addIntervalToTree();
    void addObjectToTree();
	void addEventToTree();
	void addObjectToTreeClose();
    void addIntervalToTreeClose();
    void addEventToTreeClose();
	void updateObjectsTree();
    void updateTree();
    void updateIntervalsTree();
    void updateEventsTree();
    void updateRulesTree();
    void prepareObjectContextMenu(const QPoint &pos);
    void prepareRuleContextMenu(const QPoint &pos);
    void editObject();
    void deleteObject();
    void editInterval();
    void deleteInterval();
    void editEvent();
    void deleteEvent();
    void editRule();
    void deleteRule();
    void addThenStatement();
    void addThenStatementClose();
    void editStatement();
    void deleteStatement();
    void editNodeStatement();
    void deleteNode();
    void addStatement();
    void addOperatorOR();
    void addOperatorAND();
    void addOperatorNOT();
    void editOperatorOR();
    void editOperatorAND();
    void editOperatorNOT();
    void addTemporalStatement();
    void editTemporalStatement();

private:
	QString objectsToString();
	//Ui::AKBEditor ui;
	QWidget* wdg;
	QVBoxLayout* objectsLay;
	QTreeWidget* treeRules;
	QTreeWidget* treeObjects;
	QPushButton* addRuleButton;
    KnowledgeBase kb;
	KBCreateObjectDialog* createObjectDialog;
    KBCreateIntervalDialog* createIntervalDialog;
    KBCreateEventDialog* createEventDialog;
    KBStatementDialog* createStatementDialog;
    KBStatementTemporalDialog* createStatementTemporalDialog;

    QString currentRuleName;

    void showAddIntervalDialog(KBInterval* interval);
    void showAddObjectDialog(KBObject* object);
    void showAddEventDialog(KBEvent* event);
    void showAddStatementDialog(KBStatement* statement);
    void showAddStatementTemporalDialog(KBStatementTemporal* statement);

};

class KBEditorPlugin : public AGUITEditorPlugin<AKBDocument, AKBEditor>
{
	using _Base = AGUITEditorPlugin<AKBDocument, AKBEditor>;
public:
	KBEditorPlugin()
		:_Base("kbs", "Knowldege Base Editor", "Knowldege Base", "Knowledge Base Editor", "Editor for knowledge base / knowledge field in AT representation language.")
	{}

	virtual AError init(QToolBar * tb, QMenu * menu);
	virtual void openFile(ADocument * file);
};


#endif // AKBEDITOR_H
