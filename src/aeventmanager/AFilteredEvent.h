#ifndef AFILTEREDEVENT_H_
#define AFILTEREDEVENT_H_

#include "AEvent.h"
#include "AEventObjectFilter.h"

class AEVENTMANAGER_EXPORT AFilteredEvent : public AEvent
{
Q_OBJECT  

private:
  AEvent *_completeEvent;
  AEventObjectFilter *_filters;
		       
public:
  AFilteredEvent(AEvent *,AEventObjectFilter *);
  AEvent *completeEvent();
  
public slots:
  void updateFilters();
  
signals:
  void filtersUpdated();
};

#endif //AFILTEREDEVENT_H_
