#include "ASphereControl.h"
#include <QDebug>
ASphereControl::ASphereControl(ICameraSceneNode *camera,
			 ISceneManager *scene,
			 IGUIEnvironment *environment,
			 IGUIElement *parent, s32 id,
			 rect<s32> rectangle)
  : ACameraControl(camera,scene,environment,parent,id,rectangle)
{ 
  _zoomIn =environment->addButton(rect<s32>(), this, -1, L"Zoom In",  L"Zoom in camera.");
  _zoomOut=environment->addButton(rect<s32>(), this, -1, L"Zoom Out", L"Zoom out camera.");

  _zoomIn ->setRelativePositionProportional(rect<f32>(0.0,0.0,0.5,1.0));
  _zoomOut->setRelativePositionProportional(rect<f32>(0.5,0.0,1.0,1.0));
  
  _zoomIn ->updateAbsolutePosition();
  _zoomOut->updateAbsolutePosition();

  setText(L"Sphere Control");
}

void ASphereControl::OnAnimate(u32 timeDeltaMs)
{
  f32 TimeDelta=(float)timeDeltaMs;

  ICameraSceneNode *camera=this->camera();
  double speed=0.05;

  vector3df curpos=camera->getPosition();  
  double distance=curpos.getLength();
  
  if(_zoomIn->isPressed())
    distance-=TimeDelta*speed;
  if(_zoomOut->isPressed())
    distance+=TimeDelta*speed;
  
  if(distance<1)
    distance=1;
  
  curpos.setLength(distance);
  camera->setPosition(curpos);
}
