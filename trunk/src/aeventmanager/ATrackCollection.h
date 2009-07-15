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

  int size();
  void addTrack(ATrack*);
  ATrack* getTrack(int idx);
  void removeTrack(int idx);
  bool containsTrack(ATrack*);

  void writeToFile(QTextStream& in);
  void loadFromXML(QDomElement ele, class AEvent* e);

 signals:
  void trackRemoved(int idx);
  void trackInserted(int idx);

 private:
  QList<ATrack*> _tracks;

  QList<ATrack*> readTracksFromXmlElement(AEvent*, const QDomElement& ele);
  void writeTrackToXmlFile(QTextStream& in,ATrack* track);


  Q_OBJECT
};

#endif //ATRACKCOLLECTION_H_
