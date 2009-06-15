#include "ATourMouseMoveAction.h"
#include <QDebug>
#include <QLabel>

ATourMouseMoveAction::ATourMouseMoveAction()
  :ATourAction()
{ 
  base=(ABase *)AMELIA::global->plugin("ABase");
}

ATourMouseMoveAction::ATourMouseMoveAction(QPoint tar)
  :ATourAction(),_target(tar)
{
  base=(ABase *)AMELIA::global->plugin("ABase");
}

QPoint ATourMouseMoveAction::target()
{
  return _target;
}

void ATourMouseMoveAction::setTarget(QPoint target)
{
  _target=target;
}

void ATourMouseMoveAction::loadFromXML(QDomElement actionElement)
{
  ATourAction::loadFromXML(actionElement);

  QDomElement targetElement=actionElement.namedItem("target").toElement();
  
  if(!targetElement.isNull())
    _target=QPoint(targetElement.attribute("x").toInt(),
		   targetElement.attribute("y").toInt());
}

void ATourMouseMoveAction::doAction()
{
  ATourAction::doAction();

  ATourMouseMoveAction *prev=previousAction<ATourMouseMoveAction*>();
  if(prev)
    {
      targetInitial=prev->target();
    }
  else
    {
      targetInitial=QCursor::pos();
    }

  mouseCursor()->show();
}

void ATourMouseMoveAction::cleanupAction()
{
  ATourAction::cleanupAction();
  mouseCursor()->hide();
}

void ATourMouseMoveAction::updateAction(double done)
{
  ATourAction::updateAction(done);

  QPoint tar=interpolate(targetInitial,target(),done);

  mouseCursor()->move(tar);
}

QWidget* ATourMouseMoveAction::mouseCursor()
{
  QWidget *_mouseCursor=base->findChild<QWidget*>("Tour_Cursor");
  
  if(!_mouseCursor)
    {
      QPixmap cursorPix(":/media/cursor.png");

      QLabel *cursor=new QLabel(base);
      cursor->setPixmap(cursorPix);
      cursor->resize(cursorPix.size());

      _mouseCursor=cursor;
      _mouseCursor->setObjectName("Tour_Cursor");
    }

  return _mouseCursor;
}

QPoint ATourMouseMoveAction::cursor()
{
  return QPoint();
}

QPoint ATourMouseMoveAction::interpolate(QPoint start,QPoint end,double time)
{
  QPoint diff=end-start;

  return start + diff*time;
}
