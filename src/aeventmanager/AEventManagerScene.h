#ifndef AEVENTMANAGERSCENE_H
#define AEVENTMANAGERSCENE_H

#include <QAbstractItemModel>

#include "AEventManager.h"

class AEVENTMANAGER_EXPORT AEventManagerScene : public QAbstractItemModel
{
  Q_OBJECT

 public:
  AEventManagerScene(AEventManager *manager);

  QModelIndex index(int, int, const QModelIndex&) const;
  QModelIndex index(AEvent* event,int col) const;
  QModelIndex index(AEventPackage* event,int col) const;

  QModelIndex parent(const QModelIndex&) const;
  int rowCount(const QModelIndex&) const;
  int columnCount(const QModelIndex&) const;
  QVariant data(const QModelIndex&, int) const;

  AEvent* getEvent(const QModelIndex&) const;
  AEventPackage* getPackage(const QModelIndex&) const;

  QModelIndex nextEvent();

  AEventPackage* activePackage();
  AEvent* activeEvent();

 public slots:
  void setActivePackage(AEventPackage*);
  void setActiveEvent(AEvent* event=0);


 private:
  AEventManager *manager;

  QVariant *root;

  AEventPackage* _activePackage;
  AEvent* _activeEvent;
};

#endif
