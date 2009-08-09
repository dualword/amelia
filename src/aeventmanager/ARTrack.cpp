#include "ARTrack.h"
#include <QDebug>
#include <math.h>

ARTrack::ARTrack():ATrack("Reconstructed Track","RTrack")
{ }

ARTrack::~ARTrack()
{ }

std::vector<irr::core::vector3df> ARTrack::getPolyline()
{
    return polyLine;
}

void ARTrack::setCotTheta(float cotTheta)
{
  // cot(theta) = 1/tan(theta)
  // tan(theta) = 1/cot(theta)
  // theta = atan(1/cot(theta))
  _theta=fabs(atan(1/cotTheta));
  _eta=-logf(tan(_theta/2));
}

QString ARTrack::RTrackType()
{
  return _RTrackType;
}
