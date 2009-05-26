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


#include "AGeometry.h"
#include <config.h>

#include "CSceneNodeAnimatorCameraOrbit.h"
#include "CSceneNodeAnimatorCameraSphere.h"
#include <ISceneNodeAnimatorCameraFPS.h>

#include "QIrrWidgetRefresher.h"

#include <QApplication>

#include <aeventmanager/ATrackCombination.h>

AGeometry::AGeometry(QWidget* parent)
        : QIrrWidget(parent), isCrappyComputer ( false ),  generateDetectorGeometry ( true ), generateCameraStats ( false ), displayFPS ( true ), offsetTest ( false ),
  background_node_f ( NULL ), background_node_s ( NULL ), frameSkipper ( 0 ), active_viewport ( AGeometry::Cam3D ) , active_cam (AGeometry::FPS), _event(0)

{
    setCursor(Qt::ArrowCursor);

    background_node_f = NULL;
    background_node_s = NULL;
    Pit_Reference = NULL;

    firstShow=true;

    // Dynamic FPS camera initial parameters
    cameraZone = 3;
    camChangeDist1 = 145;
    camChangeDist2 = 1000;
    BBscale = 35;
    CameraBB = NULL;

    // Control variables for the dynamic hiding of parts of ATLAS
    isTC_on = true;
    isLAr_on = true;
    isSCT_on = true;
    isPix_on = true;
    TC_switch = 1;
    LAr_switch = 1;
    SCT_switch = 1;
    Pix_switch = 1;

    // Cameras
    camera[0] = NULL;
    camera[1] = NULL;
    camera[2] = NULL;
    camera[3] = NULL;

    pos = core::vector3df ( 0,0,0 );
    rot = core::vector3df ( 0,0,0 );
    scale = core::vector3df ( 0,0,0 );

    moduleDistanceFromCam = 0;
    moduleAngleFromCam = 0;
    cameraLoc = core::vector3df ( 0,0,0 ); //camera position for Moses Mode, initialized to zero
    DCamPos = core::vector3df ( 0,0,0 );


    force_target = false;


    //Switches for the different modes
    detectorMode = false;
    eventAnalysisMode = true;
    multiMediaMode = false;

    allowTrackSelection = false;

    selectedTrackBox = NULL;

    rt=0;

    multiSelectButton=0;
    zoomIn=0;
    zoomOut=0;

    connect(this,SIGNAL(trackSelected(ATrack*)),
	    this,SLOT(makeDirty()));
    connect(this,SIGNAL(trackDeselected(ATrack*)),
	    this,SLOT(makeDirty()));

    // Prepare visility controls
    _detectorVisibility.setValue(true);
    visibilityMapper.setMapping(&_detectorVisibility,8);
    connect(&_detectorVisibility,SIGNAL(valueChanged(bool)),
	    &visibilityMapper,SLOT(map()));
    _trackerVisibility.setValue(true);
    visibilityMapper.setMapping(&_trackerVisibility,9);
    connect(&_trackerVisibility,SIGNAL(valueChanged(bool)),
	    &visibilityMapper,SLOT(map()));
    _calorimetersVisibility.setValue(true);
    visibilityMapper.setMapping(&_calorimetersVisibility,10);
    connect(&_calorimetersVisibility,SIGNAL(valueChanged(bool)),
	    &visibilityMapper,SLOT(map()));
    _pixelsVisibility.setValue(true);
    visibilityMapper.setMapping(&_pixelsVisibility,7);
    connect(&_pixelsVisibility,SIGNAL(valueChanged(bool)),
	    &visibilityMapper,SLOT(map()));
    _sctVisibility.setValue(true);
    visibilityMapper.setMapping(&_sctVisibility,6);
    connect(&_sctVisibility,SIGNAL(valueChanged(bool)),
	    &visibilityMapper,SLOT(map()));
    _trtVisibility.setValue(true);
    visibilityMapper.setMapping(&_trtVisibility,5);
    connect(&_trtVisibility,SIGNAL(valueChanged(bool)),
	    &visibilityMapper,SLOT(map()));
    _larVisibility.setValue(true);
    visibilityMapper.setMapping(&_larVisibility,4);
    connect(&_larVisibility,SIGNAL(valueChanged(bool)),
	    &visibilityMapper,SLOT(map()));
    _tileVisibility.setValue(true);
    visibilityMapper.setMapping(&_tileVisibility,3);
    connect(&_tileVisibility,SIGNAL(valueChanged(bool)),
	    &visibilityMapper,SLOT(map()));
    _muonSpecVisibility.setValue(true);
    visibilityMapper.setMapping(&_muonSpecVisibility,1);
    connect(&_muonSpecVisibility,SIGNAL(valueChanged(bool)),
	    &visibilityMapper,SLOT(map()));
    _magnetsVisibility.setValue(true);
    visibilityMapper.setMapping(&_magnetsVisibility,2);
    connect(&_magnetsVisibility,SIGNAL(valueChanged(bool)),
	    &visibilityMapper,SLOT(map()));
    _pitVisibility.setValue(true);
    visibilityMapper.setMapping(&_pitVisibility,8);
    connect(&_pitVisibility,SIGNAL(valueChanged(bool)),
	    &visibilityMapper,SLOT(map()));

    connect(&visibilityMapper,SIGNAL(mapped(int)),
	    this,SLOT(switchVisibility(int)));
}



AGeometry::~AGeometry()
{
    if (rt) rt->drop();}

