/**************** License agreement ******************

AMELIA Copyright (c) 2008, The Regents of the University of California,
through Lawrence Berkeley National Laboratory.  All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
U.S. Dept. of Energy nor the names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches, or upgrades to the features,
functionality or performance of the source code ("Enhancements") to anyone; however, if you choose to
make your Enhancements available either publicly, or directly to Lawrence Berkeley National Laboratory,
without imposing a separate written license agreement for such Enhancements, then you hereby grant the
following license: a  non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software, distribute,
and sublicense such enhancements or derivative works thereof, in binary and source code form.
******************************************************/

#include "AHelix3DNode.h"
#include <QDebug>

AHelix3DNode::AHelix3DNode ( scene::ISceneNode* parent, ISceneManager* smgr,  s32 id ,ATrack* track)
  : ATrack3DNode ( parent, smgr, id ,track),_blinkTimer(-1),_blinkCount(0)
{
  boxSizeAnim = new CRelativeScaleSceneNodeAnimator(smgr,1.5);
  this->setName ( "AHelix3DNode" );
}


AHelix3DNode::~AHelix3DNode()
{
  boxSizeAnim->drop();
}

void AHelix3DNode::setTrackStyle ( Style style )
{
  ATrack3DNode::setTrackStyle(style);

  switch(style)
    {
    default:
    case Basic:
      //simple line track, no boxes
      deleteBoxes();
      isLineVisible = true;
      color=vividColor;
      addAnimator(boxSizeAnim);
      emit lookChanged();
      break;
      
    case Selectable:
      //simple line visible in vivid color, boxes present for selection but invisible
      createBoxes();
      isLineVisible = true;
      removeAnimator(boxSizeAnim);
      setScale(vector3df(1,1,1));
      updateAbsolutePosition(); // Update the scale
      // Make sure to animate each box to the coorect size and make it visible
      // There is no need to make them invisible, since they should never be painted
      for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
	{
	  ( *it )->setVisible ( true );
	  boxSizeAnim->animateNode((*it),0);
	}
      break;
      
    case Selected:
      //simple line invisible, boxes visible with original color
      createBoxes();
      removeAnimator(boxSizeAnim);
      setScale(vector3df(1,1,1));
      updateAbsolutePosition();
      // Restore the color for the boxes. In case it was changed (ei: disabled)
      for ( vector<scene::ISceneNode*>::iterator it = boxSegments.begin() ; it < boxSegments.end(); it++ )
	{
	  ( *it )->setVisible ( true );
	  ( *it )->setDebugDataVisible ( EDS_OFF );
	  ( *it )->setMaterialType ( video::EMT_SOLID );
	  ( *it )->setMaterialTexture ( 0, SceneManager->getVideoDriver()->getTexture ( "" ) );
	  ( *it )->getMaterial(0).EmissiveColor=this->color;
	}
      if(!getTrack()->isInteresting())
	{ // Uninteresting tracks should have the line visible, and blink
	  isLineVisible = true;
	  if(_blinkTimer==-1)
	    { // If timer inactive, start it
	      _blinkCount=0;
	      _blinkTimer=startTimer(100);
	    }
	} 
      else
	{
	  isLineVisible = false;
	}
      emit lookChanged();
      break;
    case Disabled:
      //simple line invisible, boxes visible in grey
      createBoxes();
      isLineVisible = false;
      // Loop over the boxes and make them gray
      for ( vector<scene::ISceneNode*>::iterator it = boxSegments.begin() ; it < boxSegments.end(); it++ )
	{
	  ( *it )->setVisible ( true );
	  ( *it )->setDebugDataVisible ( EDS_OFF );
	  ( *it )->setMaterialType ( video::EMT_SOLID );
	  ( *it )->setMaterialTexture ( 0, SceneManager->getVideoDriver()->getTexture ( "" ) );
	  video::SMaterial* m = & ( *it )->getMaterial ( 0 );
	  m->EmissiveColor = video::SColor ( 0,122,122,122 );
	}
      emit lookChanged();
      break;
  
      //simple line invisible, boxes invisible if present
      /*if ( style == 5 )
	{
	isLineVisible = false;
	if ( boxMode )
	{
	for ( vector<scene::ISceneNode*>::iterator it = boxSegments.begin() ; it < boxSegments.end(); it++ )
	    {
	      ( *it )->setVisible ( false );
	    }
	}
      
	}*/
    }
}

void AHelix3DNode::select()
{
  setTrackStyle(Selected);
}

