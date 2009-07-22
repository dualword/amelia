#ifndef QABSTRACTTABLEMODELWITHCONTEXTMENU_H
#define QABSTRACTTABLEMODELWITHCONTEXTMENU_H

#include <QAbstractTableModel>
#include <QMenu>

class QAbstractTableModelWithContextMenu : public QAbstractTableModel
{
public:
  static int MenuDataRole;
  
  QAbstractTableModelWithContextMenu(QObject *parent=0);
  void setMenu(QMenu *);
  QMenu *menu();
  
  QVariant data(const QModelIndex&, int role=Qt::DisplayRole ) const;
  
private:
  QMenu *_menu;
  
  Q_OBJECT
};

#endif // QABSTRACTTABLEMODELWITHCONTEXTMENU_H
