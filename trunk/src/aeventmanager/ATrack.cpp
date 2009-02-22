#include "ATrack.h"

unsigned int ATrack::highestTrackID=0;

ATrack::ATrack()
{
  trackID=++highestTrackID;
  selectionID=trackID;
}
