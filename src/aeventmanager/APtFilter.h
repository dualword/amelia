#ifndef APTFILTER_H_
#define APTFILTER_H_

#include "ATrackFilter.h"

class APtFilter : public ATrackFilter
{
 Q_OBJECT

 private:
  int _minPt;
  
 public:
  APtFilter(float minPt,ATrackFilter* next=0);

  float minPt();
  bool checkTrack(ATrack* track);
  
 public slots:
  void setMinPt(float);
  void setMinPtMeV(int);
  
};

#endif //APTFILTER_H_
