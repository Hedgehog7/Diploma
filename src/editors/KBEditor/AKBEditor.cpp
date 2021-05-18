#include "AKBEditor.h"
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <Qt>
#include <QDebug>
#include <QDate>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QJsonObject>
#include "vptr.h"
#include "KBObject.h"
#include "KBStatementWidget.h"

int AKBEditor::ruleIterator = 0;
const char* THEN_NODE = "then";
const char* IF_NODE = "if";
const char* STATEMENT_NODE = "STATEMENT_NODE";
const char* TEMPORAL_STATEMENT = "TEMPORAL_STATEMENT";
const char* IF_STATEMENT_NODE = "IF_STATEMENT_NODE";

const char* RULE_NODE = "RULE_NODE";
const char* OPERATOR = "OPERATOR";
APlugin * AT_CREATE_PLUGIN_FN()
{
	return new KBEditorPlugin();
}

AKBEditor::AKBEditor(AGUIEditorPlugin * _plug, QWidget *parent)
    :AGUIEditorInstance(_plug, parent)
{

	//-------------------init UI---------------------
	//new widgets
	wdg = new QWidget(this);
	QHBoxLayout * mainLay = new QHBoxLayout;
	QVBoxLayout * ruleLay = new QVBoxLayout;
	//QVBoxLayout * objectsLay = new QVBoxLayout;
	objectsLay = new QVBoxLayout;
	//QTreeWidget * rules = new QTreeWidget;
	treeRules = new QTreeWidget;
	treeObjects = new QTreeWidget;
	treeObjects->setColumnCount(2);
    treeRules->setColumnCount(2);
    treeObjects->setContextMenuPolicy(Qt::CustomContextMenu);
    treeRules->setContextMenuPolicy(Qt::CustomContextMenu);
	//QTreeWidget * newObjects = new QTreeWidget;
	QHBoxLayout * bRUleLay = new QHBoxLayout;
	QHBoxLayout * bObjectsLay = new QHBoxLayout;
	QPushButton * bAddRule = new QPushButton("Add rule");
    QPushButton * bAddObjects = new QPushButton("Add object");
	QPushButton * bAddIntervals = new QPushButton("Add interval");
	QPushButton * bAddEvents = new QPushButton("Add event");

	//rules space
	mainLay->addLayout(ruleLay);
	ruleLay->addWidget(treeRules);
	ruleLay->addLayout(bRUleLay);
	bRUleLay->addWidget(bAddRule);

	//objects space
	mainLay->addLayout(objectsLay);
	objectsLay->addWidget(treeObjects);
	objectsLay->addLayout(bObjectsLay);
	bObjectsLay->addWidget(bAddObjects);
	bObjectsLay->addWidget(bAddIntervals);
	bObjectsLay->addWidget(bAddEvents);

	//set workspace
	wdg->setLayout(mainLay);
	setCentralWidget(wdg);

    updateTree();
	//buttons actions
	connect(bAddRule, &QPushButton::clicked, this, &AKBEditor::addRuleToTree);
	connect(bAddObjects, &QPushButton::clicked, this, &AKBEditor::addObjectToTree);
	connect(bAddIntervals, &QPushButton::clicked, this, &AKBEditor::addIntervalToTree);
	connect(bAddEvents, &QPushButton::clicked, this, &AKBEditor::addEventToTree);
    connect(treeObjects, &QTreeWidget::customContextMenuRequested, this, &AKBEditor::prepareObjectContextMenu);
    connect(treeRules, &QTreeWidget::customContextMenuRequested, this, &AKBEditor::prepareRuleContextMenu);
	//-----------------init data--------------------------

}

AKBEditor::~AKBEditor()
{

}

void AKBEditor::resizeButton1(){

}

void AKBEditor::showDocument()
{
	/*for (int i = 0; i < 30; ++i)
	{
		QStringList data;
		data << QString("Rule #%1").arg(i);
		auto new_it = new QTreeWidgetItem(treeRules, data);
	}*/
}

