#include "QAbstractTableModelWithContextMenu.h"

int QAbstractTableModelWithContextMenu::MenuDataRole=31;

QAbstractTableModelWithContextMenu::QAbstractTableModelWithContextMenu(QObject *parent)
 : QAbstractTableModel(parent)
{
    _menu=new QMenu();
}

void QAbstractTableModelWithContextMenu::setMenu(QMenu *menu)
{
    if(_menu!=0) _menu->deleteLater();
    _menu=menu;
}

QMenu *QAbstractTableModelWithContextMenu::menu()
{
    return _menu;
}

QVariant QAbstractTableModelWithContextMenu::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();
  
  if(role == Qt::UserRole)
    {
      QList<QAction *> actions=_menu->findChildren<QAction*>();
      if(actions.size()>0)
	{
	  for(int i=0;i<actions.size();i++)
	    {
	      QVariant data=this->data(index,QAbstractTableModelWithContextMenu::MenuDataRole);
	      actions[i]->setData(data);
	    }
	  
	  return QVariant::fromValue<QObject *>(_menu);
	}
    }
  else if(role==QAbstractTableModelWithContextMenu::MenuDataRole)
    {
      return QVariant();
    }
  
  return QVariant();
}
