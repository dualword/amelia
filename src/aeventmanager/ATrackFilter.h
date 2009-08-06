#ifndef ATRACKFILTER_H_
#define ATRACKFILTER_H_

#include "AEventObject.h"

#include <QObject>

class ATrackFilter : public QObject
{
 Q_OBJECT

 private:
  ATrackFilter *_nextFilter;

 public:
  ATrackFilter(ATrackFilter *next=0);
  virtual bool checkTrack(AEventObject* track) =0;

  ATrackFilter* nextFilter();

 signals:
  void filterUpdated();
};

#endif //ATRACKFILTER_H_