void AKBEditor::addRuleToTree() 
{
    static int ruleCount = 0;
    KBRule* rule = new KBRule;
    rule->m_name = "Rule" + QString::number(ruleCount);
    kb.addRule(*rule);
    ruleCount++;
    updateRulesTree();
}


void AKBEditor::showAddObjectDialog(KBObject* object) {
    createObjectDialog = new KBCreateObjectDialog(object, this);
    connect(createObjectDialog, &KBCreateObjectDialog::accepted, this, &AKBEditor::addObjectToTreeClose);
    createObjectDialog->exec();
    updateTree();
}

void AKBEditor::showAddEventDialog(KBEvent* event)
{
    qDebug() << "Enter addEventToTree";
    createEventDialog = new KBCreateEventDialog(event, &kb, this);
    qDebug() << "Connect accepted -> addEventToTreeClose";
    connect(createEventDialog, &KBCreateEventDialog::accepted, this, &AKBEditor::addEventToTreeClose);
    qDebug() << "createEventDialog exec";
    createEventDialog->exec();
    qDebug() << "Leave addEventToTree";
    updateTree();
}


void AKBEditor::showAddStatementDialog(KBStatement* statement)
{
    qDebug() << "Enter showAddStatementDialog";
    createStatementDialog = new KBStatementDialog(statement, &kb, this);
    qDebug() << "Connect accepted -> addThenStatementClose";
//    connect(createStatementDialog, &KBStatementDialog::accepted, this, &AKBEditor::addThenStatementClose);
    qDebug() << "createRuleDialog exec";
    createStatementDialog->exec();
//    m_rule->m_thenStatement = createStatementDialog->m_backup;
//    rule->m_thenStatement = createStatementDialog->m_backup;
    updateRulesTree();
    qDebug() << "Leave createStatementDialog";
}

void AKBEditor::showAddStatementTemporalDialog(KBStatementTemporal *statement)
{
    qDebug() << "Enter showAddStatementTemporalDialog";
    createStatementTemporalDialog = new KBStatementTemporalDialog(statement, &kb, this);
    qDebug() << "Connect accepted -> addThenStatementClose";
//    connect(createStatementDialog, &KBStatementDialog::accepted, this, &AKBEditor::addThenStatementClose);
    qDebug() << "createRuleDialog exec";
    createStatementTemporalDialog->exec();
//    m_rule->m_thenStatement = createStatementDialog->m_backup;
//    rule->m_thenStatement = createStatementDialog->m_backup;
    updateRulesTree();
    qDebug() << "Leave createStatementDialog";
}


void AKBEditor::addThenStatementClose() {
    qDebug() << "Enter addThenStatementClose";
//    KBStatement statement = *statement;
    qDebug() << "Add Statement";
//    qDebug() << kb.rulesToString();
    qDebug() << "Update Rule tree";
    updateRulesTree();
    qDebug() << "Leave addThenStatementClose";
}

void AKBEditor::addObjectToTree()
{
    KBObject* object = new KBObject;
    showAddObjectDialog(object);
}

void AKBEditor::showAddIntervalDialog(KBInterval* interval) {
    qDebug() << "Enter addIntervalToTree";
    createIntervalDialog = new KBCreateIntervalDialog(interval, &kb, this);
    qDebug() << "Connect accepted -> addInterlToTreeClose";
    connect(createIntervalDialog, &KBCreateIntervalDialog::accepted, this, &AKBEditor::addIntervalToTreeClose);
    qDebug() << "createIntervalDialog exec";
    createIntervalDialog->exec();
    qDebug() << "Leave addIntervalToTree";
    updateTree();
}

void AKBEditor::addIntervalToTree()
{
    if (kb.objects.count() < 1) {
        auto msgBox = new QMessageBox(QMessageBox::Critical, "Not enough objects", "You must create at least one object first! Then try again.", QMessageBox::Ok, this);
        msgBox->exec();
        delete msgBox;
    } else {
        KBInterval* interval = new KBInterval;
        showAddIntervalDialog(interval);
    }
}

