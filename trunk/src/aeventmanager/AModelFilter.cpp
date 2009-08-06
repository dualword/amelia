#include "AModelFilter.h"
#include <QDebug>
#include "ARTrack.h"

AModelFilter::AModelFilter(QString jetType,QString misEtType,QString trackType,ATrackFilter *next)
  :ATrackFilter(next),_jetType(jetType),_misEtType(misEtType),_trackType(trackType)
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

bool AModelFilter::checkTrack(AEventObject* track)
{
  if(track->type()==AEventObject::eJet &&((AJet*)track)->jetType()!=_jetType)
    return false;

  if(track->type()==AEventObject::eMissingEt &&((AMisET*)track)->misETType()!=_misEtType)
    return false;

  if(track->type()==AEventObject::eRTrack &&
     (((ARTrack*)track)->RTrackType()!="CombinedInDetTracks" && // 14.x+
      ((ARTrack*)track)->RTrackType()!="Tracks")) // 12.x
    {
      return false;
    }

  if((track->type()==AEventObject::eSTrack && _trackType!="STrack")
     || (track->type()==AEventObject::eRTrack && _trackType!="RTrack"))
    return false;
  
  return ATrackFilter::checkTrack(track);
}

