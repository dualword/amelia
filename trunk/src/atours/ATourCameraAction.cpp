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
  QDomElement cameraElement=actionElement.namedItem("camera").toElement();
  QDomElement targetElement=actionElement.namedItem("target").toElement();

  position=APoint3D(cameraElement.attribute("x").toInt(),
		    cameraElement.attribute("y").toInt(),
		    cameraElement.attribute("z").toInt() );
  
  target=APoint3D(targetElement.attribute("x").toInt(),
		  targetElement.attribute("y").toInt(),
		  targetElement.attribute("z").toInt() );
  
  ATourAction::loadFromXML(actionElement);
}

void ATourCameraAction::update(double done)
{
  ATourAction::update(done);

  APoint3D pos;
  APoint3D tar;
  if(!previousAction() || done==1)
    {
      pos=position;
      tar=target;
    }
  else
    {
      APoint3D posIni=((ATourCameraAction*)previousAction())->position;
      APoint3D posFin=position;
      pos=interpolate(posIni,posFin,done);

      APoint3D tarIni=((ATourCameraAction*)previousAction())->target;
      APoint3D tarFin=target;
      tar=interpolate(tarIni,tarFin,done);
    }

  geo->setCameraPosition(pos);
  geo->setCameraTarget(tar);
}

void ATourCameraAction::prepare()
{
  ATourAction::prepare();

  // Set the viewport to 3D
  if(geo->viewport()!=AGeometry::Cam3D)
    geo->setViewport(AGeometry::Cam3D);

  // Store the current target/position, because this is the starting point
  position=geo->cameraPosition();
  target=geo->cameraTarget();    

  // Switch to FPS mode.
  // This is better, since we'll probably end up not
  // facing the center
  if(geo->camera()!=AGeometry::FPS)
    {
      geo->setCamera(AGeometry::FPS,false);
      geo->setCameraPosition(position);
      geo->setCameraTarget(target);
    }
  
  geo->lockCamera();
}

void ATourCameraAction::cleanup()
{
  ATourAction::cleanup();

  geo->unlockCamera();
}

APoint3D ATourCameraAction::interpolate(APoint3D start,APoint3D end,double time)
{
  APoint3D diff=end-start;

  return start + diff*time;
}
