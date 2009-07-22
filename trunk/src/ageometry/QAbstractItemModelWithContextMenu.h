#ifndef QABSTRACTITEMMODELWITHCONTEXTMENU_H
#define QABSTRACTITEMMODELWITHCONTEXTMENU_H

#include <QAbstractItemModel>
#include <QMenu>

class QAbstractItemModelWithContextMenu : public QAbstractItemModel
{
public:
  static int MenuDataRole;
  
  QAbstractItemModelWithContextMenu(QObject *parent=0);
  void setMenu(QMenu *);
  QMenu *menu();
  
  QVariant data(const QModelIndex&, int role=Qt::DisplayRole ) const;
  
private:
  QMenu *_menu;
  
  Q_OBJECT
};

#endif // QABSTRACTITEMMODELWITHCONTEXTMENU_H