void AGeometry::load()
{
  QTime time;
  time.start();
  
  //First load stuff originally loaded by ABase...
  //getFileSystem()->addFolderFileArchive ( getFileSystem()->getWorkingDirectory() );
  getFileSystem()->addFolderFileArchive ( "./media/" );
  getFileSystem()->addFolderFileArchive ( "./media/tours" );
  getFileSystem()->addFolderFileArchive ( "./media/events" );  getFileSystem()->addFolderFileArchive ( TOURS_PREFIX );
  getFileSystem()->addFolderFileArchive ( MEDIA_PREFIX );
  getFileSystem()->addFolderFileArchive ( EVENTS_PREFIX );
  
  //These first three lines are part of an offset test for the Irrlicht ray generator
  cube = getSceneManager()->addCubeSceneNode();
  cube->getMaterial ( 0 ).EmissiveColor.set ( 0,255,0,0 );
  cube->setScale ( core::vector3df ( 5,5,5 ) );
  cube->setPosition ( core::vector3df ( 400,1500,400 ) );
  cube->setVisible(offsetTest);
  
  tar_node = getSceneManager()->addEmptySceneNode();
  cam_node = getSceneManager()->addEmptySceneNode();
  OrthoCameraFront.buildProjectionMatrixOrthoLH ( 240.0f,180.0f,-400.0f,400.0f );
  OrthoCameraSide.buildProjectionMatrixOrthoLH ( 240.0f,180.0f,-400.0f,400.0f );
  getFileSystem()->addZipFileArchive ( "AtlasGeometry.aml" );
  
  //*****************CHANGED************************//
  
  //Base->Gui->statusmessage = Base->GetGuiEnv()->addStaticText ( L"", irr::core::rect<s32> ( 465, 650, 780, 750 ), false );
  //Base->Gui->statusmessage->setOverrideColor ( SColor ( 255,255,0,0 ) );
  //Base->Gui->trackinfo = Base->GetGuiEnv()->addStaticText ( L"", irr::core::rect<s32> ( 420, 670, 780, 770 ), false );
  //Base->Gui->trackinfo->setOverrideColor ( SColor ( 255,255,255,255 ) );
  
  
  /***************** PREPARE BB MODELS ********************/
  
  /*    for (vector<module>::iterator it = allModules.begin(); it!=allModules.end(); it++)
        {
	it->theModule->getAbsoluteTransformation().transformBox(it->theModule->getTransformedBoundingBox());
	it->theModule->setID(1);
	//it->theModule->setDebugDataVisible(true);
        }*/
  /********************************************************/
  

  cameraSwitcher=new CSceneNodeAnimatorCameraSwitch(getSceneManager());
  
  //Create the dynamic camera and define some variables
  
  camera[1] = getSceneManager()->addCameraSceneNode();
  camera[1]->setName("FrontCam");
  camera[1]->setInputReceiverEnabled ( false );
  camera[1]->setPosition ( core::vector3df ( 0,0,-1 ) );
  camera[1]->setTarget ( core::vector3df ( 0,0,0 ) );
  camera[1]->setProjectionMatrix ( OrthoCameraFront );
  camera[1]->setID(Projective);
  
  camera[2] = getSceneManager()->addCameraSceneNode();
  camera[2]->setName("SideCam");
  camera[2]->setInputReceiverEnabled ( false );
  camera[2]->setPosition ( core::vector3df ( 1,0,0 ) );
  camera[2]->setTarget ( core::vector3df ( 0,0,0 ) );
  camera[2]->setProjectionMatrix ( OrthoCameraSide );
  camera[2]->setID(Orthogonal);
  
  camera[3] = getSceneManager()->addCameraSceneNode();
  camera[3]->setName("SphereCam");
  camera[3]->setInputReceiverEnabled ( false );
  camera[3]->setFarValue ( 22000.0f );
  camera[3]->setPosition ( core::vector3df ( 250,0,0 ) );
  camera[3]->setTarget ( core::vector3df ( 0,0,0 ) );
  camera[3]->addAnimator(new scene::CSceneNodeAnimatorCameraSphere());
  camera[3]->setID(Maya);
  
  camera[0] = getSceneManager()->addCameraSceneNodeFPS ( 0, 40.0f, 100.0f );
  camera[0]->setName("FPSCam");
  camera[0]->setInputReceiverEnabled ( false );
  camera[0]->setPosition ( core::vector3df ( 1200,500,-1200 ) );
  camera[0]->setTarget ( core::vector3df ( 0,0,0 ) );
  camera[0]->setFarValue ( 22000.0f );
  camera[0]->setID(FPS);
  
  //Prepare spinning logo
  getFileSystem()->addZipFileArchive ( "logo.aml" );
  
  ICameraSceneNode *_logoCamera=getSceneManager()->addCameraSceneNode(0,
								      vector3df(1200,500,-1200),
								      vector3df(1200,500,-3000));
  _logoCamera->updateAbsolutePosition();
  
  getSceneManager()->loadScene("logo.irr");
  _logoNode=getSceneManager()->getSceneNodeFromName("LoadingNode");
  _logoNode->setPosition(core::vector3df(1200,425,-1400));
  _logoNode->setRotation(core::vector3df(-90,-180,0));
  
  
  _logoLight=getSceneManager()->addLightSceneNode(0,
								   core::vector3df(1200,500,-1300), 
								   video::SColorf(1.0f, 1.0f, 1.0f), 
								   200.0f);
  
  _logoAnim=getSceneManager()->createFlyCircleAnimator(core::vector3df(1200,500,-1200),
									   50.f,
									   0.004,
									   core::vector3df(1,1,-1)); 
  _logoLight->addAnimator(_logoAnim);
  forceUpdate();
  
  core::vector3df camRot = camera[0]->getRotation();
  core::vector3df DCamPos = core::vector3df ( 0,0,0 );
  
  //This is the camera bounding box, used to define the Moses mode area
  CameraBB = getSceneManager()->addCubeSceneNode ( 1.0f, 0, -1, camera[0]->getPosition() ,camera[0]->getRotation(), core::vector3df ( 55,55,55 ) );
  CameraBB->setID ( 0 );
  CameraBB->setName ("Moses Mode Box");
  CameraBB->setVisible(false);
  createFlatGeometry();
  
  zoomIn=getGUIEnvironment()->addButton(core::rect<s32>(width()-250,height()-40,width()-140,height()-20), 0, 100, L"Zoom In", L"Zoom in camera.");
  zoomIn->setVisible(false);
  zoomOut=getGUIEnvironment()->addButton(core::rect<s32>(width()-130,height()-40,width()-30,height()-20), 0, 100, L"Zoom Out", L"Zoom out camera.");
  zoomOut->setVisible(false);

  //Create the geometry
  createAtlasGeometry();
  
  forceUpdate(); //Make sure the timer is correct!
  
  qDebug() << "Loaded AGeometry (" << time.elapsed() << " ms)";
}

