#ifndef APLANNERWIDGET_H
#define APLANNERWIDGET_H

#include "config.h"
#include <QWidget>

#include "ATPlanner.h"
#include <edfd_cover/APlan.h>

class ATPlanner;
class APlan;
class QTreeWidget;

class AT_PLANNER_API APlannerWidget : public QWidget
{
	Q_OBJECT

public:
	APlannerWidget(ATPlanner * _planner, QWidget *parent = 0);
	~APlannerWidget();

	/*
	Is called when plan was rebuilt.
	*/
	virtual void planRebuilt(ATPlanner * planner, APlan * plan);

	void displayPlan(APlan * new_plan);
	void updatePlan();
signals:
	void startArchDocumentCreated(ADocumentProjectNode * arch_doc_node);
	void projectStructureChanged();
	void executeTaskRequested(APlanTask *);

private:
	//Ui::APlannerWidget * ui;
	ATPlanner * m_pPlanner;
	QTreeWidget * treePlan;
	APlan * pPlan;
};


#endif // APLANNERWIDGET_H
