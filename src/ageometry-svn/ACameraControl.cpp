#include "ACameraControl.h"

ACameraControl::ACameraControl(ICameraSceneNode *camera,
			       ISceneManager *scene,
			       IGUIEnvironment *environment,
			       IGUIElement *parent, s32 id,
			       rect<s32> rectangle)
  : IGUIElement(EGUIET_ELEMENT,environment,parent,id,rectangle),
    _camera(camera),SceneManager(scene),_onlyIfEnabled(false)
{ }

ICameraSceneNode* ACameraControl::camera()
{
  return _camera;
}

void ACameraControl::setOnlyIfEnabled(bool onlyIfEnabled)
{
  _onlyIfEnabled=onlyIfEnabled;
}

bool ACameraControl::onlyIfEnabled()
{
  return _onlyIfEnabled;
}

void ACameraControl::handleNewActiveCamera(ICameraSceneNode *cam)
{
  if((_onlyIfEnabled && _camera->isInputReceiverEnabled()) || cam!=_camera)
    {
      if(isVisible())
	setVisible(false);
    }
  else
    if(!isVisible())
      setVisible(true);

}

void ACameraControl::OnPostRender(u32 timeMs)
{
  IGUIElement::OnPostRender(timeMs);

  if(!isVisible()) return;
  if(lastUpdate==0)
    {
      lastUpdate=timeMs;
      return;
    }

  u32 TimeDelta=(timeMs-lastUpdate);
  if(TimeDelta>0)
    OnAnimate(TimeDelta);
  lastUpdate=timeMs;
}

void ACameraControl::OnAnimate(u32 timeDeltaMs)
{ }