void AGeometry::executeMosesMode(core::vector3df camPos)
{
  if (!CameraBB) return;
  
  switch(_cropMode)
    {
    case MosesMode:
      BBscale = sqrt ( camPos.X*camPos.X + camPos.Y*camPos.Y + camPos.Z*camPos.Z ) *0.8 +25;
      CameraBB->setPosition ( camPos );
      break;
    case WedgeMode:
      BBscale = 15000;
      CameraBB->setPosition ( core::vector3df ( 0,0,0 ) );
      break;
    }

    CameraBB->setRotation ( getSceneManager()->getActiveCamera()->getRotation() );
    CameraBB->setScale ( core::vector3df ( BBscale,BBscale,BBscale ) );
    CameraBB->updateAbsolutePosition();
    
    core::aabbox3d<f32> BBBox=CameraBB->getTransformedBoundingBox();
    vector3df pos=BBBox.getCenter();
    
    for ( vector<scene::ISceneNode*>::iterator itb = allModules.begin(); ( itb ) !=allModules.end(); itb++ )
      {
	moduleAngleFromCam = angleBetween ( *itb, ( getSceneManager()->getActiveCamera()->getTarget() *0.5- getSceneManager()->getActiveCamera()->getPosition() ) );
	
	if ( ( BBBox.intersectsWithBox ( ( *itb )->getTransformedBoundingBox() ) ) && ( moduleAngleFromCam<1.0f ))
	  {
	    ( *itb )->setVisible ( false );
	  }
	else
	  {
	    ( *itb )->setVisible ( true );
	  }
      }
}

void AGeometry::restoreMosesMode()
{
  for ( vector<scene::ISceneNode*>::iterator itb = allModules.begin(); ( itb ) !=allModules.end(); itb++ )
    ( *itb )->setVisible ( true );
  CameraBB->setVisible ( false );
}

QString AGeometry::detectorSelection( core::position2di pos )
{
  ISceneCollisionManager* colmgr = getSceneManager()->getSceneCollisionManager();
  ISceneNode* selection=colmgr->getSceneNodeFromScreenCoordinatesBB(pos,8);
  
  // Make sure that the ID is 8, and not -1. Stupid bitmask...
  if(selection && selection->getID()==8)
    {
      QString partName(selection->getName());
      if(partName=="TC_Barrel" || 
	 partName=="TCB_Reference" || 
	 partName=="TC_Reference")
	return "TC_Reference";

      else if(partName=="EMC_Reference" ||
	      partName=="EMCB_Reference" ||
	      partName=="EMC_Barrel")
	return "EMC_Reference";

      else if(partName=="TRT_Reference" ||
	      partName=="TRTB_Reference" ||
	      partName=="TRT_Barrel")
	return "TRT_Reference";

      else if(partName=="Magnets_Reference" ||
	      partName=="Toroid_Magnet")
	return "Magnets_Reference";

      else if(partName=="SCT_Reference" ||
	      partName=="SCTB_Reference" ||
	      partName=="SCT_Barrel_L1" ||
	      partName=="SCT_Barrel_L2" ||
	      partName=="SCT_Barrel_L3" ||
	      partName=="SCT_Barrel_L4" ||
	      partName=="SCT_Disks")
	return "SCT_Reference";

      else if(partName=="Pixels_Reference" ||
	      partName=="PixelsB_Reference" ||
	      partName=="Pixels_Barrel_L1" ||
	      partName=="Pixels_Barrel_L2" ||
	      partName=="Pixels_Barrel_L3" ||
	      partName=="Pixels_Tube" ||
	      partName=="Pixels_Rings" ||
	      partName=="Pixels_Frame_Barrel" ||
	      partName=="Pixels_Frame_EC" ||
	      partName=="Pixels_Disks")
	return "Pixels_Reference";

      else if(partName=="Muons_Reference" ||
	      partName=="MuonSpectrometer")
	return "Muons_Reference";
    }

  return "";
}

ATrack3DNode* AGeometry::trackSelection ( core::position2di pos )
{
  if ( eventAnalysisMode )
    {
      ISceneCollisionManager* colmgr = getSceneManager()->getSceneCollisionManager();
      line3d<f32> ray = colmgr->getRayFromScreenCoordinates ( pos, getSceneManager()->getActiveCamera() );
      ISceneNode *selectedSceneNode = colmgr->getSceneNodeFromRayBB ( ray, 16 );
      qDebug() << "Selected " << selectedSceneNode;
      ITriangleSelector* selector;
      vector3df target;
      triangle3df triangle;
      
      ATrack3DNode *selectedNode=0;
      
      for ( int i=0; i<allTracks.size(); i++)
	{
	  if(!allTracks[i]->isVisible()) continue;
	  ATrack *track=allTracks[i]->getTrack();
	  
	  if ( selectedSceneNode && ( track->type() == ATrack::eSTrack || track->type() == ATrack::eMissingEt ) ) //tracks
	    {
	      if ( selectedSceneNode->getParent() == allTracks[i] )
		{
		  selectedNode = allTracks[i];
		  
		  break;
		}
	    }
	  
	  if ( track->type() == ATrack::eJet ) //jets
	    {
	      AJet3DNode* jet =  (AJet3DNode*)allTracks[i];
	      selector = jet->Pyramid->getTriangleSelector();
	      
	      if ( colmgr->getCollisionPoint ( ray, selector, target, triangle ) )
		{
		  selectedNode = jet;
		  break;
		}
	      
	    }
	  
	}

      return selectedNode;
    }
  return 0;
}


void AGeometry::renderViewport(int view)
{
    int camId;
    if (view==Cam3D) camId=active_cam;
    else camId=view;

    irr::video::SColor color (255,0,0,0);

    if (rt==0) //Try to create an render texture if one does not exist...
    {
        if (getVideoDriver()->queryFeature(video::EVDF_RENDER_TO_TARGET))
        {
            rt = getVideoDriver()->addRenderTargetTexture(core::dimension2d<s32>(256,256));
	    rt->grab();
        }

        if (rt==0)
        {
            emit viewportUpdated(view,QImage());
            return;
        }
    }

    //Render screenshot
    QImage image;


    //New View
    ICameraSceneNode *originalCamera=getSceneManager()->getActiveCamera();
    originalCamera->grab();
    getSceneManager()->setActiveCamera ( camera[ camId ] );
    setupView(view);

    getVideoDriver()->setRenderTarget(rt, true, true, color);
    getSceneManager()->drawAll();

    setupView(active_viewport);
    getVideoDriver()->setRenderTarget(0);
    getSceneManager()->setActiveCamera (originalCamera);
    originalCamera->drop();

    uchar* tmpdata=(uchar*)rt->lock (true);

    dimension2d<s32> size=rt->getSize();

    image=QImage(tmpdata,size.Width,size.Height,QIrrWidget::Irr2Qt_ColorFormat(rt->getColorFormat()));
    rt->unlock();

    emit viewportUpdated(view,image);
}

