#include "AMisET.h"

AMisET::AMisET():ATrack("Missing Energy",ATrack::eMissingEt)
{ }

AMisET::~AMisET()
{ }

void AMisET::setMisETType(QString mtype)
{
  _misETType=mtype;
}

QString AMisET::misETType()
{
  return _misETType;
}
