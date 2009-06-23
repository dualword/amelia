#include "ATrack.h"

unsigned int ATrack::highestTrackID=0;

ATrack::ATrack():_type(ATrack::eUndefined),_interesting(true)
{}

ATrack::ATrack(QString name,ATrack::eType type):_name(name),_type(type),_interesting(true)
{ }

ATrack::ATrack(const ATrack& o)
{
  this->_type=o._type;
  this->_name=o._name;
  this->_trackID=o._trackID;
  this->_selectionID=o._selectionID;
  this->_charge=o._charge;
  this->_pt=o._pt;
}

QString ATrack::name()
{
  return _name;
}

void ATrack::setName(QString name)
{
  _name=name;
}

ATrack::eType ATrack::type()
{
  return _type;
}

unsigned int ATrack::trackID()
{
  return _trackID;
}

void ATrack::setTrackID(unsigned int trackID)
{
  _trackID=trackID;
}

unsigned int ATrack::selectionID()
{
  return _selectionID;
}

void ATrack::setSelectionID(unsigned int selectionID)
{
  _selectionID=selectionID;
}


int ATrack::charge()
{
  return _charge;
}

void ATrack::setCharge(int charge)
{
  _charge=charge;
}

float ATrack::Pt()
{
  return _pt;
}

void ATrack::setPt(float pt)
{
  _pt=pt;
}

bool ATrack::isInteresting()
{
  return _interesting;
}

void ATrack::setInteresting(bool interesting)
{
  _interesting=interesting;
}
