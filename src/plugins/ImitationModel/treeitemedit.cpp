#include "treeitemedit.h"

#include <QStringList>

TreeItemEdit::TreeItemEdit(const QVector<QVariant> &data, TreeItemEdit *parent)
{
    parentItem = parent;
    itemData = data;
}

TreeItemEdit::~TreeItemEdit()
{
    qDeleteAll(childItems);
}

TreeItemEdit *TreeItemEdit::child(int number)
{
    return childItems.value(number);
}

int TreeItemEdit::childCount() const
{
    return childItems.count();
}

int TreeItemEdit::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItemEdit*>(this));

    return 0;
}

int TreeItemEdit::columnCount() const
{
    return itemData.count();
}

QVariant TreeItemEdit::data(int column) const
{
    return itemData.value(column);
}

bool TreeItemEdit::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
		TreeItemEdit *item = new TreeItemEdit(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool TreeItemEdit::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (TreeItemEdit *child, childItems)
        child->insertColumns(position, columns);

    return true;
}

TreeItemEdit *TreeItemEdit::parent()
{
    return parentItem;
}

bool TreeItemEdit::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

bool TreeItemEdit::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (TreeItemEdit *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

bool TreeItemEdit::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}