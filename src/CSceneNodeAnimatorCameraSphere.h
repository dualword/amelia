// Copyright (C) 2008 Karol Krizka
// This file is part of the AMELIA Porject.
// For conditions of distribution and use, see copyright notice at http://amelia.sf.net

#ifndef SCENE_NODE_ANIMATOR_CAMERASPHERE_H_
#define SCENE_NODE_ANIMATOR_CAMERASPHERE_H_

#include "irrlicht.h"

namespace irr
{
  namespace scene
  {
    class CSceneNodeAnimatorCameraSphere : public ISceneNodeAnimator
    {
    public:
      
      //! constructor
      CSceneNodeAnimatorCameraSphere(gui::ICursorControl* _cursorControl,f32 moveSpeed=0.01,f32 zoomSpeed=10,f32 maxZoom=1);
      
      //! destructor
      virtual ~CSceneNodeAnimatorCameraSphere();
      
      //! animates a scene node
      virtual void animateNode(ISceneNode* node, u32 timeMs);
      
      //! recieves events
      virtual bool OnEvent(const SEvent& event);
      
      //! Returns type of the scene node animator
      virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_UNKNOWN; }
      
      //! This animator will receive events when attached to the active camera
      virtual bool isEventReceiverEnabled() const { return true; }
      
    private:
      f32 zoomSpeed;
      f32 moveSpeed;
      f32 queueZoom;
	  f32 maxZoom;

	  bool toggleMouse;
	  core::position2df cursorPos;
	  core::position2df cursorCenter;

	  gui::ICursorControl* cursorControl;
      f32 KeyControl[4];
      u32 lastUpdate;
    };


} // end namespace scene
} // end namespace irr

#endif

