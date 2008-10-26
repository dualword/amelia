// Copyright (C) 2008 Karol Krizka
// This file is part of the AMELIA Project.
// For conditions of distribution and use, see copyright notice at http://amelia.sf.net

#ifndef SCENE_NODE_ANIMATOR_CAMERASWITCH_H_
#define SCENE_NODE_ANIMATOR_CAMERASWITCH_H_

#include "irrlicht.h"
using namespace irr;
using namespace scene;

class CSceneNodeAnimatorCameraSwitch : public scene::ISceneNodeAnimator
{
 public:

  //! constructor
  CSceneNodeAnimatorCameraSwitch(scene::ISceneManager*);

  //! destructor
  virtual ~CSceneNodeAnimatorCameraSwitch();

  //! animates a scene node
  virtual void animateNode(scene::ISceneNode* node, u32 timeMs);

  //! recieves events
  virtual bool OnEvent(const SEvent& event);

  //! Returns type of the scene node animator
  virtual scene::ESCENE_NODE_ANIMATOR_TYPE getType() const { return scene::ESNAT_UNKNOWN; }

  //! This animator will receive events when attached to the active camera
  virtual bool isEventReceiverEnabled() const { return true; }

  void setTargetCamera(scene::ICameraSceneNode *target);

  //! We need to define this method because it's a pure virtual function on ISceneNodeAnimator
  virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0) { return 0;};

 private:
  scene::ISceneManager *smgr;
  scene::ICameraSceneNode *targetCam;

  scene::ICameraSceneNode *movingCam;

  u32 lastUpdate;
  bool enabledReceiver;

  core::vector3df calculateDelta(core::vector3df start,core::vector3df end,f32 timeDiff);
};

#endif