void AGeometry::dynamicCameraSpeed(core::vector3df camPos)  //Modifying camera speed based on proximity to detector...
{
    float camR = sqrt ( camPos.Y*camPos.Y + camPos.X*camPos.X );

    unsigned int newCameraZone=0;
    if ( camR < camChangeDist1)
    {
        newCameraZone=1;
    }
    else if ( camR < camChangeDist2 )
    {
        newCameraZone=2;
    }
    else
    {
        newCameraZone=3;
    }

    //Switching a new zone
    if (cameraZone!=newCameraZone)
    {
        //What should be a new zone
        f32 newCameraSpeed=0.0f;
        switch (newCameraZone)
        {
        case 1:
            newCameraSpeed=10.0f;
            break;
        case 2:
            newCameraSpeed=100.0f;
            break;
        case 3:
            newCameraSpeed=400.0f;
            break;
        }

        qDebug() << "Changing FPS camera speed to " << newCameraSpeed;
        //The FPS Animator should always be the first one...
        core::list<ISceneNodeAnimator*>::ConstIterator anims=camera[0]->getAnimators().begin();
        ISceneNodeAnimatorCameraFPS *anim=(ISceneNodeAnimatorCameraFPS*)*anims;
        //Make sure to scale the speed by 1000, which is what the FPS constructor does but not the setter
        anim->setMoveSpeed(newCameraSpeed/1000);
        cameraZone=newCameraZone;
    }

}

void AGeometry::dynamicHidingOfModules(core::vector3df camPos)
{

    float camR = sqrt ( camPos.Y*camPos.Y + camPos.X*camPos.X );

    //Part I, hide

    if ( isTC_on && camR > 1000 )
    {
        isTC_on = false;
        TC_switch = 1;
    }

    if ( isLAr_on && camR > 400 )
    {
        isLAr_on = false;
        LAr_switch = 1;
    }

    if ( isSCT_on && camR > 250 )
    {
        isSCT_on = false;
        SCT_switch = 1;
    }

    if ( isPix_on && camR > 75 )
    {
        isPix_on = false;
        Pix_switch = 1;
    }

    if ( ( isTC_on || isLAr_on || isSCT_on || isPix_on ) && ( camPos.Z < -400 || camPos.Z > 400 ) )
    {
        isPix_on = false;
        Pix_switch = 1;
        isSCT_on = false;
        SCT_switch = 1;
        isLAr_on = false;
        LAr_switch = 1;
        isTC_on = false;
        TC_switch = 1;
    }



    //Part II, reveal

    if ( !isTC_on && ((camR < 1000 && camPos.Z>= -400 && camPos.Z <= 400) ) )
    {
        isTC_on = true;
        TC_switch = 1;
    }

    if ( !isLAr_on && ((camR < 400 && camPos.Z>= -400 && camPos.Z <= 400) ) )
    {
        isLAr_on = true;
        LAr_switch = 1;
    }

    if ( !isSCT_on && ((camR < 250 && camPos.Z>= -400 && camPos.Z <= 400 ) ) )
    {
        isSCT_on = true;
        SCT_switch = 1;
    }

    if ( !isPix_on && ((camR < 75 && camPos.Z>= -400 && camPos.Z <= 400) ) )
    {
        isPix_on = true;
        Pix_switch = 1;
    }


    //Part III,  change states

    if ( TC_switch == 1 || LAr_switch == 1 || SCT_switch == 1 || Pix_switch == 1 )
    {

        if ( TC_switch == 1 && getSceneManager()->getSceneNodeFromName ( "TCB_Reference" ))
        {
            getSceneManager()->getSceneNodeFromName ( "TCB_Reference" )->setVisible ( isTC_on );
        }

        if ( LAr_switch == 1 && getSceneManager()->getSceneNodeFromName ( "EMCB_Reference" ) )
        {
            getSceneManager()->getSceneNodeFromName ( "EMCB_Reference" )->setVisible ( isLAr_on );
        }


        if ( SCT_switch == 1  && getSceneManager()->getSceneNodeFromName ( "SCTB_Reference" )  && getSceneManager()->getSceneNodeFromName ( "TRTB_Reference" ) )
        {
            getSceneManager()->getSceneNodeFromName ( "SCTB_Reference" )->setVisible ( isSCT_on );
            getSceneManager()->getSceneNodeFromName ( "TRTB_Reference" )->setVisible ( isSCT_on );
        }

        if ( Pix_switch == 1  && getSceneManager()->getSceneNodeFromName ( "PixelsB_Reference" ) )
        {
            getSceneManager()->getSceneNodeFromName ( "PixelsB_Reference" )->setVisible ( isPix_on );
        }

        TC_switch = 0;
        LAr_switch = 0;
        SCT_switch = 0;
        Pix_switch = 0;
    }


}

void AGeometry::execute()
{  
  if(firstShow)
    {
      printf("First Show\n");
      //Remove the loading nodes
      _logoNode->remove();
      _logoLight->remove();
      _logoAnim->drop();
      setCamera(AGeometry::Maya);
      firstShow=false;
    }


  // Main 3D view
  if(hasCameraMoved())
    {
      core::vector3df camPos = getSceneManager()->getActiveCamera()->getPosition();
      
      dynamicCameraSpeed(camPos);
      //dynamicHidingOfModules(camPos);
      
      if ( _cropMode!=NoneMode )
	{
	  executeMosesMode(camPos);
	}
           
	if (camera[0] && force_target)
	{
	  
	  //vector3df vect = tar_node->getPosition () - camera[0]->getPosition ();
	  //camera[0]->setRotation (vect.getHorizontalAngle ());
	  camera[0]->setTarget (tar_node->getPosition());
	  camera[0]->setPosition (cam_node->getPosition());
	}
      //qDebug() << "STUFF UPDATED";
    }
  
  ICameraSceneNode *active=getSceneManager()->getActiveCamera();
  if(zoomIn && zoomIn->isPressed())
    {
      vector3df curpos=active->getPosition();
      curpos*=0.95;
      active->setPosition(curpos);
    }
  if(zoomOut && zoomOut->isPressed())
    {
      vector3df curpos=active->getPosition();
      curpos*=1.05;
      active->setPosition(curpos);
    }
  
  QIrrWidget::execute();
}

