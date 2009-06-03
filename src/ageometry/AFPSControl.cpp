#include "AFPSControl.h"

AFPSControl::AFPSControl(ICameraSceneNode *camera,
			 ISceneManager *scene,
			 IGUIEnvironment *environment,
			 IGUIElement *parent, s32 id,
			 rect<s32> rectangle)
  : IGUIElement(EGUIET_ELEMENT,environment,parent,id,rectangle),
    camera(0),anim(0),lastUpdate(0)
{ 
  SceneManager=scene;


  upButton=environment->addButton(rect<s32>(),this,-1,L"Up",L"");
  rightButton=environment->addButton(rect<s32>(),this,-1,L"Right",L"");
  downButton=environment->addButton(rect<s32>(),this,-1,L"Down",L"");
  leftButton=environment->addButton(rect<s32>(),this,-1,L"Left",L"");

  forwardButton=environment->addButton(rect<s32>(),this,-1,L"Forward",L"");
  backButton=environment->addButton(rect<s32>(),this,-1,L"Back",L"");

  grabButton=environment->addButton(rect<s32>(),this,-1,L"G",L"");

  core::rect<f32> upRect(0.2,0,0.8,0.2);
  core::rect<f32> rightRect(0.8,0.2,1,0.8);
  core::rect<f32> downRect(0.2,0.8,0.8,1);
  core::rect<f32> leftRect(0,0.2,0.2,0.8);

  core::rect<f32> forwardRect(0.25,0.25,0.75,0.5);
  core::rect<f32> backRect(0.25,0.5,0.75,0.75);

  core::rect<f32> grabRect(0.8,0,1,0.2);

  upButton->setRelativePositionProportional(upRect);
  rightButton->setRelativePositionProportional(rightRect);
  downButton->setRelativePositionProportional(downRect);
  leftButton->setRelativePositionProportional(leftRect);

  forwardButton->setRelativePositionProportional(forwardRect);
  backButton->setRelativePositionProportional(backRect);

  grabButton->setRelativePositionProportional(grabRect);

  list<ISceneNodeAnimator*>::ConstIterator anims=camera->getAnimators().begin();
  ISceneNodeAnimatorCameraFPS *anim=(ISceneNodeAnimatorCameraFPS*)*anims;
  this->anim=anim;
  this->camera=camera;
}

bool AFPSControl::OnEvent(const SEvent &event)
{
  switch(event.EventType)
    {
    case EET_GUI_EVENT:
      switch(event.GUIEvent.EventType)
	{
	case EGET_BUTTON_CLICKED:
	  if(event.GUIEvent.Caller==grabButton)
	    camera->setInputReceiverEnabled(true);
	  break;
	default:
	  break;
	}
      break;
    default:
      break;
    }
  return false;
}

void AFPSControl::OnPostRender(u32 timeMs)
{
  if(camera->isInputReceiverEnabled() || SceneManager->getActiveCamera()!=camera)
    {
      if(isVisible())
	setVisible(false);
      return;
    }
  else
    if(!isVisible())
      setVisible(true);
  


  if(lastUpdate==0)
    {
      lastUpdate=timeMs;
      return;
    }

  vector3df pos = camera->getAbsolutePosition();
  vector3df target = (camera->getTarget() - pos);
  vector3df relativeRotation = target.getHorizontalAngle();

  target.normalize();

  f32 TimeDelta=(float)(timeMs-lastUpdate);
  f32 RotateSpeed=anim->getRotateSpeed()*TimeDelta/1000.;
  f32 MoveSpeed=anim->getMoveSpeed()*TimeDelta;
  
  if(upButton->isPressed())
    relativeRotation.X-=RotateSpeed;
  if(rightButton->isPressed())
    relativeRotation.Y+=RotateSpeed;
  if(downButton->isPressed())
    relativeRotation.X+=RotateSpeed;
  if(leftButton->isPressed())
    relativeRotation.Y-=RotateSpeed;

  if(forwardButton->isPressed())
    pos+=target*MoveSpeed;
  if(backButton->isPressed())
    pos-=target*MoveSpeed;

  
  target.set(0,0,1);
  
  core::matrix4 mat;
  mat.setRotationDegrees(core::vector3df(relativeRotation.X, relativeRotation.Y, 0));
  mat.transformVect(target);

  
  camera->setTarget(target + pos);
  camera->setPosition(pos);

  lastUpdate=timeMs;
}
