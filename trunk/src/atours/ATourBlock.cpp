#include "ATourBlock.h"

#include <QTimeLine>
#include <QDebug>

#include "ATourMouseConnectAction.h"

ATourBlock::ATourBlock()
  :_duration(0)
{ }

int ATourBlock::duration()
{
  return _duration;
}

void ATourBlock::loadBlock(QDomElement blockElement)
{
  QDomElement actionElement=blockElement.firstChildElement("action");
  while(!actionElement.isNull())
    {
      QString type=actionElement.attribute("type");
      ATourAction *action=ATourAction::newInstance(type);

      qDebug() << type << " " << action;

      if(action)
	{
	  action->loadFromXML(actionElement);
	  
	  addAction(action);
	  
	  QPoint pos=action->cursor();
	  if(!pos.isNull())
	    {
	      ATourMouseConnectAction *connectAction=new ATourMouseConnectAction();
	      connectAction->connectTo(action);
	      addAction(connectAction);
	    }
	    
	  int _tmpDuration=action->time()+action->duration();
	  if(_duration<_tmpDuration) _duration=_tmpDuration;
	}
      
      actionElement=actionElement.nextSiblingElement("action");
    }
}

void ATourBlock::updateFrame(int frame)
{
  QTimeLine *timeLine=qobject_cast<QTimeLine *>(sender());

  QMap<QString,ATourAction *>::const_iterator iter=actions.begin();
  QMap<QString,ATourAction *>::const_iterator iterE=actions.end();
  for(;iter!=iterE;iter++)
    {
      QString type=iter.key();
      ATourAction *topaction=iter.value();
      ATourAction *action=topaction->getActionFor(frame);

      // Handle changes in action (act/undo)
      if(lastActions[type]!=action)
	{
	  if(lastActions[type]) lastActions[type]->undo();
	  if(action) action->act();

	  lastActions[type]=action;
	}

      if(!action) continue;

      int duration=action->duration();
      int start=action->time();
      int end=start+duration;
      double done=frame-start;
      if(frame<=end)
	action->update(done/((double)duration));
      else
	action->update(1);
    }
}

void ATourBlock::prepare()
{
  QMap<QString,ATourAction *>::const_iterator iter=actions.begin();
  QMap<QString,ATourAction *>::const_iterator iterE=actions.end();
  for(;iter!=iterE;iter++)
    {
      ATourAction *action=iter.value();
      action->prepare();
    }

  iter=actions.begin();
  iterE=actions.end();
  for(;iter!=iterE;iter++)
    {
      lastActions[iter.key()]=0;
    }
}

void ATourBlock::cleanup()
{
  QMap<QString,ATourAction *>::const_iterator iter=actions.begin();
  QMap<QString,ATourAction *>::const_iterator iterE=actions.end();
  for(;iter!=iterE;iter++)
    {
      ATourAction *action=iter.value();
      action->cleanup();
    }
}

void ATourBlock::addAction(ATourAction *action)
{
  QString type=action->metaObject()->className();

  if(!actions.contains(type))
    actions[type]=ATourAction::newInstance(type);

  actions[type]->insertAction(action);

  ATourAction *iter=actions[type];
  while(iter)
    {
      qDebug() << "FOUND: " << iter << " " << iter->time();
      iter=iter->nextAction();
    }
}