float AGeometry::angleBetween ( scene::ISceneNode* module, core::vector3df cam )
{
    core::vector3df a,b;
    core::matrix4 c;
    if ( module->getType() == ESNT_CUBE )
    {
        a = core::vector3df ( 0,1,0 );
    }
    /*else if (mType==100)
    {
        a = core::vector3df(0,-1,-1);
    }*/
    else
    {
        a = core::vector3df ( 0,0,-1 );
    }
    c = module->getAbsoluteTransformation();
    c.rotateVect ( a );
    b = cam;
    //first return statement is identical to the commented second, just written out explicitly (and without the arccos)
    //return (((a.X*b.X)+(a.Y*b.Y)+(a.Z*b.Z)) / (sqrt(a.X*a.X+a.Y*a.Y+a.Z*a.Z) * sqrt(b.X*b.X+b.Y*b.Y+b.Z*b.Z)));
    return ( acos ( ( a.dotProduct ( b ) / ( b.getLength() * a.getLength() ) ) ) ); //returns radian angle between 2 vectors
}

void AGeometry::switchVisibility ( int modType )
{
  if ( generateDetectorGeometry )
    {
      switch ( modType )
	{
	case 0: //Pit
	  if ( Pit_Reference )
	    Pit_Reference->setVisible ( _pitVisibility.value()) ;
	  if ( getSceneManager()->getSceneNodeFromName ( "Shielding_JT" ) )
	    {
	      getSceneManager()->getSceneNodeFromName ( "Shielding_JT" )->setVisible ( _pitVisibility.value());
	    }
	  break;


	case 1: //Muon Chambers
	  if ( Muons_Reference )
	    Muons_Reference->setVisible ( _muonSpecVisibility.value());
	  break;

	case 2: // Magnets
	  if ( Magnets_Reference )
	    Magnets_Reference->setVisible ( _magnetsVisibility.value() );
	  break;

	case 3: // Hadronic Calorimeter
	  if ( TC_Reference )
	    TC_Reference->setVisible ( _tileVisibility.value());
	  break;

	case 4: // EM Calorimeter
	  if ( EMC_Reference )
	    EMC_Reference->setVisible ( _larVisibility.value() );
	  break;

	case 5: // TRT
	  if ( TRT_Reference )
	    TRT_Reference->setVisible ( _trtVisibility.value() );
	  break;

	case 6: // SCT
	  if ( SCT_Reference )
	    SCT_Reference->setVisible ( _sctVisibility.value() );
	  break;

	case 7: // Pixels
	  if ( Pixels_Reference )
	    Pixels_Reference->setVisible ( _pixelsVisibility.value() );
	  break;

	case 8: // Detector
	  if(Atlas_Reference)
	    Atlas_Reference->setVisible(_detectorVisibility.value());
	  break;
	case 9: // Tracker
	  if(Trackers_Reference)
	    Trackers_Reference->setVisible(_trackerVisibility.value());
	  break;
	case 10: // Calorimeters
	  if(Calorimeters_Reference)
	    Calorimeters_Reference->setVisible(_calorimetersVisibility.value());
	  break;
        }
      makeDirty();
    }
}

QBoolSync *AGeometry::detectorVisibility()
{
  return &_detectorVisibility;
}

QBoolSync *AGeometry::calorimetersVisibility()
{
  return &_calorimetersVisibility;
}

QBoolSync *AGeometry::trackerVisibility()
{
  return &_trackerVisibility;
}

QBoolSync *AGeometry::pixelsVisibility()
{
  return &_pixelsVisibility;
}

QBoolSync *AGeometry::sctVisibility()
{
  return &_sctVisibility;
}

QBoolSync *AGeometry::trtVisibility()
{
  return &_trtVisibility;
}

QBoolSync *AGeometry::larVisibility()
{
  return &_larVisibility;
}

QBoolSync *AGeometry::tileVisibility()
{
  return &_tileVisibility;
}

QBoolSync *AGeometry::muonSpecVisibility()
{
  return &_muonSpecVisibility;
}

QBoolSync *AGeometry::magnetsVisibility()
{
  return &_magnetsVisibility;
}

QBoolSync *AGeometry::pitVisibility()
{
  return &_pitVisibility;
}

void AGeometry::createFlatGeometry()
{
	float mscale = 0.4; //model scale

    ///* Background images for the viewports

    //Irr->GetDevice()->getFileSystem()->addZipFileArchive("media/Background_Front.pk3");
    scene::IAnimatedMesh* bgf = getSceneManager()->getMesh ( "Background_Front.X" );

    background_node_f = getSceneManager()->addAnimatedMeshSceneNode ( bgf, 0, -1, core::vector3df ( 0,0,-240 ), core::vector3df ( 0,90,0 ) );
    background_node_f->getMaterial ( 0 ).DiffuseColor.set ( 255,222,222,222 );
    background_node_f->getMaterial ( 0 ).EmissiveColor.set ( 0,111,111,111 );
    background_node_f->setScale ( core::vector3df ( mscale,mscale,mscale ) );
    background_node_f->setMaterialFlag ( video::EMF_LIGHTING, false );
    background_node_f->setMaterialType ( video::EMT_LIGHTMAP_ADD );
    background_node_f->setID ( 5 );
    background_node_f->setName("Background_Front.X");
    background_node_f->setVisible ( false );


    scene::IAnimatedMesh* bgs = getSceneManager()->getMesh ( "Background_Side.X" );

    background_node_s = getSceneManager()->addAnimatedMeshSceneNode ( bgs, 0, -1, core::vector3df ( 400,0,0 ), core::vector3df ( 0,90,90 ) );
    background_node_s->getMaterial ( 0 ).DiffuseColor.set ( 255,222,222,222 );
    background_node_s->getMaterial ( 0 ).EmissiveColor.set ( 0,111,111,111 );
    background_node_s->setScale ( core::vector3df ( mscale,mscale,mscale ) );
    background_node_s->setMaterialFlag ( video::EMF_LIGHTING, false );
    background_node_s->setMaterialType ( video::EMT_LIGHTMAP_ADD );
    background_node_s->setID ( 5 );
    background_node_s->setName("Background_Side.X");
    background_node_s->setVisible ( false );

    renderViewport(AGeometry::Orthogonal);
    renderViewport(AGeometry::Projective);
}