void AKBEditor::addEventToTree()
{
    qDebug() << "addEventToTree";
    if (kb.objects.count() < 1) {
        auto msgBox = new QMessageBox(QMessageBox::Critical, "Not enough objects", "You must create at least one object first! Then try again.", QMessageBox::Ok, this);
        msgBox->exec();
        delete msgBox;
    } else {
        KBEvent* event = new KBEvent;
        showAddEventDialog(event);
    }

}

void AKBEditor::addObjectToTreeClose()
{
    KBObject* obj = createObjectDialog->m_object;
    kb.addObject(*obj);
    qDebug() << kb.objectsToString();
    updateTree();
}

void AKBEditor::addIntervalToTreeClose()
{
    qDebug() << "addIntervalToTreeClose";
    KBInterval interval = *createIntervalDialog->m_interval;
    qDebug() << "Add interval";
    kb.addInterval(interval);
    qDebug() << "Update tree";
    qDebug() << kb.intervalsToString();
    updateTree();
}

void AKBEditor::addEventToTreeClose()
{
    qDebug() << "addEventToTreeClose";
    KBEvent event = *createEventDialog->m_event;
    qDebug() << "Add Event";
    kb.addEvent(event);
    qDebug() << "Update tree";
    qDebug() << kb.eventsToString();
    updateTree();
}

void AKBEditor::updateTree() {
    treeObjects->clear();
    updateObjectsTree();
    updateIntervalsTree();
    updateEventsTree();
}

void AKBEditor::updateIntervalsTree() {
    QTreeWidgetItem* treeIntervalRoot = new QTreeWidgetItem(treeObjects);
    treeIntervalRoot->setText(0, "Intervals");
    treeIntervalRoot->setExpanded(true);

    for (auto it_interval = kb.intervals.begin(); it_interval != kb.intervals.end(); it_interval++) {
        auto intervalItem = new QTreeWidgetItem(treeIntervalRoot);
        intervalItem->setExpanded(true);
        QVariant v(it_interval->m_name);
        intervalItem->setData(0, Qt::UserRole + 1, v);
        intervalItem->setText(0, it_interval->m_name);

        auto openItem = new QTreeWidgetItem(intervalItem);
        v.setValue(QString("Open"));
        openItem->setData(0, Qt::UserRole + 1, v);
        openItem->setText(0, QString("Open"));

        v.setValue(it_interval->m_open.toString());
        openItem->setData(1, Qt::UserRole + 1, v);
        openItem->setText(1, it_interval->m_open.toString());

        auto closeItem = new QTreeWidgetItem(intervalItem);
        v.setValue(QString("Close"));
        closeItem->setData(0, Qt::UserRole + 1, v);
        closeItem->setText(0, QString("Close"));

        v.setValue(it_interval->m_close.toString());
        closeItem->setData(1, Qt::UserRole + 1, v);
        closeItem->setText(1, it_interval->m_close.toString());
    }
}

void AKBEditor::updateEventsTree() {
    QTreeWidgetItem* treeEventRoot = new QTreeWidgetItem(treeObjects);
    treeEventRoot->setText(0, "Events");
    treeEventRoot->setExpanded(true);

    for (auto it_event = kb.events.begin(); it_event != kb.events.end(); it_event++) {
        auto eventItem = new QTreeWidgetItem(treeEventRoot);
        eventItem->setExpanded(true);
        QVariant v(it_event->m_name);
        eventItem->setData(0, Qt::UserRole + 1, v);
        eventItem->setText(0, it_event->m_name);

        auto formulaItem = new QTreeWidgetItem(eventItem);
        v.setValue(QString("Formula"));
        formulaItem->setData(0, Qt::UserRole + 1, v);
        formulaItem->setText(0, QString("Formula"));

        v.setValue(it_event->m_formula.toString());
        formulaItem->setData(1, Qt::UserRole + 1, v);
        formulaItem->setText(1, it_event->m_formula.toString());
    }

}

