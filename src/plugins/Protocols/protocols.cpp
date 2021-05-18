#include "protocols.h"
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

APlugin * AT_CREATE_PLUGIN_FN()
{
	return new Protocols();
}

//Для корректного отображения русского текста в Qt
QString Protocols::ru(const char * text)
{
	return QTextCodec::codecForName("CP1251")->toUnicode(text);
}

Protocols::Protocols()
	:AUtilityPlugin("Protocols", "Analyze of protocols")
{
}

Protocols::~Protocols(){}

const std::vector<std::string>Protocols::getCommands()
{
	std::vector<std::string> res;
	res.push_back("acquire_expert");
	res.push_back("acquire_database");
	return std::move(res);
}

AError Protocols::executeCommand(const std::string & script, std::string & answer)
{
	return AError();
}

int Protocols::executeTask(ATaskExecutionWindow * wnd, const std::string & cmd_id, const std::vector<ADocumentRef*> & input_docs, const std::vector<ADocumentRef*> & out_docs)
{
	auto lb = new QLabel("Protocols start", wnd);

	auto window = new QWidget(wnd);
	wnd->setCentralWidget(window);
	wnd->setContentsMargins(0, 0, 500, 600);

	//Отображение окна
	wnd->setWindowTitle(ru("Протокол интервьюирования эксперта"));

	auto layout = new QVBoxLayout;
	//layout->setGeometry(QRect(10, 100, 300, 30));

	/*
	QComboBox* cbox = new QComboBox(window);
	QStringList protocolNames;
	for (int i = 0; i < 2; i++)
	{
		protocolNames << ru("Протокол №") + QString::number(i + 1);
	}
	protocolNames << ru("Объединенный протокол");
	cbox->addItems(protocolNames);
	*/

	//auto chooseProtocol = new QLabel(ru("Выберите протокол:"), window);
	auto chooseAction = new QLabel(ru("Выберите способ представления протокола интервьюирования эксперта:"), window);
	auto buttonHtmlRepresentation = new QPushButton(ru("Расширенный HTML формат гипертекста"), window);
	auto buttonTreeRepresentation = new QPushButton(ru("Графическое представление №1"), window);
	auto buttonEdit = new QPushButton(ru("Открыть протокол на редактирование"), window);
	auto buttonWindows = new QPushButton(ru("Графическое представление №2"), window);
	auto buttonColumns = new QPushButton(ru("Графическое представление №3"), window);

	//layout->addWidget(chooseProtocol);
	//layout->addWidget(cbox);
	
	layout->addWidget(chooseAction);
	layout->addWidget(buttonHtmlRepresentation);
	layout->addWidget(buttonTreeRepresentation);
	layout->addWidget(buttonWindows);
	layout->addWidget(buttonColumns);
	layout->addWidget(buttonEdit);

	window->setStyleSheet("QPushButton { max-width: 20em } ");
	window->setStyleSheet("QComboBox { max-width: 20em } ");
	//window->setStyleSheet("QLabel { font: bold 14px } ");

	QFont font;
	font.setBold(true);
	//chooseProtocol->setFont(font);
	chooseAction->setFont(font);

	window->setLayout(layout);

	QFile file("default.txt");
	file.open(QIODevice::ReadOnly);
	TreeModel* model = new TreeModel(ru(file.readAll()));
	file.close();
	
	//Обработчики нажатия кнопок

	QObject::connect(buttonEdit, &QPushButton::released, [=] {
		MainWindow* myWindow = new MainWindow;
		myWindow->setGeometry(500, 200, 775, 750);
		myWindow->setFixedSize(775, 750);
		myWindow->setWindowTitle(ru("Протокол интервьюирования эксперта"));
		myWindow->show();
	});

	QObject::connect(buttonTreeRepresentation, &QPushButton::released, [=] {
		auto myWindow = new QWidget;
		myWindow->setGeometry(500, 200, 775, 775);
		//myWindow->setFixedSize(775, 1000);
		
		QTreeView *view = new QTreeView(myWindow);
		view->setModel(model);
		view->setFixedSize(775, 1000);

		myWindow->setWindowTitle(ru("Протокол интервьюирования эксперта"));
		myWindow->show();
	});

	QObject::connect(buttonHtmlRepresentation, &QPushButton::released, [=] {
		auto myWindow = new QWidget;
		myWindow->setGeometry(400, 400, 400, 300);
		myWindow->setFixedSize(400, 300);
		model->createHT();
		myWindow->setWindowTitle(ru("Информация"));

		QLabel *label = new QLabel(myWindow);
		label->setText(ru("Файл 'Protocol.html' создан!"));
		label->setStyleSheet("font: bold 18px; margin: auto; ");
		myWindow->show();
	});

	QObject::connect(buttonWindows, &QPushButton::released, [=] {
		auto myWindow = new QWidget;
		myWindow->setGeometry(100, 100, 500, 400);
	
		model->createWindows(&myWindow);
		myWindow->setWindowTitle(ru("Протокол интервьюирования эксперта"));
		myWindow->show();
	});

	QObject::connect(buttonColumns, &QPushButton::released, [=] {
		auto cview = new QColumnView;
		cview->setGeometry(100, 250, 775, 500);
		model->createColumns(&cview);
		
		cview->show();
		cview->setWindowTitle(ru("Протокол интервьюирования эксперта"));
	});


	for (auto doc : out_docs)
	{
		auto path = doc->absolutePath();
		QFile file(path);
		bool r = file.open(QIODevice::WriteOnly);
		perform_check(r);
	}

	return 0;
}
