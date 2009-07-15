#include "QAbstractTreeItem.h"

QAbstractTreeItem::QAbstractTreeItem(QObject *data,int row,QAbstractTreeItem *parentItem)
  : QObject(0),_data(data),_parentItem(parentItem),_row(row)
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

int QAbstractTreeItem::row()
{
  return _row;
}

void QAbstractTreeItem::setRow(int row)
{
  _row=row;
}

