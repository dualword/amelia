// Copyright (C) 2008 Karol Krizka
// This file was created for the AMELIA project
// For conditions of distribution and use, see copyright notice at http://amelia.sf.net

#include "CSceneNodeAnimatorCameraSwitch.h"
#include <math.h>

CSceneNodeAnimatorCameraSwitch::CSceneNodeAnimatorCameraSwitch(scene::ISceneManager *_smgr)
  :smgr(_smgr),targetCam(0),movingCam(0),lastUpdate(0)
{
#ifdef _DEBUG
  setDebugName("CSceneNodeAnimatorCameraSwitch");
#endif
}

CSceneNodeAnimatorCameraSwitch::~CSceneNodeAnimatorCameraSwitch()
{
  smgr->addToDeletionQueue(movingCam);
  movingCam->drop();
}

void CSceneNodeAnimatorCameraSwitch::animateNode(scene::ISceneNode* node, u32 timeMs)
{
  if (node->getType() != scene::ESNT_CAMERA)
    return;

  if(targetCam==0) return;

  scene::ICameraSceneNode* camera = static_cast<scene::ICameraSceneNode*>(node);

  if(lastUpdate==0) lastUpdate=timeMs;
  f32 timeDiff=timeMs-lastUpdate;
  lastUpdate=timeMs;
  if(timeDiff==0) return;

  core::vector3df currentPosition=camera->getPosition();
  core::vector3df currentTarget=camera->getTarget();

  core::vector3df finalPosition = targetCam->getPosition();
  core::vector3df finalTarget = targetCam->getTarget();

  core::vector3df posDiff=calculateDelta(currentPosition,finalPosition,timeDiff);
  core::vector3df tarDiff=calculateDelta(currentTarget,finalTarget,timeDiff*50);

  f32 posDiffLength=posDiff.getLength();
  f32 tarDiffLength=tarDiff.getLength();

  if(tarDiffLength>0 || posDiffLength>0) //Are we there yet?
    { //Nope, update position/target
      camera->setPosition(currentPosition + posDiff);
      camera->setTarget(currentTarget + tarDiff);
    }
  else
    { //Yup, set the target camera
      //We need to make sure the active cammera was not switched in the meantime...
      if(smgr->getActiveCamera()==movingCam)
	{
	  smgr->setActiveCamera(targetCam);
	}
      movingCam->setInputReceiverEnabled(false);
      targetCam=0;
      smgr->addToDeletionQueue(movingCam);
      movingCam=0;
    }

}

bool CSceneNodeAnimatorCameraSwitch::OnEvent(const SEvent& event)
{
  if(targetCam!=0) return false;
  else return true;
}

void CSceneNodeAnimatorCameraSwitch::setTargetCamera(scene::ICameraSceneNode *target)
{
  //Disable the input reciever of the last camera
  smgr->getActiveCamera()->setInputReceiverEnabled(false);

  core::vector3df initialPosition=smgr->getActiveCamera()->getPosition();
  core::vector3df initialTarget=smgr->getActiveCamera()->getTarget();

  if(!movingCam)
    {
      movingCam=smgr->addCameraSceneNode(0,initialPosition,initialTarget);
      movingCam->addAnimator(this);
      movingCam->setID(-1);
    }
  else
    {
      movingCam->setPosition(initialPosition);
      movingCam->setTarget(initialTarget);
    }

  targetCam=target;

  //Let's get this show on the road!
  smgr->setActiveCamera(movingCam);

  lastUpdate=0;
}

core::vector3df CSceneNodeAnimatorCameraSwitch::calculateDelta(core::vector3df start,core::vector3df end,f32 timeDiff)
{
  core::vector3df delta(end-start);
  
  f32 moveBy=timeDiff;
  if(delta.getLength()>=moveBy)
    delta.setLength(moveBy);

  //if(delta.getLength()>=100)
  //delta.setLength(100);

  return delta;
}
