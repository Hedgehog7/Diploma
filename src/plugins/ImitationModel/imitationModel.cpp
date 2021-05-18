#include "imitationModel.h"
#include <ATGUI/ATaskExecutionWindow.h>
#include <ATProject/ADocumentRef.h>
#include <ATCore/check.h>
#include <QLabel>
#include <QFile>

#include <QString>
#include <QTextCodec>
#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QRect>

#include "mainwindow.h"
#include "treemodel.h"
#include <QApplication>
#include <QTreeView>
#include <QColumnView>

#include <QProcess>
#include <QDir>

APlugin * AT_CREATE_PLUGIN_FN()
{
	return new ImitationModel();
}

//Для корректного отображения русского текста в Qt
QString ImitationModel::ru(const char * text)
{
	return QTextCodec::codecForName("CP1251")->toUnicode(text);
}

ImitationModel::ImitationModel()
	:AUtilityPlugin("ImitationModel", "Analyze of ImitationModel")
{
}

ImitationModel::~ImitationModel() {}


const std::vector<std::string>ImitationModel::getCommands()
{
	std::vector<std::string> res;
	res.push_back("acquire_expert");
	res.push_back("acquire_database");
	return std::move(res);
}

AError ImitationModel::executeCommand(const std::string & script, std::string & answer)
{
	return AError();
}


int ImitationModel::executeTask(ATaskExecutionWindow * wnd, const std::string & cmd_id, const std::vector<ADocumentRef*> & input_docs, const std::vector<ADocumentRef*> & out_docs)
{
	auto lb = new QLabel("ImitationModel start", wnd);

	auto window = new QWidget(wnd);
	wnd->setCentralWidget(window);
	wnd->setContentsMargins(0, 0, 500, 600);

	wnd->setWindowTitle(ru("Подсистема имитационного моделирования"));

	auto layout = new QVBoxLayout;

	auto buttonComputationModel = new QPushButton(ru("Компонент поддержки расчета состояний имитационной модели"), window);
	auto buttonATTranslator = new QPushButton(ru("Транслятор языка РДОАТ"), window);
	auto buttonVisualEditor = new QPushButton(ru("Vusual Editor"), window);
	auto buttonVisualization = new QPushButton(ru("Редактор моделей"), window);
	auto buttonVisualizer = new QPushButton(ru("Компонент визуализации"), window);
	auto buttonLabs = new QPushButton(ru("Лабораторные работы"), window);
	auto buttonEditor = new QPushButton(ru("Редактор визуальных объектов"), window);

	layout->addWidget(buttonLabs);
	layout->addWidget(buttonVisualization);
	layout->addWidget(buttonVisualizer);
	layout->addWidget(buttonComputationModel);
	layout->addWidget(buttonATTranslator);
	layout->addWidget(buttonVisualEditor);
	layout->addWidget(buttonEditor);

	window->setStyleSheet("QPushButton { width: 30em; height: 10em } ");

	QFont font;
	font.setBold(true);

	window->setLayout(layout);

	QObject::connect(buttonEditor, &QPushButton::released, [=] {
		QString file = "C:/Projects/at_environment/src/plugins/ImitationModel/IM/Editor/Editor.exe";
		QProcess::execute(file);
	});
	QObject::connect(buttonLabs, &QPushButton::released, [=] {
		QString file = "C:/Projects/at_environment/src/plugins/ImitationModel/IM/Labs/LabProgram.exe";
		QProcess::execute(file);
	});
	QObject::connect(buttonVisualizer, &QPushButton::released, [=] {
		QString file = "C:/Projects/at_environment/src/plugins/ImitationModel/IM/Vizualizer/Vizualizer.exe";
		QProcess::execute(file);
	});
	QObject::connect(buttonVisualization, &QPushButton::released, [=] {
		QString file = "C:/Projects/at_environment/src/plugins/ImitationModel/IM/Visualization/Visualization.exe";
		QProcess::execute(file);
	});
	QObject::connect(buttonATTranslator, &QPushButton::released, [=] {
		QString file = "C:/Projects/at_environment/src/plugins/ImitationModel/IM/Translation/Translation.exe";
		QProcess::execute(file);
	});
	QObject::connect(buttonComputationModel, &QPushButton::released, [=] {
		QString file = "C:/Projects/at_environment/src/plugins/ImitationModel/IM/Computation/ComputationModel.exe";
		QProcess::execute(file);
	});
	QObject::connect(buttonVisualEditor, &QPushButton::released, [=] {
		QString file = "C:/Projects/at_environment/src/plugins/ImitationModel/IM/VisualEditor/VisualEditor.exe";
		QProcess::execute(file);
	});
	return 0;

}
