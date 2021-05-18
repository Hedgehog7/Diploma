#include "treeitem.h"
#include "treemodel.h"

#include <QStringList>
#include <QTextCodec>
#include <QFile>
#include <QTextBlockFormat>
#include <QTextCursor>

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStandardItemModel>
#include <QColumnView>

QString TreeModel::ru(const char * text)
{
	return QTextCodec::codecForName("CP1251")->toUnicode(text);
}

TreeModel::TreeModel(const QString &data, QObject *parent)
	: QAbstractItemModel(parent)
{
	QList<QVariant> rootData;
	rootData << ru("Протокол интервью"); //<< "Summary";
	rootItem = new TreeItem(rootData);
	setupModelData(data.split(QString("\n")), rootItem);
}

TreeModel::~TreeModel()
{
	delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
	int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem *parentItem = childItem->parentItem();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
	QList<TreeItem*> parents;
	QList<int> indentations;
	parents << parent;
	indentations << 0;

	int number = 0;

	while (number < lines.count()) {
		int position = 0;
		while (position < lines[number].length()) {
			if (lines[number].at(position) != ' ')
				break;
			position++;
		}

		QString lineData = lines[number].mid(position).trimmed();

		if (!lineData.isEmpty()) {
			// Read the column data from the rest of the line.
			QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
			QList<QVariant> columnData;
			for (int column = 0; column < columnStrings.count(); ++column)
				columnData << columnStrings[column];

			if (position > indentations.last()) {
				// The last child of the current parent is now the new parent
				// unless the current parent has no children.

				if (parents.last()->childCount() > 0) {
					parents << parents.last()->child(parents.last()->childCount() - 1);
					indentations << position;
				}
			}
			else {
				while (position < indentations.last() && parents.count() > 0) {
					parents.pop_back();
					indentations.pop_back();
				}
			}

			// Append a new item to the current parent's list of children.
			parents.last()->appendChild(new TreeItem(columnData, parents.last()));
		}

		++number;
	}
}

void TreeModel::showTree(TreeItem *currentItem, QString *str)
{
	*str += currentItem->data(0).toString() + "\n";
	if (!currentItem->childCount())
	{
		*str += "\n";
		return;
	}	

	for (int i = 0; i < currentItem->childCount(); i++)
	{
		*str += currentItem->child(i)->data(0).toString() + "\n";
	}
	*str += "\n";
	for (int i = 0; i < currentItem->childCount(); i++)
	{
		showTree(currentItem->child(i)->child(0), str);
	}
}

void TreeModel::createHT() 
{
	QTextDocument* text = new QTextDocument();
	
	QFont font;
	font.setPointSize(16);
	font.setFamily("Courier New");
	text->setDefaultFont(font);

	QTextCursor* cursor = new QTextCursor(text);
	QTextBlockFormat format;
	cursor->setBlockFormat(format);
	cursor->insertHtml("<h1>" + ru("Протокол интервьюирования эксперта") + "</h1>" + "<hr style='width:10'/>");
	
	QString str;
	//showTree(this->rootItem->child(0), &str);
	
	createHTML(this->rootItem->child(0), &cursor, format);

	QFile file;
	file.setFileName("Protocol.html");

	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		//file.write(str.toUtf8());
		file.write(text->toHtml("utf-8").toUtf8());
		file.close();
	}
}

void TreeModel::createHTML(TreeItem *currentItem, QTextCursor** cursor, QTextBlockFormat format)
{
	QString str;
		
	str += currentItem->data(0).toString() + "\n";
	if (!currentItem->childCount())
	{
		str += "\n";
		(*cursor)->insertBlock(format);
		QString name;
		getNumber(currentItem, &name);
		(*cursor)->insertHtml("<a name = \"" + name + "\">" +
			"<pre>" + str + "</pre></a><hr style = 'width:10' />");
		return;
	}

	for (int i = 0; i < currentItem->childCount(); i++)
	{
		QString href;
		getNumber(currentItem->child(i)->child(0), &href);
		str += "    <i><a href=\"#" + href + "\">" + 
			currentItem->child(i)->data(0).toString() + "</a>\n";
	}
	str += "</i>\n";

	(*cursor)->insertBlock(format);
	//(*cursor)->insertHtml("<pre>" + str + "</pre><hr style='width:10'/>");
	QString name;
	getNumber(currentItem, &name);
	(*cursor)->insertHtml("<a name = \"" + name + "\">" +
		"<pre>" + str + "</pre></a><hr style = 'width:10' />");
	
	for (int i = 0; i < currentItem->childCount(); i++)
	{
		createHTML(currentItem->child(i)->child(0), cursor, format);
	}
}

