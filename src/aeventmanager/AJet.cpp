#include "AJet.h"

#include <math.h>

AJet::AJet():AEventObject("Jet",AEventObject::eJet)
{ 
  setCharge(0);
}

void AJet::setJetType(QString jtype)
{
  _jetType=jtype;
}

QString AJet::jetType()
{
  return _jetType;
}

float AJet::getTl()
{
  return 0.5 * ( exp (eta) - exp (-eta));
}

float AJet::theta()
{
  return 2*atan ( exp ( -eta ) );
}
