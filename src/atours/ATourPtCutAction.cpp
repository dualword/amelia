#include "ATourPtCutAction.h"

#include <ageometry/AGeoPlugin.h>

#include <QDebug>

ATourPtCutAction::ATourPtCutAction()
  :ATourAction()
{
  AGeoPlugin *geo=(AGeoPlugin*)AMELIA::global->plugin("AGeometry");
  ptFilter=geo->ptCutSync();
}

void ATourPtCutAction::loadFromXML(QDomElement actionElement)
{
  ATourAction::loadFromXML(actionElement);
  value=actionElement.attribute("value").toDouble();
}

void ATourPtCutAction::doAction()
{
  ATourPtCutAction *prev=previousAction<ATourPtCutAction*>();
  if(prev)
    valueInitial=prev->value;
  else
    valueInitial=ptFilter->value();
}

void ATourPtCutAction::updateAction(double done)
{
  double delta=value-valueInitial;
  ptFilter->setValue(valueInitial+delta*done);
}

void ATourPtCutAction::undoAction()
{
  ptFilter->setValue(valueInitial);
}
