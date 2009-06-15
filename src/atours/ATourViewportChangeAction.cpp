#include "ATourViewportChangeAction.h"
#include <QDebug>

#include <AMELIA.h>
#include <ageometry/AViewport.h>

ATourViewportChangeAction::ATourViewportChangeAction()
  :ATourAction()
{ 
  geoplugin=(AGeoPlugin *)AMELIA::global->plugin("AGeometry");

  geo=(AGeometry*)geoplugin->findWidget("Geometry");
}

QString ATourViewportChangeAction::widgetOfInterest()
{
  AViewport *LeftViewport=(AViewport*)geoplugin->findWidget("LeftViewport");
  AViewport *RightViewport=(AViewport*)geoplugin->findWidget("RightViewport");
  qDebug() << LeftViewport << " " << RightViewport;

  if(LeftViewport->viewport()==viewport)
    return "LeftViewport";
  if(RightViewport->viewport()==viewport)
    return "RightViewport";

  return ATourAction::widgetOfInterest();
}

void ATourViewportChangeAction::loadFromXML(QDomElement actionElement)
{
  ATourAction::loadFromXML(actionElement);

  QDomElement viewportElement=actionElement.namedItem("viewport").toElement();

  QString viewportString=viewportElement.attribute("name");
  if(viewportString=="Cam3D")
    viewport=AGeometry::Cam3D;
  else if(viewportString=="Front")
    viewport=AGeometry::Front;
  else if(viewportString=="Side")
    viewport=AGeometry::Side;
}

void ATourViewportChangeAction::doAction()
{
  ATourAction::doAction();

  ATourViewportChangeAction *prev=previousAction<ATourViewportChangeAction*>();
  if(prev)
    {
      viewportInitial=prev->viewport;
    }
  else
    {
      viewportInitial=geo->viewport();
    }

  geo->setViewport(viewport);
}

void ATourViewportChangeAction::undoAction()
{
  ATourAction::undoAction();
  geo->setViewport(viewportInitial);
}
