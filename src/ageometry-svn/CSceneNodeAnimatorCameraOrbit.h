// Copyright (C) 2008 Karol Krizka
// This file is part of the AMELIA Porject.
// For conditions of distribution and use, see copyright notice at http://amelia.sf.net

#ifndef SCENE_NODE_ANIMATOR_CAMERAORBIT_H_
#define SCENE_NODE_ANIMATOR_CAMERAORBIT_H_

#include <irrlicht.h>

namespace irr
{
  namespace scene
  {
    class CSceneNodeAnimatorCameraOrbit : public ISceneNodeAnimator
    {
    public:
      
      //! constructor
      CSceneNodeAnimatorCameraOrbit(core::vector3df normal,f32 moveSpeed=0.0005,f32 zoomSpeed=0.1);
      
      //! destructor
      virtual ~CSceneNodeAnimatorCameraOrbit();
      
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
      core::vector3df normal;

      f32 KeyControl[4];
      u32 lastUpdate;
    };


} // end namespace scene
} // end namespace irr

#endif

