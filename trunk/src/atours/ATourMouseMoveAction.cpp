#include "ATourMouseMoveAction.h"
#include <QDebug>
#include <QLabel>

ATourMouseMoveAction::ATourMouseMoveAction()
  :ATourAction()
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

void ATourMouseMoveAction::update(double done)
{
  ATourAction::update(done);

  QWidget *cursor=mouseCursor();

  if(target().isNull())
    {
      cursor->hide();
      return;
    }

  if(cursor->isHidden())
    cursor->show();

  QPoint tar;
  if(!previousAction() || done==1)
    {
      tar=target();
    }
  else
    {
      QPoint tarIni=((ATourMouseMoveAction*)previousAction())->target();
      if(tarIni.isNull()) tarIni=base->mapFromGlobal(QCursor::pos());
      QPoint tarFin=target();
      tar=interpolate(tarIni,tarFin,done);
    }

  cursor->move(tar);
}

void ATourMouseMoveAction::cleanup()
{
  ATourAction::cleanup();

  mouseCursor()->hide();
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

QPoint ATourMouseMoveAction::interpolate(QPoint start,QPoint end,double time)
{
  QPoint diff=end-start;

  return start + diff*time;
}
