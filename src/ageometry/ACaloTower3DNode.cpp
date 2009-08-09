#include "ACaloTower3DNode.h"
#include <QDebug>

ACaloTower3DNode::ACaloTower3DNode ( scene::ISceneNode* parent, ISceneManager* smgr,  s32 id , AShower *shower)
  : AEventObject3DNode ( parent, smgr, id, shower )
{
  this->setName ( "ACaloTower3DNode" );

  Material.Lighting = false;

  const float Pi = 3.14159265358979f;
  const float RadDeg = 180.0f/Pi;
  
  const float width=0.5;
  //Logarithmic scale for the height, based on et
  //The minumum height is 1.1, which starts at Et of 1 GeV
  //We check that shower->et()/0.3>3, because is it is less than (approximation of natural number ) 3, then log() of it will be negative
  const float height=(shower->et()/0.3>3) ? 20*logf(shower->et()/0.3) : 1.1;

  float detectorRadius;
  if(shower->calorimeter()=="LAr")
    {
      vividColor=video::SColor(255,255,0,0);
      detectorRadius=50;
    }
  else if(shower->calorimeter()=="TILE")
    {
      vividColor=video::SColor(255,0,255,0);
      detectorRadius=80;
    }
  color=vividColor;
  
  // move to the side. x/y position is dictated by phi
  // Z position is dictated by the detector radius and theta
  setPosition(vector3df(detectorRadius*sin(shower->phi()),
			detectorRadius*cos(shower->phi()),
			detectorRadius/tan(shower->theta())));
  setRotation(vector3df(0,0,-shower->phi()*RadDeg)); //Rotate to point in correct phi

  float dz=height/tan(shower->theta());

  // What does the normal do?
  /* Bottom vertices */
  Vertices[0] = video::S3DVertex(-width,0,-width,
				 0,-1,0,
				 color, 0, 0);
  Vertices[1] = video::S3DVertex(width,0,-width,
				 0,-1,0,
				 color, 0, 0);
  Vertices[2] = video::S3DVertex(width,0,width,
				 0,-1,0,
				 color, 0, 0);
  Vertices[3] = video::S3DVertex(-width,0,width,
				 0,-1,0,
				 color, 0, 0);

  /* Top vertices */
  Vertices[4] = video::S3DVertex(-width,height,-width+dz,
				 0,-1,0,
				 color, 0, 0);
  Vertices[5] = video::S3DVertex(width,height,-width+dz,
				 0,-1,0,
				 color, 0, 0);
  Vertices[6] = video::S3DVertex(width,height,width+dz,
				 0,-1,0,
				 color, 0, 0);
  Vertices[7] = video::S3DVertex(-width,height,width+dz,
				 0,-1,0,
				 color, 0, 0);

  /* Create bounding box */
  Box.reset(Vertices[0].Pos);
  for (s32 i=1; i<8; ++i)
    Box.addInternalPoint(Vertices[i].Pos);
}


ACaloTower3DNode::~ACaloTower3DNode()
{ }

void ACaloTower3DNode::setTrack ( AShower* shower )
{
  AEventObject3DNode::setTrack(shower);
}

void ACaloTower3DNode::setStyle ( AEventObject3DNode::Style style )
{
  AEventObject3DNode::setStyle(style);
  return;
  switch(style)
    {
    default:
    case Basic:
      //unselected style
      setVisible(true);
      color=vividColor;
      emit lookChanged();
      break;

    case Selected:
      //selected style
      setVisible(true);
      color = video::SColor ( 0,255,255,0 );
      emit lookChanged();
      break;
    }
}

void ACaloTower3DNode::OnRegisterSceneNode()
{
  if ( IsVisible )
    SceneManager->registerNodeForRendering ( this );
  
  ISceneNode::OnRegisterSceneNode();
}

void ACaloTower3DNode::render()
{
  u16 indices[] = { 0,1,3, 1,2,3, //Bottom
		    4,7,5, 5,7,6, //Top
		    0,4,1, 1,4,5,
		    0,3,7, 0,7,4,
		    1,5,2, 2,5,6,
		    3,2,7, 2,6,7
  };

  // Set the color of the box, in case it changed
  for(int i=0;i<8;i++)
      Vertices[i].Color=color;

  video::IVideoDriver* driver = SceneManager->getVideoDriver();
  
  driver->setMaterial(Material);
  driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
  driver->drawIndexedTriangleList(&Vertices[0], 8, &indices[0], 12);
}

const core::aabbox3d<f32>& ACaloTower3DNode::getBoundingBox() const
{
  return Box;
}

video::SMaterial& ACaloTower3DNode::getMaterial ( s32 i )
{
  return Material;
}
