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

#include "CSceneNodeAnimatorCameraFOV.h"
#include "CSceneNodeAnimatorCameraSphere.h"
#include <ISceneNodeAnimatorCameraFPS.h>

#include "QIrrWidgetRefresher.h"

#include "ASTrack3DNode.h"
#include "ARTrack3DNode.h"
#include "AJet3DNode.h"
#include "AMisET3DNode.h"
#include "ACaloTower3DNode.h"

#include <QApplication>

#include <aeventmanager/ATrackCombination.h>

AGeometry::AGeometry(QWidget* parent)
        : QIrrWidget(parent),
        isCrappyComputer(false),generateDetectorGeometry(true),generateCameraStats(false),
        allowTrackSelection(false),firstShow(true),offsetTest(false),
        _event(0),
        detector3d_node( NULL ), background_node_f ( NULL ), background_node_s ( NULL ),
        modelScale(0.4f),
        active_viewport ( AGeometry::Cam3D ) , active_cam (AGeometry::Lock),
        fpsControl(0),sphereControl(0),sideControl(0),frontControl(0)
{
    setCursor(Qt::ArrowCursor);

    Pit_Reference = NULL;

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
    cameras[0] = NULL;
    cameras[1] = NULL;
    cameras[2] = NULL;
    cameras[3] = NULL;
    cameras[4] = NULL;

    pos = core::vector3df ( 0,0,0 );
    rot = core::vector3df ( 0,0,0 );
    scale = core::vector3df ( 0,0,0 );

    moduleDistanceFromCam = 0;
    moduleAngleFromCam = 0;
    cameraLoc = core::vector3df ( 0,0,0 ); //camera position for Moses Mode, initialized to zero
    DCamPos = core::vector3df ( 0,0,0 );



    //Switches for the different modes
    detectorMode = false;
    eventAnalysisMode = true;

    selectedTrackBox = NULL;

    rt=0;

    multiSelectButton=0;

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
    visibilityMapper.setMapping(&_pitVisibility,0);
    connect(&_pitVisibility,SIGNAL(valueChanged(bool)),
            &visibilityMapper,SLOT(map()));

    connect(&visibilityMapper,SIGNAL(mapped(int)),
            this,SLOT(switchVisibility(int)));
}



AGeometry::~AGeometry()
{
    if (rt) rt->drop();
}