void AKBEditor::addNodeToTree(KBNode* node, QTreeWidgetItem* parentItem) {
    auto item = new QTreeWidgetItem(parentItem);
    if (node->m_type == "statement") {
        KBStatement* statement = VPtr<KBStatement>::asPtr(node->m_data);
        item->setData(0, Qt::UserRole, QVariant(QJsonObject({{"enabled", true}, {"type", IF_STATEMENT_NODE}})));
        item->setData(1, Qt::UserRole, VPtr<KBNode>::asQVariant(node));
        item->setText(0, statement->statementString());
        item->setText(1, statement->withString());
    } else if (node->m_type == TEMPORAL_STATEMENT) {
        KBStatementTemporal* statement = VPtr<KBStatementTemporal>::asPtr(node->m_data);
        item->setData(0, Qt::UserRole, QVariant(QJsonObject({{"enabled", true}, {"type", TEMPORAL_STATEMENT}})));
        item->setData(1, Qt::UserRole, VPtr<KBNode>::asQVariant(node));
        item->setText(0, statement->toString());
    } else {
        item->setText(0, node->m_data.toString());
        item->setExpanded(true);
        auto type = OPERATOR;
        if (node->m_type != "operator") {
            type = IF_NODE;
        }
        item->setData(0, Qt::UserRole, QVariant(QJsonObject({{"enabled", true}, {"type", type}})));
        item->setData(1, Qt::UserRole, VPtr<KBNode>::asQVariant(node));

        for (auto it = node->m_children.begin(); it != node->m_children.end(); it++) {
            addNodeToTree(&(*it), item);
        }
    }
}

void AKBEditor::addStatementToTree(KBStatement** pp_statement, QTreeWidgetItem* parentItem) {
    auto statementItem = new QTreeWidgetItem(parentItem);
    statementItem->setData(0, Qt::UserRole, QVariant(QJsonObject({{"enabled", true}, {"type", STATEMENT_NODE}})));
    statementItem->setData(1, Qt::UserRole, VPtr<KBStatement>::asQVariant(*pp_statement));
    statementItem->setData(1, Qt::UserRole + 1, VPtr<KBStatement*>::asQVariant(pp_statement));
    statementItem->setText(0, (*pp_statement)->statementString());
    statementItem->setText(1, (*pp_statement)->withString());
}

void AKBEditor::updateRulesTree()
{
    treeRules->clear();
    for (auto it_rule = kb.rules.begin(); it_rule != kb.rules.end(); it_rule++) {
        QTreeWidgetItem* treeRuleItem = new QTreeWidgetItem(treeRules);
        treeRuleItem->setText(0, it_rule->m_name);
        treeRuleItem->setExpanded(true);
        treeRuleItem->setData(0, Qt::UserRole, QVariant(QJsonObject({{"enabled", true}, {"type", RULE_NODE}})));

        addNodeToTree(it_rule->m_ifNode, treeRuleItem);

        auto thenItem = new QTreeWidgetItem(treeRuleItem);
        thenItem->setText(0, "THEN");
        if (it_rule->m_thenStatement == nullptr) {
            thenItem->setData(0, Qt::UserRole, QVariant(QJsonObject({{"enabled", true}, {"type", THEN_NODE}})));
        } else {
            addStatementToTree(&it_rule->m_thenStatement, thenItem);
        };
        thenItem->setExpanded(true);
    }
}

