/********************************************************************************
** Form generated from reading UI file 'APlannerWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APLANNERWIDGET_H
#define UI_APLANNERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_APlannerWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeCurrentPlan;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;
    QPushButton *bnGenerateGeneralPlan;
    QPushButton *bnBuildStartModel;
    QPushButton *bnBuildDetailedPlan;

    void setupUi(QWidget *APlannerWidget)
    {
        if (APlannerWidget->objectName().isEmpty())
            APlannerWidget->setObjectName(QStringLiteral("APlannerWidget"));
        APlannerWidget->resize(400, 351);
        verticalLayout = new QVBoxLayout(APlannerWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        treeCurrentPlan = new QTreeWidget(APlannerWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("Task Name"));
        treeCurrentPlan->setHeaderItem(__qtreewidgetitem);
        treeCurrentPlan->setObjectName(QStringLiteral("treeCurrentPlan"));
        treeCurrentPlan->header()->setDefaultSectionSize(140);

        verticalLayout->addWidget(treeCurrentPlan);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_2 = new QPushButton(APlannerWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        bnGenerateGeneralPlan = new QPushButton(APlannerWidget);
        bnGenerateGeneralPlan->setObjectName(QStringLiteral("bnGenerateGeneralPlan"));

        horizontalLayout->addWidget(bnGenerateGeneralPlan);

        bnBuildStartModel = new QPushButton(APlannerWidget);
        bnBuildStartModel->setObjectName(QStringLiteral("bnBuildStartModel"));

        horizontalLayout->addWidget(bnBuildStartModel);

        bnBuildDetailedPlan = new QPushButton(APlannerWidget);
        bnBuildDetailedPlan->setObjectName(QStringLiteral("bnBuildDetailedPlan"));

        horizontalLayout->addWidget(bnBuildDetailedPlan);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(APlannerWidget);

        QMetaObject::connectSlotsByName(APlannerWidget);
    } // setupUi

    void retranslateUi(QWidget *APlannerWidget)
    {
        APlannerWidget->setWindowTitle(QApplication::translate("APlannerWidget", "APlannerWidget", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeCurrentPlan->headerItem();
        ___qtreewidgetitem->setText(2, QApplication::translate("APlannerWidget", "Execute", nullptr));
        ___qtreewidgetitem->setText(1, QApplication::translate("APlannerWidget", "Done", nullptr));
        pushButton_2->setText(QApplication::translate("APlannerWidget", "Options...", nullptr));
        bnGenerateGeneralPlan->setText(QApplication::translate("APlannerWidget", "Build Common Plan", nullptr));
        bnBuildStartModel->setText(QApplication::translate("APlannerWidget", "Build Start Model", nullptr));
        bnBuildDetailedPlan->setText(QApplication::translate("APlannerWidget", "Build Detailed Plan", nullptr));
    } // retranslateUi

};

namespace Ui {
    class APlannerWidget: public Ui_APlannerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APLANNERWIDGET_H
