#ifndef AEVENTPACKAGE_H
#define AEVENTPACKAGE_H

#include <QList>
#include <QString>

#include "AEvent.h"

class AEventPackage : public QObject
{
 public:
  AEventPackage(const QString& location, QObject *parent=0);
  ~AEventPackage();
  
  void load();
  bool isLoaded();
  
  void setName(QString);
  QString name();

  QString location();
  
  int eventCount();
  int indexOf(AEvent*);

  void addEvent(AEvent*);
  void removeEvent(AEvent*);
  AEvent* event(int);

public slots:
  void save();
  
  
private:
  QList<AEvent *> events;
  
  QString _name;
  QString _location;
  bool _loaded;
  
  void loadMetaInfo();
  void loadLogBook();
  
  Q_OBJECT
};

#endif
