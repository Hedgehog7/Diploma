
#include "ATaskExecutionWindow.h"
#include <edfd_cover/APlan.h>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QLabel>

ATaskExecutionPanel::ATaskExecutionPanel(QWidget * parent)
	:_Base(parent)
{
	auto lyt = new QVBoxLayout;

	bnDiscard = new QPushButton(tr("Discard"), this);
	lyt->addWidget(bnDiscard);

	bnFinish = new QPushButton(tr("Finish"), this);
	lyt->addWidget(bnFinish);

	lyt->addStretch(0);

	setLayout(lyt);
}

ATaskExecutionWindow::ATaskExecutionWindow(APlanTask * _task, QWidget * _parent)
	:_Base(_parent), task(_task), state(state_t::execution)
{
	task->inProgress = true;

	auto dock = new QDockWidget(this);
	dock->setWindowTitle(tr("Task Control"));
	auto wdg_control = new ATaskExecutionPanel(dock);
	
	dock->setWidget(wdg_control);
	dock->setAllowedAreas(Qt::DockWidgetArea::RightDockWidgetArea);

	addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dock);
	dock->show();

	connect(wdg_control->bnDiscard, &QPushButton::clicked, [=]() {
		state = state_t::discarded;
		emit discardTask();
		//close();
	});

	connect(wdg_control->bnFinish, &QPushButton::clicked, [=]() {
		state = state_t::finished;
		emit commitTask();
		//close();
	});
}

ATaskExecutionWindow::~ATaskExecutionWindow()
{
	task->inProgress = false;
}

void ATaskExecutionWindow::closeEvent(QCloseEvent * event)
{
	_Base::closeEvent(event);

	if (state == state_t::execution)
	{
		emit discardTask();
	}
}
