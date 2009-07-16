#ifndef QABSTRACTTREEITEM_H_
#define QABSTRACTTREEITEM_H_

#include <QObject>

class QAbstractTreeItem : public QObject
{
 public:
  QAbstractTreeItem(QObject *data=0,int row=0,QAbstractTreeItem *parentItem=0);

  QObject* data();
  void setData(QObject *data);

  int row();
  void setRow(int row);

  QAbstractTreeItem* parentItem();
  void setParentItem(QAbstractTreeItem *parent);

  void addChildItem(QAbstractTreeItem *item);
  QAbstractTreeItem* getChildItem(int idx);
  void removeChildItem(int idx);
  int numChildItems();

 private:
  QObject *_data; //Data being held
  int _row; //The row of the parent

  QAbstractTreeItem *_parentItem; //The parent of this item
  QList<QAbstractTreeItem*> childItems;

  Q_OBJECT
};

#endif // QABSTRACTTREEITEM_H_
