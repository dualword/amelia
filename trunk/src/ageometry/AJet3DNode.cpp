#include "AJet3DNode.h"
#include <QDebug>

AJet3DNode::AJet3DNode ( scene::ISceneNode* parent, ISceneManager* smgr,  s32 id , AJet *jet)
  : AEventObject3DNode ( parent, smgr, id, jet )
{
  this->setName ( "AJet3DNode" );
  createJetPyramids();
}


AJet3DNode::~AJet3DNode()
{ }

void AJet3DNode::setTrack ( AJet* track )
{
  AEventObject3DNode::setTrack(track);
}

void AJet3DNode::setStyle ( AEventObject3DNode::Style style )
{
  AEventObject3DNode::setStyle(style);

  video::SMaterial* m = &Pyramid->getMaterial ( 0 );
  switch(style)
    {
    default:
    case Basic:
      //unselected jet style
      Pyramid->setVisible(true);
      m->EmissiveColor = video::SColor ( 0,100,100,100 );
      //Pyramid->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR ); //For em frac
      emit lookChanged();
      break;

    case Selected:
      //selected jet style
      Pyramid->setVisible(true);
      m->EmissiveColor = video::SColor ( 0,255,0,0 );
      //Pyramid->setMaterialType ( video::EMT_SOLID ); //For em frac
      emit lookChanged();
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

void AJet3DNode::createJetPyramids()
{
  AJet *jet=(AJet*)track();

  float pi = 3.1415926f;
  float c = 180/pi;
  float eta = jet->eta();
  float theta = 2*atan( exp ( -eta ) );
  float et = jet->et();
  float e = fabs(et*jet->getTl());
  
  core::vector3df zero = core::vector3df ( 0,0,0 );
  //core::vector3df scale = core::vector3df ( 0.5,0.5,1+0.06*log(e));
  core::vector3df scale = core::vector3df ( 0.5,0.5,0.02*e);
  //core::vector3df scale = core::vector3df ( 0.5,0.5,1.);
  core::vector3df rot = core::vector3df ( -theta * c, 0, -jet->phi() * c ); //
  
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
  setTriangleSelector ( selector );
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

  // Set EM fraction
  /*ITexture *bg = SceneManager->getVideoDriver()->getTexture("jetemfrac.png");
  m->setTexture(0,bg);
  matrix4 mtx;
  mtx.buildTextureTransform(0,vector2df(0,0),vector2df(0,0.25),vector2df(1,1-jet->emFraction()));
  m->setTextureMatrix(0,mtx);*/
  
  nodeBox->getTransformedBoundingBox();
  nodeBox->setParent ( this );
  //nodeBox->setDebugDataVisible(EDS_BBOX);
  //nodeBox->setVisible(false);
  Pyramid = nodeBox;
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
