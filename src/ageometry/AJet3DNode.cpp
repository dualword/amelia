#include "AJet3DNode.h"

AJet3DNode::AJet3DNode ( scene::ISceneNode* parent, ISceneManager* smgr,  s32 id , AJet *jet)
  : ATrack3DNode ( parent, smgr, id, jet )
{
  this->setName ( "AJet3DNode" );
  createJetPyramids();
}


AJet3DNode::~AJet3DNode()
{ }

int AJet3DNode::getTrackNumber()
{
  return this->trackNumber;
}

void AJet3DNode::setTrack ( AJet* track )
{
  ATrack3DNode::setTrack(track);
}

void AJet3DNode::setTrackStyle ( ATrack3DNode::Style style )
{
  ATrack3DNode::setTrackStyle(style);

  video::SMaterial* m = &Pyramid->getMaterial ( 0 );
  switch(style)
    {
    default:
    case Basic:
      //unselected jet style
      Pyramid->setVisible(true);
      m->EmissiveColor = video::SColor ( 0,100,100,100 );
      break;

    case Selected:
      //selected jet style
      Pyramid->setVisible(true);
      m->EmissiveColor = video::SColor ( 0,255,0,0 );
      break;
    }
      //invisible jet style
  /*if ( style == 8 )
	{
	  Pyramid->setVisible(false);
	}
      
      //restore the jet visibility
      if ( style == 9 )
	{
	  Pyramid->setVisible ( true );
	  }*/
}

void AJet3DNode::select()
{
  setTrackStyle(Selected);
}

void AJet3DNode::deselect()
{
  setTrackStyle(Basic);
}

void AJet3DNode::createJetPyramids()
{
  float pi = 3.1415926;
  float c = 180/pi;
  float theta = 2*atan ( exp ( -(((AJet*)getTrack())->eta )) );
  core::vector3df zero = core::vector3df ( 0,0,0 );
  core::vector3df scale = core::vector3df ( 0.5,0.5,1 );
  //core::vector3df rot = core::vector3df(phi * c - 90, 90, theta * c);
  core::vector3df rot = core::vector3df ( -theta * c, 0, -((AJet*)getTrack())->phi * c ); //
  
  scene::IAnimatedMesh* pyramid = SceneManager->getMesh ( "jet.X" );
  scene::ISceneNode* nodeBox = 0;
  nodeBox = SceneManager->addMeshSceneNode ( pyramid );
  nodeBox->setPosition ( zero );
  nodeBox->setRotation ( rot );
  nodeBox->updateAbsolutePosition();
  nodeBox->setScale ( scale );
  nodeBox->getTransformedBoundingBox();
  nodeBox->setID ( 18 );
  
  scene::ITriangleSelector* selector = 0;
  selector = SceneManager->createOctTreeTriangleSelector ( pyramid, nodeBox, 128 );
  nodeBox->setTriangleSelector ( selector );
  selector->drop();
  
  video::SMaterial* m = &nodeBox->getMaterial ( 0 );
  nodeBox->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
  nodeBox->setMaterialFlag ( video::EMF_GOURAUD_SHADING , false );
  nodeBox->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
  nodeBox->setMaterialFlag ( video::EMF_LIGHTING , true );
  nodeBox->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
  nodeBox->setAutomaticCulling ( EAC_OFF );
  m->EmissiveColor = video::SColor ( 0,100,100,100 );
  m->DiffuseColor = video::SColor ( 0,0,0,0 );
  m->AmbientColor = video::SColor ( 0,0,0,0 );
  m->Shininess = 0 ;
  
  nodeBox->getTransformedBoundingBox();
  nodeBox->setParent ( this );
  //nodeBox->setDebugDataVisible(EDS_BBOX);
  //nodeBox->setVisible(false);
  Pyramid = nodeBox;
}


void AJet3DNode::setBoxesVisibility ( bool boxVisibility )
{
  Pyramid->setVisible ( boxVisibility );
}

void AJet3DNode::setBoxesSelected ( bool boxesSelected )
{
  video::SMaterial* m = &Pyramid->getMaterial ( 0 );
  boxesSelected ? m->EmissiveColor = video::SColor ( 0,122,122,122 ) : m->EmissiveColor = this->color ;
}


void AJet3DNode::OnRegisterSceneNode()
{
  if ( IsVisible )
    SceneManager->registerNodeForRendering ( this );
  
  
  ISceneNode::OnRegisterSceneNode();
}


const core::aabbox3d<f32>& AJet3DNode::getBoundingBox() const
{
  return Pyramid->getBoundingBox();
}



video::SMaterial& AJet3DNode::getMaterial ( s32 i )
{
  return Material;
}


float AJet3DNode::getTl()
{
  float tL = 0.5 * ( exp (((AJet*)getTrack())->eta) - exp (-(((AJet*)getTrack())->eta)));
  return tL;
}

