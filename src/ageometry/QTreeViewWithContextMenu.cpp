#include "QTreeViewWithContextMenu.h"
#include <QDebug>
#include <QMenu>
#include <QVariant>

QTreeViewWithContextMenu::QTreeViewWithContextMenu(QWidget *parent)
 : QTreeView(parent)
{ }

void QTreeViewWithContextMenu::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex itemAt=this->indexAt(event->pos());
    QAbstractItemModel *model=this->model();
    QVariant menuVariant=model->data(itemAt,Qt::UserRole);
    
    if(menuVariant.isValid())
    {
        QObject *menuObj=menuVariant.value<QObject*>();
	qDebug() << menuObj;
        if(!menuObj->inherits("QMenu")) return;

        QMenu *menu=(QMenu*)menuObj;
        menu->exec(event->globalPos());
    }
}
