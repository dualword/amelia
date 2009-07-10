#include "AParticleFilter.h"

AParticleFilter::AParticleFilter(ATrackFilter *next)
  :ATrackFilter(next)
{
  _showElectrons=true;
  _showMuons=true;
  _showHadrons=true;
  _showPhotons=true;
  _showJets=true;
  _showMissingEt=true;
 }

void AParticleFilter::setShowElectrons(bool toggle)
{
  _showElectrons=toggle;
  emit filterUpdated();
  emit showElectronsChanged(toggle);
}

void AParticleFilter::setShowMuons(bool toggle)
{
  _showMuons=toggle;
  emit filterUpdated();
  emit showMuonsChanged(toggle);
}

void AParticleFilter::setShowHadrons(bool toggle)
{
  _showHadrons=toggle;
  emit filterUpdated();
  emit showHadronsChanged(toggle);
}

void AParticleFilter::setShowPhotons(bool toggle)
{
  _showPhotons=toggle;
  emit filterUpdated();
  emit showPhotonsChanged(toggle);
}

void AParticleFilter::setShowJets(bool toggle)
{
  _showJets=toggle;
  emit filterUpdated();
  emit showJetsChanged(toggle);
}

void AParticleFilter::setShowMissingEt(bool toggle)
{
  _showMissingEt=toggle;
  emit filterUpdated();
  emit showMissingEtChanged(toggle);
}

bool AParticleFilter::showElectrons()
{
  return _showElectrons;
}

bool AParticleFilter::showMuons() 
{
  return _showMuons;
}

bool AParticleFilter::showHadrons() 
{ 
  return _showHadrons;
}

bool AParticleFilter::showPhotons() 
{ 
  return _showPhotons;
}

bool AParticleFilter::showJets()
{ 
  return _showJets;
}

bool AParticleFilter::showMissingEt() 
{
  return _showMissingEt;
}

bool AParticleFilter::checkTrack(ATrack *track)
{
  if(track->type()==ATrack::eSTrack)
    {
      ASTrack* strack=(ASTrack*)track;
      if(!_showElectrons && strack->isElectron()) return false;
      if(!_showMuons && strack->isMuon()) return false;
      if(!_showHadrons && (strack->isChargedHadron() || strack->isNeutralHadron() ) ) return false;
      if(!_showPhotons && strack->isPhoton()) return false;
    }

  if(!_showJets && track->type()==ATrack::eJet) return false;
  if(!_showMissingEt && track->type()==ATrack::eMissingEt) return false;

  return ATrackFilter::checkTrack(track);
}
