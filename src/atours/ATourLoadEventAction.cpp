#include "ATourLoadEventAction.h"
#include <QDebug>

ATourLoadEventAction::ATourLoadEventAction()
  :ATourAction()
{ 
  geo=(AGeoPlugin*)AMELIA::global->plugin("AGeometry");
}

void ATourLoadEventAction::loadFromXML(QDomElement actionElement)
{
  eventFile=actionElement.attribute("file");

  ATourAction::loadFromXML(actionElement);
}

void ATourLoadEventAction::act()
{
  ATourAction::act();

  if(eventFile.isEmpty())
    geo->unloadEvent();
  else
    geo->loadEvent(eventFile);
}
