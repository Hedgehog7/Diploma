
#include "APlannerWidget.h"
#include "ATPlanner.h"
#include <edfd_cover/APlan.h>
#include <ATProject/AProject.h>
#include <ATGUI/APluginManager.h>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMessageBox>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <iostream>
//#include "ui_APlannerWidget.h"

Q_DECLARE_METATYPE(APlanTask*);

APlannerWidget::APlannerWidget(ATPlanner * _planner, QWidget *parent)
	: QWidget(parent), m_pPlanner(_planner)
{
//	ui = new Ui::APlannerWidget;
//	ui->setupUi(this);
    treePlan = new QTreeWidget(this);
    treePlan->header()->close();
    connect(treePlan, &QTreeWidget::itemDoubleClicked, [=](QTreeWidgetItem * item, int) {
        if(item->isDisabled())
            return;
        auto task = item->data(0, Qt::UserRole).value<APlanTask*>();
        if (task && (!task->closed))
        {
            for (auto t : this->pPlan->tasks)
            {
                if (t->inProgress)
                {
                    int ret = QMessageBox::warning(this, tr("ATPlanner"),
                        tr("Some other task is currently executing."),
                        QMessageBox::Ok);
                    return;
                }
            }

            emit executeTaskRequested(task);
        }
    });


    auto lyt = new QVBoxLayout;
    lyt->addWidget(treePlan);
	
    auto bnGeneratePlan = new QPushButton(tr("Generate Plan"));
    //Build generalized plan
    connect(bnGeneratePlan, &QPushButton::clicked, [=](){m_pPlanner->buildGeneralizedPlan();});
    lyt->addWidget(bnGeneratePlan);

    setLayout(lyt);

	//Build start architecture model
	/*connect(ui->bnBuildStartModel, &QPushButton::clicked, [=](){

		//Check if architecture model already exists in project
		auto old_arch = m_pPlanner->project()->architectureDocument();
		if(old_arch)
		{
			auto res = QMessageBox::warning(this, tr("Warning"), tr("Architecture model document already is in project. Would you like to rebuild new and overwrite it?."), QMessageBox::Ok | QMessageBox::Cancel);
			if(res == QMessageBox::Cancel)
				return;
			else
				m_pPlanner->project()->removeDocumentsWithExtension("arch");
		}

		//auto arch_doc_node = m_pPlanner->buildStartingArchitectureModel(command_executor());
		//emit startArchDocumentCreated(arch_doc_node);
		emit projectStructureChanged();
	});

	connect(ui->bnBuildDetailedPlan, &QPushButton::clicked, [=](){
		AError res = m_pPlanner->buildDetailPlan();
		if(!res.OK())
			AError::criticalErrorOccured(res);
	});*/
}

APlannerWidget::~APlannerWidget()
{

}

void APlannerWidget::planRebuilt(ATPlanner * planner, APlan * plan)
{
	displayPlan(plan);
}

void APlannerWidget::displayPlan(APlan * new_plan)
{
	pPlan = new_plan;
	updatePlan();

	

	/*for(auto & t : new_plan->tasks())
	{
		QStringList row;
		row << QString::fromStdString(t->name());
		QTreeWidgetItem * gtask_item = new QTreeWidgetItem(ui->treeCurrentPlan, row);

		ui->treeCurrentPlan->addTopLevelItem(gtask_item);
	}*/

	/*for(auto & dg : new_plan->displayGroups())
	{
		QStringList row;
		row << QString::fromStdString(dg.general_task->name());
		QTreeWidgetItem * gtask_item = new QTreeWidgetItem(ui->treeCurrentPlan, row);
		ui->treeCurrentPlan->addTopLevelItem(gtask_item);

		QString icon_name;

		switch (dg.general_task->dfdType())
		{
		case DFDElement::Type::Function:
			icon_name = "rounded-rectangle.png";
			break;
		case DFDElement::Type::NFFunction:
			icon_name = "trapeze.png";
			break;
		case DFDElement::Type::Storage:
			icon_name = "db.png";
			break;
		default:
			break;
		}
		
		gtask_item->setIcon(0, QIcon(":/ATPlanner/Resources/" + icon_name));

		for(auto st : dg.sub_tasks)
		{
			QTreeWidgetItem * sub_item = new QTreeWidgetItem(gtask_item);
			sub_item->setText(0, QString::fromStdString(st->name()));

			//Checkbox
			QCheckBox * cb = new QCheckBox(ui->treeCurrentPlan);
			cb->setEnabled(false);
			//cb->setCheckable(false);
			cb->setChecked(st->done());
			ui->treeCurrentPlan->setItemWidget(sub_item, 1, cb);

			//Button for executing
			QPushButton * bnExecute = new QPushButton(ui->treeCurrentPlan);
			bnExecute->setText("Execute");
			bnExecute->setEnabled(!st->done());
			connect(bnExecute, &QPushButton::clicked, [=](){
				int res = m_pPlanner->executeTask(st);
				if(!res)
				{
					bnExecute->setEnabled(false);
					//cb->setCheckable(true);
					cb->setChecked(true);
					//cb->setCheckable(false);
				}
			});

			ui->treeCurrentPlan->setItemWidget(sub_item, 2, bnExecute);

		}
	}*/
}