void AGeometry::createAtlasGeometry()
{
  QIrrWidgetRefresher ref(this);

  if ( generateDetectorGeometry )
    {

      ref.allModules=&this->allModules;
      getSceneManager()->loadScene ( "geometry.irr" , &ref );
      Atlas_Reference=getSceneManager()->getSceneNodeFromName( "Atlas_Reference" );
      Trackers_Reference=getSceneManager()->getSceneNodeFromName( "Trackers_Reference" );
      Calorimeters_Reference=getSceneManager()->getSceneNodeFromName( "Calorimeters_Reference" );

      Muons_Reference=getSceneManager()->getSceneNodeFromName( "Muons_Reference" );
      Magnets_Reference=getSceneManager()->getSceneNodeFromName( "Magnets_Reference" );
      TC_Reference=getSceneManager()->getSceneNodeFromName( "TC_Reference" );
      EMC_Reference=getSceneManager()->getSceneNodeFromName( "EMC_Reference" );
      TRT_Reference=getSceneManager()->getSceneNodeFromName( "TRT_Reference" );
      SCT_Reference=getSceneManager()->getSceneNodeFromName( "SCT_Reference" );
      Pixels_Reference=getSceneManager()->getSceneNodeFromName( "Pixels_Reference" );

      Atlas_Reference->setVisible(true);

      ref.allModules=0;
      if ( !isCrappyComputer )
	  {
            getSceneManager()->loadScene ( "ATLAS_Pit.lvl" , &ref );
            Pit_Reference=getSceneManager()->getSceneNodeFromName( "Pit_Reference" );
	  }
    }
}

ATrack* AGeometry::selectTrackByID (int ID, bool multi)
{
  if (!multi) //If we are not doing multiselection, the deselect anything selected
    {
      while (!selectedTracks.isEmpty())
        {
	  ATrack3DNode *node=selectedTracks.back();
	  node->deselect();
	  selectedTracks.pop_back();
	  emit trackDeselected(node->getTrack());
        }
    }
  
  //Loop through all the tracks...
  for ( int i=0;i<allTracks.size();i++)
    {
      ATrack* selectedTrack = allTracks[i]->getTrack();
      if ( selectedTrack->trackID() == ID ) //Found it
	{
	  allTracks[i]->select();
	  selectedTracks.push_back(allTracks[i]);
	  emit trackSelected(selectedTrack);
	  return selectedTrack;
	}
    }
  return NULL;
}

ATrack* AGeometry::deselectTrackByID (int ID)
{
  ATrack* tr;
  for (int i=0;i<selectedTracks.size();i++)
    {
      tr=selectedTracks[i]->getTrack();
      if (tr->trackID() == ID)
        {
	  selectedTracks[i]->deselect();
	  emit trackDeselected(tr);
	  selectedTracks.removeAt(i);
	  return tr;
        }
    }
  return NULL;
}

void AGeometry::clearTrackSelection()
{
  ATrack* tr;
  while(selectedTracks.size()>0)
    {
      tr=selectedTracks[0]->getTrack();
      deselectTrackByID(tr->trackID());
    }
}

void AGeometry::grabControl()
{
  if ( active_viewport == AGeometry::Cam3D && active_cam==AGeometry::FPS)
    {
      getSceneManager()->getActiveCamera()->setInputReceiverEnabled ( true );
      setCursor( QCursor( Qt::BlankCursor ) );
      allowTrackSelection=false;
      setFocus();
      emit cameraControlSwitched(true);
    }
    
}

void AGeometry::releaseControl()
{
  if ( active_viewport == AGeometry::Cam3D && active_cam==AGeometry::FPS)
    {
      getSceneManager()->getActiveCamera()->setInputReceiverEnabled ( false );
      setCursor( QCursor( Qt::ArrowCursor ) );
      emit cameraControlSwitched(false);
      //allowTrackSelection=(XmlEvt->EventComplete.Tracks.size()>0); //Only allow track selection if event loaded, which is when number of tracks >0
    }
}

void AGeometry::resizeEvent( QResizeEvent* event )
{
  if(rt) rt->drop();
  rt=0;

  QIrrWidget::resizeEvent(event);
}

void AGeometry::mouseClickEvent(QMouseEvent *event)
{
  if ( active_viewport == AGeometry::Cam3D )
    {
      core::position2di posMouse = core::position2di(event->x(),event->y());
      bool Shift =((QApplication::keyboardModifiers() & (Qt::ShiftModifier | Qt::ControlModifier)) > 0);
      
      if(event->button()==Qt::LeftButton)
	{
	  //If you clicked an FPS camera and it does not have input, give it input! This allows us to focus the camera by clicking on AGeometry
	  if (!getSceneManager()->getActiveCamera()->isInputReceiverEnabled() && !allowTrackSelection && !offsetTest)
	    {
	      grabControl();
	    }
	  if (event->button()==Qt::LeftButton)
	    {
	      if(allowTrackSelection)
		{ //Start track selection
		  ATrack3DNode *selected=trackSelection(posMouse);
	      
	      //If shifty/ctrly no clicky, then we do not have a multi-track selection and so we deselect everything, but the clicked ray
	      if (!Shift)
		    { // Deselect every selected track
		  while (!selectedTracks.isEmpty())
		    {
		      ATrack3DNode *node=selectedTracks.back();
		      node->deselect();
		      selectedTracks.pop_back();
		      emit trackDeselected(node->getTrack());
		    }
		    }
		  
	      if (selected)
		{
		  int idx=selectedTracks.indexOf(selected);
		  if (idx==-1) //Make sure the track is not already selected
		    {
		      selected->select();
		      selectedTracks.push_back(selected);
		      emit trackSelected(selected->getTrack());
		      qDebug() << "Found and selected a track...";
			}
		      else //else deselect it
			{
			  selectedTracks.removeAt(idx);
			  selected->deselect();
			  emit trackDeselected(selected->getTrack());
			  qDebug() << "Found and delselected a track...";
			}
		      return;
		    }
		} //End track selection
	      
	      if(!Shift) // When we press shift, then for sure we only want track selection
	      { //Start detector selection
	      QString partName=detectorSelection(posMouse);
	      if(partName!="")
		{
		  emit detectorPartSelected(partName);
		  return;
		}
	      } //End detector selection
	    }
	}
      
      //This next block is part of an offset test for the Irrlicht ray generator
      if ( offsetTest )
	{
	  
	  line3d<f32> ray = getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(posMouse, getSceneManager()->getActiveCamera());
	  cube->setPosition ( ray.start + ray.getVector().normalize() * 180.0 ); // put it some distance away
	  cube->updateAbsolutePosition();
	  /*                cout << "cube X: " <<  cube->getPosition().X << "  cube Y: " <<  cube->getPosition().Y <<endl;
			    cout << "pos2D X: " <<  posMouse.X << "  pos2D Y: " <<  posMouse.Y <<endl;
			    cout << "RayStart X: " <<  ray.start.X << "  RayStart Y: " <<  ray.start.Y <<endl;
			    cout << "RayEnd X: " <<  ray.end.X << "  RayEnd Y: " <<  ray.end.Y <<endl;
			    cout << ""  <<endl;
			    cout << ""  <<endl;
			    cout << ""  <<endl;
			    cout << "Width: " <<  width() << "  Heigth: " <<  height() <<endl;*/
	}

    }
  
  emit emptySelection();

  QIrrWidget::mouseClickEvent(event);
}


