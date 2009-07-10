#include "ATourParticleFilterAction.h"

#include <QDebug>

#include <AMELIA.h>

#include <ageometry/AGeoPlugin.h>

ATourParticleFilterAction::ATourParticleFilterAction()
  :ATourAction()
{ 
  AGeoPlugin *geoplugin=(AGeoPlugin*)AMELIA::global->plugin("AGeometry");
  filter=geoplugin->particleFilter();
}

Q_INVOKABLE ATourParticleFilterAction::ATourParticleFilterAction(QString particle)
{
  AGeoPlugin *geoplugin=(AGeoPlugin*)AMELIA::global->plugin("AGeometry");
  filter=geoplugin->particleFilter();

  this->particle=particle;
}

QString ATourParticleFilterAction::type()
{
  return ATourAction::type()+"_"+particle;
}

void ATourParticleFilterAction::loadFromXML(QDomElement actionElement)
{
  ATourAction::loadFromXML(actionElement);

  particle=actionElement.attribute("particle");
  state=(actionElement.attribute("state")=="on")?true:false;

  if(particle=="electrons")
    setWidgetOfInterest("checkBox_Electrons");
  else if(particle=="muons")
    setWidgetOfInterest("checkBox_Muons");
  else if(particle=="jets")
    setWidgetOfInterest("checkBox_Jets");
  else if(particle=="photons")
    setWidgetOfInterest("checkBox_Photons");
  else if(particle=="hadrons")
    setWidgetOfInterest("checkBox_NeutralHadrons");
  else if(particle=="met")
    setWidgetOfInterest("checkBox_MissingEt");
}

void ATourParticleFilterAction::act()
{
  if(particle=="electrons")
    filter->setShowElectrons(state);
  else if(particle=="muons")
    filter->setShowMuons(state);
  else if(particle=="jets")
    filter->setShowJets(state);
  else if(particle=="photons")
    filter->setShowPhotons(state);
  else if(particle=="hadrons")
    filter->setShowHadrons(state);
  else if(particle=="met")
    filter->setShowMissingEt(state);
}

void ATourParticleFilterAction::prepare()
{
  if(particle=="electrons")
    state=filter->showElectrons();
  else if(particle=="muons")
    state=filter->showMuons();
  else if(particle=="jets")
    state=filter->showJets();
  else if(particle=="photons")
    state=filter->showPhotons();
  else if(particle=="hadrons")
    state=filter->showHadrons();
  else if(particle=="met")
    state=filter->showMissingEt();
}
