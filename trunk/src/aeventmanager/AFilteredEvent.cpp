#include "AFilteredEvent.h"

AFilteredEvent::AFilteredEvent(AEvent *event,AEventObjectFilter *headFilter)
  :AEvent(),_completeEvent(event),_filters(headFilter)
{
  AEventObjectFilter *filter=headFilter;
  while(filter)
    {
      connect(filter,SIGNAL(filterUpdated()),
	      this,SLOT(updateFilters()));
      filter=filter->nextFilter();
    }

  if(event->inherits("AFilteredEvent"))
    connect(event,SIGNAL(filtersUpdated()),
	    this,SLOT(updateFilters()));

  updateFilters();
}

AEvent *AFilteredEvent::completeEvent()
{
  return _completeEvent;
}

void AFilteredEvent::updateFilters()
{
  FCALshowers=_completeEvent->FCALshowers;
  LArshowers=_completeEvent->LArshowers;
  HECshowers=_completeEvent->HECshowers;
  TILEshowers=_completeEvent->TILEshowers;

  Objects.clear();
  Tracks.clear();
  Jets.clear();
  MisET.clear();
  numTracks=0;
  numChargedHadrons=0;
  numNeutralHadrons=0;
  numPhotons=0;
  numElectrons=0;
  numNeutrinos=0;
  numMuons=0;
  numJets=0;

  numShowers = FCALshowers.size() + HECshowers.size() + LArshowers.size() + TILEshowers.size();

  for(int i=0;i<_completeEvent->Objects.size();i++)
    {
      if(_filters->check(_completeEvent->Objects[i]))
	addObject(_completeEvent->Objects[i]);
    }
  
  emit filtersUpdated();
}
