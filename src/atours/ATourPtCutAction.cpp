#include "ATourPtCutAction.h"

#include <ageometry/AGeoPlugin.h>

#include <QDebug>

ATourPtCutAction::ATourPtCutAction()
  :ATourAction()
{
  AGeoPlugin *geo=(AGeoPlugin*)AMELIA::global->plugin("AGeometry");
  ptFilter=geo->ptFilter();

  setWidgetOfInterest("PtCutoff_Slider");
}

void ATourPtCutAction::loadFromXML(QDomElement actionElement)
{
  value=actionElement.attribute("value").toDouble();

  ATourAction::loadFromXML(actionElement); 
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

  ptFilter->setMinPt(usevalue);
}

void ATourPtCutAction::prepare()
{
  ATourAction::prepare();

  value=ptFilter->minPt();
}