void APlannerWidget::updatePlan()
{
	treePlan->clear();

	auto htn_pres = pPlan->hierarchyPresentation();

	/*auto ti_iter = htn_pres->tree.top_level_iterator();

	ti_iter.for_each_branch([=](const AHierarchyPlanPresentation::tree_t::level_iterator & group_level) {
		QStringList row;
		row << QString::fromStdString(group_level.representative()->group);
		QTreeWidgetItem * group_item = new QTreeWidgetItem(treePlan, row);

		group_level.for_each_branch([=](const AHierarchyPlanPresentation::tree_t::level_iterator & stage_level) {
			QStringList row;
			row << QString::fromStdString(stage_level.representative()->stage);
			QTreeWidgetItem * stage_item = new QTreeWidgetItem(group_item, row);

			stage_level.for_each_element([=](APlanTask * task) {
				QStringList row;
				row << QString::fromStdString(task->name);
				QTreeWidgetItem * task_item = new QTreeWidgetItem(stage_item, row);
				task_item->setData(0, Qt::UserRole, qVariantFromValue(task));

				task_item->setFlags(task_item->flags() & (~Qt::ItemFlag::ItemIsUserCheckable));
				if (task->closed)
				{
					task_item->setCheckState(0, Qt::Checked);
				}
				else
				{
					task_item->setCheckState(0, Qt::Unchecked);
					if (pPlan->isAvailable(task))
					{
						task_item->setDisabled(false);
					}
					else
						task_item->setDisabled(true);
				}
			});
		});
	});*/

	for (auto gtask : htn_pres->generalTasks)
	{
        QStringList row;
        row << QString::fromStdWString(gtask->name);
        QTreeWidgetItem * group_item = new QTreeWidgetItem(treePlan, row);

        for (auto stage : gtask->stages)
        {
            QStringList row;
            row << QString::fromStdWString(stage->name);
            QTreeWidgetItem * stage_item = new QTreeWidgetItem(group_item, row);

            for (auto task : stage->tasks)
            {
                QStringList row;
                row << QString::fromStdString(task->name);
                QTreeWidgetItem * task_item = new QTreeWidgetItem(stage_item, row);
                task_item->setData(0, Qt::UserRole, qVariantFromValue(task));
                task_item->setFlags(task_item->flags() & (~Qt::ItemFlag::ItemIsUserCheckable));
                if (task->closed)
                {
                    task_item->setCheckState(0, Qt::Checked);
                }
                else
                {
                    task_item->setCheckState(0, Qt::Unchecked);
                    if (pPlan->isAvailable(task))
                    {
                        task_item->setDisabled(false);
                    }
                    else
                        task_item->setDisabled(true);
                }
                //TODO color text by available rucs
                //APluginManager* temp_check = new APluginManager;
                if (m_pPlanner->m_pPluginManager->plugins.count(task->executorId) == 0){
                    task_item->setTextColor(0, QColor(255,0,0));
                }

            }
        }
	}

    treePlan->expandAll();
}
