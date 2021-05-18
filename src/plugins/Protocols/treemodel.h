#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QTextDocument>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QPushButton>
#include <QColumnView>
#include <QStandardItem>
#include <memory>
class QStandardItemModel;

class TreeItem;

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit TreeModel(const QString &data, QObject *parent = 0);
	QString ru(const char * text);
	~TreeModel();

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column,
		const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	void createHT();
	void createHTML(TreeItem * currentItem, QTextCursor ** cursor, QTextBlockFormat format);
	void getNumber(TreeItem * currentItem, QString * number);

	void createWindows(QWidget** start);
	void createNewWindow(QWidget* prevWindow, TreeItem * currentItem, QPushButton * currentButton);
	
	void createColumns(QColumnView ** cview);

	void createNewColumn(QStandardItem * currentItem, TreeItem * currentQuestion, QColumnView ** cview);
	
			
private:
	void setupModelData(const QStringList &lines, TreeItem *parent);
	void showTree(TreeItem * currentItem, QString * str);

	TreeItem *rootItem;

	std::unique_ptr<QStandardItemModel> _model;
};

#endif // TREEMODEL_H

