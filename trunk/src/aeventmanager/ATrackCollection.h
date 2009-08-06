#ifndef ATRACKCOLLECTION_H_
#define ATRACKCOLLECTION_H_

#include <QList>
#include <QMap>
#include <QString>
#include <QObject>
#include <QDomElement>

#include "AEventObject.h"
#include "AEventAnalysisData.h"

class AEVENTMANAGER_EXPORT ATrackCollection : public AEventAnalysisData
{
 public:
  Q_INVOKABLE ATrackCollection(QString modulename);
  ~ATrackCollection();

  int size();
  void addTrack(AEventObject*);
  AEventObject* getTrack(int idx);
  void removeTrack(int idx);
  bool containsTrack(AEventObject*);

  void writeToFile(QTextStream& in);
  void loadFromXML(QDomElement ele, class AEvent* e);

 public slots:
  void handleTrackUpdated();

 signals:
  void trackRemoved(int idx);
  void trackInserted(int idx);

 private:
  QList<AEventObject*> _tracks;

  QList<AEventObject*> readTracksFromXmlElement(AEvent*, const QDomElement& ele);
  void writeTrackToXmlFile(QTextStream& in,AEventObject* track);


  Q_OBJECT
};

#endif //ATRACKCOLLECTION_H_
