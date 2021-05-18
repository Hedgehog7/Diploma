
#ifndef ATEnvironment_APluginsWidget_h
#define ATEnvironment_APluginsWidget_h

#include <QWidget>
#include "ui_APluginsWidget.h"

class APluginManager;

class APluginsWidget : public QWidget
{
	Q_OBJECT

public:
	APluginsWidget(APluginManager * plug_mgr, QWidget *parent = 0);
	~APluginsWidget();
	void updateData();
private slots:
    void on_treePlugins_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
	Ui::APluginsWidget ui;
	APluginManager * m_pPlugMgr;
};

#endif // APLANNERWIDGET_H
