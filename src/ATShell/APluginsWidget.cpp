#include "APluginsWidget.h"
#include <ATGUI/APluginManager.h>
#include <QtWidgets/QTreeWidgetItem>

APluginsWidget::APluginsWidget(APluginManager * plug_mgr, QWidget *parent)
	: QWidget(parent), m_pPlugMgr(plug_mgr)
{
	ui.setupUi(this);

	updateData();
}

APluginsWidget::~APluginsWidget()
{

}

void APluginsWidget::updateData()
{
	ui.treePlugins->clear();

    const int rucs_number = 9;
    static const char * TypeNames[] = {"ATTextEditor",
                                       "at_verifier",
                                      "data_mining",
                                      "DSFEditor",
                                      "EDFDEditor",
                                      "KBEditor",
                                      "KMPZ",
                                      "merger",
                                      "Protocols"};
    static const char * DescriptionNames[] = {"Execute ATTextEditor",
                                              "Execute ATVerifier",
                                             "Execute DataMining",
                                             "Execute DSFEditor",
                                             "Execute EDFDEditor",
                                             "Execute KBEditor",
                                             "Execute KMPZ",
                                             "Execute Merger",
                                             "Execute Protocols"};
    static const char * PathNames[] = {"~/Documents/AT/build/build-name/ATWorkbench/plugins/libATTextEditor.so",
                                       "~/Documents/AT/build/build-name/ATWorkbench/plugins/libATVerifier.so",
                                       "~/Documents/AT/build/build-name/ATWorkbench/plugins/libDataMining.so",
                                      "~/Documents/AT/build/build-name/ATWorkbench/plugins/libDSFEditor.so",
                                      "~/Documents/AT/build/build-name/ATWorkbench/plugins/libEDFDEditor.so",
                                      "~/Documents/AT/build/build-name/ATWorkbench/plugins/libKBEditor.so",
                                      "~/Documents/AT/build/build-name/ATWorkbench/plugins/libKMPZ.so",
                                      "~/Documents/AT/build/build-name/ATWorkbench/plugins/libMerger.so",
                                      "~/Documents/AT/build/build-name/ATWorkbench/plugins/libProtocols.so"};

    QList<QTreeWidgetItem *> group_items;
    //QList<QString> item_list[sizeof(&TypeNames)];
    QList<QString> item_list[rucs_number];
    for (int i = 0; i < rucs_number; i++){
        item_list[i].append(QString::fromUtf8(TypeNames[i]));
        item_list[i].append(QString::fromUtf8(DescriptionNames[i]));
        item_list[i].append(QString::fromUtf8(PathNames[i]));
        group_items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(item_list[i])));
    }

//	for (int i = 0; i < static_cast<int>(APlugin::Type::Count); ++i)
//	{
//		QString type_name = QString::fromUtf8(TypeNames[i]);
//		auto group_item = new QTreeWidgetItem((QTreeWidget*)0, QStringList(type_name));
		
//		for(auto & plugin : m_pPlugMgr->plugins(static_cast<APlugin::Type>(i)))
//		{
//			APlugin * plug = plugin->plugin();

//			QStringList cols;
//			cols.append(QString::fromStdString(plug->name()));
//			cols.append(QString::fromStdString(plug->description()));
//			cols.append(QString::fromWCharArray(plugin->path()));

//			auto plugin_item = new QTreeWidgetItem((QTreeWidget*)0, cols);

//			group_item->addChild(plugin_item);
//		}

//		group_items.append(group_item);
//	}
	
	ui.treePlugins->insertTopLevelItems(0, group_items);

	for(auto gi : group_items)
        gi->setExpanded(true);
}

void APluginsWidget::on_treePlugins_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{

}
