#include "ATourMonitorAction.h"
#include <QDebug>

ATourMonitorAction::ATourMonitorAction()
  :ATourAction()
{ 
  base=(ABase *)AMELIA::global->plugin("ABase");
}

void ATourMonitorAction::loadFromXML(QDomElement actionElement)
{
  ATourAction::loadFromXML(actionElement);

  monitor=actionElement.attribute("monitor");
}

void ATourMonitorAction::act()
{
  base->changeToMonitor(monitor);
}

void ATourMonitorAction::prepare()
{
  monitor=base->currentGroup()+"/"+base->currentMonitor();
}
