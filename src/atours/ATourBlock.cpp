#include "ATourBlock.h"

#include <QTimeLine>
#include <QDebug>

ATourBlock::ATourBlock()
  :_duration(0),lastAction(0)
{ }

int ATourBlock::duration()
{
  return _duration;
}

void ATourBlock::reset()
{
  lastAction=0;
}

void ATourBlock::addAction(ATourAction *action)
{
  if(act.size()>0)
    {
      action->setPreviousAction(act[act.size()-1]);
      act[act.size()-1]->setNextAction(action);
    }

  act.push_back(action);
  actt.push_back(_duration);

  _duration+=action->duration();
}

void ATourBlock::updateFrame(int frame)
{
  QTimeLine *timeLine=qobject_cast<QTimeLine *>(sender());

  int at=actt.size()-1;
  for(int i=actt.size()-1;i>=0;i--)
    {
      if(frame >= actt[i])
	{
	  at=i;
	  break;
	}
    }

  ATourAction *action=act[at];
  int actiont=actt[at];

  if(action!=lastAction)
    {
      if(lastAction)
	{
	  lastAction->endAction();
	  if(timeLine->direction()==QTimeLine::Backward)
	    lastAction->undoAction();
	}
      lastAction=action;

      action->doAction();
    }
    
  double timeInAction=frame-actiont;
  action->updateAction(timeInAction/(double)action->duration());
  

}

