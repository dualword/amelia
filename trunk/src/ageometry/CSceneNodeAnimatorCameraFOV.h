// Copyright (C) 2008 Karol Krizka
// This file is part of the AMELIA Porject.
// For conditions of distribution and use, see copyright notice at http://amelia.sf.net

#ifndef SCENE_NODE_ANIMATOR_CAMERAFOV_H_
#define SCENE_NODE_ANIMATOR_CAMERAFOV_H_

#include "irrlicht.h"

namespace irr
{
  namespace scene
  {
    class CSceneNodeAnimatorCameraFOV : public ISceneNodeAnimator
    {
    public:

      //! constructor
      CSceneNodeAnimatorCameraFOV(f32 fovSpeed=0.000001f);

      //! destructor
      virtual ~CSceneNodeAnimatorCameraFOV();

      //! animates a scene node
      virtual void animateNode(ISceneNode* node, u32 timeMs);

      //! recieves events
      virtual bool OnEvent(const SEvent& event);

      //! Returns type of the scene node animator
      virtual ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_UNKNOWN; }

      //! This animator will receive events when attached to the active camera
      virtual bool isEventReceiverEnabled() const { return true; }

      //! We need to define this method because it's a pure virtual function on ISceneNodeAnimator
      virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0) { return 0; };

      void setMaxFOV(f32);
      f32 maxFOV();
      void setMinFOV(f32);
      f32 minFOV();

    private:
      f32 fovSpeed;
      f32 queueFOV;

      f32 _maxFOV;
      f32 _minFOV;
      
      u32 KeyControl[2];
      u32 lastUpdate;
    };


} // end namespace scene
} // end namespace irr

#endif

