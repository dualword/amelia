#ifndef APTFILTER_H_
#define APTFILTER_H_

#include "ATrackFilter.h"

class AEVENTMANAGER_EXPORT APtFilter : public ATrackFilter
{
 public:
  APtFilter(double minPt,ATrackFilter* next=0);
  
  double minPt();
  bool checkTrack(ATrack* track);
	
 signals:
  void minPtChanged(double);
			
 public slots:
  void setMinPt(double);

 private:
  int _minPt;  

  Q_OBJECT
};

#endif //APTFILTER_H_