void AGeometry::keyPressEvent ( QKeyEvent* event )
{
  switch ( event->key() )
    {
      //toggles moses mode
    case Qt::Key_P:
      camera[0]->setPosition ( core::vector3df ( 0,0,-1200 ) );
      camera[0]->setTarget ( core::vector3df ( 0,0,0 ) );
      return;
      break;
      
    case Qt::Key_G:
      //selectTrackByID ( 11 );
      return;
      break;
      
      //toggles camera/cursor control
    case Qt::Key_Space:
      if (getSceneManager()->getActiveCamera()->isInputReceiverEnabled()) releaseControl();
      else grabControl();
      return;
      break;
      
      //sets PtCutoff to 1GeV
    case Qt::Key_C:
      //XmlEvt->PtCutoff ( 1 );
      return;
      break;
      
    // Toggle detector system visibility
    case Qt::Key_0:
      _detectorVisibility.setValue(!_detectorVisibility.value());
      return;
      break;
      
    case Qt::Key_F1:
      qDebug() << "Active Camera " << active_viewport;
      qDebug() << "\tPosition: (" << getSceneManager()->getActiveCamera()->getPosition().X << ","
	       << getSceneManager()->getActiveCamera()->getPosition().Y << ","
	       << getSceneManager()->getActiveCamera()->getPosition().Z << ")";
      qDebug() << "\tTarget: (" << getSceneManager()->getActiveCamera()->getTarget().X << ","
	       << getSceneManager()->getActiveCamera()->getTarget().Y << ","
	       << getSceneManager()->getActiveCamera()->getTarget().Z << ")";
      return;
      break;
    case Qt::Key_F2:
      offsetTest=!offsetTest;
      qDebug() << "offsetTest =" << offsetTest;
      cube->setVisible(offsetTest);
      return;
    case Qt::Key_F4:
      if ( getSceneManager()->getSceneNodeFromName ( "Atlas_Reference" ) )
	getSceneManager()->getSceneNodeFromName ( "Atlas_Reference" )->setVisible ( !getSceneManager()->getSceneNodeFromName ( "Atlas_Reference" )->isVisible() );
      return;
    case Qt::Key_F5:
      setEnabled(!isEnabled());
      return;
    default:
      //Don't do anything
      break;
    }
  
  return QIrrWidget::keyPressEvent(event);
}

/*void setRecursiveMaterialFlag(ISceneNode *node_,video::E_MATERIAL_FLAG flag,bool status)
{
  printf("Set %p %s \n",node_,node_->getName());
  node_->setMaterialFlag(flag,status);
  
  // recursive call to children
  const core::list<scene::ISceneNode*> & children = node_->getChildren();
  core::list<scene::ISceneNode*>::ConstIterator it = children.begin();
  for ( ; it != children.end(); ++it )
    {
      setRecursiveMaterialFlag( *it ,flag,status);
    }
    }*/

void AGeometry::contextMenuEvent( QContextMenuEvent *event )
{
  QMenu menu;

  //If a combination menu exists, add it to a list
  if(_comboMenu)
    {
      //Prepare the combination object holding all the selected tracks
      ATrackCombination *combination=new ATrackCombination();
      for(int i=0;i<selectedTracks.size();i++)
	combination->addTrack(selectedTracks[i]->getTrack());
      
      //For each item in the combo menu, set it's data to the combined tracks
      QList<QAction *> actions=_comboMenu->findChildren<QAction*>();
      if(actions.size()>0)
	{
	  for(int i=0;i<actions.size();i++)
	    {
	      QVariant data=QVariant::fromValue<QObject*>(combination);
	      actions[i]->setData(data);
	    }
	}

      //Add to the menu
      QAction *comboMenuAction=menu.addMenu(_comboMenu);

      //Enable only if 2 or more tracks are selected
      if(combination->size()<2)
	comboMenuAction->setEnabled(false);
      else
      	comboMenuAction->setEnabled(true);
    }

  /* If a detector part is selected, add it to the menu */
  QPoint qpos=event->pos();

  core::position2di pos(qpos.x(),qpos.y());
  QString partSelected=detectorSelection(pos);
  if(partSelected!="")
    {
      // Add menu entry
      if(_detectorMenu.contains(partSelected))
	{
	  menu.addMenu(_detectorMenu[partSelected]);
	}
      else
	{
	  QMenu *partMenu=menu.addMenu(partSelected);
	  partMenu->setEnabled(false);
	}

      // Highlight
      //TODO Highlight somehow...
      //ISceneNode *partNode=getSceneManager()->getSceneNodeFromName(partSelected.toAscii().data());
    }

  //Display!!
  menu.exec(event->globalPos());
}

void AGeometry::clearEvent()
{
  _event=0;

  clearTrackSelection();
  ISceneManager* mngr=getSceneManager();
  for(int i=0;i<allTracks.size();i++)
    {
      mngr->addToDeletionQueue(allTracks[i]);
    }
  allTracks.clear();
  allJets.clear();
}

