#ifndef AEVENTPACKAGE_H
#define AEVENTPACKAGE_H

#include <QList>
#include <QString>

#include "AXmlEvent.h"

class AEventPackage
{
public:
  AEventPackage();
  ~AEventPackage();
  
  void load(const QString&);
  void save();

  void setName(QString);
  QString name();

  int eventCount();
  Aevent* event(int);
  
private:
  QList<Aevent *> events;
  
  QString _name;
  QString location;
};

#endif