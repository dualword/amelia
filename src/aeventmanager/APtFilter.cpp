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
  emit minPtMeVChanged(_minPt*1000);
}

void APtFilter::setMinPtMeV(double _minPt)
{
  setMinPt(_minPt/1000.);
}

bool APtFilter::checkTrack(ATrack* track)
{
  if(track->Pt()<minPt() && track->type()==ATrack::eSTrack) return false;
  if(track->Pt()<minPt() && track->type()==ATrack::eJet) return false;
  return ATrackFilter::checkTrack(track);
}
