#ifndef APTFILTER_H_
#define APTFILTER_H_

#include "AEventObjectFilter.h"

class AEVENTMANAGER_EXPORT APtFilter : public AEventObjectFilter
{
 public:
  APtFilter(double minPt,AEventObjectFilter* next=0);
  
  double minPt();
  bool check(AEventObject* object);
	
 signals:
  void minPtChanged(double);
			
 public slots:
  void setMinPt(double);

 private:
  int _minPt;  

  Q_OBJECT
};

#endif //APTFILTER_H_
