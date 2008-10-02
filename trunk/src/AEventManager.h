#ifndef AEVENTMANAGER_H
#define AEVENTMANAGER_H

#include <QAbstractItemModel>

#include "AEventPackage.h"

class AEventManager : public QAbstractItemModel
{
Q_OBJECT

 public:
  AEventManager(QString);

  QModelIndex index(int, int, const QModelIndex&) const;
  QModelIndex parent(const QModelIndex&) const;
  int rowCount(const QModelIndex&) const;
  int columnCount(const QModelIndex&) const;
  QVariant data(const QModelIndex&, int) const;

  Aevent* getEvent(const QModelIndex&);
  AEventPackage getPackage(const QModelIndex&);

  void setActivePackage(const QModelIndex&);

  QModelIndex nextEvent();
  
 public slots:
  void eventLoaded(QString);
  void eventUnloaded();
  
  void tagActiveEvent(const QString&);
  void tagEvent(QModelIndex,const QString&);
  void detagActiveEvent(const QString&);
  void detagEvent(QModelIndex,const QString&);
 
 private:
  QList<AEventPackage> packages;
  QVariant *root;

  QModelIndex activePackage,activeEvent;
};

#endif 
