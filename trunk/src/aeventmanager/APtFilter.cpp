#include "APtFilter.h"

APtFilter::APtFilter(double minPt,ATrackFilter *nextFilter)
  :ATrackFilter(nextFilter),_minPt(minPt)
{}

double APtFilter::minPt()
{
  return this->_minPt;
}

void APtFilter::setMinPt(double _minPt)
{
  this->_minPt=_minPt;

  emit filterUpdated();
  emit minPtChanged(_minPt);
}

bool APtFilter::checkTrack(AEventObject* track)
{
  if(track->Pt()<minPt() && track->type()==AEventObject::eSTrack) return false;
  if(track->Pt()<minPt() && track->type()==AEventObject::eRTrack) return false;
  if(track->Pt()<minPt() && track->type()==AEventObject::eJet) return false;
  return ATrackFilter::checkTrack(track);
}
