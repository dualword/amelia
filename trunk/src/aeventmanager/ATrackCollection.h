#ifndef ATRACKCOLLECTION_H_
#define ATRACKCOLLECTION_H_

#include <QList>
#include <QMap>
#include <QString>
#include <QObject>
#include <QDomElement>

#include "ATrack.h"
#include "AEventAnalysisData.h"

class AEVENTMANAGER_EXPORT ATrackCollection : public AEventAnalysisData
{
 public:
  Q_INVOKABLE ATrackCollection(QString modulename);
  ~ATrackCollection();

  QList<QString> listCollections();
  QList<ATrack*> getCollection(QString);
  void setCollection(QString,QList<ATrack*>);

  void writeToFile(QTextStream& in);
  void loadFromXML(QDomElement ele, class AEvent* e);

 private:
  QMap<QString,QList<ATrack*> > _collections;

  QList<ATrack*> readTracksFromXmlElement(AEvent*, const QDomElement& ele);
  void writeTrackToXmlFile(QTextStream& in,ATrack* track);


  Q_OBJECT
};

#endif //ATRACKCOLLECTION_H_