void AKBEditor::updateObjectsTree()
{
	//add objects root
	QTreeWidgetItem* treeObjectRoot = new QTreeWidgetItem(treeObjects);
	treeObjectRoot->setText(0, "Objects");
    treeObjectRoot->setExpanded(true);
    for (auto it_object = kb.objects.begin(); it_object != kb.objects.end(); it_object++) {
		auto objectItem = new QTreeWidgetItem(treeObjectRoot);
        objectItem->setExpanded(true);
		QVariant v(it_object->m_name);
		objectItem->setData(0, Qt::UserRole + 1, v);
		objectItem->setText(0, it_object->m_name);

		for (auto it_attribute = it_object->m_attributes.begin(); it_attribute != it_object->m_attributes.end(); it_attribute++) {
			auto attrItem = new QTreeWidgetItem(objectItem);
			//attribute text
			v.setValue(it_attribute->m_name);
			attrItem->setData(0, Qt::UserRole + 1, v);
			attrItem->setText(0, it_attribute->m_name);

			v.setValue(it_attribute->m_type);
			attrItem->setData(1, Qt::UserRole + 1, v);
			attrItem->setText(1, it_attribute->m_type);
		}
    }
}

void AKBEditor::prepareObjectContextMenu(const QPoint& pos)
{
    QModelIndex index = treeObjects->indexAt(pos);
    QStringList sl;
    sl << "Objects" << "Intervals" << "Events";
    if (!index.isValid())
        return;
    auto item = treeObjects->itemAt(pos);
    auto parent = item->parent();
    qDebug() << "Parent addr: " << parent;
    if (!(parent && sl.contains(parent->text(0))))
        return;
    qDebug() << "Parent text: " << parent->text(0);

    qDebug() << "Item addr: " << item;
    qDebug() << "Item text: " << item->text(0);

    QMenu menu(this);
    QAction *editAct = new QAction(tr("&Edit"), treeObjects);
    QAction *deleteAct = new QAction(tr("&Delete"), treeObjects);

    deleteAct->setData(item->text(0));
    editAct->setData(item->text(0));

    if (parent->text(0) == "Objects") {
        connect(editAct, &QAction::triggered, this, &AKBEditor::editObject);
        connect(deleteAct, &QAction::triggered, this, &AKBEditor::deleteObject);
    } else if (parent->text(0) == "Intervals") {
        connect(editAct, &QAction::triggered, this, &AKBEditor::editInterval);
        connect(deleteAct, &QAction::triggered, this, &AKBEditor::deleteInterval);
    } else {
        connect(editAct, &QAction::triggered, this, &AKBEditor::editEvent);
        connect(deleteAct, &QAction::triggered, this, &AKBEditor::deleteEvent);
    }

    menu.addAction(editAct);
    menu.addAction(deleteAct);

    menu.exec(treeObjects->mapToGlobal(pos));
}

