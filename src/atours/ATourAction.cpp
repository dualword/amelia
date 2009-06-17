#include "ATourAction.h"
#include <QDebug>
#include <QList>
#include <QApplication>
#include <QRect>
#include <QWidget>

QMap<QString,QMetaObject> ATourAction::_listOfActionTypes;

ATourAction::ATourAction()
  :_previous(0),_next(0),_stateless(false),_duration(0),_time(0)
{ }

void ATourAction::setDuration(int _duration)
{
  this->_duration=_duration;
}

int ATourAction::duration()
{
  return _duration;
}

void ATourAction::setTime(int time)
{
  _time=time;
}

int ATourAction::time()
{
  return _time;
}

bool ATourAction::isStateless()
{
  return _stateless;
}

void ATourAction::setStateless(bool stateless)
{
  _stateless=stateless;
}

void ATourAction::setNextAction(ATourAction *next)
{
  _next=next;
}

ATourAction* ATourAction::nextAction()
{
  return _next;
}
 
void ATourAction::setPreviousAction(ATourAction *previous)
{
  _previous=previous;
}

ATourAction* ATourAction::previousAction()
{
  return _previous;
}

void ATourAction::loadFromXML(QDomElement actionElement)
{
  _duration=actionElement.attribute("duration").toInt();
  _time=actionElement.attribute("time").toInt();
}

bool ATourAction::canPerform(int frame)
{
  int start=_time;
  int end=_time+_duration;
  int nextEnd=(nextAction())?nextAction()->time():end;

  if(frame < start) return false;

  if(frame <= end) return true;
  
  if(!isStateless() && frame < nextEnd) return true;
  if(!isStateless() && !nextAction()) return true;
  
  return false;
}

void ATourAction::act()
{ }

void ATourAction::undo()
{ }

void ATourAction::update(double done)
{ }

void ATourAction::prepare()
{ }

void ATourAction::cleanup()
{ }

QString ATourAction::widgetOfInterest()
{
  return QString();
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

  //qDebug() << "Map to " << widget->mapTo(widgetParent,center);

  return widget->mapTo(widgetParent,center);
}

void ATourAction::insertAction(ATourAction *action)
{
  bool insert=false;
  if(_next)
    {
      int nextTime=_next->time();
      int time=action->time();
      if(time<nextTime)
	insert=true;
    }
  else
    insert=true;

  if(insert)
    {
      if(nextAction())
	nextAction()->setPreviousAction(action);
      action->setNextAction(nextAction());

      setNextAction(action);
      action->setPreviousAction(this);
    }
  else if(_next)
    nextAction()->insertAction(action);
}

ATourAction* ATourAction::getActionFor(int frame)
{
  if(canPerform(frame) && !(nextAction() && nextAction()->canPerform(frame)))
    return this;
  
  if(nextAction())
    return nextAction()->getActionFor(frame);
  
  return 0;
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
