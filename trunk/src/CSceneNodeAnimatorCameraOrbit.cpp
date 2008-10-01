// Copyright (C) 2008 Karol Krizka
// This file was created for the AMELIA project
// For conditions of distribution and use, see copyright notice at http://amelia.sf.net

#include "CSceneNodeAnimatorCameraOrbit.h"
#include <math.h>

namespace irr
{
  namespace scene
  {
    
    //! constructor
    CSceneNodeAnimatorCameraOrbit::CSceneNodeAnimatorCameraOrbit(core::vector3df _normal,f32 _moveSpeed,f32 _zoomSpeed):moveSpeed(_moveSpeed),zoomSpeed(_zoomSpeed),normal(_normal)
    {
#ifdef _DEBUG
      setDebugName("CSceneNodeAnimatorCameraOrbit");
#endif

      for(int i=0;i<4;i++)
	{
	  KeyControl[i]=0;
	}
    }
    
    
    //! destructor
    CSceneNodeAnimatorCameraOrbit::~CSceneNodeAnimatorCameraOrbit()
    {
    }
    
    
    
    //! animates a scene node
    void CSceneNodeAnimatorCameraOrbit::animateNode(ISceneNode* node, u32 timeMs)
    {
      if (node->getType() != ESNT_CAMERA)
		return;
      
      ICameraSceneNode* camera = static_cast<ICameraSceneNode*>(node);
      
      core::vector3df Pos = camera->getPosition();
      core::vector3df Target = camera->getTarget();
      core::vector3df Diff(Target - Pos);

      f32 timeDiff=timeMs-lastUpdate;
	   lastUpdate=timeMs;

      f32 dMove=(KeyControl[1]-KeyControl[3])*timeDiff*moveSpeed*Diff.getLength();
      s32 dRadius=(KeyControl[2]-KeyControl[0])*timeDiff*zoomSpeed;
      
      if(dMove==0 && dRadius==0) return;

      f32 radius=Diff.getLength()+dRadius;
      if(radius<0.2) radius=0.2;
      
      core::vector3df dDiff;
      
      dDiff=Diff.crossProduct(normal);
      dDiff.setLength(dMove);
      Diff=Diff+dDiff;
      Diff.setLength(radius);
	  
      
      camera->setPosition(Target - Diff);
      if(dMove>0) camera->setUpVector(dDiff);
    }
    
    bool CSceneNodeAnimatorCameraOrbit::OnEvent(const SEvent& event)
    {
      if (event.EventType == EET_KEY_INPUT_EVENT)
	{
	  f32 PressedDown=(event.KeyInput.PressedDown)?1:0;
	  switch(event.KeyInput.Key)
	    {
	    case KEY_RIGHT:
	      KeyControl[0]=PressedDown;
	      return true;
	      break;
	    case KEY_DOWN:
	      KeyControl[1]=PressedDown;
	      return true;
	      break;
	    case KEY_LEFT:
	      KeyControl[2]=PressedDown;
	      return true;
	      break;
	    case KEY_UP:
	      KeyControl[3]=PressedDown;
	      return true;
	      break;
	    }  
	}
	switch(event.MouseInput.Event)
	  {
	  case EMIE_MOUSE_WHEEL:
	    //queueZoom-=event.MouseInput.Wheel*zoomSpeed;
	    return true;
	    break;
        }
	return false;
    }
    
  } // end namespace scene
} // end namespace irr

