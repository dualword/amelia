#include "AMisET3DNode.h"

AMisET3DNode::AMisET3DNode ( scene::ISceneNode* parent, ISceneManager* smgr,  s32 id , AMisET* miset)
  : ATrack3DNode ( parent, smgr, id ,miset)
{
  boxSizeAnim = new CRelativeScaleSceneNodeAnimator(smgr);
  this->setName ( "AMisET3DNode" );

  createMisEtBoxes();

  setTrackStyle(Basic);
}


AMisET3DNode::~AMisET3DNode()
{
  boxSizeAnim->drop();
}

int AMisET3DNode::getTrackNumber()
{
  return this->trackNumber;
}

void AMisET3DNode::setTrack ( AMisET* track )
{
  ATrack3DNode::setTrack(track);
}

void AMisET3DNode::setTrackStyle ( Style style )
{
  ATrack3DNode::setTrackStyle(style);
  switch(style)
    {
    case Selected:
      //selected Missing Et style
      for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
	{
	  
	  ( *it )->setVisible ( true );
	  ( *it )->setDebugDataVisible ( EDS_OFF );
	  ( *it )->setMaterialType ( video::EMT_SOLID );
	  ( *it )->setMaterialTexture ( 0, SceneManager->getVideoDriver()->getTexture ( "" ) );
	  video::SMaterial* m = & ( *it )->getMaterial ( 0 );
	  m->EmissiveColor = video::SColor ( 0,255,255,122 );
	}
      break;
      
    case Basic:
    default:
      //unselected Missing Et style
      for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
	{
	  video::SMaterial* m = & ( *it )->getMaterial ( 0 );
	  m->EmissiveColor = video::SColor ( 0,0,0,0 );
	}
      break;
      
      //invisible Missing Et style
      /*if ( style == 12 )
        {
	for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
            {
	    ( *it )->setVisible ( false );
            }
	    }
	    
	    //restore the Missing Et visibility
	    if ( style == 13 )
	    {
	    for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
            {
	    ( *it )->setVisible ( true );
	    }
	    }*/
    }
}

void AMisET3DNode::select()
{
  setTrackStyle(Selected);
}

void AMisET3DNode::deselect()
{
  setTrackStyle(Basic);
}


void AMisET3DNode::createMisEtBoxes() //for Missing Et
{
  core::vector3df zero = core::vector3df ( 0,0,0 );
  end = core::vector3df ( ((AMisET*)getTrack())->etx, ((AMisET*)getTrack())->ety,0 );
  
  core::vector3df rot = end.getHorizontalAngle();
  core::vector3df scale = core::vector3df ( 5,5, end.getLength() );
  
  scene::IAnimatedMesh* trackCube = SceneManager->getMesh ( "CubeUnit.X" );
  scene::ISceneNode* nodeBox = 0;
  nodeBox = SceneManager->addMeshSceneNode ( trackCube );
  nodeBox->setPosition ( end/2 );
  nodeBox->setRotation ( rot );
  nodeBox->setScale ( scale );
  
  video::SMaterial* m = &nodeBox->getMaterial ( 0 );
  nodeBox->setMaterialType ( video::EMT_SOLID );
  nodeBox->setMaterialFlag ( video::EMF_GOURAUD_SHADING , false );
  nodeBox->setMaterialFlag ( video::EMF_LIGHTING , true );
  nodeBox->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
  nodeBox->setAutomaticCulling ( EAC_OFF );
  nodeBox->addAnimator(boxSizeAnim);
  color = video::SColor ( 0,0,0,0 );
  m->EmissiveColor = this->color ;
  m->DiffuseColor = video::SColor ( 0,0,0,0 );
  m->AmbientColor = video::SColor ( 0,0,0,0 );
  m->Shininess = 128 ;
  
  nodeBox->getTransformedBoundingBox();
  nodeBox->setParent ( this );
  //nodeBox->setDebugDataVisible(true);
  nodeBox->setID ( 16 );
  nodeBox->setVisible ( true );
  this->boxSegments.push_back ( nodeBox );
}


void AMisET3DNode::constructNeutral()
{
  video::SMaterial m;
  m.EmissiveColor = this->color ;
  SceneManager->getVideoDriver()->setMaterial ( m );
  
  SceneManager->getVideoDriver()->setTransform ( video::ETS_WORLD, core::matrix4() );
  SceneManager->getVideoDriver()->draw3DLine ( this->start, this->end ,this->color);
}


void AMisET3DNode::setBoxesVisibility ( bool boxVisibility )
{
  for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
    {
      ( *it )->setVisible ( boxVisibility );
    }
}

void AMisET3DNode::setBoxesSelected ( bool boxesSelected )
{
  for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
    {
      video::SMaterial* m = & ( *it )->getMaterial ( 0 );
      
      boxesSelected ? m->EmissiveColor = video::SColor ( 0,122,122,122 ) : m->EmissiveColor = this->color ;
    }
}




void AMisET3DNode::OnRegisterSceneNode()
{
  if ( IsVisible )
    SceneManager->registerNodeForRendering ( this );
  
  
  ISceneNode::OnRegisterSceneNode();
}


const core::aabbox3d<f32>& AMisET3DNode::getBoundingBox() const
{
  return Box;
}



video::SMaterial& AMisET3DNode::getMaterial ( s32 i )
{
  return Material;
}
