#include "ATourAction.h"
#include <QDebug>
#include <QList>
#include <QApplication>
#include <QRect>
#include <QWidget>

QMap<QString,QMetaObject> ATourAction::_listOfActionTypes;

ATourAction::ATourAction()
  :_previousAction(0),_nextAction(0)
{ }

void ATourAction::setDuration(int _duration)
{
  this->_duration=_duration;
}

int ATourAction::duration()
{
  return _duration;
}

ATourAction* ATourAction::previousAction()
{
  if(subactions.size()>0)
    {
      return subactions[subactions.size()-1];
    }
  else
    {
      return _previousAction;
    }
}

void ATourAction::setPreviousAction(ATourAction *action)
{
  _previousAction=action;
  if(subactions.size()>0)
    subactions[0]->setPreviousAction(_previousAction);
}

ATourAction* ATourAction::nextAction()
{
  if(subactions.size()>0)
    {
      return subactions[0];
    }
  else
    {
      return _nextAction;
    }
}

void ATourAction::setNextAction(ATourAction *action)
{
  _nextAction=action;
  if(subactions.size()>0)
    subactions[subactions.size()-1]->setNextAction(_nextAction);
}

void ATourAction::doAction()
{ 
  for(int i=0;i<subactions.size();i++)
    subactionsPerformed[i]=false;
}

void ATourAction::updateAction(double done)
{ 
  int frame=frameFromTime(done);
  for(int i=0;i<subactions.size();i++)
    {
      ATourAction *subaction=subactions[i];
      
      //Should be performed
      if(frame<=subaction->duration())
	{
	  // Has not yet started
	  if(!subactionsPerformed[i])
	    {
	      subaction->doAction();
	      subactionsPerformed[i]=true;
	      //qDebug() << "DO " << subaction;
	    }

	  double time=((double)frame)/((double)subaction->duration());
	  subaction->updateAction(time);
	  //qDebug() << "UPDATE " << subaction << " " << time << " " << frame;
	}

      //Subaction is over, and it is in performed state. Undo!
      if(frame>subaction->duration() && subactionsPerformed[i])
	{
	  subaction->endAction();
	  subactionsPerformed[i]=false;
	  //qDebug() << "UNDO " << subaction;
	}      
    }
}

void ATourAction::endAction()
{
  for(int i=0;i<subactions.size();i++)
    {
      ATourAction *subaction=subactions[i];
      //Action is over, and it is in performed state. Undo!
      if(subactionsPerformed[i])
	{
	  subaction->endAction();
	  subactionsPerformed[i]=false;
	  //qDebug() << "END " << subaction;
	}      
    }
}

void ATourAction::undoAction()
{
  for(int i=0;i<subactions.size();i++)
    {
      ATourAction *subaction=subactions[i];
      //Action is over, and it is in performed state. Undo!
      if(subactionsPerformed[i])
	{
	  subaction->undoAction();
	  subactionsPerformed[i]=false;
	  //qDebug() << "UNDO " << subaction;
	}      
    }
}

void ATourAction::cleanupAction()
{
  for(int i=0;i<subactions.size();i++)
    {
      ATourAction *subaction=subactions[i];
      subaction->cleanupAction();
    }
}

void ATourAction::loadFromXML(QDomElement actionElement)
{
  _duration=actionElement.attribute("duration").toInt();

  QDomNodeList hintNodes=actionElement.elementsByTagName("hint");
  for(int i=0;i<hintNodes.size();i++)
    {
      QDomElement hintElement=hintNodes.at(i).toElement();
      QString hintType=hintElement.attribute("type");
      if(hintType=="Widget")
	{
	  _widgetOfInterest=hintElement.attribute("name");
	}
    }

  QDomNodeList actionNodes=actionElement.elementsByTagName("action");
  for(int i=0;i<actionNodes.size();i++)
    {
      QDomElement actionElement=actionNodes.at(i).toElement();
      QString type=actionElement.attribute("type");
      
      ATourAction* subaction=ATourAction::newInstance(type);
      subaction->loadFromXML(actionElement);

      addSubaction(subaction);
    }
}

int ATourAction::frameFromTime(double time)
{
  return _duration*time;
}

void ATourAction::addSubaction(ATourAction* subaction)
{
  if(duration()<subaction->duration())
    setDuration(subaction->duration());

  subaction->setNextAction(_nextAction);
  if(subactions.size()>0)
    subaction->setPreviousAction(subactions[subactions.size()-1]);
  else
    subaction->setPreviousAction(_previousAction);
  
  subactions.push_back(subaction);
  subactionsPerformed.push_back(false);
}

void ATourAction::addActionType(QMetaObject metaobj)
{
  _listOfActionTypes[metaobj.className()]=metaobj;
}

ATourAction* ATourAction::newInstance(QString classname)
{
  if(!_listOfActionTypes.contains(classname))
    return 0;
  
  QObject* obj=_listOfActionTypes[classname].newInstance();
  return qobject_cast<ATourAction*>(obj);
}

QString ATourAction::widgetOfInterest()
{
  return _widgetOfInterest;
}

QPoint ATourAction::cursor()
{
  QString widgetOfInterest=this->widgetOfInterest();
  if(widgetOfInterest.isEmpty()) return QPoint();

  QWidget *widget=0;
  QWidget *widgetParent=0;
  QList<QWidget*> wdgs=QApplication::topLevelWidgets();
  for(int i=0;i<wdgs.size();i++)
    {
      widget=wdgs[i]->findChild<QWidget*>(widgetOfInterest);
      if(widget)
	{
	  widgetParent=wdgs[i];
	  break;
	}
    }

  if(!widget) return QPoint();

  QRect geo(QPoint(0,0),widget->size());
  QPoint center=geo.center();

  return widget->mapTo(widgetParent,center);
}
