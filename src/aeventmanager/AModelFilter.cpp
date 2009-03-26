#include "AModelFilter.h"

AModelFilter::AModelFilter(AJet::jType jetType,AMisET::mType misEtType,ATrackFilter *next)
  :ATrackFilter(next),_jetType(jetType),_misEtType(misEtType)
{ }

void AModelFilter::setJetType(AJet::jType jetType)
{
  _jetType=jetType;
  emit filterUpdated();
}

void AModelFilter::setJetType(QString type)
{
  AJet::jType jtype;
  if (type == QString("Kt4H1TopoJets"))
    {
      jtype = AJet::jKt4H1TopoJets;
    }
  if (type == QString("Cone4H1TopoJets"))
    {
      jtype = AJet::jCone4H1TopoJets;
    }
  if (type == QString("Kt4H1TowerJets"))
    {
      jtype = AJet::jKt4H1TowerJets;
    }
  if (type == QString("Cone4H1TowerJets"))
    {
      jtype = AJet::jCone4H1TowerJets;
    }

  setJetType(jtype);
}

AJet::jType AModelFilter::jetType()
{
  return _jetType;
}

QString AModelFilter::jetTypeString()
{
  QString jtype;
  if (_jetType == AJet::jKt4H1TopoJets)
    {
      jtype = QString("Kt4H1TopoJets");
    }
  if (_jetType == AJet::jCone4H1TopoJets)
    {
      jtype = QString("Cone4H1TopoJets");
    }
  if (_jetType == AJet::jKt4H1TowerJets)
    {
      jtype = QString("Kt4H1TowerJets");
    }
  if (_jetType == AJet::jCone4H1TowerJets)
    {
      jtype = QString("Cone4H1TowerJets");
    }

  return jtype;
}

void AModelFilter::setMisEtType(AMisET::mType misEtType)
{
  _misEtType=misEtType;
  emit filterUpdated();
}

void AModelFilter::setMisEtType(QString type)
{
  AMisET::mType mtype;
  
  if (type == QString("MET_Final"))
    {
      mtype = AMisET::mMET_Final;
    }
  if (type == QString("MET_RefMuon"))
    {
      mtype = AMisET::mMET_RefMuon;
    }
  if (type == QString("MET_Calib"))
    {
      mtype = AMisET::mMET_Calib;
    }
  if (type == QString("MET_RefFinal"))
    {
      mtype = AMisET::mMET_RefFinal;
    }
  if (type == QString("MET_Truth"))
    {
      mtype = AMisET::mMET_Truth;
    }
  setMisEtType(mtype);
}

AMisET::mType AModelFilter::misEtType()
{
  return _misEtType;
}

QString AModelFilter::misEtTypeString()
{
  QString mtype;

  if (_misEtType == AMisET::mMET_Final)
    {
      mtype = QString("MET_Final");
    }
  if (_misEtType == AMisET::mMET_RefMuon)
    {
      mtype = QString("MET_RefMuon");
    }
  if (_misEtType == AMisET::mMET_Calib)
    {
      mtype = QString("MET_Calib");
    }
  if (_misEtType == AMisET::mMET_RefFinal)
    {
      mtype = QString("MET_RefFinal");
    }
  if (_misEtType == AMisET::mMET_Truth)
    {
      mtype = QString("MET_Truth");
    }
  return mtype;
}

bool AModelFilter::checkTrack(ATrack* track)
{
  if(track->type()==ATrack::eJet &&((AJet*)track)->JetType!=_jetType)
    return false;

  if(track->type()==ATrack::eMissingEt &&((AMisET*)track)->MisETType!=_misEtType)
    return false;
  
  return ATrackFilter::checkTrack(track);
}

