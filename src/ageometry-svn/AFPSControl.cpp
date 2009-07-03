#include "AFPSControl.h"

AFPSControl::AFPSControl(ICameraSceneNode *camera,
			 ISceneManager *scene,
			 IGUIEnvironment *environment,
			 IGUIElement *parent, s32 id,
			 rect<s32> rectangle)
  : ACameraControl(camera,scene,environment,parent,id,rectangle),
    MaxVerticalAngle(88.0f),
    anim(0)
{ 
  setOnlyIfEnabled(true);

  _1Button=environment->addButton(rect<s32>(),this,-1,L"Up",L"");
  _2Button=environment->addButton(rect<s32>(),this,-1,L"",L"");
  _3Button=environment->addButton(rect<s32>(),this,-1,L"Left",L"");
  _4Button=environment->addButton(rect<s32>(),this,-1,L"",L"");
  _5Button=environment->addButton(rect<s32>(),this,-1,L"Down",L"");
  _6Button=environment->addButton(rect<s32>(),this,-1,L"",L"");
  _7Button=environment->addButton(rect<s32>(),this,-1,L"Right",L"");
  _8Button=environment->addButton(rect<s32>(),this,-1,L"",L"");

  forwardButton=environment->addButton(rect<s32>(),this,-1,L"Forward",L"");
  backButton=environment->addButton(rect<s32>(),this,-1,L"Back",L"");

  core::rect<f32> _1Rect(0.2f,0.0f,0.8f,0.2f);
  core::rect<f32> _2Rect(0.8f,0.0f,1.0f,0.2f);
  core::rect<f32> _3Rect(0.8f,0.2f,1.0f,0.8f);
  core::rect<f32> _4Rect(0.8f,0.8f,1.0f,1.0f);
  core::rect<f32> _5Rect(0.2f,0.8f,0.8f,1.0f);
  core::rect<f32> _6Rect(0.0f,0.8f,0.2f,1.0f);
  core::rect<f32> _7Rect(0.0f,0.2f,0.2f,0.8f);
  core::rect<f32> _8Rect(0.0f,0.0f,0.2f,0.2f);

  core::rect<f32> forwardRect(0.25f,0.25f,0.75f,0.50f);
  core::rect<f32>    backRect(0.25f,0.50f,0.75f,0.75f);

  _1Button->setRelativePositionProportional(_1Rect);
  _2Button->setRelativePositionProportional(_2Rect);
  _3Button->setRelativePositionProportional(_3Rect);
  _4Button->setRelativePositionProportional(_4Rect);
  _5Button->setRelativePositionProportional(_5Rect);
  _6Button->setRelativePositionProportional(_6Rect);
  _7Button->setRelativePositionProportional(_7Rect);
  _8Button->setRelativePositionProportional(_8Rect);

  forwardButton->setRelativePositionProportional(forwardRect);
  backButton->setRelativePositionProportional(backRect);

  list<ISceneNodeAnimator*>::ConstIterator anims=camera->getAnimators().begin();
  ISceneNodeAnimatorCameraFPS *anim=(ISceneNodeAnimatorCameraFPS*)*anims;
  this->anim=anim;
}

void AFPSControl::OnAnimate(u32 timeDeltaMs)
{
  vector3df pos = camera()->getAbsolutePosition();
  vector3df target = (camera()->getTarget() - pos);
  vector3df relativeRotation = target.getHorizontalAngle();

  target.normalize();

  f32 TimeDelta=(float)timeDeltaMs;
  f32 RotateSpeed=anim->getRotateSpeed()*TimeDelta/1000.;
  f32 MoveSpeed=anim->getMoveSpeed()*TimeDelta;
  
  if(_1Button->isPressed())
    relativeRotation.X-=RotateSpeed;
  if(_2Button->isPressed())
    {
      relativeRotation.X-=RotateSpeed;
      relativeRotation.Y+=RotateSpeed;
    }
  if(_3Button->isPressed())
    relativeRotation.Y+=RotateSpeed;
  if(_4Button->isPressed())
    {
      relativeRotation.X+=RotateSpeed;
      relativeRotation.Y+=RotateSpeed;
    }
  if(_5Button->isPressed())
    relativeRotation.X+=RotateSpeed;
  if(_6Button->isPressed())
    {
      relativeRotation.X+=RotateSpeed;
      relativeRotation.Y-=RotateSpeed;
    }
  if(_7Button->isPressed())
    relativeRotation.Y-=RotateSpeed;
  if(_8Button->isPressed())
    {
      relativeRotation.X-=RotateSpeed;
      relativeRotation.Y-=RotateSpeed;
    }

  if(relativeRotation.X > MaxVerticalAngle*2 &&
     relativeRotation.X < 360.0f-MaxVerticalAngle)
    relativeRotation.X = 360.0f-MaxVerticalAngle;
  else if(relativeRotation.X > MaxVerticalAngle &&
	  relativeRotation.X < 360.0f-MaxVerticalAngle)
    relativeRotation.X = MaxVerticalAngle;

  if(forwardButton->isPressed())
    pos+=target*MoveSpeed;
  if(backButton->isPressed())
    pos-=target*MoveSpeed;

  
  target.set(0,0,1);
  
  core::matrix4 mat;
  mat.setRotationDegrees(core::vector3df(relativeRotation.X, relativeRotation.Y, 0));
  mat.transformVect(target);

  
  camera()->setTarget(target + pos);
  camera()->setPosition(pos);
}