void AGeometry::setEvent(AFilteredEvent* e)
{
  makeDirty();
  if(!_event)
    {
      disconnect(_event,SIGNAL(filtersUpdated()),
		 this,SLOT(updateTracks()));
      disconnect(_event,SIGNAL(filtersUpdated()),
		 this,SLOT(makeDirty()));

      AEvent *completeE=e->completeEvent();
      for(int i=0;i<completeE->Tracks.size();i++)
	{
	  ATrack3DNode *node=0;
	  ATrack* track=completeE->Tracks[i];
	  if(track->type()==ATrack::eJet)
	    {
	      AJet* jet=(AJet*)track;
	      node=new AJet3DNode(getSceneManager()->getRootSceneNode(),getSceneManager(),0,jet);
	      allJets.push_back((AJet3DNode*)node);
	    }
	  
	  if(track->type()==ATrack::eSTrack)
	    {
	      ASTrack* str=(ASTrack*)track;
	      node=new ASTrack3DNode(getSceneManager()->getRootSceneNode(),getSceneManager(),0,str);
	    }
	  
	  if(track->type()==ATrack::eMissingEt)
	    {
	      AMisET* miset=(AMisET*)track;
	      node=new AMisET3DNode(getSceneManager()->getRootSceneNode(),getSceneManager(),0,miset);
	    }
	  
	  if(node)
	    {
	      node->setVisible(false);
	      allTracks.push_back(node);
	    }
	}
    }

  _event=e;
  
  connect(_event,SIGNAL(filtersUpdated()),
	  this,SLOT(updateTracks()));
  connect(_event,SIGNAL(filtersUpdated()),
	  this,SLOT(makeDirty()));
  updateTracks();

  allowTrackSelection=true;
}

AFilteredEvent* AGeometry::event()
{
  return _event;
}

//Switch the current camera, viewport clicked
void AGeometry::setViewport(int to)
{
    if (to==active_viewport) return; //Already using it
    
    int camId;
    if (to==AGeometry::Cam3D) camId=active_cam;
    else camId=to;

    int from=active_viewport;

    getSceneManager()->setActiveCamera (camera[camId]);

    setupView(to);

    emit viewportSwitched(from,to);
    active_viewport=to;
    renderViewport(from);

    qDebug() << "Active viewport switched from " << from << " to " << to;
}

void AGeometry::setCamera(int to,bool animate)
{
    if (to==active_cam) return; //Already using it

    switch (to)
    {
    case AGeometry::FPS:
      actSphere->setChecked(false);
      actFPS->setChecked(true);
      setCropMode(MosesMode);
      break;
    case AGeometry::Maya:
      actFPS->setChecked(false);
      actSphere->setChecked(true);
      setCropMode(WedgeMode);
      camera[AGeometry::Maya]->setInputReceiverEnabled(true); //Maya always wants the input receiver enabled
      break;
    }
    camera[AGeometry::FPS]->setInputReceiverEnabled(false);
    setCursor(Qt::ArrowCursor);

    if (active_viewport==AGeometry::Cam3D)
      if(animate)
        cameraSwitcher->setTargetCamera(camera[to]);
      else
	getSceneManager()->setActiveCamera(camera[to]);

    active_cam=to;
    renderViewport(AGeometry::Cam3D);

    zoomIn->setVisible(to==AGeometry::Maya);
    zoomOut->setVisible(to==AGeometry::Maya);
}

void AGeometry::setCropMode(int newMode)
{
  if(newMode==_cropMode) return;
  
  _cropMode=newMode;
  if(_cropMode==NoneMode)
    restoreMosesMode();
  else
    executeMosesMode(getSceneManager()->getActiveCamera()->getPosition());
  makeDirty();
  
  emit cropModeSwitched(newMode);
}

void AGeometry::setupView(int view)
{
    bool fps,f,s;
    switch (view)
    {
    case AGeometry::Cam3D:
        f=false;
        s=false;
        fps=true;
        break;
    case AGeometry::Orthogonal:
        f=true;
        s=false;
        fps=false;
        break;
    case AGeometry::Projective:
        f=false;
        s=true;
        fps=false;
        break;
    }

    background_node_f->setVisible (f);
    background_node_s->setVisible (s);

    if ( getSceneManager()->getSceneNodeFromName ( "Atlas_Reference" ) )
      getSceneManager()->getSceneNodeFromName ( "Atlas_Reference" )->setVisible ( fps && _detectorVisibility.value() );

    if ( Pit_Reference )
      Pit_Reference->setVisible ( fps && _pitVisibility.value() );

	if(view==Cam3D)
	{
		if(active_cam==Maya) 
		{
			if(zoomIn) zoomIn->setVisible(true);
			if(zoomOut) zoomOut->setVisible(true);
		}
		if(multiSelectButton) multiSelectButton->setVisible(true);
	}
	else
	{
		if(multiSelectButton) multiSelectButton->setVisible(false);
		if(zoomIn) zoomIn->setVisible(false);
		if(zoomOut) zoomOut->setVisible(false);
	}
}

void AGeometry::addCamAnimator (irr::core::array<vector3df> p)
{

    ISceneNodeAnimator *anim = getSceneManager()->createFollowSplineAnimator (0, p, 1);

    //camera[0]->addAnimator (anim);
    cam_node->addAnimator (anim);
    anim->drop ();

    force_target = true;

//    cout << "ANIMATOR" << endl;

}



void AGeometry::addTarAnimator (irr::core::array<vector3df> p)
{

    ISceneNodeAnimator *anim = getSceneManager()->createFollowSplineAnimator (0, p, 1);

    tar_node->addAnimator (anim);
    anim->drop ();

    force_target = true;

}


void AGeometry::removeCamAnimator ()
{

    cam_node->removeAnimators ();

    force_target = false;

}


void AGeometry::removeTarAnimator ()
{

    tar_node->removeAnimators ();

    force_target = false;
}

void AGeometry::setComboMenu(QMenu *comboMenu)
{
  _comboMenu=comboMenu;
}

void AGeometry::addToDetectorMenu(QString partName,QAction *action)
{
  if(!_detectorMenu.contains(partName))
    {
      QMenu *newMenu=new QMenu(partName);
      _detectorMenu[partName]=newMenu;
    }

  _detectorMenu[partName]->addAction(action);
}

void AGeometry::updateTracks()
{
  for(int i=0;i<allTracks.size();i++)
    {
      if(_event->Tracks.contains(allTracks[i]->getTrack()))
	allTracks[i]->setVisible(true);
      else
	allTracks[i]->setVisible(false);
    }

  renderViewport(AGeometry::Orthogonal);
  renderViewport(AGeometry::Projective);
  renderViewport(AGeometry::Cam3D);
}
