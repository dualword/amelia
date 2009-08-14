#include "ATourSelectTrackAction.h"

#include <QDebug>

#include <irrlicht.h>
using namespace irr;

#include <AMELIA.h>

#include <ageometry/AGeoPlugin.h>

ATourSelectTrackAction::ATourSelectTrackAction()
  :ATourAction()
{ 
  AGeoPlugin *geoplugin=(AGeoPlugin*)AMELIA::global->plugin("AGeometry");
  geo=geoplugin->findWidget<AGeometry*>("Geometry");

  setWidgetOfInterest("Geometry");
}

Q_INVOKABLE ATourSelectTrackAction::ATourSelectTrackAction(QString msg)
{
  AGeoPlugin *geoplugin=(AGeoPlugin*)AMELIA::global->plugin("AGeometry");
  geo=geoplugin->findWidget<AGeometry*>("Geometry");

  this->trackid=msg.toInt();

  setWidgetOfInterest("Geometry");
}

QString ATourSelectTrackAction::type()
{
  return ATourAction::type()+"_"+QString(trackid);
}

QPoint ATourSelectTrackAction::cursor()
{
  scene::ISceneNode *node=geo->getTrackNodeByID(trackid);
  if(!node) return ATourAction::cursor();
  
  core::vector3df pos=node->getBoundingBox().getCenter();

  scene::ISceneCollisionManager *colmngr=geo->getSceneManager()->getSceneCollisionManager();

  core::position2d<s32> spos=colmngr->getScreenCoordinatesFrom3DPosition(pos);

  QPoint qpos(spos.X,spos.Y);

  return geo->mapTo(geo->window(),qpos);
}

void ATourSelectTrackAction::loadFromXML(QDomElement actionElement)
{
  ATourAction::loadFromXML(actionElement);

  trackid=actionElement.attribute("id").toInt();
  state=(actionElement.attribute("state","on")=="on")?true:false;
  multi=(actionElement.attribute("multi","false")=="true")?true:false;
}

void ATourSelectTrackAction::act()
{
  geo->selectTrackByID(trackid,multi);
}

void ATourSelectTrackAction::undo()
{
  geo->deselectTrackByID(trackid);
}

void ATourSelectTrackAction::prepare()
{
  state=geo->isSelected(trackid);
}