void AGeometry::load()
{
    QTime time;
    time.start();

    //First load stuff originally loaded by ABase...
    getFileSystem()->addFileArchive ( "media/",true,true,EFAT_FOLDER);
    getFileSystem()->addFileArchive ( MEDIA_PREFIX ,true,true,EFAT_FOLDER);

    //These first three lines are part of an offset test for the Irrlicht ray generator
    cube = getSceneManager()->addCubeSceneNode();
    cube->getMaterial ( 0 ).EmissiveColor.set ( 0,255,0,0 );
    cube->setScale ( core::vector3df ( 5,5,5 ) );
    cube->setPosition ( core::vector3df ( 400,1500,400 ) );
    cube->setVisible(offsetTest);

    getFileSystem()->addFileArchive ( "AtlasGeometry.aml" ,true,true,EFAT_ZIP);

    cameraSwitcher=new CSceneNodeAnimatorCameraSwitch(getSceneManager());

    //Create the dynamic camera and define some variables

    cameras[0] = getSceneManager()->addCameraSceneNodeFPS ( 0, 40.0f, 100.0f );
    cameras[0]->setName("FPSCam");
    cameras[0]->setInputReceiverEnabled ( false );
    cameras[0]->setPosition ( core::vector3df ( 1200,500,-1200 ) );
    cameras[0]->setTarget ( core::vector3df ( 0,0,0 ) );
    cameras[0]->setFarValue ( 22000.0f );
    cameras[0]->setID(FPS);

    cameras[1] = getSceneManager()->addCameraSceneNode();
    cameras[1]->setName("FrontCam");
    cameras[1]->setInputReceiverEnabled ( false );
    cameras[1]->setFarValue ( 100000.0f );
    cameras[1]->setPosition ( core::vector3df ( 0,0,-30400.0 ) );
    cameras[1]->setFOV ( 0.005f );
    cameras[1]->setTarget ( core::vector3df ( 0,0,0 ) );
    scene::CSceneNodeAnimatorCameraFOV *frontAnimator=new scene::CSceneNodeAnimatorCameraFOV();
    frontAnimator->setMaxFOV(0.0185f);
    cameras[1]->addAnimator(frontAnimator);
    cameras[1]->setID(Side);

    cameras[2] = getSceneManager()->addCameraSceneNode();
    cameras[2]->setName("SideCam");
    cameras[2]->setInputReceiverEnabled ( false );
    cameras[2]->setFarValue ( 100000.0f );
    cameras[2]->setPosition ( core::vector3df ( 30400.0f,0,0 ) );
    cameras[2]->setFOV ( 0.005f );
    cameras[2]->setTarget ( core::vector3df ( 0,0,0 ) );
    scene::CSceneNodeAnimatorCameraFOV *sideAnimator=new scene::CSceneNodeAnimatorCameraFOV();
    sideAnimator->setMaxFOV(0.006f);
    cameras[2]->addAnimator(sideAnimator);
    cameras[2]->setID(Front);

    cameras[3] = getSceneManager()->addCameraSceneNode();
    cameras[3]->setName("SphereCam");
    cameras[3]->setInputReceiverEnabled ( false );
    cameras[3]->setFarValue ( 22000.0f );
    cameras[3]->setPosition ( core::vector3df ( 250,0,0 ) );
    cameras[3]->setTarget ( core::vector3df ( 0,0,0 ) );
    cameras[3]->addAnimator(new scene::CSceneNodeAnimatorCameraSphere());
    cameras[3]->setID(Sphere);

    cameras[4] = getSceneManager()->addCameraSceneNode();
    cameras[4]->setName("LockCam");
    cameras[4]->setInputReceiverEnabled ( false );
    cameras[4]->setFarValue ( 22000.0f );
    cameras[4]->setPosition ( core::vector3df ( 12000,5000,-12000 ) );
    cameras[4]->setTarget ( core::vector3df ( 0,0,0 ) );
    cameras[4]->setID(Lock);

    fpsControl=new AFPSControl(cameras[0],
                               getSceneManager(),
                               getGUIEnvironment(),
                               getGUIEnvironment()->getRootGUIElement(),
                               -1,core::rect<s32>(0,0,100,100));
    fpsControl->setVisible(false);
    connect(this,SIGNAL(cameraSwitched(ICameraSceneNode*)),
            fpsControl,SLOT(handleNewActiveCamera(ICameraSceneNode*)));
    fpsControl->setVisible(false);

    sphereControl=new ASphereControl(cameras[3],
                                     getSceneManager(),
                                     getGUIEnvironment(),
                                     getGUIEnvironment()->getRootGUIElement(),
                                     -1,core::rect<s32>(0,0,100,20));
    connect(this,SIGNAL(cameraSwitched(ICameraSceneNode*)),
            sphereControl,SLOT(handleNewActiveCamera(ICameraSceneNode*)));
    sphereControl->setVisible(false);

    sideControl=new AFOVControl(cameras[2],
                                getSceneManager(),
                                getGUIEnvironment(),
                                getGUIEnvironment()->getRootGUIElement(),
                                -1,core::rect<s32>(0,0,20,300));
    sideControl->setVisible(false);
    connect(this,SIGNAL(cameraSwitched(ICameraSceneNode*)),
            sideControl,SLOT(handleNewActiveCamera(ICameraSceneNode*)));
    sideControl->setVisible(false);

    frontControl=new AFOVControl(cameras[1],
                                 getSceneManager(),
                                 getGUIEnvironment(),
                                 getGUIEnvironment()->getRootGUIElement(),
                                 -1,core::rect<s32>(0,0,20,300));
    connect(this,SIGNAL(cameraSwitched(ICameraSceneNode*)),
            frontControl,SLOT(handleNewActiveCamera(ICameraSceneNode*)));
    frontControl->setVisible(false);


    //Prepare spinning logo
    getFileSystem()->addFileArchive ( "logo.aml" ,EFAT_ZIP);

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
              0.004f,
              core::vector3df(1,1,-1));
    _logoLight->addAnimator(_logoAnim);
    forceUpdate();

    core::vector3df camRot = cameras[0]->getRotation();
    core::vector3df DCamPos = core::vector3df ( 0,0,0 );

    //This is the camera bounding box, used to define the Moses mode area
    CameraBB = getSceneManager()->addCubeSceneNode ( 1.0f, 0, -1, cameras[0]->getPosition() ,cameras[0]->getRotation(), core::vector3df ( 55,55,55 ) );
    CameraBB->setID ( 0 );
    CameraBB->setName ("Moses Mode Box");
    CameraBB->setVisible(false);

    //Create the geometry
    createAtlasGeometry();
    createFlatGeometry();

    _rootObjectsNode=getSceneManager()->addEmptySceneNode();

    forceUpdate(); //Make sure the timer is correct!

    qDebug() << "Loaded AGeometry (" << time.elapsed() << " ms)";
}

