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
  value=actionElement.attribute("value").toDouble();

  ATourAction::loadFromXML(actionElement); 
}

QString ATourPtCutAction::widgetOfInterest()
{
  return "PtCutoff_Slider";
}

void ATourPtCutAction::update(double done)
{
  ATourAction::update(done);

  double usevalue;
  if(!previousAction() || done==1)
    {
      usevalue=value;
    }
  else
    {
      double valueIni=((ATourPtCutAction*)previousAction())->value;
      double valueFin=value;
      double delta=valueFin-valueIni;
      usevalue=valueIni+delta*done;
    }

  ptFilter->setValue(usevalue);
}

void ATourPtCutAction::prepare()
{
  ATourAction::prepare();

  value=ptFilter->value();
}
