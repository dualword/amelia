#include "ATourMouseConnectAction.h"
#include <QDebug>
#include <QLabel>
#include <QCursor>

#include <AMELIA.h>

ATourMouseConnectAction::ATourMouseConnectAction()
  :ATourMouseMoveAction(),action(0)
{ 
  base=(ABase *)AMELIA::global->plugin("ABase");
}

void ATourMouseConnectAction::connectTo(ATourAction *act)
{
  int time=act->time()-1000;
  setTime(time);
  setDuration(1000);
  action=act;
}

QPoint ATourMouseConnectAction::target()
{
  if(action)
      return action->cursor();
  else
      return ATourMouseMoveAction::target();
}
