#ifndef QABSTRACTTREEITEM_H_
#define QABSTRACTTREEITEM_H_

#include <QObject>

class QAbstractTreeItem : public QObject
{
 public:
  QAbstractTreeItem(QObject *data=0,QAbstractTreeItem *parentItem=0, int pRow=0);

  QObject* data();
  void setData(QObject *data);

  QAbstractTreeItem* parentItem();
  void setParentItem(QAbstractTreeItem *parent);

  int parentRow();
  void setParentRow(int pRow);


 private:
  QObject *_data; //Data being held
  QAbstractTreeItem *_parentItem; //The parent of this item
  int _parentRow; //The row of the parent

  Q_OBJECT
};

#endif // QABSTRACTTREEITEM_H_