void AHelix3DNode::deselect()
{
  setTrackStyle(Basic);
}


void AHelix3DNode::drawPolyline()
{
  video::SMaterial m;
  for ( vector<core::vector3df>::iterator it = this->curvePoints.begin() ; it < this->curvePoints.end()-1; it++ )
    {
      m.EmissiveColor = this->color ;
      SceneManager->getVideoDriver()->setMaterial ( m );

      SceneManager->getVideoDriver()->setTransform ( video::ETS_WORLD, core::matrix4() );
      SceneManager->getVideoDriver()->draw3DLine ( *it, *(it+1) );
    }
}

void AHelix3DNode::createBoxes()
{
  if(boxSegments.size()>0) return; //Boxes already exist!

  scene::IAnimatedMesh* trackCube = SceneManager->getMesh ( "CubeUnit.X" );
  for ( vector<core::vector3df>::iterator it = this->curvePoints.begin() ; it < this->curvePoints.end()-1; it++ )
    {
      // Connect the box from point "initial" to point "final"
      core::vector3df initial=*it;
      core::vector3df final=*(it+1);

      core::vector3df vect = final - initial;
      core::vector3df pos = vect/2 + initial; // Center of the box should be in direction of vect from the initial point.
      core::vector3df rot = vect.getHorizontalAngle();
      core::vector3df scale = core::vector3df ( 0.2f,0.2f,vect.getLength() );
      
      // Create the actual node..
      scene::ISceneNode* nodeBox = 0;
      
      nodeBox = SceneManager->addMeshSceneNode ( trackCube );
      nodeBox->setPosition ( pos );
      nodeBox->setRotation ( rot );
      nodeBox->setScale ( scale );
      video::SMaterial* m = &nodeBox->getMaterial ( 0 );
      nodeBox->setMaterialType ( video::EMT_SOLID );
      nodeBox->setMaterialFlag ( video::EMF_GOURAUD_SHADING , false );
      nodeBox->setMaterialFlag ( video::EMF_LIGHTING , true );
      nodeBox->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
      nodeBox->setAutomaticCulling ( EAC_OFF );
      nodeBox->addAnimator(boxSizeAnim); // Animates the box side depending on camera distance
      m->EmissiveColor = this->color ; // Use current color
      m->DiffuseColor = video::SColor ( 0,0,0,0 );
      m->AmbientColor = video::SColor ( 0,0,0,0 );
      m->Shininess = 128 ;
      
      nodeBox->setParent ( this );
      nodeBox->getTransformedBoundingBox();
      nodeBox->setID ( 16 );
      nodeBox->setVisible ( false );
      this->boxSegments.push_back ( nodeBox );
    }
}

void AHelix3DNode::deleteBoxes()
{
  if(boxSegments.size()==0) return; // No boxes are created

  for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
    {
      (*it)->remove();
    }
  boxSegments.clear();
}

void AHelix3DNode::setBoxesVisibility ( bool boxVisibility )
{
  for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
    {
      ( *it )->setVisible ( boxVisibility );
    }
}

void AHelix3DNode::OnRegisterSceneNode()
{
  if ( IsVisible )
    SceneManager->registerNodeForRendering ( this );
  
  
  ISceneNode::OnRegisterSceneNode();
}

void AHelix3DNode::render()
{
  if ( this->isLineVisible )
    {
      drawPolyline();
    }
}


const core::aabbox3d<f32>& AHelix3DNode::getBoundingBox() const
{
  return Box;
}



video::SMaterial& AHelix3DNode::getMaterial ( s32 i )
{
  return Material;
}

void AHelix3DNode::timerEvent(QTimerEvent *event)
{
  // 1) Toggle the box visibility
  for ( vector<scene::ISceneNode*>::iterator it = boxSegments.begin() ; it < boxSegments.end(); it++ )
    {
      ( *it )->setVisible ( !( *it )->isVisible() );
    }
  
  // 2) Increment count
  _blinkCount++;

  // 3) Stop blinking if we blinked 10 times
  // This number should always be even, to ensure that we end with the track being off
  // The following the is value of _blinkCount followed by the visibility of the boxes, at this point in the code.
  // 1:OFF - 2:ON - 3:OFF - 4:ON - 5:OFF
  if(_blinkCount>10)
    {
      killTimer(_blinkTimer);
      _blinkTimer=-1;
      deselect(); // Manually make sure that this track is deselected
    }
  
  // 4) Let QIrrWidget know that our look has changed, so it should redraw the frame
  emit lookChanged();
}
