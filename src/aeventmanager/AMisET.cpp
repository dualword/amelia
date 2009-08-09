#include "AMisET.h"

AMisET::AMisET()
  : AEventObject("Missing Energy",AEventObject::eMissingEt),_etx(0),_ety(0)
{ }

AMisET::~AMisET()
{ }

void AMisET::setEt(float et)
{
  _et=et;
  _pt=et;
}

void AMisET::setEtX(float etx)
{
  _etx=etx;
  _phi=atan2(_ety,_etx);
}

float AMisET::etx()
{
  return _etx;
}

void AMisET::setEtY(float ety)
{
  _ety=ety;
  _phi=atan2(_ety,_etx);
}

float AMisET::ety()
{
  return _ety;
}

void AMisET::setMisETType(QString mtype)
{
  _misETType=mtype;
}

QString AMisET::misETType()
{
  return _misETType;
}
