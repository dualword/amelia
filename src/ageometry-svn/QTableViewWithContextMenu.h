#ifndef QTABLEVIEWWITHCONTEXTMENU_H
#define QTABLEVIEWWITHCONTEXTMENU_H

#include <QTableView>
#include <QContextMenuEvent>

class QTableViewWithContextMenu : public QTableView
{
public:
    QTableViewWithContextMenu(QWidget *parent=0);

protected:
    void contextMenuEvent(QContextMenuEvent *);
};

#endif // QTABLEVIEWWITHCONTEXTMENU_H
