#include "AEventObject.h"

AEventObject::AEventObject(QString name,AEventObject::eType type)
  :_type(type),_name(name),_isInteresting(true),
   _eta(0),_theta(0),_phi(0),_pt(0),_et(0)
{ }

AEventObject::AEventObject(const AEventObject& o)
{
  this->_type=o._type;
  this->_name=o._name;
  this->_ID=o._ID;
}

QString AEventObject::name()
{
  return _name;
}

void AEventObject::setName(QString name)
{
  _name=name;
  emit updated();
}

float AEventObject::eta()
{
  return _eta;
}

float AEventObject::phi()
{
  return _phi;
}

float AEventObject::theta()
{
  return _theta;
}

float AEventObject::pt()
{
  return _pt;
}

float AEventObject::et()
{
  return _et;
}

float AEventObject::getTl()
{
  return 0.5 * ( exp (_eta) - exp (-_eta));
}

AEventObject::eType AEventObject::type()
{
  return _type;
}

unsigned int AEventObject::ID()
{
  return _ID;
}

bool AEventObject::isInteresting()
{
  return _isInteresting;
}
