#ifndef AEVENTMANAGERTREEVIEW_H_
#define AEVENTMANAGERTREEVIEW_H_

#include <QTreeView>

#include "AEventManagerScene.h"
#include "AEventManagerDefines.h"

class AEVENTMANAGER_EXPORT AEventManagerTreeView : public QTreeView
{
 Q_OBJECT

 public:
  AEventManagerTreeView(QWidget *parent=0);

 public slots:
  void activate(const QModelIndex& index);
  void clickNextEvent();

 signals:
  void eventClicked(AEvent*);
};

#endif //AEVENTMANAGERTREEVIEW_H_