void AKBEditor::prepareRuleContextMenu(const QPoint &pos)
{
    QModelIndex index = treeRules->indexAt(pos);
    if (!index.isValid())
        return;
    auto item = treeRules->itemAt(pos);
    auto parent = item->parent();
    qDebug() << "Parent addr: " << parent;
    auto data = item->data(0, Qt::UserRole).toJsonObject();
    qDebug() << "Item addr: " << item;
    qDebug() << "Item text: " << item->text(0);

    if (!data.value("enabled").toBool()) {
        qDebug() << "Item disabled" << item;
        return;
    }
    QMenu menu(this);

    if (data.value("type").toString() == RULE_NODE) {
        QAction *deleteRule = new QAction(tr("&Delete"), treeRules);
        deleteRule->setData(item->text(0));
        connect(deleteRule, &QAction::triggered, this, &AKBEditor::deleteRule);
        menu.addAction(deleteRule);
    } else if (data.value("type").toString() == THEN_NODE) {
            QAction *addAct = new QAction(tr("&Add statement"), treeRules);
            addAct->setData(parent->text(0));

            connect(addAct, &QAction::triggered, this, &AKBEditor::addThenStatement);

            menu.addAction(addAct);
     } else if (data.value("type").toString() == STATEMENT_NODE) {
            QAction *editStatement = new QAction(tr("&Edit"), treeRules);
            QAction *deleteStatement = new QAction(tr("&Delete"), treeRules);

            editStatement->setData(item->data(1, Qt::UserRole));
            deleteStatement->setData(item->data(1, Qt::UserRole + 1));

            connect(editStatement, &QAction::triggered, this, &AKBEditor::editStatement);
            connect(deleteStatement, &QAction::triggered, this, &AKBEditor::deleteStatement);

            menu.addAction(editStatement);
            menu.addAction(deleteStatement);
    } else if (data.value("type").toString() == IF_STATEMENT_NODE || data.value("type").toString() == TEMPORAL_STATEMENT) {
           QAction *editStatement = new QAction(tr("&Edit"), treeRules);
           QAction *deleteStatement = new QAction(tr("&Delete"), treeRules);

           editStatement->setData(item->data(1, Qt::UserRole));
           deleteStatement->setData(item->data(1, Qt::UserRole));

           if (data.value("type").toString() == TEMPORAL_STATEMENT) {
               connect(editStatement, &QAction::triggered, this, &AKBEditor::editTemporalStatement);
           } else {
               connect(editStatement, &QAction::triggered, this, &AKBEditor::editNodeStatement);
           }
           connect(deleteStatement, &QAction::triggered, this, &AKBEditor::deleteNode);

           menu.addAction(editStatement);
           menu.addAction(deleteStatement);
    } else {

        auto node =  VPtr<KBNode>::asPtr(item->data(1, Qt::UserRole));

        if (data.value("type").toString() == IF_NODE && node->m_children.count() > 0) {
            return;
        }

        if (!(node->m_data.toString() == "NOT" && node->m_children.count() > 0)) {
            auto addStatementMenu = menu.addMenu(tr("Add &Statement"));

            QAction *addStatement = new QAction(tr("&Statement"), treeRules);
            QAction *addStatementTemporal = new QAction(tr("&Temporal Statement"), treeRules);

            addStatement->setData(item->data(1, Qt::UserRole));
            addStatementTemporal->setData(item->data(1, Qt::UserRole));

            connect(addStatement, &QAction::triggered, this, &AKBEditor::addStatement);
            connect(addStatementTemporal, &QAction::triggered, this, &AKBEditor::addTemporalStatement);

            addStatementMenu->addAction(addStatement);
            addStatementMenu->addAction(addStatementTemporal);

            auto addOperatorMenu = menu.addMenu(tr("Add &Operator"));
            QAction *operatorOR = new QAction(tr("&OR"), treeRules);
            QAction *operatorAND = new QAction(tr("&AND"), treeRules);
            QAction *operatorNOT = new QAction(tr("&NOT"), treeRules);

            operatorOR->setData(item->data(1, Qt::UserRole));
            operatorAND->setData(item->data(1, Qt::UserRole));
            operatorNOT->setData(item->data(1, Qt::UserRole));

            connect(operatorOR, &QAction::triggered, this, &AKBEditor::addOperatorOR);
            connect(operatorAND, &QAction::triggered, this, &AKBEditor::addOperatorAND);
            connect(operatorNOT, &QAction::triggered, this, &AKBEditor::addOperatorNOT);

            addOperatorMenu->addAction(operatorAND);
            addOperatorMenu->addAction(operatorOR);
            addOperatorMenu->addAction(operatorNOT);
        }

        if (data.value("type").toString() == OPERATOR) {
            auto editOperatorMenu = menu.addMenu(tr("&Edit"));
            QAction *editOperatorOR = new QAction(tr("&OR"), treeRules);
            QAction *editOperatorAND = new QAction(tr("&AND"), treeRules);
            QAction *editOperatorNOT = new QAction(tr("&NOT"), treeRules);

            editOperatorOR->setData(item->data(1, Qt::UserRole));
            editOperatorAND->setData(item->data(1, Qt::UserRole));
            editOperatorNOT->setData(item->data(1, Qt::UserRole));

            connect(editOperatorOR, &QAction::triggered, this, &AKBEditor::editOperatorOR);
            connect(editOperatorAND, &QAction::triggered, this, &AKBEditor::editOperatorAND);
            connect(editOperatorNOT, &QAction::triggered, this, &AKBEditor::editOperatorNOT);



            editOperatorMenu->addAction(editOperatorOR);
            editOperatorMenu->addAction(editOperatorAND);
            editOperatorMenu->addAction(editOperatorNOT);

            QAction *deleteOperator = new QAction(tr("&Delete"), treeRules);
            deleteOperator->setData(item->data(1, Qt::UserRole));
            connect(deleteOperator, &QAction::triggered, this, &AKBEditor::deleteNode);
            menu.addAction(deleteOperator);
        }

    }
    menu.exec(treeRules->mapToGlobal(pos));
}

