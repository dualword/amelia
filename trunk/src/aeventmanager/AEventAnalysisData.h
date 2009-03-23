#ifndef AEVENTANALYSISDATA_H_
#define AEVENTANALYSISDATA_H_

#include <QList>
#include <QMap>
#include <QString>
#include <QObject>

#include "ATrack.h"

class AEventAnalysisData : public QObject
{
 public:
  AEventAnalysisData(QString modulename);
  ~AEventAnalysisData();

  QList<QString> listCollections();
  QList<ATrack*> getCollection(QString);
  void setCollection(QString,QList<ATrack*>);

 signals:
  void updated();

 private:
  QString _modulename;
  
  QMap<QString,QList<ATrack*> > _collections;

  Q_OBJECT
};

#endif //AEVENTANALYSISDATA_H_
