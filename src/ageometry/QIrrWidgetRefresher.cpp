#include "QIrrWidgetRefresher.h"

#include <QApplication>

QIrrWidgetRefresher::QIrrWidgetRefresher(QIrrWidget *p)
  : allModules(0),p(p)
{
  time.start();

  /**
     ESNT_CUBE    simple cube scene node 
     ESNT_SPHERE    Sphere scene node. 
     ESNT_TEXT    Text Scene Node. 
     ESNT_WATER_SURFACE    Water Surface Scene Node. 
     ESNT_TERRAIN    Terrain Scene Node. 
     ESNT_SKY_BOX    Sky Box Scene Node. 
     ESNT_SHADOW_VOLUME    Shadow Volume Scene Node. 
     ESNT_OCT_TREE    OctTree Scene Node. 
     ESNT_MESH    Mesh Scene Node. 
     ESNT_LIGHT    Light Scene Node. 
     ESNT_EMPTY    Empty Scene Node. 
     ESNT_DUMMY_TRANSFORMATION    Dummy Transformation Scene Node. 
     ESNT_CAMERA    Camera Scene Node. 
     ESNT_BILLBOARD    Billboard Scene Node. 
     ESNT_ANIMATED_MESH    Animated Mesh Scene Node. 
     ESNT_PARTICLE_SYSTEM    Particle System Scene Node. 
     ESNT_MD3_SCENE_NODE    Quake3 Model Scene Node ( has tag to link to ). 
     ESNT_CAMERA_MAYA    Maya Camera Scene Node Legacy, for loading version <= 1.4.x .irr files 
     ESNT_CAMERA_FPS    First Person Shooter Camera Legacy, for loading version <= 1.4.x .irr files 
     ESNT_UNKNOWN    Unknown scene node. 
     ESNT_ANY    Will match with any scene node when checking types.
  **/

  //printf("ESNT_CUBE %d\n",ESNT_CUBE);
  //printf("ESNT_MESH %d\n",ESNT_MESH);
  //printf("ESNT_LIGHT %d\n",ESNT_LIGHT);
  //printf("ESNT_EMPTY %d\n",ESNT_EMPTY);
  //printf("ESNT_UNKNOWN %d\n",ESNT_UNKNOWN);
  //printf("ESNT_ANY %d\n",ESNT_ANY);
}
  
io::IAttributes* QIrrWidgetRefresher::createUserData(ISceneNode *forSceneNode)
{ return 0; }

void QIrrWidgetRefresher::OnCreateNode(ISceneNode *node)
{
  if(allModules!=0)
  {
    if ( ( node->getType() != ESNT_EMPTY ) &&
	 ( node->getType() != ESNT_LIGHT ) &&
	 ( node->getType() != ESNT_UNKNOWN ) )
      if ( ( node->getID() != 4 ) && ( node->getID() !=7 ) )
	{
	  node->setID ( 0x0008 );
	  allModules->push_back ( node );
	}
  }
  if( time.elapsed()>40)
    {
      QApplication::processEvents();
      p->forceUpdate();
      time.restart();
    }
}

void QIrrWidgetRefresher::OnReadUserData(ISceneNode *forSceneNode, io::IAttributes *userData)
{ }
