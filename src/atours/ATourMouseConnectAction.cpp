#include "ATourMouseConnectAction.h"
#include <QDebug>
#include <QApplication>

ATourMouseConnectAction::ATourMouseConnectAction()
  :ATourMouseMoveAction()
{ }

void ATourMouseConnectAction::loadFromXML(QDomElement actionElement)
{
  ATourAction::loadFromXML(actionElement);
}

void ATourMouseConnectAction::doAction()
{
  ATourMouseMoveAction::doAction();

  ATourAction *next=nextAction();
  QPoint tar=next->cursor();
  setTarget(tar);
}