void AKBEditor::editObject()
{
    qDebug() << "editObject";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    QString objectName = action->data().toString();
    KBObject* object= kb.getObject(objectName);
    showAddObjectDialog(object);
}

void AKBEditor::deleteObject()
{
    qDebug() << "deleteObject";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    QString name = action->data().toString();
    kb.deleteObject(name);
    updateTree();
}

void AKBEditor::editInterval()
{
    qDebug() << "editInterval";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    QString intervalName = action->data().toString();
    KBInterval* interval = kb.getInterval(intervalName);
    showAddIntervalDialog(interval);
}

void AKBEditor::deleteInterval()
{
    qDebug() << "deleteInterval";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    QString name = action->data().toString();
    kb.deleteInterval(name);
    updateTree();
}

void AKBEditor::editEvent()
{
    qDebug() << "editEvent";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    QString eventName = action->data().toString();
    KBEvent* event= kb.getEvent(eventName);
    showAddEventDialog(event);
}

void AKBEditor::deleteEvent()
{
    qDebug() << "deleteEvent";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    QString name = action->data().toString();
    kb.deleteEvent(name);
    updateTree();

}

void AKBEditor::editRule()
{
    qDebug() << "editRule";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    QString ruleName = action->data().toString();
    KBRule* rule = kb.getRule(ruleName);
//    showAddRuleDialog(rule);
}

void AKBEditor::deleteRule()
{
    qDebug() << "deleteRule";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    QString ruleName = action->data().toString();
    kb.deleteRule(ruleName);
    updateRulesTree();
}
void AKBEditor::addThenStatement() {
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction*> (obj);
    QString name = action->data().toString();
    if (kb.objects.count() < 1) {
        auto msgBox = new QMessageBox(QMessageBox::Critical, "Not enough objects", "You must create at least one object first! Then try again.", QMessageBox::Ok, this);
        msgBox->exec();
        delete msgBox;
    } else {
        auto rule = kb.getRule(name);
        rule->m_thenStatement = new KBStatement;
        showAddStatementDialog(rule->m_thenStatement);
    }
}

void AKBEditor::editNodeStatement() {
    qDebug() << "editNodeStatement";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBNode* node = VPtr<KBNode>::asPtr(action->data());
    KBStatement* statement = VPtr<KBStatement>::asPtr(node->m_data);
    showAddStatementDialog(statement);
}

void AKBEditor::deleteNode() {
    qDebug() << "editNodeStatement";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBNode* node = VPtr<KBNode>::asPtr(action->data());
    node->die();
    updateRulesTree();
}


void AKBEditor::addStatement() {
    qDebug() << "editEvent";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBNode* node = VPtr<KBNode>::asPtr(action->data());
    if (kb.objects.count() < 1) {
        auto msgBox = new QMessageBox(QMessageBox::Critical, "Not enough objects", "You must create at least one object first! Then try again.", QMessageBox::Ok, this);
        msgBox->exec();
        delete msgBox;
    } else {
        KBStatement* statement = new KBStatement;
        KBNode* child = new KBNode;
        child->m_type = "statement";
        child->m_data = VPtr<KBStatement>::asQVariant(statement);
        child->m_parent = node;
        node->m_children.append(*child);
        showAddStatementDialog(statement);
    }
}

