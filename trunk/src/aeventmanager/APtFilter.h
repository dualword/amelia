#ifndef APTFILTER_H_
#define APTFILTER_H_

#include "ATrackFilter.h"

class AEVENTMANAGER_EXPORT APtFilter : public ATrackFilter
{
 Q_OBJECT

 private:
  int _minPt;
  
 public:
  APtFilter(double minPt,ATrackFilter* next=0);

  double minPt();
  bool checkTrack(ATrack* track);
  
 public slots:
  void setMinPt(double);
  void setMinPtMeV(double);
};

#endif //APTFILTER_H_
