#include "QTableViewWithContextMenu.h"
#include <QDebug>
#include <QMenu>
#include <QVariant>

QTableViewWithContextMenu::QTableViewWithContextMenu(QWidget *parent):QTableView(parent)
{
}

void QTableViewWithContextMenu::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex itemAt=this->indexAt(event->pos());
    QAbstractItemModel *model=this->model();
    QVariant menuVariant=model->data(itemAt,Qt::UserRole);

    if(menuVariant.isValid())
    {
        QObject *menuObj=menuVariant.value<QObject*>();
        if(!menuObj->inherits("QMenu")) return;

        QMenu *menu=(QMenu*)menuObj;
        menu->exec(event->globalPos());
    }
}
