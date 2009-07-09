#include "AFOVControl.h"
#include "CSceneNodeAnimatorCameraFOV.h"

AFOVControl::AFOVControl(ICameraSceneNode *camera,
			 ISceneManager *scene,
			 IGUIEnvironment *environment,
			 IGUIElement *parent, s32 id,
			 rect<s32> rectangle)
  : ACameraControl(camera,scene,environment,parent,id,rectangle)
{ 
  core::rect<s32> _rect(position2d<s32>(0,0),getRelativePosition().getSize());
  _fov=environment->addScrollBar(false,_rect,this,-1);

  core::rect<f32> _fovRect(0.0,0.0,1.0,1.0);
  
  _fov->setRelativePositionProportional(_fovRect);

  list<ISceneNodeAnimator*>::ConstIterator anims=camera->getAnimators().begin();
  CSceneNodeAnimatorCameraFOV *anim=(CSceneNodeAnimatorCameraFOV*)*anims;

  _fov->setMax(anim->maxFOV()*10000);
  _fov->setMin(anim->minFOV()*10000);

  _fov->setSmallStep(1);
  _fov->setLargeStep(100);
}

bool AFOVControl::OnEvent(const SEvent& event)
{
  if(!isVisible()) return false;
  
  if(event.EventType==EET_GUI_EVENT)
    {
      if(event.GUIEvent.Caller==_fov && event.GUIEvent.EventType==EGET_SCROLL_BAR_CHANGED)
	{
	  float fov=_fov->getPos()/10000.;
	  camera()->setFOV(fov);
	  return true;
	}
    }
  return false;
}

void AFOVControl::setVisible(bool value)
{
  if(value)
    _fov->setPos(camera()->getFOV()*10000);
  ACameraControl::setVisible(value);
}

void AFOVControl::OnAnimate(u32 timeDeltaMs)
{
  _fov->setPos(camera()->getFOV()*10000);
}
