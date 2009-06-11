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

void ATourMonitorAction::doAction()
{
  ATourMonitorAction *prev=previousAction<ATourMonitorAction*>();
  if(prev)
    {
      monitorInitial=monitor;
    }
  else
    {
      monitorInitial=base->currentGroup()+"/"+base->currentMonitor();
    }


  base->changeToMonitor(monitor);
}

void ATourMonitorAction::undoAction()
{
  base->changeToMonitor(monitorInitial);
}
