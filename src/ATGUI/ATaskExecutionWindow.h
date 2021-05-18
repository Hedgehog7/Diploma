
#ifndef ATGUI_ATaskExecutionWindow_h
#define ATGUI_ATaskExecutionWindow_h

#include <QtWidgets/QMainWindow>
#include "config.h"

class APlanTask;
class QWidget;
class QPushButton;

class ATaskExecutionPanel : public QWidget
{
	using _Base = QWidget;
public:
	ATaskExecutionPanel(QWidget * parent = nullptr);
	QPushButton *bnDiscard, *bnFinish;
};

class AT_GUI_API ATaskExecutionWindow : public QMainWindow
{
	Q_OBJECT

	using _Base = QMainWindow;
	enum class state_t {execution, finished, discarded};
public:
	ATaskExecutionWindow(APlanTask * _task, QWidget * _parent = nullptr);
	~ATaskExecutionWindow();

	APlanTask * task;
protected:
	state_t state;

	virtual void closeEvent(QCloseEvent * closeEvent) override;
signals:
	void discardTask();
	void commitTask();
};

#endif
