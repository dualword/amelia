#include "QAbstractTreeItem.h"

QAbstractTreeItem::QAbstractTreeItem(QObject *data,QAbstractTreeItem *parentItem, int pRow)
  : QObject(0),_data(data),_parentItem(parentItem),_parentRow(pRow)
{ }

QObject* QAbstractTreeItem::data()
{
  return _data;
}

void QAbstractTreeItem::setData(QObject *data)
{
  _data=data;
}

QAbstractTreeItem* QAbstractTreeItem::parentItem()
{
  return _parentItem;
}

void QAbstractTreeItem::setParentItem(QAbstractTreeItem *parentItem)
{
  _parentItem=parentItem;
}

int QAbstractTreeItem::parentRow()
{
  return _parentRow;
}

void QAbstractTreeItem::setParentRow(int parentRow)
{
  _parentRow=parentRow;
}

