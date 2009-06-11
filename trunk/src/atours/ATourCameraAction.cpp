#include "ATourCameraAction.h"
#include <QDebug>

#include <ageometry/AGeoPlugin.h>

ATourCameraAction::ATourCameraAction()
  :ATourAction()
{ 
  AGeoPlugin *geoplugin=(AGeoPlugin *)AMELIA::global->plugin("AGeometry");

  geo=(AGeometry*)geoplugin->findWidget("Geometry");
}

void ATourCameraAction::loadFromXML(QDomElement actionElement)
{
  ATourAction::loadFromXML(actionElement);

  QDomElement cameraElement=actionElement.namedItem("camera").toElement();
  QDomElement targetElement=actionElement.namedItem("target").toElement();

  position=APoint3D(cameraElement.attribute("x").toInt(),
		    cameraElement.attribute("y").toInt(),
		    cameraElement.attribute("z").toInt() );

  target=APoint3D(targetElement.attribute("x").toInt(),
		  targetElement.attribute("y").toInt(),
		  targetElement.attribute("z").toInt() );
}

void ATourCameraAction::doAction()
{
  ATourAction::doAction();

  ATourCameraAction *prev=previousAction<ATourCameraAction*>();
  if(prev)
    {
      positionInitial=prev->position;
      targetInitial=prev->target;
    }
  else
    {
      positionInitial=geo->cameraPosition();
      targetInitial=geo->cameraTarget();
    }
  geo->setCameraPosition(positionInitial);
  geo->setCameraTarget(targetInitial);
}

void ATourCameraAction::updateAction(double done)
{
  ATourAction::updateAction(done);

  APoint3D pos=interpolate(positionInitial,position,done);
  geo->setCameraPosition(pos);
  
  APoint3D tar=interpolate(targetInitial,target,done);
  geo->setCameraTarget(tar);

}

APoint3D ATourCameraAction::interpolate(APoint3D start,APoint3D end,double time)
{
  APoint3D diff=end-start;

  return start + diff*time;
}
