#include "APtFilter.h"

APtFilter::APtFilter(float minPt,ATrackFilter *nextFilter)
  :ATrackFilter(nextFilter),_minPt(minPt)
{}

float APtFilter::minPt()
{
  return this->_minPt;
}

void APtFilter::setMinPt(float _minPt)
{
  this->_minPt=_minPt;

  emit filterUpdated();
}

void APtFilter::setMinPtMeV(int _minPt)
{
  this->_minPt=((float)_minPt)/1000;

  emit filterUpdated();
}

bool APtFilter::checkTrack(ATrack* track)
{
  if(track->Pt()<minPt() && track->type()==ATrack::eSTrack) return false;
  return ATrackFilter::checkTrack(track);
}
