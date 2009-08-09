#include "AParticleFilter.h"

AParticleFilter::AParticleFilter(AEventObjectFilter *next)
  :AEventObjectFilter(next)
{
  _showElectrons=true;
  _showMuons=true;
  _showHadrons=true;
  _showPhotons=true;
  _showJets=true;
  _showMissingEt=true;
  _showCalorimeters=true;
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

void AParticleFilter::setShowCalorimeters(bool toggle)
{
  _showCalorimeters=toggle;
  emit filterUpdated();
  emit showCalorimetersChanged(toggle);
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

bool AParticleFilter::showCalorimeters() 
{
  return _showCalorimeters;
}

bool AParticleFilter::check(AEventObject *object)
{
  if(object->type()==AEventObject::eTrack)
    {
      ATrack* track=(ATrack*)object;
      if(!_showElectrons && track->isElectron()) return false;
      if(!_showMuons && track->isMuon()) return false;
      if(!_showHadrons && (track->isChargedHadron() || track->isNeutralHadron() ) ) return false;
      if(!_showPhotons && track->isPhoton()) return false;
    }

  if(!_showJets && object->type()==AEventObject::eJet) return false;
  if(!_showMissingEt && object->type()==AEventObject::eMissingEt) return false;
  if(!_showCalorimeters && object->type()==AEventObject::eShower) return false;

  return AEventObjectFilter::check(object);
}
