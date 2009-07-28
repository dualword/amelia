// Copyright (C) 2008 Karol Krizka
// This file was created for the AMELIA project
// For conditions of distribution and use, see copyright notice at http://amelia.sf.net

#include "CSceneNodeAnimatorCameraFOV.h"
#include <math.h>

namespace irr
{
  namespace scene
  {
    
    //! constructor
    CSceneNodeAnimatorCameraFOV::CSceneNodeAnimatorCameraFOV(f32 _fovSpeed)
      : fovSpeed(_fovSpeed),queueFOV(0),_maxFOV(1.3f),_minFOV(0.001f)
    {
#ifdef _DEBUG
      setDebugName("CSceneNodeAnimatorCameraFOV");
#endif
      
      for(int i=0;i<2;i++)
	{
	  KeyControl[i]=0;
	}
    }
    
    
    //! destructor
    CSceneNodeAnimatorCameraFOV::~CSceneNodeAnimatorCameraFOV()
    { }
    
    //! animates a scene node
    void CSceneNodeAnimatorCameraFOV::animateNode(ISceneNode* node, u32 timeMs)
    {
      if (node->getType() != ESNT_CAMERA)
	return;

      ICameraSceneNode *camera=(ICameraSceneNode*)node;

      u32 timeDiff=timeMs-lastUpdate;
      lastUpdate=timeMs;

      if(KeyControl[0])
	queueFOV+=fovSpeed*timeDiff;
      else if(KeyControl[1])
	queueFOV-=fovSpeed*timeDiff;
      
      if(fabs(queueFOV)<0.0001) return;

      f32 fov=camera->getFOV();
      fov+=queueFOV;

      if(fov<_minFOV) fov=_minFOV;
      if(fov>_maxFOV) fov=_maxFOV;
      
      camera->setFOV(fov);

      queueFOV=0;
    }
    
    bool CSceneNodeAnimatorCameraFOV::OnEvent(const SEvent& event)
    {
      if (event.EventType == EET_KEY_INPUT_EVENT)
	{
	  u32 PressedDown=(event.KeyInput.PressedDown)?1:0;
	  switch(event.KeyInput.Key)
	    {
	    case KEY_DOWN:
	    case KEY_MINUS:
	      KeyControl[0]=PressedDown;
	      return true;
	    case KEY_PLUS:
	    case KEY_UP:
	      KeyControl[1]=PressedDown;
	      return true;
	    default:
	      break;
	    }
	}	
      
      if(event.EventType == EET_MOUSE_INPUT_EVENT)
	{
	  switch(event.MouseInput.Event)
	    {
	    case EMIE_MOUSE_WHEEL:
	      queueFOV-=event.MouseInput.Wheel*fovSpeed;
	      return true;
	      break;
	    default:
	      break;
	    }
	}
      return false;
    }

    void CSceneNodeAnimatorCameraFOV::setMaxFOV(f32 maxFOV)
    {
      _maxFOV=maxFOV;
    }
    
    f32 CSceneNodeAnimatorCameraFOV::maxFOV()
    {
      return _maxFOV;
    }
    
    void CSceneNodeAnimatorCameraFOV::setMinFOV(f32 minFOV)
    {
      _minFOV=minFOV;
    }    

    f32 CSceneNodeAnimatorCameraFOV::minFOV()
    {
      return _minFOV;
    }
    
  } // end namespace scene
} // end namespace irr