void TreeModel::getNumber(TreeItem *currentItem, QString *number)
{
	if (currentItem == this->rootItem) return;
	*number += QString::number(currentItem->row());
	getNumber(currentItem->parentItem(), number);
}

void TreeModel::createWindows(QWidget** start) {
	auto buttonNode = new QPushButton(ru("Начало"), *start);

	buttonNode->setStyleSheet("background-color: lightgrey;"
		"border-style: outset;"
		"border-width: 2px;"
		"border-radius: 10px;"
		"border-color: beige;"
		"font: bold 30px;"
		"min-width: 10em;"
		"padding: 6px;");

	auto layout = new QVBoxLayout;
	layout->addWidget(buttonNode);
	(*start)->setLayout(layout);

	createNewWindow(*start, this->rootItem->child(0), buttonNode);
}

void TreeModel::createNewWindow(QWidget* prevWindow, TreeItem *currentItem, QPushButton *currentButton) {
	auto newWindow = new QWidget;
	newWindow->setWindowTitle(currentButton->text());

	newWindow->setGeometry(100, 100, 500, 400);

	auto layout = new QVBoxLayout;
	layout->addSpacing(20);

	auto question = new QLabel;
	question->setText(currentItem->data(0).toString());
	question->setStyleSheet("font: bold 18px;");
	layout->addWidget(question);

	for (int i = 0; i < currentItem->childCount(); i++)
	{
		auto buttonNode = new QPushButton(currentItem->child(i)->data(0).toString());
		layout->addWidget(buttonNode);

		buttonNode->setStyleSheet("background-color: mintcream;"
			"border-style: outset;"
			"border-width: 2px;"
			"border-radius: 10px;"
			"border-color: lightgrey;"
			"font: bold 18px;"
			"padding: 6px;");
		
		createNewWindow(newWindow, currentItem->child(i)->child(0), buttonNode);
	}

	auto buttonBack = new QPushButton(ru("Назад"));

	buttonBack->setStyleSheet("background-color: lightgrey;"
		"border-style: outset;"
		"border-width: 2px;"
		"border-radius: 10px;"
		"border-color: beige;"
		"font: bold 18px;"
		"max-width: 10em;"
		"padding: 6px;");

	layout->addWidget(buttonBack);
	layout->addStretch(1);
	newWindow->setLayout(layout);

	newWindow->setVisible(false);
	QObject::connect(currentButton, &QPushButton::released, [=] {
		prevWindow->setVisible(false);
		newWindow->setVisible(true);
	});

	QObject::connect(buttonBack, &QPushButton::released, [=] {
		newWindow->setVisible(false);
		prevWindow->setVisible(true);
	});
}

void TreeModel::createColumns(QColumnView** cview) {
	_model.reset(new QStandardItemModel());

	auto start = new QStandardItem(ru("Начало"));
	start->setEditable(false);
	start->setFont(QFont("Times New Roman", 12, QFont::Bold));

	_model->appendRow(start);

	createNewColumn(start, this->rootItem->child(0), cview);
	(*cview)->setModel(_model.get());
}

void TreeModel::createNewColumn(QStandardItem *currentItem, TreeItem *currentQuestion, QColumnView** cview) {
	/*
	if (!currentQuestion->childCount()) {
		connect(*cview, &QColumnView::updatePreviewWidget, [=](const QModelIndex & idx) {
			auto newWidget = new QWidget;
			auto layout = new QVBoxLayout;

			auto label = new QLabel(currentQuestion->data(0).toString());
			layout->addWidget(label);
			layout->addStretch(1);
			newWidget->setLayout(layout);

			(*cview)->setPreviewWidget(newWidget);
		});
		return;
	}*/

	auto question = new QStandardItem(currentQuestion->data(0).toString());
	question->setSelectable(false);
	question->setEditable(false);
	question->setFont(QFont("Times New Roman", 12, QFont::Bold));

	currentItem->appendRow(question);
	
	for (int i = 0; i < currentQuestion->childCount(); i++)
	{
		auto answer = new QStandardItem(currentQuestion->child(i)->data(0).toString());
		answer->setEditable(false);
		answer->setFont(QFont("Times New Roman", 11));

		currentItem->appendRow(answer);
		createNewColumn(answer, currentQuestion->child(i)->child(0), cview);
	}

}