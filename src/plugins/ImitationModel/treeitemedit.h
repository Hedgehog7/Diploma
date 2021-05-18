
#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

class TreeItemEdit
{
public:
    explicit TreeItemEdit(const QVector<QVariant> &data, TreeItemEdit *parent = 0);
    ~TreeItemEdit();

	TreeItemEdit *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
	TreeItemEdit *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

private:
    QList<TreeItemEdit*> childItems;
    QVector<QVariant> itemData;
	TreeItemEdit *parentItem;
};

#endif // TREEITEM_H