void AGeometry::executeMosesMode(core::vector3df camPos)
{
    if (!CameraBB) return;

    switch (_cropMode)
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
    if (selection && selection->getID()==8)
    {
        QString partName(selection->getName());
        if (partName=="TC_Barrel" ||
                partName=="TCB_Reference" ||
                partName=="TC_Reference")
            return "TC_Reference";

        else if (partName=="EMC_Reference" ||
                 partName=="EMCB_Reference" ||
                 partName=="EMC_Barrel")
            return "EMC_Reference";

        else if (partName=="TRT_Reference" ||
                 partName=="TRTB_Reference" ||
                 partName=="TRT_Barrel")
            return "TRT_Reference";

        else if (partName=="Magnets_Reference" ||
                 partName=="Toroid_Magnet")
            return "Magnets_Reference";

        else if (partName=="SCT_Reference" ||
                 partName=="SCTB_Reference" ||
                 partName=="SCT_Barrel_L1" ||
                 partName=="SCT_Barrel_L2" ||
                 partName=="SCT_Barrel_L3" ||
                 partName=="SCT_Barrel_L4" ||
                 partName=="SCT_Disks")
            return "SCT_Reference";

        else if (partName=="Pixels_Reference" ||
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

        else if (partName=="Muons_Reference" ||
                 partName=="MuonSpectrometer")
            return "Muons_Reference";
    }

    return "";
}

AEventObject3DNode* AGeometry::objectSelection ( core::position2di pos )
{
  qDebug() << "objectSelection";
    if ( eventAnalysisMode && allowTrackSelection )
    {
        ISceneCollisionManager* colmgr = getSceneManager()->getSceneCollisionManager();
        line3d<f32> ray = colmgr->getRayFromScreenCoordinates ( pos, getSceneManager()->getActiveCamera() );

        // Put all of the tracks into a selectable state, and store the previous state.
        QMap<AEventObject3DNode*,AEventObject3DNode::Style> oldStyles;
        for (int i=0;i<allObjects.size();i++)
        {
            if (allObjects[i]->isVisible())
            {
                aabbox3d<f32> box=allObjects[i]->getTransformedBoundingBox();
                if (box.intersectsWithLine(ray))
                {
                    oldStyles[allObjects[i]]=allObjects[i]->style();
                    allObjects[i]->setStyle(AEventObject3DNode::Selectable);
                }
            }
        }

        // Find the selected scene node
        ISceneNode *selectedSceneNode = colmgr->getSceneNodeFromRayBB ( ray, 16, false, _rootObjectsNode );
        ITriangleSelector* selector;
        vector3df target;
        triangle3df triangle;

        //Find the track that this scene node belongs to
        AEventObject3DNode *selectedNode=0;
        for ( int i=0; i<allObjects.size(); i++)
        {
            if (!allObjects[i]->isVisible()) continue;
            AEventObject *track=allObjects[i]->track();

            if ( selectedSceneNode && ( track->type() == AEventObject::eTrack || track->type() == AEventObject::eMissingEt ) ) //tracks
            {
                if ( selectedSceneNode->getParent() == allObjects[i] )
                {
                    selectedNode = allObjects[i];
                    break;
                }
            }
            else if ( track->type() == AEventObject::eShower || track->type() == AEventObject::eJet ) //jets
            {
                selector = allObjects[i]->getTriangleSelector();
                ISceneNode *outNode; //TODO: Take advantage of this somehow..
                if ( colmgr->getCollisionPoint ( ray, selector, target, triangle ,outNode))
                {
		  selectedNode = allObjects[i];
		  break;
                }
            }

        }

        // Restore the previous state to all of the tracks.
        QMap<AEventObject3DNode*,AEventObject3DNode::Style>::const_iterator iter=oldStyles.begin();
        QMap<AEventObject3DNode*,AEventObject3DNode::Style>::const_iterator iterE=oldStyles.end();
        for (;iter!=iterE;iter++)
        {
            iter.key()->setStyle(iter.value());
        }
        return selectedNode;
    }
    return 0;
}


void AGeometry::renderViewport(int view)
{
    if (view==active_viewport) return; //Time saving hack. Never render the visible viewport, since it is not in the two smaller viewports.
    int camId;
    if (view==Cam3D) camId=active_cam;
    else camId=view;

    irr::video::SColor color (255,0,0,0);

    if (rt==0) //Try to create an render texture if one does not exist...
    {
        if (getVideoDriver()->queryFeature(video::EVDF_RENDER_TO_TARGET))
        {
            rt = getVideoDriver()->addRenderTargetTexture(core::dimension2d<u32>(256,256));
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
    getSceneManager()->setActiveCamera ( cameras[ camId ] );
    setupView(view);

    getVideoDriver()->setRenderTarget(rt, true, true, color);
    getSceneManager()->drawAll();

    setupView(active_viewport);
    getVideoDriver()->setRenderTarget(0);
    getSceneManager()->setActiveCamera (originalCamera);
    originalCamera->drop();

    uchar* tmpdata=(uchar*)rt->lock ();

    dimension2d<u32> size=rt->getSize();

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
        core::list<ISceneNodeAnimator*>::ConstIterator anims=cameras[0]->getAnimators().begin();
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
    if (firstShow)
    {
        qDebug() << "First Show";
        //Remove the loading nodes
        _logoNode->remove();
        _logoLight->remove();
        _logoAnim->drop();
        setCamera(AGeometry::Sphere);
        firstShow=false;
    }


    // Main 3D view
    if (hasCameraMoved())
    {
        core::vector3df camPos = getSceneManager()->getActiveCamera()->getPosition();

        dynamicCameraSpeed(camPos);
        //dynamicHidingOfModules(camPos);

        if ( _cropMode!=NoneMode )
        {
            executeMosesMode(camPos);
        }
        //qDebug() << "STUFF UPDATED";
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
                getSceneManager()->getSceneNodeFromName ( "Shielding_JT" )->setVisible ( _pitVisibility.value());
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
            if (Atlas_Reference)
                Atlas_Reference->setVisible(_detectorVisibility.value());
            break;
        case 9: // Tracker
            if (Trackers_Reference)
                Trackers_Reference->setVisible(_trackerVisibility.value());
            break;
        case 10: // Calorimeters
            if (Calorimeters_Reference)
                Calorimeters_Reference->setVisible(_calorimetersVisibility.value());
            break;
        }

        //TODO: Uncomment after demo, have to do performance tests
        //if(active_viewport!=AGeometry::Cam3D)
        //renderViewport(AGeometry::Cam3D);

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
    /* Load front view */
    scene::IAnimatedMesh* bgf = getSceneManager()->getMesh ( "Background_Front.X" );
    background_node_f = getSceneManager()->addAnimatedMeshSceneNode ( bgf, 0, -1, core::vector3df ( 0,0,-240 ), core::vector3df ( 0,90,0 ) );
    background_node_f->getMaterial ( 0 ).DiffuseColor.set ( 255,222,222,222 );
    background_node_f->getMaterial ( 0 ).EmissiveColor.set ( 0,111,111,111 );
    background_node_f->setScale ( core::vector3df ( modelScale,modelScale,modelScale ) );
    background_node_f->setMaterialFlag ( video::EMF_LIGHTING, false );
    background_node_f->setMaterialType ( video::EMT_LIGHTMAP_ADD );
    background_node_f->setID ( 5 );
    background_node_f->setName("Background_Front.X");
    background_node_f->setVisible ( false );

    /* Load side view */
    scene::IAnimatedMesh* bgs = getSceneManager()->getMesh ( "Background_Side.X" );
    background_node_s = getSceneManager()->addAnimatedMeshSceneNode ( bgs, 0, -1, core::vector3df ( 400,0,0 ), core::vector3df ( 0,90,90 ) );
    background_node_s->getMaterial ( 0 ).DiffuseColor.set ( 255,222,222,222 );
    background_node_s->getMaterial ( 0 ).EmissiveColor.set ( 0,111,111,111 );
    background_node_s->setScale ( core::vector3df ( modelScale,modelScale,modelScale ) );
    background_node_s->setMaterialFlag ( video::EMF_LIGHTING, false );
    background_node_s->setMaterialType ( video::EMT_LIGHTMAP_ADD );
    background_node_s->setID ( 5 );
    background_node_s->setName("Background_Side.X");
    background_node_s->setVisible ( false );

    /* Render viewports */
    renderViewport(AGeometry::Front);
    renderViewport(AGeometry::Side);
}

void AGeometry::createAtlasGeometry()
{
    QIrrWidgetRefresher ref(this);

    if ( generateDetectorGeometry )
    {

        /* Load the ATLAS Detector and get references */
        ref.allModules=&this->allModules;
        //getSceneManager()->loadScene ( "geometry.irr" , &ref );
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

        /* Load the Pit */
        if ( !isCrappyComputer )
        {
            ref.allModules=0;
            getSceneManager()->loadScene ( "ATLAS_Pit.lvl" , &ref );
            Pit_Reference=getSceneManager()->getSceneNodeFromName( "Pit_Reference" );
        }

        /* Add all of the parts to a central parent node */
        detector3d_node=getSceneManager()->addEmptySceneNode();
        if (Pit_Reference)
        {
            Pit_Reference->setParent(detector3d_node);
            Pit_Reference->setScale(vector3df(modelScale,modelScale,modelScale));
        }
        if (Atlas_Reference)
        {
            Atlas_Reference->setVisible(true);
            Atlas_Reference->setParent(detector3d_node);
        }
    }
}

void AGeometry::selectTrackByID(unsigned int ID, bool multi)
{
    if (!multi) //If we are not doing multiselection, the deselect anything selected
    {
        while (!selectedObjects.isEmpty())
        {
            AEventObject3DNode *node=selectedObjects.back();
            node->deselect();
            selectedObjects.pop_back();
            emit deselected(node->track());
        }
    }

    //Loop through all the tracks...
    for ( int i=0;i<allObjects.size();i++)
    {
        AEventObject* selectedTrack = allObjects[i]->track();
        if ( selectedTrack->ID() == ID ) //Found it
        {
            allObjects[i]->select();
            selectedObjects.push_back(allObjects[i]);
            emit selected(selectedTrack);
            return;
        }
    }
}

void AGeometry::deselectTrackByID(unsigned int ID)
{
    AEventObject* tr;
    for (int i=0;i<selectedObjects.size();i++)
    {
        tr=selectedObjects[i]->track();
        if (tr->ID() == ID)
        {
            selectedObjects[i]->deselect();
            emit deselected(tr);
            selectedObjects.removeAt(i);
            return;
        }
    }
}

AEventObject3DNode* AGeometry::getTrackNodeByID(unsigned int ID)
{
    if (!_event) return 0;

    //Loop through all the tracks...
    for ( int i=0;i<allObjects.size();i++)
    {
        AEventObject* selectedTrack = allObjects[i]->track();
        if ( selectedTrack->ID() == ID ) //Found it
        {
            return allObjects[i];
        }
    }

    AEventObject *track=_event->completeEvent()->getObjectById(ID);

    if (track)
        return createNode(track);
    else
        return 0;
}

bool AGeometry::isSelected(unsigned int ID)
{
    AEventObject* tr;
    for (int i=0;i<selectedObjects.size();i++)
    {
        tr=selectedObjects[i]->track();
        if (tr->ID() == ID)
        {
            return true;
        }
    }
    return false;
}

void AGeometry::clearTrackSelection()
{
    AEventObject* tr;
    while (selectedObjects.size()>0)
    {
        tr=selectedObjects[0]->track();
        deselectTrackByID(tr->ID());
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
        allowTrackSelection=true;
        emit cameraControlSwitched(false);
    }
}

void AGeometry::resizeEvent( QResizeEvent* event )
{
    if (rt) rt->drop();
    rt=0;

    if (fpsControl)
    {
        QSize size=event->size();
        core::rect<s32> fpsPos(size.width()-100,0,size.width(),100);
        fpsControl->setRelativePosition(fpsPos);
    }

    if (sphereControl)
    {
        QSize size=event->size();
        ////core::rect<s32> spherePos(size.width()-100,0,size.width(),100);
        core::rect<s32> spherePos(size.width()-100,size.height()-20,size.width(),size.height());
        sphereControl->setRelativePosition(spherePos);
        sphereControl->updateAbsolutePosition();
    }

    if (frontControl)
    {
        QSize size=event->size();
        core::rect<s32> frontPos(size.width()-20,size.height()/2-150,size.width(),size.height()/2+150);
        frontControl->setRelativePosition(frontPos);
    }

    if (sideControl)
    {
        QSize size=event->size();
        core::rect<s32> sidePos(size.width()-20,size.height()/2-150,size.width(),size.height()/2+150);
        sideControl->setRelativePosition(sidePos);
    }

    QIrrWidget::resizeEvent(event);
}

void AGeometry::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (getSceneManager()->getActiveCamera()->isInputReceiverEnabled()) releaseControl();
    else grabControl();
}



void AGeometry::mouseClickEvent(QMouseEvent *event)
{
    core::position2di posMouse = core::position2di(event->x(),event->y());
    bool Shift =((QApplication::keyboardModifiers() & (Qt::ShiftModifier | Qt::ControlModifier)) > 0);

    if (event->button()==Qt::LeftButton)
    {
        if (allowTrackSelection)
        { //Start track selection
            AEventObject3DNode *selectedNode=objectSelection(posMouse);

            //If shifty/ctrly no clicky, then we do not have a multi-track selection and so we deselect everything, but the clicked ray
            if (!Shift)
            { // Deselect every selected track
                while (!selectedObjects.isEmpty())
                {
                    AEventObject3DNode *node=selectedObjects.back();
                    node->deselect();
                    selectedObjects.pop_back();
                    emit deselected(node->track());
                }
            }

            if (selectedNode)
            {
                int idx=selectedObjects.indexOf(selectedNode);
                if (idx==-1) //Make sure the track is not already selected
                {
                    selectedNode->select();
                    if (selectedNode->track()->isInteresting())
                    {
                        selectedObjects.push_back(selectedNode);
                        emit selected(selectedNode->track());
                        qDebug() << "Found and selected a track...";
                    }
                }
                else //else deselect it
                {
                    selectedObjects.removeAt(idx);
                    selectedNode->deselect();
                    emit deselected(selectedNode->track());
                    qDebug() << "Found and delselected a track...";
                }
            } //End track selection
            return;
        }

        if (!Shift) // When we press shift, then for sure we only want track selection
        { //Start detector selection
            QString partName=detectorSelection(posMouse);
            if (partName!="")
            {
                emit detectorPartSelected(partName);
                return;
            }
        } //End detector selection

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
        //toggles camera/cursor control
    case Qt::Key_Space:
        if (getSceneManager()->getActiveCamera()->isInputReceiverEnabled()) releaseControl();
        else grabControl();
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
    if (_comboMenu)
    {
        //Prepare the combination object holding all the selected tracks
        ATrackCombination *combination=new ATrackCombination();
        for (int i=0;i<selectedObjects.size();i++)
            combination->addTrack(selectedObjects[i]->track());

        //For each item in the combo menu, set it's data to the combined tracks
        QList<QAction *> actions=_comboMenu->findChildren<QAction*>();
        if (actions.size()>0)
        {
            for (int i=0;i<actions.size();i++)
            {
                QVariant data=QVariant::fromValue<QObject*>(combination);
                actions[i]->setData(data);
            }
        }

        //Add to the menu
        QAction *comboMenuAction=menu.addMenu(_comboMenu);

        //Enable only if 2 or more tracks are selected
        if (combination->size()<2)
            comboMenuAction->setEnabled(false);
        else
            comboMenuAction->setEnabled(true);
    }

    /* If a detector part is selected, add it to the menu */
    QPoint qpos=event->pos();

    core::position2di pos(qpos.x(),qpos.y());
    QString partSelected=detectorSelection(pos);
    if (partSelected!="")
    {
        // Add menu entry
        if (_detectorMenu.contains(partSelected))
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
    if (!_event) return;


    disconnect(_event,SIGNAL(filtersUpdated()),
               this,SLOT(updateTracks()));
    disconnect(_event,SIGNAL(filtersUpdated()),
               this,SLOT(makeDirty()));

    clearTrackSelection();

    for (int i=0;i<allObjects.size();i++)
    {
        allObjects[i]->remove();
        allObjects[i]->deleteLater();
    }
    allObjects.clear();

    renderViewport(AGeometry::Front);
    renderViewport(AGeometry::Side);
    renderViewport(AGeometry::Cam3D);
    _event=0;
    makeDirty();
}

void AGeometry::setEvent(AFilteredEvent* e)
{
    clearEvent();

    _event=e;
    connect(_event,SIGNAL(filtersUpdated()),
            this,SLOT(updateTracks()));
    connect(_event,SIGNAL(filtersUpdated()),
            this,SLOT(makeDirty()));
    updateTracks();

    allowTrackSelection=true;
    _detectorVisibility.setValue(false);
    makeDirty();
}

AFilteredEvent* AGeometry::event()
{
    return _event;
}

int AGeometry::viewport()
{
    return active_viewport;
}

int AGeometry::camera()
{
    return active_cam;
}

AEventObject3DNode* AGeometry::createNode(AEventObject* track)
{
    AEventObject3DNode *node=0;
    if (track->type()==AEventObject::eJet)
    {
        AJet* jet=(AJet*)track;
        node=new AJet3DNode(_rootObjectsNode,getSceneManager(),0,jet);
    }
    else if (track->type()==AEventObject::eTrack)
    {
      ATrack *tr=(ATrack*)track;
      if(tr->trackType()=="STrack")
	{
	  ASTrack* str=(ASTrack*)track;
	  node=new ASTrack3DNode(_rootObjectsNode,getSceneManager(),0,str);
	}
      else if(tr->trackType()=="RTrack")
	{
	  ARTrack* rtr=(ARTrack*)track;
	  node=new ARTrack3DNode(_rootObjectsNode,getSceneManager(),0,rtr);
	}
    }
    else if (track->type()==AEventObject::eMissingEt)
    {
        AMisET* miset=(AMisET*)track;
        node=new AMisET3DNode(_rootObjectsNode,getSceneManager(),0,miset);
    }
    else if (track->type()==AEventObject::eShower)
    {
      AShower* shower=(AShower*)track;
      node=new ACaloTower3DNode(_rootObjectsNode,getSceneManager(),0,shower);
    }

    if (node)
    {
        node->setVisible(false);
        allObjects.push_back(node);
	connect(node,SIGNAL(lookChanged()),
		this,SLOT(makeDirty()));
    }

    return node;
}

void AGeometry::lockCamera()
{
    int camId;
    if (active_viewport==AGeometry::Cam3D) camId=active_cam;
    else camId=active_viewport;

    cameras[Lock]->setPosition(cameras[camId]->getPosition());
    cameras[Lock]->setTarget(cameras[camId]->getTarget());
    cameras[Lock]->setFOV(cameras[camId]->getFOV());
    cameras[Lock]->setFarValue(cameras[camId]->getFarValue());

    getSceneManager()->setActiveCamera(cameras[Lock]);
}

void AGeometry::unlockCamera()
{
    int camId;
    if (active_viewport==AGeometry::Cam3D) camId=active_cam;
    else camId=active_viewport;

    cameras[camId]->setPosition(cameras[Lock]->getPosition());
    if (camId!=Sphere) cameras[camId]->setTarget(cameras[Lock]->getTarget());
    cameras[camId]->setFOV(cameras[Lock]->getFOV());
    cameras[camId]->setFarValue(cameras[Lock]->getFarValue());

    getSceneManager()->setActiveCamera(cameras[camId]);

    vector3df tar=cameras[camId]->getTarget();
}

//Switch the current camera, viewport clicked
void AGeometry::setViewport(int to)
{
    if (to==active_viewport) return; //Already using it

    // Determine if the camera is locked, if so unlock to store the current state
    bool locked=getSceneManager()->getActiveCamera()==cameras[Lock];
    if (locked) unlockCamera();

    // Setup ids
    int camId;
    if (to==AGeometry::Cam3D) camId=active_cam;
    else camId=to;
    int from=active_viewport;

    // Switch to the new camera and view
    getSceneManager()->setActiveCamera (cameras[camId]);
    if (to==AGeometry::Side || to==AGeometry::Front)
        cameras[to]->setInputReceiverEnabled(true);
    setupView(to);

    // Notify of the new state
    emit viewportSwitched(from,to);
    active_viewport=to;
    renderViewport(from);

    // Relock the camera if needed
    if (locked) lockCamera();

    qDebug() << "Active viewport switched from " << from << " to " << to;
}

void AGeometry::setCamera(int to,bool animate)
{
    if (to==active_cam) return; //Already using it

    // Determine if the camera is locked, if so unlock to store the current state
    bool locked=getSceneManager()->getActiveCamera()==cameras[Lock];
    if (locked) unlockCamera();

    switch (to)
    {
    case AGeometry::FPS:
        actFPS->setChecked(true);
        setCropMode(MosesMode);
        break;
    case AGeometry::Sphere:
        actSphere->setChecked(true);
        setCropMode(WedgeMode);
        cameras[AGeometry::Sphere]->setInputReceiverEnabled(true); //Sphere always wants the input receiver enabled
        break;
    }
    cameras[AGeometry::FPS]->setInputReceiverEnabled(false);
    setCursor(Qt::ArrowCursor);
    active_cam=to;

    // Keep the camera locked
    if (locked)
    {
        to=Lock;
        if (locked) lockCamera();
    }

    // Determine the correct way to switch the camera
    if (active_viewport==AGeometry::Cam3D)
    {
        if (animate)
            cameraSwitcher->setTargetCamera(cameras[to]);
        else
            getSceneManager()->setActiveCamera(cameras[to]);
    }

    renderViewport(AGeometry::Cam3D);
}

void AGeometry::setCropMode(int newMode)
{
    if (newMode==_cropMode) return;

    _cropMode=newMode;
    if (_cropMode==NoneMode)
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
    case AGeometry::Front:
        f=true;
        s=false;
        fps=false;
        break;
    case AGeometry::Side:
        f=false;
        s=true;
        fps=false;
        break;
    }

    if (background_node_f) background_node_f->setVisible (f);
    if (background_node_s) background_node_s->setVisible (s);
    if (detector3d_node)   detector3d_node->setVisible (fps);

    if (view==Cam3D)
    {
        if (multiSelectButton) multiSelectButton->setVisible(true);
    }
    else
    {
        if (multiSelectButton) multiSelectButton->setVisible(false);
    }
}

void AGeometry::setCameraPosition(APoint3D pos)
{
    vector3df irrpos(pos.x(),pos.y(),pos.z());
    getSceneManager()->getActiveCamera()->setPosition(irrpos);
}

void AGeometry::setCameraTarget(APoint3D pos)
{
    vector3df irrpos(pos.x(),pos.y(),pos.z());
    getSceneManager()->getActiveCamera()->setTarget(irrpos);
}

void AGeometry::setCameraFOV(float fov)
{
    getSceneManager()->getActiveCamera()->setFOV(fov);
}

void AGeometry::setCameraFarValue(float farvalue)
{
    getSceneManager()->getActiveCamera()->setFarValue(farvalue);
}


APoint3D AGeometry::cameraPosition()
{
    return APoint3D(getSceneManager()->getActiveCamera()->getPosition());
}

APoint3D AGeometry::cameraTarget()
{
    return APoint3D(getSceneManager()->getActiveCamera()->getTarget());
}

float AGeometry::cameraFOV()
{
    return getSceneManager()->getActiveCamera()->getFOV();
}

float AGeometry::cameraFarValue()
{
    return getSceneManager()->getActiveCamera()->getFarValue();
}

void AGeometry::setComboMenu(QMenu *comboMenu)
{
    _comboMenu=comboMenu;
}

void AGeometry::addToDetectorMenu(QString partName,QAction *action)
{
    if (!_detectorMenu.contains(partName))
    {
        QMenu *newMenu=new QMenu(partName);
        _detectorMenu[partName]=newMenu;
    }

    _detectorMenu[partName]->addAction(action);
}

void AGeometry::updateTracks()
{
    if (!_event) return;

    QSet<AEventObject*> objects=QSet<AEventObject*>::fromList(_event->Objects);

    // Toggle the visibility of all the tracks
    for (int i=0;i<allObjects.size();i++)
    {
        if (_event->Objects.contains(allObjects[i]->track()))
            allObjects[i]->setVisible(true);
        else
            allObjects[i]->setVisible(false);

        objects.remove(allObjects[i]->track());
    }

    QSet<AEventObject*>::const_iterator iter=objects.begin();
    QSet<AEventObject*>::const_iterator iterE=objects.end();
    for (;iter!=iterE;++iter)
    {
        AEventObject3DNode *node=createNode(*iter);
        if(node) node->setVisible(true);
    }

    renderViewport(AGeometry::Front);
    renderViewport(AGeometry::Side);
    renderViewport(AGeometry::Cam3D);
}
