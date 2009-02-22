// Copyright (C) 2008 Karol Krizka
// This file was created for the AMELIA project
// For conditions of distribution and use, see copyright notice at http://amelia.sf.net

#include "CSceneNodeAnimatorCameraSphere.h"
#include <math.h>

namespace irr
{
  namespace scene
  {
    
    //! constructor
    CSceneNodeAnimatorCameraSphere::CSceneNodeAnimatorCameraSphere(f32 _moveSpeed,f32 _zoomSpeed,f32 _maxZoom):moveSpeed(_moveSpeed),zoomSpeed(_zoomSpeed),queueZoom(0.0f),toggleMouse(false),maxZoom(_maxZoom)
    {
#ifdef _DEBUG
      setDebugName("CSceneNodeAnimatorCameraSphere");
#endif
      
      for(int i=0;i<4;i++)
	{
	  KeyControl[i]=0;
	}
    }
    
    
    //! destructor
    CSceneNodeAnimatorCameraSphere::~CSceneNodeAnimatorCameraSphere()
    { }
    
    //! animates a scene node
    void CSceneNodeAnimatorCameraSphere::animateNode(ISceneNode* node, u32 timeMs)
    {
      if (node->getType() != ESNT_CAMERA)
	return;

      //There are times when there is no need to torture the CPU
      // with sines and cosines. So when there is no mouse input/output,
      // then just give up.
      bool needToDo=(cursorPos!=cursorCenter);
      needToDo|=(queueZoom!=0);
      for(int i=0;i<4;i++)
	{
	  needToDo|=(KeyControl[i]==1);
	}
      if(!needToDo) return;
      ICameraSceneNode* camera = static_cast<ICameraSceneNode*>(node);
      
      core::vector3df Pos = camera->getPosition();
      core::vector3df Target = camera->getTarget();
      core::vector3df Diff(Target - Pos);
      
      u32 timeDiff=timeMs-lastUpdate;
      
      f32 queuePhi=0;
      f32 queueTheta=0;
      
      if(cursorPos!=cursorCenter)
	{
	  s32 dX=cursorPos.X-cursorCenter.X;
	  s32 dY=cursorPos.Y-cursorCenter.Y;
	  
	  queuePhi+=dX*moveSpeed*10;
	  queueTheta+=dY*moveSpeed*10;

	  cursorCenter=cursorPos;
	}
      
      f32 dPhi=(KeyControl[3]-KeyControl[1])*timeDiff*moveSpeed+queuePhi;
      f32 dTheta=(KeyControl[0]-KeyControl[2])*timeDiff*moveSpeed+queueTheta;
      f32 r=Diff.getLength()+queueZoom;
      if(r<maxZoom) r=maxZoom;
      
      core::vector3df angles=Diff.getHorizontalAngle();
      f32 theta=angles.X;
      f32 phi=angles.Y;
      
      phi=phi+dPhi;
      theta=theta+dTheta;

      theta=theta*core::PI/180;
      phi=phi*core::PI/180;
      
      f32 Z=r*cos(phi)*cos(theta);
      f32 X=r*sin(phi)*cos(theta);
      f32 Y=-r*sin(theta);
      
      Diff=core::vector3df(X,Y,Z);
      
      lastUpdate=timeMs;

      camera->setPosition(Target - Diff);
      
      queueZoom=0;
      queueTheta=0;
      queuePhi=0;
    }
    
    bool CSceneNodeAnimatorCameraSphere::OnEvent(const SEvent& event)
    {
      if (event.EventType == EET_KEY_INPUT_EVENT)
	{
	  f32 PressedDown=(event.KeyInput.PressedDown)?1:0;
	  switch(event.KeyInput.Key)
	    {
	    case KEY_UP:
	      KeyControl[0]=PressedDown;
	      return true;
	    case KEY_RIGHT:
	      KeyControl[1]=PressedDown;
	      return true;
	    case KEY_DOWN:
	      KeyControl[2]=PressedDown;
	      return true;
	    case KEY_LEFT:
	      KeyControl[3]=PressedDown;
	      return true;
	    }
	}	
      
      if(event.EventType == EET_MOUSE_INPUT_EVENT)
	{
	  switch(event.MouseInput.Event)
	    {
	    case EMIE_MOUSE_WHEEL:
	      queueZoom-=event.MouseInput.Wheel*zoomSpeed;
	      return true;
	      break;
	    case EMIE_LMOUSE_PRESSED_DOWN:
	      cursorCenter=core::position2d<s32>(event.MouseInput.X,event.MouseInput.Y);
	      cursorPos=core::position2d<s32>(event.MouseInput.X,event.MouseInput.Y);
	      toggleMouse=true;
	      return true;
	      break;
	    case EMIE_LMOUSE_LEFT_UP:
	      toggleMouse=false;
	      return true;
	      break;
	    case EMIE_MOUSE_MOVED:
	      if(toggleMouse)
		cursorPos=core::position2d<s32>(event.MouseInput.X,event.MouseInput.Y);
	      return true;
	      break;
	    default:
	      return true;
	      break;
	    }
	}
      return false;
    }
    
  } // end namespace scene
} // end namespace irr

