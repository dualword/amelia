#include "AEventObjectFilter.h"

AEventObjectFilter::AEventObjectFilter(AEventObjectFilter *next):_nextFilter(next)
{ }

AEventObjectFilter* AEventObjectFilter::nextFilter()
{
  return _nextFilter;
}

bool AEventObjectFilter::check(AEventObject *object)
{
  if(_nextFilter)
    return _nextFilter->check(object);
  else
    return true;
}
