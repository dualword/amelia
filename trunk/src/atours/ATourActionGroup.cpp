#include "ATourActionGroup.h"
#include <QDebug>

ATourActionGroup::ATourActionGroup()
  :ATourAction()
{ }

void ATourActionGroup::doAction()
{
  for(int i=0;i<actions.size();i++)
    actionsPerformed[i]=false;
}

void ATourActionGroup::updateAction(double done)
{ 
  int frame=frameFromTime(done);
  for(int i=0;i<actions.size();i++)
    {
      ATourAction *action=actions[i];

      //Should be performed
      if(frame<=action->duration())
	{
	  // Has not yet started
	  if(!actionsPerformed[i])
	    {
	      action->doAction();
	      actionsPerformed[i]=true;
	      //qDebug() << "DO " << action;
	    }

	  double time=((double)frame)/((double)action->duration());
	  action->updateAction(time);
	  //qDebug() << "UPDATE " << action << " " << time << " " << frame;
	}

      //Action is over, and it is in performed state. Undo!
      if(frame>action->duration() && actionsPerformed[i])
	{
	  action->undoAction();
	  actionsPerformed[i]=false;
	  //qDebug() << "UNDO " << action;
	}      
    }
}

void ATourActionGroup::undoAction()
{
  for(int i=0;i<actions.size();i++)
    {
      ATourAction *action=actions[i];
      //Action is over, and it is in performed state. Undo!
      if(actionsPerformed[i])
	{
	  action->undoAction();
	  actionsPerformed[i]=false;
	  //qDebug() << "UNDO " << action;
	}      
    }
}
  
void ATourActionGroup::loadFromXML(QDomElement actionElement)
{ 
}
