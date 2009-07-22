#ifndef QTREEVIEWWITHCONTEXTMENU_H
#define QTREEVIEWWITHCONTEXTMENU_H

#include <QTreeView>
#include <QContextMenuEvent>

class QTreeViewWithContextMenu : public QTreeView
{
public:
    QTreeViewWithContextMenu(QWidget *parent=0);

protected:
    void contextMenuEvent(QContextMenuEvent *);
};

#endif // QTREEVIEWWITHCONTEXTMENU_H
