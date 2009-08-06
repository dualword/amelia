#include "AMisET.h"

AMisET::AMisET():AEventObject("Missing Energy",AEventObject::eMissingEt)
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
