#include "AParticleFilter.h"

AParticleFilter::AParticleFilter(ATrackFilter *next)
  :ATrackFilter(next)
{
  _showElectrons=true;
  _showMuons=true;
  _showChargedHadrons=true;
  _showNeutralHadrons=true;
  _showPhotons=true;
  _showJets=true;
  _showMissingEt=true;
 }

void AParticleFilter::setShowElectrons(bool toggle)
{
  _showElectrons=toggle;
  emit filterUpdated();
}

void AParticleFilter::setShowMuons(bool toggle)
{
  _showMuons=toggle;
  emit filterUpdated();
}

void AParticleFilter::setShowChargedHadrons(bool toggle)
{
  _showChargedHadrons=toggle;
  emit filterUpdated();
}

void AParticleFilter::setShowNeutralHadrons(bool toggle)
{
  _showNeutralHadrons=toggle;
  emit filterUpdated();
}

void AParticleFilter::setShowPhotons(bool toggle)
{
  _showPhotons=toggle;
  emit filterUpdated();
}

void AParticleFilter::setShowJets(bool toggle)
{
  _showJets=toggle;
  emit filterUpdated();
}

void AParticleFilter::setShowMissingEt(bool toggle)
{
  _showMissingEt=toggle;
  emit filterUpdated();
}

bool AParticleFilter::checkTrack(ATrack *track)
{
  if(track->type()==ATrack::eSTrack)
    {
      ASTrack* strack=(ASTrack*)track;
      if(!_showElectrons && strack->isElectron()) return false;
      if(!_showMuons && strack->isMuon()) return false;
      if(!_showChargedHadrons && strack->isChargedHadron()) return false;
      if(!_showNeutralHadrons && strack->isNeutralHadron()) return false;
      if(!_showPhotons && strack->isPhoton()) return false;
    }

  if(!_showJets && track->type()==ATrack::eJet) return false;
  if(!_showMissingEt && track->type()==ATrack::eMissingEt) return false;

  return ATrackFilter::checkTrack(track);
}
