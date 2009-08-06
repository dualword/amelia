#include "AEventObject.h"

unsigned int AEventObject::highestTrackID=0;

AEventObject::AEventObject()
 :_type(AEventObject::eUndefined),_interesting(true)
{}

AEventObject::AEventObject(QString name,AEventObject::eType type)
  :_type(type),_name(name),_interesting(true)
{ }

AEventObject::AEventObject(const AEventObject& o)
{
  this->_type=o._type;
  this->_name=o._name;
  this->_trackID=o._trackID;
  this->_selectionID=o._selectionID;
  this->_charge=o._charge;
  this->_pt=o._pt;
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

AEventObject::eType AEventObject::type()
{
  return _type;
}

unsigned int AEventObject::trackID()
{
  return _trackID;
}

void AEventObject::setTrackID(unsigned int trackID)
{
  _trackID=trackID;
}

unsigned int AEventObject::selectionID()
{
  return _selectionID;
}

void AEventObject::setSelectionID(unsigned int selectionID)
{
  _selectionID=selectionID;
}

int AEventObject::charge()
{
  return _charge;
}

void AEventObject::setCharge(int charge)
{
  _charge=charge;
}

float AEventObject::Pt()
{
  return _pt;
}

void AEventObject::setPt(float pt)
{
  _pt=pt;
}

bool AEventObject::isInteresting()
{
  return _interesting;
}

void AEventObject::setInteresting(bool interesting)
{
  _interesting=interesting;
}
