#include "AModelFilter.h"
#include <QDebug>
#include "ARTrack.h"
#include "AShower.h"

AModelFilter::AModelFilter(QString jetType,QString misEtType,QString trackType,AEventObjectFilter *next)
  :AEventObjectFilter(next),_jetType(jetType),_misEtType(misEtType),_trackType(trackType)
{ }

void AModelFilter::setJetType(QString jetType)
{
  _jetType=jetType;
  emit filterUpdated();
}

QString AModelFilter::jetType()
{
  return _jetType;
}

void AModelFilter::setMisEtType(QString misEtType)
{
  _misEtType=misEtType;
  emit filterUpdated();
}

QString AModelFilter::misEtType()
{
  return _misEtType;
}

void AModelFilter::setTrackType(QString ttype)
{
  _trackType=ttype;
  emit filterUpdated();
}

QString AModelFilter::trackType()
{
  return _trackType;
}

bool AModelFilter::check(AEventObject* object)
{
  if(object->type()==AEventObject::eJet &&((AJet*)object)->jetType()!=_jetType)
    return false;

  if(object->type()==AEventObject::eMissingEt &&((AMisET*)object)->misETType()!=_misEtType)
    return false;

  if(object->type()==AEventObject::eTrack &&
     ((ATrack*)object)->trackType()=="RTrack" &&
     (((ARTrack*)object)->RTrackType()!="CombinedInDetTracks" && // 14.x+
      ((ARTrack*)object)->RTrackType()!="Tracks")) // 12.x
    {
      return false;
    }
  
  if(object->type()==AEventObject::eTrack && ((ATrack*)object)->trackType()!=_trackType)
    return false;

  return AEventObjectFilter::check(object);
}

