#include "ATrackFilter.h"

ATrackFilter::ATrackFilter(ATrackFilter *next):_nextFilter(next)
{ }

ATrackFilter* ATrackFilter::nextFilter()
{
  return _nextFilter;
}

bool ATrackFilter::checkTrack(ATrack *track)
{
  if(_nextFilter)
    return _nextFilter->checkTrack(track);
  else
    return true;
}
