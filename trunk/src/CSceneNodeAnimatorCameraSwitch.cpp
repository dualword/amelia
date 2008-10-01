// Copyright (C) 2008 Karol Krizka
// This file was created for the AMELIA project
// For conditions of distribution and use, see copyright notice at http://amelia.sf.net

#include "CSceneNodeAnimatorCameraSwitch.h"
#include <math.h>

CSceneNodeAnimatorCameraSwitch::CSceneNodeAnimatorCameraSwitch(scene::ISceneManager *_smgr)
  :smgr(_smgr),targetCam(0),movingCam(0)
{
#ifdef _DEBUG
  setDebugName("CSceneNodeAnimatorCameraSwitch");
#endif
  //movingCam=smgr->addCameraSceneNode(0);
  //movingCam->addAnimator(this);
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

  f32 timeDiff=timeMs-lastUpdate;
  lastUpdate=timeMs;
  if(timeDiff==0) return;

  core::vector3df Pos = camera->getPosition();
  core::vector3df Target = camera->getTarget();

  core::vector3df tPos = targetCam->getPosition();
  core::vector3df tTarget = targetCam->getTarget();
  core::vector3df t=targetCam->getPosition();
  t=camera->getPosition();

  core::vector3df posDiff=calculateDelta(Pos,tPos,timeDiff);
  core::vector3df tarDiff=calculateDelta(Target,tTarget,timeDiff);
  t=posDiff;

  f32 posDiffLength=posDiff.getLength();
  f32 tarDiffLength=tarDiff.getLength();

  if(tarDiffLength>0 || posDiffLength>0) //Are we there yet?
    { //Nope, update position/target

      camera->setPosition(Pos + posDiff);
      camera->setTarget(Target + tarDiff);
    }
  else
    { //Yup, set the target camera
      //We need to make sure the active cammera was not switched in the meantime...
      if(smgr->getActiveCamera()==movingCam)
	{
	  targetCam->setInputReceiverEnabled(enabledReceiver);
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
  //Copy receiver states
  enabledReceiver=smgr->getActiveCamera()->isInputReceiverEnabled();
  smgr->getActiveCamera()->setInputReceiverEnabled(false);


  if(!movingCam)
    {
      movingCam=smgr->addCameraSceneNode(0,smgr->getActiveCamera()->getPosition(),smgr->getActiveCamera()->getTarget());
      movingCam->addAnimator(this);
    }
  else
    {
      core::vector3df t=smgr->getActiveCamera()->getPosition();
      movingCam->setPosition(smgr->getActiveCamera()->getPosition());
      movingCam->setTarget(smgr->getActiveCamera()->getTarget());
    }

  movingCam->setInputReceiverEnabled(enabledReceiver);

  targetCam=target;
  core::vector3df t=target->getPosition();
  movingCam->setAspectRatio( 0.8/0.6 ); //This is the ratio of the normal cameras?


  //Let's get this show on the road!
  smgr->setActiveCamera(movingCam);
}

core::vector3df CSceneNodeAnimatorCameraSwitch::calculateDelta(core::vector3df start,core::vector3df end,f32 timeDiff)
{
  core::vector3df delta(end-start);
  core::vector3df t=delta;

  f32 deltaLength=delta.getLength();

  deltaLength*=timeDiff*0.001;

  if(deltaLength>200) deltaLength=200;
  else if(deltaLength<0.5) deltaLength=0;
  else if(deltaLength<1) deltaLength=1;

  delta.setLength(deltaLength);

  return delta;
}
