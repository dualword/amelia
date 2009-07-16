#include "QAbstractTreeItem.h"

QAbstractTreeItem::QAbstractTreeItem(QObject *data,int row,QAbstractTreeItem *parentItem)
  : QObject(parentItem),_data(data),_row(row),_parentItem(parentItem)
{ }

QObject* QAbstractTreeItem::data()
{
  return _data;
}

void QAbstractTreeItem::setData(QObject *data)
{
  _data=data;
}

int QAbstractTreeItem::row()
{
  return _row;
}

void QAbstractTreeItem::setRow(int row)
{
  _row=row;
}

QAbstractTreeItem* QAbstractTreeItem::parentItem()
{
  return _parentItem;
}

void QAbstractTreeItem::setParentItem(QAbstractTreeItem *parentItem)
{
  setParent(parentItem);
  _parentItem=parentItem;
}

void QAbstractTreeItem::addChildItem(QAbstractTreeItem *childItem)
{
  childItems.push_back(childItem);
}

void QAbstractTreeItem::removeChildItem(int idx)
{
  childItems.removeAt(idx);
}

QAbstractTreeItem* QAbstractTreeItem::getChildItem(int idx)
{
  return childItems[idx];
}

int QAbstractTreeItem::numChildItems()
{
  return childItems.size();
}
