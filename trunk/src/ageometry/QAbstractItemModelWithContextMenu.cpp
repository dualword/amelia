#include "QAbstractItemModelWithContextMenu.h"
#include <QDebug>

int QAbstractItemModelWithContextMenu::MenuDataRole=31;

QAbstractItemModelWithContextMenu::QAbstractItemModelWithContextMenu(QObject *parent)
  : QAbstractItemModel(parent),_menu(0)
{ }

void QAbstractItemModelWithContextMenu::setMenu(QMenu *menu)
{
  if(_menu!=0)
      _menu->deleteLater();
  
  _menu=menu;
}

QMenu *QAbstractItemModelWithContextMenu::menu()
{
  return _menu;
}

QVariant QAbstractItemModelWithContextMenu::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || _menu==0)
    return QVariant();
  
  if(role == Qt::UserRole)
    {
      QList<QAction *> actions=_menu->findChildren<QAction*>();
      if(actions.size()>0)
	{
	  for(int i=0;i<actions.size();i++)
	    {
	      QVariant data=this->data(index,QAbstractItemModelWithContextMenu::MenuDataRole);
	      actions[i]->setData(data);
	    }
	  return QVariant::fromValue<QObject *>(_menu);
	}
    }
  else if(role==QAbstractItemModelWithContextMenu::MenuDataRole)
    {
      return QVariant();
    }
  
  return QVariant();
}
