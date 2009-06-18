#include "ATourViewportChangeAction.h"
#include <QDebug>

#include <AMELIA.h>
#include <ageometry/AViewport.h>

ATourViewportChangeAction::ATourViewportChangeAction()
  :ATourAction()
{ 
  geoplugin=(AGeoPlugin *)AMELIA::global->plugin("AGeometry");
  geo=(AGeometry*)geoplugin->findWidget("Geometry");

  setCursorable(true);
}

QString ATourViewportChangeAction::widgetOfInterest()
{
  AViewport *LeftViewport=(AViewport*)geoplugin->findWidget("LeftViewport");
  AViewport *RightViewport=(AViewport*)geoplugin->findWidget("RightViewport");

  if(LeftViewport->viewport()==viewport)
    return "LeftViewport";
  if(RightViewport->viewport()==viewport)
    return "RightViewport";

  int previous_viewport=(previousAction())?((ATourViewportChangeAction*)previousAction())->viewport:-1;
  if(LeftViewport->viewport()==previous_viewport)
    return "LeftViewport";
  if(RightViewport->viewport()==previous_viewport)
    return "RightViewport";

  return ATourAction::widgetOfInterest();
}

void ATourViewportChangeAction::loadFromXML(QDomElement actionElement)
{
  ATourAction::loadFromXML(actionElement);

  QDomElement viewportElement=actionElement.namedItem("viewport").toElement();

  QString viewportString=viewportElement.attribute("name");

  if(viewportString=="Side")
    viewport=AGeometry::Side;
  else if(viewportString=="Front")
    viewport=AGeometry::Front;
  else// if(viewportString=="Cam3D")
    viewport=AGeometry::Cam3D;
}

void ATourViewportChangeAction::act()
{
  ATourAction::act();

  geo->setViewport(viewport);
}

void ATourViewportChangeAction::prepare()
{
  ATourAction::prepare();

  viewport=geo->viewport();
}
