#ifndef AEVENTMANAGERTREEVIEW_H_
#define AEVENTMANAGERTREEVIEW_H_

#include <QTreeView>
#include <QContextMenuEvent>

#include "AEventManagerScene.h"
#include "AEventManagerDefines.h"

class AEVENTMANAGER_EXPORT AEventManagerTreeView : public QTreeView
{
 public:
  AEventManagerTreeView(QWidget *parent=0);
					  
 public slots:
  void activate(const QModelIndex& index);
  void clickNextEvent();
  void toggleRename();
  
 signals:
  void eventClicked(AEvent*);
  void packageActivated(AEventPackage*);
  
 protected:
  void contextMenuEvent(QContextMenuEvent *event);

 private:
  QModelIndex lastSelectedIndex;

  Q_OBJECT
};

#endif //AEVENTMANAGERTREEVIEW_H_
