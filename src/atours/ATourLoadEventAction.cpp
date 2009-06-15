#include "ATourLoadEventAction.h"
#include <QDebug>

ATourLoadEventAction::ATourLoadEventAction()
  :ATourAction()
{ 
  geo=(AGeoPlugin*)AMELIA::global->plugin("AGeometry");
}

void ATourLoadEventAction::loadFromXML(QDomElement actionElement)
{
  ATourAction::loadFromXML(actionElement);
  eventFile=actionElement.attribute("file");
}

void ATourLoadEventAction::doAction()
{
  ATourAction::doAction();

  ATourLoadEventAction *prev=nextAction<ATourLoadEventAction*>();
  if(prev)
    eventFileIninial=prev->eventFile;
  else
    eventFileIninial="";

  geo->loadEvent(eventFile);
}

void ATourLoadEventAction::undoAction()
{
  ATourAction::undoAction();
  if(eventFileIninial.isEmpty())
    geo->unloadEvent();
  else
    geo->loadEvent(eventFileIninial);
}
