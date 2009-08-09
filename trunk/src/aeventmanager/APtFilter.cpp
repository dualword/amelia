#include "APtFilter.h"

#include "ATrack.h"
#include "AJet.h"

APtFilter::APtFilter(double minPt,AEventObjectFilter *nextFilter)
  :AEventObjectFilter(nextFilter),_minPt(minPt)
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

bool APtFilter::check(AEventObject* object)
{
  if(object->type()==AEventObject::eTrack && object->pt()<minPt()) return false;
  if(object->type()==AEventObject::eJet && object->et()<minPt()) return false;
  if(object->type()==AEventObject::eShower && object->et()<minPt()) return false;
  return AEventObjectFilter::check(object);
}
