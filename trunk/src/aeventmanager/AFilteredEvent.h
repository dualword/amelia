#ifndef AFILTEREDEVENT_H_
#define AFILTEREDEVENT_H_

#include "AEvent.h"
#include "ATrackFilter.h"

class AEVENTMANAGER_EXPORT AFilteredEvent : public AEvent
{
Q_OBJECT  

private:
  AEvent *_completeEvent;
  ATrackFilter *_filters;
		       
public:
  AFilteredEvent(AEvent *,ATrackFilter *);
  AEvent *completeEvent();
  
public slots:
  void updateFilters();
  
signals:
  void filtersUpdated();
};

#endif //AFILTEREDEVENT_H_