void AKBEditor::editStatement() {
    qDebug() << "editEvent";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBStatement* statement = VPtr<KBStatement>::asPtr(action->data());
    showAddStatementDialog(statement);
}

void AKBEditor::deleteStatement() {
    qDebug() << "deleteEvent";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBStatement** statement = VPtr<KBStatement*>::asPtr(action->data());
    delete *statement;
    *statement = nullptr;
    updateRulesTree();
}

void AKBEditor::editOperatorOR() {
    qDebug() << "editOperatorOR";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBNode* node = VPtr<KBNode>::asPtr(action->data());
    node->m_data = QVariant("OR");
    updateRulesTree();
}

void AKBEditor::editOperatorAND() {
    qDebug() << "editOperatorOR";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBNode* node = VPtr<KBNode>::asPtr(action->data());
    node->m_data = QVariant("AND");
    updateRulesTree();
}


void AKBEditor::editOperatorNOT() {
    qDebug() << "editOperatorOR";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBNode* node = VPtr<KBNode>::asPtr(action->data());
    node->m_data = QVariant("NOT");
    auto first = node->m_children.first();
    node->m_children.clear();
    node->m_children.append(first);
    updateRulesTree();
}

void AKBEditor::addTemporalStatement()
{
    qDebug() << "editEvent";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBNode* node = VPtr<KBNode>::asPtr(action->data());
    if (kb.intervals.count() < 1 && kb.events.count() < 1) {
        auto msgBox = new QMessageBox(QMessageBox::Critical, "Not enough intervals or events", "You must create at least one event or interval first! Then try again.", QMessageBox::Ok, this);
        msgBox->exec();
        delete msgBox;
    } else {
        KBStatementTemporal* statement = new KBStatementTemporal;
        KBNode* child = new KBNode;
        child->m_type = TEMPORAL_STATEMENT;
        child->m_data = VPtr<KBStatementTemporal>::asQVariant(statement);
        child->m_parent = node;
        node->m_children.append(*child);
        showAddStatementTemporalDialog(statement);
    }
}

void AKBEditor::editTemporalStatement()
{
    qDebug() << "editTemporalStatement";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBNode* node = VPtr<KBNode>::asPtr(action->data());
    KBStatementTemporal* statement = VPtr<KBStatementTemporal>::asPtr(node->m_data);
    showAddStatementTemporalDialog(statement);
}


void AKBEditor::addOperatorOR() {
    qDebug() << "addOperatorOR";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBNode* node = VPtr<KBNode>::asPtr(action->data());
    KBNode* child = new KBNode;
    child->m_type = "operator";
    child->m_data = QVariant("OR");
    child->m_parent = node;
    node->m_children.append(*child);
    updateRulesTree();
}

void AKBEditor::addOperatorAND() {
    qDebug() << "addOperatorAND";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBNode* node = VPtr<KBNode>::asPtr(action->data());
    KBNode* child = new KBNode;
    child->m_type = "operator";
    child->m_data = QVariant("AND");
    child->m_parent = node;
    node->m_children.append(*child);
    updateRulesTree();
}


void AKBEditor::addOperatorNOT() {
    qDebug() << "addOperatorNOT";
    QObject* obj = sender();
    QAction *action = qobject_cast<QAction *>(obj);
    KBNode* node = VPtr<KBNode>::asPtr(action->data());
    KBNode* child = new KBNode;
    child->m_type = "operator";
    child->m_data = QVariant("NOT");
    child->m_parent = node;
    node->m_children.append(*child);
    updateRulesTree();
}


AError KBEditorPlugin::init(QToolBar * tb, QMenu * menu)
{
	return AError();
}

void KBEditorPlugin::openFile(ADocument * file)
{
}
