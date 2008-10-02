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

#include <QApplication>

AGeometry::AGeometry(QWidget* parent)
        : QIrrWidget(parent), isCrappyComputer ( false ),  generateDetectorGeometry ( true ), generateCameraStats ( false ), displayFPS ( true ), offsetTest ( false ),
  background_node_f ( NULL ), background_node_s ( NULL ), frameSkipper ( 0 ), active_viewport ( AGeometry::Cam3D ) , active_cam (AGeometry::FPS)

{
    setCursor(Qt::ArrowCursor);

    background_node_f = NULL;
    background_node_s = NULL;

    // Module visibility switches
    vis_Pixels = false;
    vis_SCT = false;
    vis_TRT = false;
    vis_EM = true;
    vis_Hadr = true;
    vis_Magnets = true;
    vis_Muons = true;

    time_switch = 0;

    // Dynamic FPS camera initial parameters
    cameraZone = 3;
    camChangeDist1 = 145;
    camChangeDist2 = 1000;
    BBscale = 35;
    sliceMode = false;

    // Control variables for the dynamic hiding of parts of ATLAS
    isTC_on = true;
    isLAr_on = true;
    isSCT_on = true;
    isPix_on = true;
    TC_switch = 1;
    LAr_switch = 1;
    SCT_switch = 1;
    Pix_switch = 1;

    module thisModule;
    pos = core::vector3df ( 0,0,0 );
    rot = core::vector3df ( 0,0,0 );
    scale = core::vector3df ( 0,0,0 );
    cam_switch_delay = 0;

    left_click = false;
    moduleDistanceFromCam = 0;
    moduleAngleFromCam = 0;
    cameraLoc = core::vector3df ( 0,0,0 ); //camera position for Moses Mode, initialized to zero
    DCamPos = core::vector3df ( 0,0,0 );
    MosesMode = true;


    force_target = false;


    //Switches for the different modes
    detectorMode = false;
    eventAnalysisMode = true;
    multiMediaMode = false;

    MosesFreeCalm = false;
    mosesRestore = false;

    isTrackSelected = false;
    allowTrackSelection = false;

    selectedTrackBox = NULL;

    XmlEvt = new XmlEvent();
    XmlEvt->Base = this;

    rt=0;
}



AGeometry::~AGeometry()
{
    if (rt) rt->drop();
}

void AGeometry::prepareAllModules ( scene::ISceneNode* node_ )
{
    if ( !node_ )
        return;

    if ( ( node_->getType() != ESNT_EMPTY ) && ( node_->getType() != ESNT_LIGHT ) )
        if ( ( node_->getID() != 4 ) && ( node_->getID() !=7 ) )
        {
            node_->setID ( 0x0008 );
            //node_->updateAbsolutePosition();
            //node_->setDebugDataVisible(true);
            allModules.push_back ( node_ );
        }

    // recursive call to children
    const core::list<scene::ISceneNode*> & children = node_->getChildren();
    core::list<scene::ISceneNode*>::ConstIterator it = children.begin();
    for ( ; it != children.end(); ++it )
    {
        prepareAllModules ( *it );
    }
}

void AGeometry::load()
{
    //First load stuff originally loaded by ABase...
  //Device->getFileSystem()->addFolderFileArchive ( Device->getFileSystem()->getWorkingDirectory() );
  Device->getFileSystem()->addFolderFileArchive ( "./media/" );
  Device->getFileSystem()->addFolderFileArchive ( "./media/tours" );
  Device->getFileSystem()->addFolderFileArchive ( "./media/events" );
  Device->getFileSystem()->addFolderFileArchive ( TOURS_PREFIX );
  Device->getFileSystem()->addFolderFileArchive ( MEDIA_PREFIX );
  Device->getFileSystem()->addFolderFileArchive ( EVENTS_PREFIX );


    //These first three lines are part of an offset test for the Irrlicht ray generator
    cube = Device->getSceneManager()->addCubeSceneNode();
    cube->getMaterial ( 0 ).EmissiveColor.set ( 0,255,0,0 );
    cube->setScale ( core::vector3df ( 5,5,5 ) );
    cube->setPosition ( core::vector3df ( 400,1500,400 ) );
    cube->setVisible(offsetTest);

    tar_node = Device->getSceneManager()->addEmptySceneNode();
    cam_node = Device->getSceneManager()->addEmptySceneNode();
    OrthoCameraFront.buildProjectionMatrixOrthoLH ( 240.0f,180.0f,-400.0f,400.0f );
    OrthoCameraSide.buildProjectionMatrixOrthoLH ( 240.0f,180.0f,-400.0f,400.0f );
    Device->getFileSystem()->addZipFileArchive ( "AtlasGeometry.aml" );


//*****************CHANGED************************//

    //Base->Gui->statusmessage = Base->GetGuiEnv()->addStaticText ( L"", irr::core::rect<s32> ( 465, 650, 780, 750 ), false );
    //Base->Gui->statusmessage->setOverrideColor ( SColor ( 255,255,0,0 ) );
    //Base->Gui->trackinfo = Base->GetGuiEnv()->addStaticText ( L"", irr::core::rect<s32> ( 420, 670, 780, 770 ), false );
    //Base->Gui->trackinfo->setOverrideColor ( SColor ( 255,255,255,255 ) );


    //Create the geometry
    createAtlasGeometry();
    //Irr->GetSceneManager()->loadScene("/atlas/Atlas.irr");

    //Place pointers for the modules on the allModules vector
    prepareAllModules ( Device->getSceneManager()->getSceneNodeFromName ( "Atlas_Reference" ) );


    /***************** PREPARE BB MODELS ********************/

    /*    for (vector<module>::iterator it = allModules.begin(); it!=allModules.end(); it++)
        {
            it->theModule->getAbsoluteTransformation().transformBox(it->theModule->getTransformedBoundingBox());
            it->theModule->setID(1);
            //it->theModule->setDebugDataVisible(true);
        }*/
    /********************************************************/


    cameraSwitcher=new CSceneNodeAnimatorCameraSwitch(Device->getSceneManager());
	
    //Create the dynamic camera and define some variables
    camera[0] = Device->getSceneManager()->addCameraSceneNodeFPS ( 0, 40.0f, 100.0f );
    camera[0]->setInputReceiverEnabled ( false );
    camera[0]->setPosition ( core::vector3df ( 1200,500,-1200 ) );
    camera[0]->setTarget ( core::vector3df ( 0,0,0 ) );
    camera[0]->setFarValue ( 22000.0f );
    camera[0]->setAspectRatio ( 0.8/0.6 );
    camera[0]->setID ( 0 );
    
    core::vector3df Target = camera[0]->getTarget();
    camera[0]->updateAbsolutePosition();
    Target = camera[0]->getTarget();

    camera[1] = Device->getSceneManager()->addCameraSceneNode();
    camera[1]->setInputReceiverEnabled ( false );
    camera[1]->setPosition ( core::vector3df ( 0,0,-1 ) );
    camera[1]->setTarget ( core::vector3df ( 0,0,0 ) );
    camera[1]->setProjectionMatrix ( OrthoCameraFront );

    camera[2] = Device->getSceneManager()->addCameraSceneNode();
    camera[2]->setInputReceiverEnabled ( false );
    camera[2]->setPosition ( core::vector3df ( 1,0,0 ) );
    camera[2]->setTarget ( core::vector3df ( 0,0,0 ) );
    camera[2]->setProjectionMatrix ( OrthoCameraSide );

    camera[3] = Device->getSceneManager()->addCameraSceneNode();
    camera[3]->setInputReceiverEnabled ( false );
    camera[3]->setFarValue ( 22000.0f );
    camera[3]->setAspectRatio ( 0.8/0.6 );
    camera[3]->setPosition ( core::vector3df ( 250,0,0 ) );
    camera[3]->setTarget ( core::vector3df ( 0,0,0 ) );
    //camera[3]->addAnimator(new CSceneNodeAnimatorCameraOrbit(core::vector3df(0,0,1)));
    camera[3]->addAnimator(new scene::CSceneNodeAnimatorCameraSphere(Device->getCursorControl()));


    core::vector3df camRot = camera[0]->getRotation();
    core::vector3df DCamPos = core::vector3df ( 0,0,0 );

    //This is the camera bounding box, used to define the Moses mode area
    CameraBB = Device->getSceneManager()->addCubeSceneNode ( 1.0f, 0, -1, camera[0]->getPosition() ,camera[0]->getRotation(), core::vector3df ( 55,55,55 ) );
    CameraBB->setID ( 0 );

    qDebug() << "Loaded AGeometry";

    renderViewport(AGeometry::Orthogonal);
    renderViewport(AGeometry::Projective);
    //Device->getSceneManager()->setActiveCamera(camera[AGeometry::FPS]);
    setCamera(AGeometry::FPS);
    setViewport(AGeometry::Cam3D);
    
    emit finishedLoading();

}

void AGeometry::executeMosesMode()
{
    if ( MosesMode )
    {
        CameraBB->updateAbsolutePosition();
        CameraBB->setVisible ( true );
        for ( vector<scene::ISceneNode*>::iterator itb = allModules.begin(); ( itb ) !=allModules.end(); itb++ )
        {
            moduleAngleFromCam = angleBetween ( *itb, ( Device->getSceneManager()->getActiveCamera()->getTarget() *0.5- Device->getSceneManager()->getActiveCamera()->getPosition() ) );

            if ( ( CameraBB->getTransformedBoundingBox().intersectsWithBox ( ( *itb )->getTransformedBoundingBox() ) ) && ( moduleAngleFromCam<1.0f ) )
            {
                ( *itb )->setVisible ( false );
            }
            else
            {
                ( *itb )->setVisible ( true );
            }
        }
    }
    if ( !MosesMode && mosesRestore ) // runs once after mosesMode
    {
        MosesFreeCalm = true;
        mosesRestore = false;
        for ( vector<scene::ISceneNode*>::iterator itb = allModules.begin(); ( itb ) !=allModules.end(); itb++ )
            ( *itb )->setVisible ( true );
    }
    CameraBB->setVisible ( false );

}

HelixSceneNode* AGeometry::trackSelection ( core::position2di pos )
{
    //Base->Gui->statstext->setText ( L"Track Selection" );
    if ( eventAnalysisMode )
    {
        ISceneCollisionManager* colmgr = Device->getSceneManager()->getSceneCollisionManager();
        line3d<f32> ray = colmgr->getRayFromScreenCoordinates ( pos, Device->getSceneManager()->getActiveCamera() );
        ISceneNode *selectedSceneNode = colmgr->getSceneNodeFromRayBB ( ray, 16 );
        ITriangleSelector* selector;
        vector3df target;
        triangle3df triangle;

        HelixSceneNode *selectedHelixNode=0;

        if ( XmlEvt->EventComplete.tracks.size() >= 1 )
        {
            //Base->Gui->statstext->setText(L"Checkpoint");

            for ( vector<track>::iterator iter = XmlEvt->EventComplete.tracks.begin(); iter < XmlEvt->EventComplete.tracks.end(); iter++ )
            {
                int control = 0;
                if ( selectedSceneNode && iter->Type == 1 ) //tracks
                {
                    if ( selectedSceneNode->getParent() == iter->node )
                    {
                        selectedHelixNode = iter->node;

                        break;
                    }
                }

                if ( iter->Type == 2 ) //jets
                {
                    selector = iter->node->Pyramid->getTriangleSelector();
                    //if (selector) wxMessageBox(_("ToolNB created") );

                    if ( colmgr->getCollisionPoint ( ray, selector, target, triangle ) )
                    {
                        selectedHelixNode = iter->node;
                        break;
                    }

                }

                if ( iter->Type == 4 ) //Missing Et
                {
                    if ( selectedSceneNode->getParent() == iter->node )
                    {
                        selectedHelixNode = iter->node;

                        break;
                    }
                }
            }
        }
        return selectedHelixNode;
    }
    return 0;
}


void AGeometry::renderViewport(int view)
{
  int camId;
  if(view==Cam3D) camId=active_cam;
  else camId=view;

    irr::video::SColor color (255,0,0,0);

    if (rt==0) //Try to create an render texture if one does not exist...
      {
      if (Device->getVideoDriver()->queryFeature(video::EVDF_RENDER_TO_TARGET))
	{
	  rt = Device->getVideoDriver()->createRenderTargetTexture(core::dimension2d<s32>(256,256));
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
    ICameraSceneNode *originalCamera=Device->getSceneManager()->getActiveCamera();
    originalCamera->grab();
    Device->getSceneManager()->setActiveCamera ( camera[ camId ] );
    setupView(view);

    Device->getVideoDriver()->setRenderTarget(rt, true, true, color);
    Device->getVideoDriver()->beginScene(true,true,color);
    Device->getSceneManager()->drawAll();
    Device->getSceneManager()->setActiveCamera (originalCamera);
    originalCamera->drop();
    setupView(active_viewport);

    Device->getVideoDriver()->setRenderTarget(0);
    Device->getSceneManager()->drawAll();
    Device->getVideoDriver()->endScene();
    
    uchar* tmpdata=(uchar*)rt->lock ();
    
    dimension2d<s32> size=rt->getSize();
    image=QImage(tmpdata,size.Width,size.Height,QIrrWidget::Irr2Qt_ColorFormat(rt->getColorFormat()));
    rt->unlock();
    
    emit viewportUpdated(view,image);

    //Device->getSceneManager()->drawAll();
    //Device->getVideoDriver()->endScene();
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

    if ( !isTC_on && ((camR < 1000 && camPos.Z>= -400 && camPos.Z <= 400) || sliceMode ) )
    {
        isTC_on = true;
        TC_switch = 1;
    }

    if ( !isLAr_on && ((camR < 400 && camPos.Z>= -400 && camPos.Z <= 400) || sliceMode ) )
    {
        isLAr_on = true;
        LAr_switch = 1;
    }

    if ( !isSCT_on && ((camR < 250 && camPos.Z>= -400 && camPos.Z <= 400 ) || sliceMode ) )
    {
        isSCT_on = true;
        SCT_switch = 1;
    }

    if ( !isPix_on && ((camR < 75 && camPos.Z>= -400 && camPos.Z <= 400) || sliceMode ) )
    {
        isPix_on = true;
        Pix_switch = 1;
    }


    //Part III,  change states

    if ( TC_switch == 1 || LAr_switch == 1 || SCT_switch == 1 || Pix_switch == 1 )
    {

        if ( TC_switch == 1 )
        {
            Device->getSceneManager()->getSceneNodeFromName ( "TCB_Reference" )->setVisible ( isTC_on );
        }

        if ( LAr_switch == 1 )
        {
            Device->getSceneManager()->getSceneNodeFromName ( "EMCB_Reference" )->setVisible ( isLAr_on );
        }


        if ( SCT_switch == 1 )
        {
            Device->getSceneManager()->getSceneNodeFromName ( "SCTB_Reference" )->setVisible ( isSCT_on );
            Device->getSceneManager()->getSceneNodeFromName ( "TRTB_Reference" )->setVisible ( isSCT_on );
        }

        if ( Pix_switch == 1 )
        {
            Device->getSceneManager()->getSceneNodeFromName ( "PixelsB_Reference" )->setVisible ( isPix_on );
        }

        TC_switch = 0;
        LAr_switch = 0;
        SCT_switch = 0;
        Pix_switch = 0;
    }


}

void AGeometry::execute()
{
    // Main 3D view
    core::vector3df camPos = Device->getSceneManager()->getActiveCamera()->getPosition();

    BBscale = sqrt ( camPos.X*camPos.X + camPos.Y*camPos.Y + camPos.Z*camPos.Z ) *0.8 +25;
    CameraBB->setPosition ( camPos );
    if ( sliceMode == true )
      {
        BBscale = 15000;
        CameraBB->setPosition ( core::vector3df ( 0,0,0 ) );
      }
    CameraBB->setRotation ( Device->getSceneManager()->getActiveCamera()->getRotation() );
    CameraBB->setScale ( core::vector3df ( BBscale,BBscale,BBscale ) );

    dynamicCameraSpeed(camPos);
    dynamicHidingOfModules(camPos);

    core::position2di pos = Device->getCursorControl()->getPosition();

    //TODO This is old code of module selection
    /*if (detectorMode)
    {
        line3d<f32> ray = Device->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(pos, Device->getSceneManager()->getActiveCamera());
        //selectedSceneNode = Irr->GetSceneManager()->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(pos, 2);
        selectedSceneNode = Device->getSceneManager()->getSceneCollisionManager()->getSceneNodeFromRayBB(ray, 2);
        if (selectedSceneNode->getID() == 2)
        {
            if (lastSelectedSceneNode && (selectedSceneNode!=lastSelectedSceneNode))
            {
                lastSelectedSceneNode->setMaterialTexture(0, Device->getVideoDriver()->getTexture("tile.jpg"));
            }

            if (selectedSceneNode && (selectedSceneNode!=lastSelectedSceneNode) && !(camera[0]->isInputReceiverEnabled()))
            {

                // TODO (Joao Pequenao#1#): Change the module selection code

                ita = allModules.begin(); // 'ita' used below as well!!!
                while (((ita->theModule) != (selectedSceneNode)) && ((ita+1) < allModules.end()))
                {
                    ita++;
                }

                if ((ita) != allModules.end())
                {
                    if (((ita->theModule) == (selectedSceneNode)) )
                    {
                        if ((ita->mType>=1)&&(ita->mType<=6)) lastSelectedSceneNode = selectedSceneNode;
                        if ((ita->mType>=1)&&(ita->mType<=6)) selectedSceneNode->setMaterialTexture(0, driver->getTexture("media/tile_selected.jpg"));
                    }
                }
                else
                {
                    selectedSceneNode =0;
                }
        }
    }
    }*/

    //  END module selection)



    if ( ( frameSkipper==0 ) && ! ( MosesFreeCalm ) )
    {
      executeMosesMode(); // every 5(?) frames
    }

    CameraBB->setVisible ( false );

    //more stuff for the dynamic camera
    DCamPos = camera[0]->getPosition() - camPos;

    if ( ( DCamPos.X*DCamPos.X + DCamPos.Y*DCamPos.Y + DCamPos.Z*DCamPos.Z != 0 ) )
        //frameskipper makes it so moses mode isn't calculated ever frame,
        //which should speed things up some
        frameSkipper++;
    if ( frameSkipper>=skipFrameNumber ) frameSkipper=0;

    if (force_target)
    {

        //vector3df vect = tar_node->getPosition () - camera[0]->getPosition ();
        //camera[0]->setRotation (vect.getHorizontalAngle ());
        camera[0]->setTarget (tar_node->getPosition());
        camera[0]->setPosition (cam_node->getPosition());

    }

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
        if ( Device->getSceneManager()->getSceneNodeFromName ( "Atlas_Reference" ) )
        {
            switch ( modType )
            {

            case 0: //Pit

                if ( Device->getSceneManager()->getSceneNodeFromName ( "Pit_Reference" ) )
                {
                    Device->getSceneManager()->getSceneNodeFromName ( "Pit_Reference" )->setVisible ( ! ( Device->getSceneManager()->getSceneNodeFromName ( "Pit_Reference" )->isVisible() ) );
                }

                if ( Device->getSceneManager()->getSceneNodeFromName ( "Shielding_JT" ) )
                {
                    Device->getSceneManager()->getSceneNodeFromName ( "Shielding_JT" )->setVisible ( ! ( Device->getSceneManager()->getSceneNodeFromName ( "Shielding_JT" )->isVisible() ) );
                }
                break;


            case 1: //Muon Chambers

                if ( Device->getSceneManager()->getSceneNodeFromName ( "Muons_Reference" ) )
                {
                    Device->getSceneManager()->getSceneNodeFromName ( "Muons_Reference" )->setVisible ( ! ( Device->getSceneManager()->getSceneNodeFromName ( "Muons_Reference" )->isVisible() ) );
                    cout << "Switched Muons" << endl;
                }
                break;


            case 2: // Magnets

                if ( Device->getSceneManager()->getSceneNodeFromName ( "Magnets_Reference" ) )
                {
                    Device->getSceneManager()->getSceneNodeFromName ( "Magnets_Reference" )->setVisible ( ! ( Device->getSceneManager()->getSceneNodeFromName ( "Magnets_Reference" )->isVisible() ) );
                }
                break;


            case 3: // Hadronic Calorimeter

                if ( Device->getSceneManager()->getSceneNodeFromName ( "TC_Reference" ) )
                {
                    Device->getSceneManager()->getSceneNodeFromName ( "TC_Reference" )->setVisible ( ! ( Device->getSceneManager()->getSceneNodeFromName ( "TC_Reference" )->isVisible() ) );
                }
                break;


            case 4: // EM Calorimeter

                if ( Device->getSceneManager()->getSceneNodeFromName ( "EMC_Reference" ) )
                {
                    Device->getSceneManager()->getSceneNodeFromName ( "EMC_Reference" )->setVisible ( ! ( Device->getSceneManager()->getSceneNodeFromName ( "EMC_Reference" )->isVisible() ) );
                }
                break;


            case 5: // TRT

                if ( Device->getSceneManager()->getSceneNodeFromName ( "TRT_Reference" ) )
                {
                    Device->getSceneManager()->getSceneNodeFromName ( "TRT_Reference" )->setVisible ( ! ( Device->getSceneManager()->getSceneNodeFromName ( "TRT_Reference" )->isVisible() ) );
                }
                break;


            case 6: // SCT

                if ( Device->getSceneManager()->getSceneNodeFromName ( "SCT_Reference" ) )
                {
                    Device->getSceneManager()->getSceneNodeFromName ( "SCT_Reference" )->setVisible ( ! ( Device->getSceneManager()->getSceneNodeFromName ( "SCT_Reference" )->isVisible() ) );
                }
                break;


            case 7: // Pixels

                if ( Device->getSceneManager()->getSceneNodeFromName ( "Pixels_Reference" ) )
                {
                    Device->getSceneManager()->getSceneNodeFromName ( "Pixels_Reference" )->setVisible ( ! ( Device->getSceneManager()->getSceneNodeFromName ( "Pixels_Reference" )->isVisible() ) );
                }
                break;

            }
        }
    }
}

void AGeometry::toggleVisibilityPixels()
{
    switchVisibility (7);
}

void AGeometry::toggleVisibilitySCT()
{
    switchVisibility (6);
}

void AGeometry::toggleVisibilityTRT()
{
    switchVisibility (5);
}

void AGeometry::toggleVisibilityLAr()
{
    switchVisibility (4);
}

void AGeometry::toggleVisibilityTile()
{
    switchVisibility (3);
}

void AGeometry::toggleVisibilityMuonSpec()
{
    switchVisibility (1);
}

void AGeometry::toggleVisibilityMagnets()
{
    switchVisibility (2);
}

void AGeometry::toggleVisibilityPit()
{
    switchVisibility (0);
}




void AGeometry::createAtlasGeometry()
{

    float mscale = 0.4; //model scale

    // file system needed to create new attributes
    //Irr->GetDevice()->getFileSystem()->addZipFileArchive("media/AtlasGeometry.aml");
    //IFileSystem* fs = Irr->GetDevice()->getFileSystem();
    video::ITexture* reflex = Device->getVideoDriver()->getTexture ( "refmap2.jpg" );

    scene::ISceneNode* node = 0;
    scene::ISceneNode* Atlas_Reference = Device->getSceneManager()->addEmptySceneNode();
    Atlas_Reference->setScale ( core::vector3df ( mscale,mscale,mscale ) );
    Atlas_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
    Atlas_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
    Atlas_Reference->setName ( "Atlas_Reference" );

    //Load first half of the ATLAS pit

    if ( generateDetectorGeometry )
    {
        if ( !isCrappyComputer )
        {

            Device->getSceneManager()->loadScene ( "ATLAS_Pit.lvl" );
            /* scene::IAnimatedMesh* Pit01 = Irr->GetSceneManager()->getMesh("Pit_part01.X");
             scene::IAnimatedMesh* Pit02 = Irr->GetSceneManager()->getMesh("Pit_part02.X");

             node = Irr->GetSceneManager()->addAnimatedMeshSceneNode(Pit01);
             node->getMaterial(0).DiffuseColor.set(255,111,111,111);
             node->getMaterial(0).EmissiveColor.set(0,50,50,50);
             node->setMaterialTexture(0, Irr->GetSceneManager()->getVideoDriver()->getTexture("STUCCO.bmp"));
             node->setMaterialFlag(video::EMF_LIGHTING, true);
             node->setMaterialType(video::EMT_SOLID);
             node->setRotation(core::vector3df(0 ,-90 ,-90 ));

             node = Irr->GetSceneManager()->addAnimatedMeshSceneNode(Pit02);
             node->getMaterial(0).DiffuseColor.set(255,111,111,111);
             node->getMaterial(0).EmissiveColor.set(0,50,50,50);
             //node->setMaterialTexture(0, Irr->GetSceneManager()->getVideoDriver()->getTexture("media/STUCCO.bmp"));
             node->setMaterialFlag(video::EMF_LIGHTING, true);
             node->setMaterialType(video::EMT_SOLID);
             node->setRotation(core::vector3df(0 ,-90 ,-90 ));*/

        }


        //Load TC Modules

        scene::ISceneNode* TC_Reference = Device->getSceneManager()->addEmptySceneNode();
        TC_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        TC_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        TC_Reference->setParent ( Atlas_Reference );
        TC_Reference->setName ( "TC_Reference" );
        scene::ISceneNode* TCB_Reference = Device->getSceneManager()->addEmptySceneNode();
        TCB_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        TCB_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        TCB_Reference->setParent ( TC_Reference );
        TCB_Reference->setName ( "TCB_Reference" );


        scene::IMesh* TC = Device->getSceneManager()->getMesh ( "TC_Barrel01.x" )->getMesh ( 0 );
        scene::IMeshSceneNode* nodeModels = NULL;

        for ( int a = 0 ; a<64  ; a++ )
        {

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( TC );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );

            //reflective material
            nodeModels->getMaterial ( 2 ).setTexture ( 0, reflex );
            nodeModels->getMaterial ( 2 ).setTexture ( 1, reflex );
            nodeModels->getMaterial ( 2 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
            nodeModels->getMaterial ( 2 ).Shininess = 10;
            nodeModels->getMaterial ( 2 ).DiffuseColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 2 ).EmissiveColor.set ( 200,222,222,222 );

            nodeModels->setRotation ( core::vector3df ( -90 + a*5.625,90,0 ) );
            nodeModels->setAutomaticCulling ( EAC_BOX );
            nodeModels->setParent ( TCB_Reference );
            nodeModels->setName ( "TC_Barrel" );
            nodeModels = NULL;




        }


        //Load EM Calorimeter EC Modules


        scene::ISceneNode* EM_Reference = Device->getSceneManager()->addEmptySceneNode();
        EM_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        EM_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        EM_Reference->setParent ( Atlas_Reference );
        EM_Reference->setName ( "EMC_Reference" );
        scene::ISceneNode* EMB_Reference = Device->getSceneManager()->addEmptySceneNode();
        EMB_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        EMB_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        EMB_Reference->setParent ( EM_Reference );
        EMB_Reference->setName ( "EMCB_Reference" );

        scene::IMesh* EM_Cal_EC = Device->getSceneManager()->getMesh ( "EM_Cal_EC_1.X" )->getMesh ( 0 );

        for ( int a = 0 ; a<8 ; a++ )
        {


            nodeModels = Device->getSceneManager()->addMeshSceneNode ( EM_Cal_EC );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );

            //reflective material
            nodeModels->getMaterial ( 3 ).setTexture ( 1, reflex );
            nodeModels->getMaterial ( 3 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
            nodeModels->getMaterial ( 3 ).Shininess = 88;
            nodeModels->getMaterial ( 3 ).DiffuseColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 3 ).EmissiveColor.set ( 0,222,222,222 );

            nodeModels->setRotation ( core::vector3df ( 0 ,90 ,0 + a*45 ) );
            nodeModels->setParent ( EM_Reference );
            nodeModels->setName ( "EMC_EC" );


            //  Other Side

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( EM_Cal_EC );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );

            //reflective material
            nodeModels->getMaterial ( 3 ).setTexture ( 1, reflex );
            nodeModels->getMaterial ( 3 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
            nodeModels->getMaterial ( 3 ).Shininess = 88;
            nodeModels->getMaterial ( 3 ).DiffuseColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 3 ).EmissiveColor.set ( 0,222,222,222 );

            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*45 ) );
            nodeModels->setParent ( EM_Reference );
            nodeModels->setName ( "EMC_EC" );

        }


        //Load EM Calorimeter Barrel Module


        scene::IMesh* EM_Cal_Barrel = Device->getSceneManager()->getMesh ( "LArg_Barrel.X" )->getMesh ( 0 );

        for ( int a = 0 ; a<16 ; a++ )
        {

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( EM_Cal_Barrel );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,200,200,200 );
            //nodeModels->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );

            nodeModels->getMaterial ( 2 ).Shininess = 1;
            nodeModels->getMaterial ( 2 ).EmissiveColor.set ( 0,177,177,177 );
            nodeModels->getMaterial ( 2 ).SpecularColor.set ( 0,80,80,80 );

            //reflective material
            nodeModels->getMaterial ( 3 ).setTexture ( 1, reflex );
            nodeModels->getMaterial ( 3 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
            nodeModels->getMaterial ( 3 ).Shininess = 88;
            nodeModels->getMaterial ( 3 ).DiffuseColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 3 ).EmissiveColor.set ( 0,222,222,222 );

            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*22.5 ) );
            nodeModels->setParent ( EMB_Reference );
            nodeModels->setName ( "EMC_Barrel" );


        }



        //Load TC Endcap Module B


        scene::IMesh* TC_EC = Device->getSceneManager()->getMesh ( "TC_Endcap01.X" )->getMesh ( 0 );

        for ( int a = 0 ; a<64 ; a++ )
        {
            nodeModels = Device->getSceneManager()->addMeshSceneNode ( TC_EC );


            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );

            //nodeModels->setScale(core::vector3df(mscale,mscale,mscale));

            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );

            //reflective material
            nodeModels->getMaterial ( 2 ).setTexture ( 0, reflex );
            nodeModels->getMaterial ( 2 ).setTexture ( 1, reflex );
            nodeModels->getMaterial ( 2 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
            nodeModels->getMaterial ( 2 ).Shininess = 10;
            nodeModels->getMaterial ( 2 ).DiffuseColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 2 ).EmissiveColor.set ( 200,222,222,222 );

            nodeModels->setRotation ( core::vector3df ( 90 + a*5.625 ,90 ,0 ) );
            nodeModels->setParent ( TC_Reference );
            nodeModels->setName ( "TC_EC" );


            //Other Side

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( TC_EC );


            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );

            //nodeModels->setScale(core::vector3df(mscale,mscale,mscale));

            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );

            //reflective material
            nodeModels->getMaterial ( 2 ).setTexture ( 0, reflex );
            nodeModels->getMaterial ( 2 ).setTexture ( 1, reflex );
            nodeModels->getMaterial ( 2 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
            nodeModels->getMaterial ( 2 ).Shininess = 10;
            nodeModels->getMaterial ( 2 ).DiffuseColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 2 ).EmissiveColor.set ( 200,222,222,222 );

            nodeModels->setRotation ( core::vector3df ( 90 + a*5.625 ,-90 ,0 ) );
            nodeModels->setParent ( TC_Reference );
            nodeModels->setName ( "TC_EC" );


        }


        //Load HEC Larg Modules A


        scene::IMesh* HEC_A = Device->getSceneManager()->getMesh ( "HEC_LArg01.x" )->getMesh ( 0 );


        for ( int a = 0 ; a<32  ; a++ )
        {
            nodeModels = Device->getSceneManager()->addMeshSceneNode ( HEC_A );

            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 0 ).SpecularColor.set ( 0,255,255,255 );

            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );

            //reflective material
            nodeModels->getMaterial ( 1 ).setTexture ( 1, reflex );
            nodeModels->getMaterial ( 1 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
            nodeModels->getMaterial ( 1 ).Shininess = 15;
            nodeModels->getMaterial ( 1 ).DiffuseColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 1 ).SpecularColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,255,255,255 );


            nodeModels->setRotation ( core::vector3df ( -90 + a*11.25,90,0 ) );
            nodeModels->setParent ( EM_Reference );
            nodeModels->setName ( "HEC_LArg" );


            //Other Side

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( HEC_A );

            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 0 ).SpecularColor.set ( 0,255,255,255 );


            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );

            //reflective material
            nodeModels->getMaterial ( 1 ).setTexture ( 1, reflex );
            nodeModels->getMaterial ( 1 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
            nodeModels->getMaterial ( 1 ).Shininess = 15;
            nodeModels->getMaterial ( 1 ).DiffuseColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 1 ).SpecularColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,255,255,255 );


            nodeModels->setRotation ( core::vector3df ( -90 + a*11.25,-90,0 ) );
            nodeModels->setParent ( EM_Reference );
            nodeModels->setName ( "HEC_LArg" );

        }

        //Load HEC Larg Modules B



        scene::IMesh* HEC_B = Device->getSceneManager()->getMesh ( "HEC_LArg02.x" )->getMesh ( 0 );

        for ( int a = 0 ; a<32  ; a++ )
        {

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( HEC_B );

            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 0 ).SpecularColor.set ( 0,255,255,255 );


            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );

            //reflective material
            nodeModels->getMaterial ( 1 ).setTexture ( 1, reflex );
            nodeModels->getMaterial ( 1 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
            nodeModels->getMaterial ( 1 ).Shininess = 15;
            nodeModels->getMaterial ( 1 ).DiffuseColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 1 ).SpecularColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,255,255,255 );

            nodeModels->setRotation ( core::vector3df ( -90 + a*11.25,90,0 ) );
            nodeModels->setParent ( EM_Reference );
            nodeModels->setName ( "HEC_LArg" );


            // Other side

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( HEC_B );

            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 0 ).SpecularColor.set ( 0,255,255,255 );


            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );

            //reflective material
            nodeModels->getMaterial ( 1 ).setTexture ( 1, reflex );
            nodeModels->getMaterial ( 1 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
            nodeModels->getMaterial ( 1 ).Shininess = 15;
            nodeModels->getMaterial ( 1 ).DiffuseColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 1 ).SpecularColor.set ( 0,255,255,255 );
            nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,255,255,255 );

            nodeModels->setRotation ( core::vector3df ( -90 + a*11.25,-90,0 ) );
            nodeModels->setParent ( EM_Reference );
            nodeModels->setName ( "HEC_LArg" );

        }



        //Load TRT Barrel

        scene::ISceneNode* TRT_Reference = Device->getSceneManager()->addEmptySceneNode();
        TRT_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        TRT_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        TRT_Reference->setParent ( Atlas_Reference );
        TRT_Reference->setName ( "TRT_Reference" );
        scene::ISceneNode* TRTB_Reference = Device->getSceneManager()->addEmptySceneNode();
        TRTB_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        TRTB_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        TRTB_Reference->setParent ( TRT_Reference );
        TRTB_Reference->setName ( "TRTB_Reference" );


        scene::IMesh* TRT_B = Device->getSceneManager()->getMesh ( "TRT_Barrel.x" )->getMesh ( 0 );


        for ( int a = 0 ; a<32  ; a++ )
        {

            //Layer I
            nodeModels = Device->getSceneManager()->addMeshSceneNode ( TRT_B );

            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );

            //nodeModels->setScale(core::vector3df(mscale,mscale,mscale));

            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );
            nodeModels->setRotation ( core::vector3df ( -90 + a*11.25,90,0 ) );
            nodeModels->setParent ( TRTB_Reference );
            nodeModels->setName ( "TRT_Barrel" );



            //Layer II
            nodeModels = Device->getSceneManager()->addMeshSceneNode ( TRT_B );

            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );

            nodeModels->setScale ( core::vector3df ( 1,0.8074,0.8074 ) );

            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );
            nodeModels->setRotation ( core::vector3df ( -90 + a*11.25+5.625,-90,0 ) );
            nodeModels->setParent ( TRTB_Reference );
            nodeModels->setName ( "TRT_Barrel" );



            //Layer III
            nodeModels = Device->getSceneManager()->addMeshSceneNode ( TRT_B );

            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );

            nodeModels->setScale ( core::vector3df ( 1,0.6525,0.6525 ) );

            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );
            nodeModels->setRotation ( core::vector3df ( -90 + a*11.25,90,0 ) );
            nodeModels->setParent ( TRTB_Reference );
            nodeModels->setName ( "TRT_Barrel" );

        }


        //Load ATLAS Toroids

        scene::ISceneNode* Magnets_Reference = Device->getSceneManager()->addEmptySceneNode();
        Magnets_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        Magnets_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        Magnets_Reference->setParent ( Atlas_Reference );
        Magnets_Reference->setName ( "Magnets_Reference" );


        scene::IMesh* Toroid = Device->getSceneManager()->getMesh ( "Toroid.X" )->getMesh ( 0 );


        for ( int a = 0 ; a<8 ; a++ )
        {
            nodeModels = Device->getSceneManager()->addMeshSceneNode ( Toroid );

            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,180 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,180 );
            nodeModels->getMaterial ( 0 ).setTexture ( 1, Device->getVideoDriver()->getTexture ( "lakerem4.bmp" ) );
            nodeModels->getMaterial ( 0 ).Shininess = 1;

            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_REFLECTION_2_LAYER );


            nodeModels->setRotation ( core::vector3df ( 90 + a*45 ,-90 ,0 ) );
            nodeModels->setParent ( Magnets_Reference );
            nodeModels->setName ( "Toroid_Magnet" );

        }


        //Load SCT Barrel

        scene::ISceneNode* SCT_Reference = Device->getSceneManager()->addEmptySceneNode();
        SCT_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        SCT_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        SCT_Reference->setParent ( Atlas_Reference );
        SCT_Reference->setName ( "SCT_Reference" );
        scene::ISceneNode* SCTB_Reference = Device->getSceneManager()->addEmptySceneNode();
        SCTB_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        SCTB_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        SCTB_Reference->setParent ( SCT_Reference );
        SCTB_Reference->setName ( "SCTB_Reference" );


        scene::IMesh* SCT_L1 = Device->getSceneManager()->getMesh ( "SCT_Barrel_L1.X" )->getMesh ( 0 );
        scene::IMesh* SCT_L2 = Device->getSceneManager()->getMesh ( "SCT_Barrel_L2.X" )->getMesh ( 0 );
        scene::IMesh* SCT_L3 = Device->getSceneManager()->getMesh ( "SCT_Barrel_L3.X" )->getMesh ( 0 );
        scene::IMesh* SCT_L4 = Device->getSceneManager()->getMesh ( "SCT_Barrel_L4.X" )->getMesh ( 0 );
        float MDistance = 12;

        for ( int d = 0; d<12; d++ ) //The 12 rings per layer of the SCT barrel
        {
            /** Layer 1 */

            for ( int a = 0 ; a<32 ; a++ )
            {

                nodeModels = Device->getSceneManager()->addMeshSceneNode ( SCT_L1 );
                nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
                nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
                nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
                //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
                nodeModels->setPosition ( core::vector3df ( 0,0, ( d-5.5 ) *MDistance ) );
                nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*9 ) );
                nodeModels->setParent ( SCTB_Reference );
                nodeModels->setName ( "SCT_Barrel_L1" );

            }


            /** Layer 2 */

            for ( int a = 0 ; a<40 ; a++ )
            {

                nodeModels = Device->getSceneManager()->addMeshSceneNode ( SCT_L2 );
                nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
                nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
                nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
                //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
                nodeModels->setPosition ( core::vector3df ( 0,0, ( d-5.5 ) *MDistance ) );
                nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*9 ) );
                nodeModels->setParent ( SCTB_Reference );
                nodeModels->setName ( "SCT_Barrel_L2" );

            }

            /** Layer 3 */

            for ( int a = 0 ; a<48 ; a++ )
            {

                nodeModels = Device->getSceneManager()->addMeshSceneNode ( SCT_L3 );
                nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
                nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
                nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
                //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
                nodeModels->setPosition ( core::vector3df ( 0,0, ( d-5.5 ) *MDistance ) );
                nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*9 ) );
                nodeModels->setParent ( SCTB_Reference );
                nodeModels->setName ( "SCT_Barrel_L3" );

            }

            /** Layer 4 */

            for ( int a = 0 ; a<56 ; a++ )
            {

                nodeModels = Device->getSceneManager()->addMeshSceneNode ( SCT_L4 );
                nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
                nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
                nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
                //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
                nodeModels->setPosition ( core::vector3df ( 0,0, ( d-5.5 ) *MDistance ) );
                nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*9 ) );
                nodeModels->setParent ( SCTB_Reference );
                nodeModels->setName ( "SCT_Barrel_L4" );

            }

        }

        //Load SCT Disks



        scene::IMesh* SCT_Disks = Device->getSceneManager()->getMesh ( "SCT_Disks.X" )->getMesh ( 0 );
        float DivAng = 360.0f/13.0f;

        for ( int a = 0 ; a<13 ; a++ )
        {

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( SCT_Disks );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );
            //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*DivAng ) );
            nodeModels->setParent ( SCT_Reference );
            nodeModels->setName ( "SCT_Disks" );


        }

        //Load Pixels Tube

        /** Tube section */

        scene::ISceneNode* Pixels_Reference = Device->getSceneManager()->addEmptySceneNode();
        Pixels_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        Pixels_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        Pixels_Reference->setParent ( Atlas_Reference );
        Pixels_Reference->setName ( "Pixels_Reference" );
        scene::ISceneNode* PixelsB_Reference = Device->getSceneManager()->addEmptySceneNode();
        PixelsB_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        PixelsB_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        PixelsB_Reference->setParent ( Pixels_Reference );
        PixelsB_Reference->setName ( "PixelsB_Reference" );


        scene::IMesh* TubeSection = Device->getSceneManager()->getMesh ( "TubeSection.X" )->getMesh ( 0 );

        for ( int a = 0 ; a<4 ; a++ )
        {

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( TubeSection );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialFlag ( video::EMF_BILINEAR_FILTER, false );
            nodeModels->setMaterialFlag ( video::EMF_TRILINEAR_FILTER, false );
            nodeModels->setMaterialFlag ( video::EMF_ANISOTROPIC_FILTER, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );
            //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*90 ) );
            nodeModels->setParent ( Pixels_Reference );
            nodeModels->setName ( "Pixels_Tube" );


            /** Internal Tube */
            nodeModels = Device->getSceneManager()->addMeshSceneNode ( TubeSection );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setScale ( core::vector3df ( 1,0.995,0.995 ) );
            //nodeModels->setMaterialType(video::EMT_LIGHTMAP_ADD);
            nodeModels->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "PixelTubeInternal.png" ) );
            nodeModels->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*90 ) );
            nodeModels->setParent ( Pixels_Reference );
            nodeModels->setName ( "Pixels_Tube" );


        }

        /** Carbon Fiber Rings */


        scene::IMesh* RingsTubeInt = Device->getSceneManager()->getMesh ( "RingsPixelsTube.X" )->getMesh ( 0 );

        for ( int a = 0 ; a<3 ; a++ )
        {

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( RingsTubeInt );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            //nodeModels->getMaterial(0).EmissiveColor.set(200,180,180,120);
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );
            nodeModels->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false ); //double sided mesh
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*120+30 ) );
            nodeModels->setParent ( Pixels_Reference );
            nodeModels->setName ( "Pixels_Rings" );


        }

        /** Metalic Rings */


        scene::IMesh* MRingsTube = Device->getSceneManager()->getMesh ( "MRingsPixTube.X" )->getMesh ( 0 );

        for ( int a = 0 ; a<2 ; a++ )
        {

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( MRingsTube );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_LIGHTMAP_ADD );
            nodeModels->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false ); //double sided mesh
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*180 ) );
            nodeModels->setParent ( Pixels_Reference );
            nodeModels->setName ( "Pixels_Rings" );


        }

        //Load Pixels Frame Barrel

        /** Part I */


        scene::IMesh* PixFrm01 = Device->getSceneManager()->getMesh ( "Pixels_Frame_Barrel.X" )->getMesh ( 0 );

        for ( int a = 0 ; a<4 ; a++ )
        {

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( PixFrm01 );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
            nodeModels->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*90+22.5 ) );
            nodeModels->setParent ( Pixels_Reference );
            nodeModels->setName ( "Pixels_Frame_Barrel" );


            /** Part II */

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( PixFrm01 );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
            nodeModels->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
            nodeModels->setRotation ( core::vector3df ( 0 ,90 ,0 + a*90-22.5 ) );
            nodeModels->setParent ( Pixels_Reference );
            nodeModels->setName ( "Pixels_Frame_Barrel" );



        }

        //Load Pixels Frame EC


        scene::IMesh* PixFrm02 = Device->getSceneManager()->getMesh ( "Pixels_Frame_EC.X" )->getMesh ( 0 );

        for ( int a = 0 ; a<4 ; a++ )
        {
            /** Part I Side I*/

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( PixFrm02 );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setScale ( core::vector3df ( 1,1,1 ) );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
            nodeModels->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*90-22.5 ) );
            nodeModels->setParent ( Pixels_Reference );
            nodeModels->setName ( "Pixels_Frame_EC" );


            /** Part II Side I*/

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( PixFrm02 );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setScale ( core::vector3df ( 1,-1,1 ) );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
            nodeModels->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*90+22.5 ) );
            nodeModels->setParent ( Pixels_Reference );
            nodeModels->setName ( "Pixels_Frame_EC" );


            /** Part I Side II*/

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( PixFrm02 );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setScale ( core::vector3df ( -1,1,1 ) );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
            nodeModels->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*90-22.5 ) );
            nodeModels->setParent ( Pixels_Reference );
            nodeModels->setName ( "Pixels_Frame_EC" );


            /** Part II Side II*/

            nodeModels = Device->getSceneManager()->addMeshSceneNode ( PixFrm02 );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 200,180,180,120 );
            nodeModels->setScale ( core::vector3df ( -1,-1,1 ) );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
            nodeModels->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*90+22.5 ) );
            nodeModels->setParent ( Pixels_Reference );
            nodeModels->setName ( "Pixels_Frame_EC" );



        }


        //Load Pixels Barrel




        scene::IMesh* Px_L1 = Device->getSceneManager()->getMesh ( "Pixel_Barrel_L1.X" )->getMesh ( 0 );
        scene::IMesh* Px_L2 = Device->getSceneManager()->getMesh ( "Pixel_Barrel_L2.X" )->getMesh ( 0 );
        scene::IMesh* Px_L3 = Device->getSceneManager()->getMesh ( "Pixel_Barrel_L3.X" )->getMesh ( 0 );
        scene::IMesh* Px_L1b = Device->getSceneManager()->getMesh ( "Pixel_Barrel_L1b.X" )->getMesh ( 0 );
        scene::IMesh* Px_L2b = Device->getSceneManager()->getMesh ( "Pixel_Barrel_L2b.X" )->getMesh ( 0 );
        scene::IMesh* Px_L3b = Device->getSceneManager()->getMesh ( "Pixel_Barrel_L3b.X" )->getMesh ( 0 );
        scene::IMesh* Px_L1C = Device->getSceneManager()->getMesh ( "Pixel_Barrel_L1_Center.X" )->getMesh ( 0 );
        scene::IMesh* Px_L2C = Device->getSceneManager()->getMesh ( "Pixel_Barrel_L2_Center.X" )->getMesh ( 0 );
        scene::IMesh* Px_L3C = Device->getSceneManager()->getMesh ( "Pixel_Barrel_L3_Center.X" )->getMesh ( 0 );
        scene::IMesh* Px_Ref = Device->getSceneManager()->getMesh ( "Pixel_Barrel_Reference.X" )->getMesh ( 0 );
        float MPDistance = 6;

        /** Center */

        /** Layer 1 */
        for ( int a = 0 ; a<22 ; a++ )
        {
            nodeModels = Device->getSceneManager()->addMeshSceneNode ( Px_L1C );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,122,122,122 );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
            //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*16.3636 ) );
            nodeModels->setParent ( PixelsB_Reference );
            nodeModels->setName ( "Pixels_Barrel_L1" );

        }

        /** Layer 2 */
        for ( int a = 0 ; a<38 ; a++ )
        {
            nodeModels = Device->getSceneManager()->addMeshSceneNode ( Px_L2C );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,122,122,122 );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
            //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*9.4737 ) );
            nodeModels->setParent ( PixelsB_Reference );
            nodeModels->setName ( "Pixels_Barrel_L2" );

        }

        /** Layer 3 */
        for ( int a = 0 ; a<52 ; a++ )
        {
            nodeModels = Device->getSceneManager()->addMeshSceneNode ( Px_L3C );
            nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
            nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,122,122,122 );
            nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
            nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
            //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
            nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*6.923077 ) );
            nodeModels->setParent ( PixelsB_Reference );
            nodeModels->setName ( "Pixels_Barrel_L3" );

        }


        /** Sides */
        for ( int d = 0; d<6; d++ ) //The 6 tilted rings of modules per layer of the Pixels barrel on each side
        {
            /** Layer 1 */

            for ( int a = 0 ; a<22 ; a++ )
            {
                /** First side */
                nodeModels = Device->getSceneManager()->addMeshSceneNode ( Px_L1 );
                nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,122,122,122 );
                nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
                nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
                //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
                nodeModels->setPosition ( core::vector3df ( 0,0, ( d-6 ) *MPDistance ) );
                nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*16.3636 ) );
                nodeModels->setParent ( PixelsB_Reference );
                nodeModels->setName ( "Pixels_Barrel_L1" );


                /** Second side*/
                nodeModels = Device->getSceneManager()->addMeshSceneNode ( Px_L1b );
                nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,122,122,122 );
                nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
                nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
                //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
                nodeModels->setPosition ( core::vector3df ( 0,0, ( d+1 ) *MPDistance ) );
                nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*16.3636 ) );
                nodeModels->setParent ( PixelsB_Reference );
                nodeModels->setName ( "Pixels_Barrel_L1" );


            }


            /** Layer 2 */

            for ( int a = 0 ; a<38 ; a++ )
            {

                /** First side*/
                nodeModels = Device->getSceneManager()->addMeshSceneNode ( Px_L2 );
                nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,122,122,122 );
                nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
                nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
                //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
                nodeModels->setPosition ( core::vector3df ( 0,0, ( d-6 ) *MPDistance ) );
                nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*9.4737 ) );
                nodeModels->setParent ( PixelsB_Reference );
                nodeModels->setName ( "Pixels_Barrel_L2" );


                /** Second side*/
                nodeModels = Device->getSceneManager()->addMeshSceneNode ( Px_L2b );
                nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,122,122,122 );
                nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
                nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
                //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
                nodeModels->setPosition ( core::vector3df ( 0,0, ( d+1 ) *MPDistance ) );
                nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*9.4737 ) );
                nodeModels->setParent ( PixelsB_Reference );
                nodeModels->setName ( "Pixels_Barrel_L2" );

            }

            /** Layer 3 */

            for ( int a = 0 ; a<52 ; a++ )
            {

                /** First side*/
                nodeModels = Device->getSceneManager()->addMeshSceneNode ( Px_L3 );
                nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,122,122,122 );
                nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
                nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
                //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
                nodeModels->setPosition ( core::vector3df ( 0,0, ( d-6 ) *MPDistance ) );
                nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*6.923077 ) );
                nodeModels->setParent ( PixelsB_Reference );
                nodeModels->setName ( "Pixels_Barrel_L1" );


                /** Second side*/
                nodeModels = Device->getSceneManager()->addMeshSceneNode ( Px_L3b );
                nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,180,180,120 );
                nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,122,122,122 );
                nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
                nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
                //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
                nodeModels->setPosition ( core::vector3df ( 0,0, ( d+1 ) *MPDistance ) );
                nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 + a*6.923077 ) );
                nodeModels->setParent ( PixelsB_Reference );
                nodeModels->setName ( "Pixels_Barrel_L3" );

            }
        }


        //Load Pixels Disks


        scene::IMesh* Px_Dsks = Device->getSceneManager()->getMesh ( "Pixel_Disks.X" )->getMesh ( 0 );

        nodeModels = Device->getSceneManager()->addMeshSceneNode ( Px_Dsks );
        nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 255,222,222,222 );
        nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,222,222,222 );
        nodeModels->getMaterial ( 1 ).EmissiveColor.set ( 0,222,222,222 );
        //nodeModels->getMaterial(2).EmissiveColor.set(0,122,122,122);
        //nodeModels->setMaterialFlag(video::EMF_LIGHTING, false);
        nodeModels->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
        nodeModels->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false ); //double sided mesh
        nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,0 ) );
        nodeModels->setParent ( Pixels_Reference );
        nodeModels->setName ( "Pixels_Disks" );


        //Load ATLAS Shields

        scene::ISceneNode* Shields_Reference = Device->getSceneManager()->addEmptySceneNode();
        Shields_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        Shields_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        Shields_Reference->setParent ( Atlas_Reference );
        Shields_Reference->setName ( "Shields_Reference" );

        //Irr->GetDevice()->getFileSystem()->addZipFileArchive("media/JT.pk3");
        scene::IMesh* jt_shd = Device->getSceneManager()->getMesh ( "JT.X" )->getMesh ( 0 );

        nodeModels = Device->getSceneManager()->addMeshSceneNode ( jt_shd );
        nodeModels->getMaterial ( 0 ).DiffuseColor.set ( 255,222,222,222 );
        nodeModels->getMaterial ( 0 ).EmissiveColor.set ( 0,111,111,111 );
        nodeModels->setMaterialFlag ( video::EMF_LIGHTING, true );
        nodeModels->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
        nodeModels->setMaterialType ( video::EMT_SOLID );
        //nodeModels->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); //double sided mesh
        nodeModels->setRotation ( core::vector3df ( 0 ,-90 ,-90 ) );
        nodeModels->setParent ( Shields_Reference );
        nodeModels->setName ( "Shielding_JT" );
        nodeModels->setID ( 4 );



        //Load ATLAS BigWheels

        scene::IMesh* bw = Device->getSceneManager()->getMesh ( "ATLAS_BigWheels.obj" )->getMesh ( 0 );

        Device->getSceneManager()->getMeshManipulator()->makePlanarTextureMapping ( bw, 0.0003f );

        scene::ISceneNode* nodee = 0;

        //TODO Don't forget to hide this by adding it to Atlas_Reference
        nodee = Device->getSceneManager()->addMeshSceneNode ( bw );
        nodee->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "square2.bmp" ) );
        nodee->getMaterial ( 0 ).EmissiveColor.set ( 255,0,45,105 );

        nodee->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
        nodee->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
        nodee->setRotation ( core::vector3df ( 0,90,0 ) );

        //Load ATLAS EWC

        scene::IMesh* ewc = Device->getSceneManager()->getMesh ( "ATLAS_EWC.obj" )->getMesh ( 0 );

        Device->getSceneManager()->getMeshManipulator()->makePlanarTextureMapping ( ewc, 0.0003f );

        scene::ISceneNode* nodef = 0;

        //TODO Don't forget to hide this Atlas_Reference
        nodef = Device->getSceneManager()->addMeshSceneNode ( ewc );
        nodef->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "square2.bmp" ) );
        nodef->getMaterial ( 0 ).EmissiveColor.set ( 0,0,45,105 );

        nodef->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, false );
        nodef->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
        nodef->setRotation ( core::vector3df ( 0,90,0 ) );



        //Load ATLAS Muon chambers

        scene::ISceneNode* Muons_Reference = Device->getSceneManager()->addEmptySceneNode();
        Muons_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        Muons_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        Muons_Reference->setParent ( Atlas_Reference );
        Muons_Reference->setName ( "Muons_Reference" );

        core::vector3df pos;
        core::vector3df rot;
        core::vector3df scale;

        float theta = 0;

        float c = .1;
        float bx = 0;
        float by = 0;

        int ii = 0;
        int k = 0;

        //loop over theta (in radians) to generate the two outer chambers
        //each iteration draws two boxes, outer box followed by inner box
        //rotation angle is converted from radians to degrees

        float zCenter1[3][12]={ {-11420,-9480,-7300,-5360,-3420,-1240,1600,3420,5360,7300,9480,11420},
            {-8420,-7080,-5860,-4400,-2700,-1000,1360,2700,4400,5860,7080,8420},
            {-6020,-5010,-4000,-2810,-1800,-790,790,1800,2810,4000,5010,6020}
        };

        float zLength1[3][12]={ {1696.9,2177.5,2177.5,1696.9,2177.5,2177.5,1456.7,2177.5,1696.9,2177.5,2177.5,1696.9},
            {1456.9,1216.4,1216.4,1696.9,1696.9,1696.9,967.1,1696.9,1696.9,1216.4,1216.4,1456.7},
            {1096.2,916,1096.2,916,1096.2,916,916,1096.2,916,1096.2,916,1096.2}
        } ;

        float zCenter2[3][14]={ {-11880,-9820,-7640,-5460,-3280,-1100,1100,3280,5460,7640,9820,11880,0,0},
            {-8635,-7415,-5870,-4410,-2580,-1000,1000,2605,4410,5860,7405,8635,0,0},
            {-6260,-5250,-4330,-3410,-2490,-1570,-560,560,1570,2490,3410,4330,5250,6260}
        };

        float zLength2[3][14]={ {1937.2,2177.5,2177.5,2177.5,2177.5,2177.5,2177.5,2177.5,2177.5,2177.5,2177.5,1937.2,0,0},
            {1456.7,976.1,1456.7,1456.7,1456.7,1696.9,1696.9,1456.7,1456.7,1456.7,976.1,1456.7,0,0},
            {1096.2,916,916,916,916,916,1096.2,1096.2,916,916,916,916,916,1096.2}
        } ;

        for ( theta = 0; theta < 2* ( core::PI ); theta += ( core::PI ) /8 )
        {
            //create sector 1: rings 2, 4, 5 (starting from the outside)
            if ( ! ( ii%2 ) )
            {

                for ( k = 0; k < 12; k ++ )
                {
                    //ring 2

                    scale = core::vector3df ( 5090 * c, 636 * c, zLength1[0][k] * c );
                    pos = core::vector3df ( 9477*cos ( theta ) * c + bx, 9477*sin ( theta ) * c + by, zCenter1[0][k] * c );
                    rot = core::vector3df ( 0, 0, 90 + ( theta * 180 ) / ( core::PI ) );
                    node = Device->getSceneManager()->addCubeSceneNode ( 1.0f, 0, -1, pos, rot, scale );


                    //node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
                    node->getMaterial ( 0 ).setTexture ( 1, Device->getVideoDriver()->getTexture ( "refmap2.jpg" ) );
                    node->getMaterial ( 0 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
                    node->getMaterial ( 0 ).Shininess = 44;
                    node->getMaterial ( 0 ).DiffuseColor.set ( 0,255,255,255 );
                    node->getMaterial ( 0 ).EmissiveColor.set ( 0,222,222,222 );
                    node->getMaterial ( 0 ).TextureLayer[1].TextureWrap = ETC_MIRROR;

                    node->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tile.png" ) );
                    node->setParent ( Muons_Reference );
                    node->setName ( "MuonSpectrometer" );


                    //ring 4
                    scale = core::vector3df ( 3700 * c, 667 * c, zLength1[1][k] * c );
                    pos = core::vector3df ( 7116*cos ( theta ) * c + bx, 7116*sin ( theta ) * c + by, zCenter1[1][k] * c );
                    rot = core::vector3df ( 0, 0, 90 + ( theta * 180 ) / ( core::PI ) );
                    node = Device->getSceneManager()->addCubeSceneNode ( 1.0f, 0, -1, pos, rot, scale );



                    //node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
                    node->getMaterial ( 0 ).setTexture ( 1, Device->getVideoDriver()->getTexture ( "refmap2.jpg" ) );
                    node->getMaterial ( 0 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
                    node->getMaterial ( 0 ).Shininess = 44;
                    node->getMaterial ( 0 ).DiffuseColor.set ( 0,255,255,255 );
                    node->getMaterial ( 0 ).EmissiveColor.set ( 0,222,222,222 );
                    node->getMaterial ( 0 ).TextureLayer[1].TextureWrap = ETC_MIRROR;


                    node->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tile.png" ) );
                    node->setParent ( Muons_Reference );
                    node->setName ( "MuonSpectrometer" );


                    //ring 5
                    scale = core::vector3df ( 2820 * c, 416 * c, zLength1[2][k] * c );
                    pos = core::vector3df ( 4926*cos ( theta ) * c + bx, 4926*sin ( theta ) * c + by, zCenter1[2][k] * c );
                    rot = core::vector3df ( 0, 0, 90 + ( theta * 180 ) / ( core::PI ) );
                    node = Device->getSceneManager()->addCubeSceneNode ( 1.0f, 0, -1, pos, rot, scale );

                    //node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
                    node->getMaterial ( 0 ).setTexture ( 1, Device->getVideoDriver()->getTexture ( "refmap2.jpg" ) );
                    node->getMaterial ( 0 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
                    node->getMaterial ( 0 ).Shininess = 44;
                    node->getMaterial ( 0 ).DiffuseColor.set ( 0,255,255,255 );
                    node->getMaterial ( 0 ).EmissiveColor.set ( 0,222,222,222 );
                    node->getMaterial ( 0 ).TextureLayer[1].TextureWrap = ETC_MIRROR;

                    node->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tile.png" ) );
                    node->setParent ( Muons_Reference );
                    node->setName ( "MuonSpectrometer" );

                }
            }

            //create rings 1, 3, 6
            else
            {
                for ( k = 1; k < 11; k++ )
                {
                    //ring 1
                    scale = core::vector3df ( 3900 * c, 647 * c, zLength2[0][k] * c );
                    pos = core::vector3df ( 10544*cos ( theta ) * c + bx, 10544*sin ( theta ) * c + by, zCenter2[0][k] * c );
                    rot = core::vector3df ( 0,0,90 + ( theta * 180 ) / ( core::PI ) );
                    node = Device->getSceneManager()->addCubeSceneNode ( 1.0f, 0,-1, pos, rot, scale );

                    //node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
                    node->getMaterial ( 0 ).setTexture ( 1, Device->getVideoDriver()->getTexture ( "refmap2.jpg" ) );
                    node->getMaterial ( 0 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
                    node->getMaterial ( 0 ).Shininess = 44;
                    node->getMaterial ( 0 ).DiffuseColor.set ( 0,255,255,255 );
                    node->getMaterial ( 0 ).EmissiveColor.set ( 0,222,222,222 );
                    node->getMaterial ( 0 ).TextureLayer[1].TextureWrap = ETC_MIRROR;

                    node->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tile.png" ) );
                    node->setParent ( Muons_Reference );
                    node->setName ( "MuonSpectrometer" );

                    //ring 3
                    scale = core::vector3df ( 3200 * c, 664 * c, zLength2[1][k] * c );
                    pos = core::vector3df ( 8070*cos ( theta ) * c + bx, 8070*sin ( theta ) * c + by, zCenter2[1][k] * c );
                    rot = core::vector3df ( 0,0,90 + ( theta * 180 ) / ( core::PI ) );
                    node = Device->getSceneManager()->addCubeSceneNode ( 1.0f, 0, -1, pos, rot, scale );

                    //node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
                    node->getMaterial ( 0 ).setTexture ( 1, Device->getVideoDriver()->getTexture ( "refmap2.jpg" ) );
                    node->getMaterial ( 0 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
                    node->getMaterial ( 0 ).Shininess = 44;
                    node->getMaterial ( 0 ).DiffuseColor.set ( 0,255,255,255 );
                    node->getMaterial ( 0 ).EmissiveColor.set ( 0,222,222,222 );
                    node->getMaterial ( 0 ).TextureLayer[1].TextureWrap = ETC_MIRROR;

                    node->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tile.png" ) );
                    node->setParent ( Muons_Reference );
                    node->setName ( "MuonSpectrometer" );

                    //ring 6
                    scale = core::vector3df ( 1820 * c, 284 * c, zLength2[2][k] * c );
                    pos = core::vector3df ( 4525*cos ( theta ) * c + bx, 4525*sin ( theta ) * c + by, zCenter2[2][k] * c );
                    rot = core::vector3df ( 0,0,90 + ( theta * 180 ) / ( core::PI ) );
                    node = Device->getSceneManager()->addCubeSceneNode ( 1.0f, 0, -1, pos, rot, scale );

                    //node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
                    node->getMaterial ( 0 ).setTexture ( 1, Device->getVideoDriver()->getTexture ( "refmap2.jpg" ) );
                    node->getMaterial ( 0 ).MaterialType = video::EMT_REFLECTION_2_LAYER;
                    node->getMaterial ( 0 ).Shininess = 44;
                    node->getMaterial ( 0 ).DiffuseColor.set ( 0,255,255,255 );
                    node->getMaterial ( 0 ).EmissiveColor.set ( 0,222,222,222 );
                    node->getMaterial ( 0 ).TextureLayer[1].TextureWrap = ETC_MIRROR;

                    node->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tile.png" ) );
                    node->setParent ( Muons_Reference );
                    node->setName ( "MuonSpectrometer" );
                }

            }
            ii++;
        }


    }
    else
    {
        scene::ISceneNode* TC_Reference = Device->getSceneManager()->addEmptySceneNode();
        TC_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        TC_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        TC_Reference->setParent ( Atlas_Reference );
        TC_Reference->setName ( "TC_Reference" );
        scene::ISceneNode* TCB_Reference = Device->getSceneManager()->addEmptySceneNode();
        TCB_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        TCB_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        TCB_Reference->setParent ( TC_Reference );
        TCB_Reference->setName ( "TCB_Reference" );


        scene::ISceneNode* EM_Reference = Device->getSceneManager()->addEmptySceneNode();
        EM_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        EM_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        EM_Reference->setParent ( Atlas_Reference );
        EM_Reference->setName ( "EMC_Reference" );
        scene::ISceneNode* EMB_Reference = Device->getSceneManager()->addEmptySceneNode();
        EMB_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        EMB_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        EMB_Reference->setParent ( EM_Reference );
        EMB_Reference->setName ( "EMCB_Reference" );


        scene::ISceneNode* TRT_Reference = Device->getSceneManager()->addEmptySceneNode();
        TRT_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        TRT_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        TRT_Reference->setParent ( Atlas_Reference );
        TRT_Reference->setName ( "TRT_Reference" );
        scene::ISceneNode* TRTB_Reference = Device->getSceneManager()->addEmptySceneNode();
        TRTB_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        TRTB_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        TRTB_Reference->setParent ( TRT_Reference );
        TRTB_Reference->setName ( "TRTB_Reference" );


        scene::ISceneNode* Magnets_Reference = Device->getSceneManager()->addEmptySceneNode();
        Magnets_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        Magnets_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        Magnets_Reference->setParent ( Atlas_Reference );
        Magnets_Reference->setName ( "Magnets_Reference" );


        scene::ISceneNode* SCT_Reference = Device->getSceneManager()->addEmptySceneNode();
        SCT_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        SCT_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        SCT_Reference->setParent ( Atlas_Reference );
        SCT_Reference->setName ( "SCT_Reference" );
        scene::ISceneNode* SCTB_Reference = Device->getSceneManager()->addEmptySceneNode();
        SCTB_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        SCTB_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        SCTB_Reference->setParent ( SCT_Reference );
        SCTB_Reference->setName ( "SCTB_Reference" );


        scene::ISceneNode* Pixels_Reference = Device->getSceneManager()->addEmptySceneNode();
        Pixels_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        Pixels_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        Pixels_Reference->setParent ( Atlas_Reference );
        Pixels_Reference->setName ( "Pixels_Reference" );
        scene::ISceneNode* PixelsB_Reference = Device->getSceneManager()->addEmptySceneNode();
        PixelsB_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        PixelsB_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        PixelsB_Reference->setParent ( Pixels_Reference );
        PixelsB_Reference->setName ( "PixelsB_Reference" );


        scene::ISceneNode* Muons_Reference = Device->getSceneManager()->addEmptySceneNode();
        Muons_Reference->setPosition ( core::vector3df ( 0,0,0 ) );
        Muons_Reference->setRotation ( core::vector3df ( 0,0,0 ) );
        Muons_Reference->setParent ( Atlas_Reference );
        Muons_Reference->setName ( "Muons_Reference" );


    }

    ///* Background images for the viewports

    //Irr->GetDevice()->getFileSystem()->addZipFileArchive("media/Background_Front.pk3");
    scene::IAnimatedMesh* bgf = Device->getSceneManager()->getMesh ( "Background_Front.X" );

    background_node_f = Device->getSceneManager()->addAnimatedMeshSceneNode ( bgf, 0, -1, core::vector3df ( 0,0,-240 ), core::vector3df ( 0,90,0 ) );
    background_node_f->getMaterial ( 0 ).DiffuseColor.set ( 255,222,222,222 );
    background_node_f->getMaterial ( 0 ).EmissiveColor.set ( 0,111,111,111 );
    background_node_f->setScale ( core::vector3df ( mscale,mscale,mscale ) );
    background_node_f->setMaterialFlag ( video::EMF_LIGHTING, false );
    background_node_f->setMaterialType ( video::EMT_LIGHTMAP_ADD );
    background_node_f->setID ( 5 );
    background_node_f->setVisible ( false );


    scene::IAnimatedMesh* bgs = Device->getSceneManager()->getMesh ( "Background_Side.X" );

    background_node_s = Device->getSceneManager()->addAnimatedMeshSceneNode ( bgs, 0, -1, core::vector3df ( 400,0,0 ), core::vector3df ( 0,90,90 ) );
    background_node_s->getMaterial ( 0 ).DiffuseColor.set ( 255,222,222,222 );
    background_node_s->getMaterial ( 0 ).EmissiveColor.set ( 0,111,111,111 );
    background_node_s->setScale ( core::vector3df ( mscale,mscale,mscale ) );
    background_node_s->setMaterialFlag ( video::EMF_LIGHTING, false );
    background_node_s->setMaterialType ( video::EMT_LIGHTMAP_ADD );
    background_node_s->setID ( 5 );
    background_node_s->setVisible ( false );




    scene::ISceneNode* nodel = 0;
    float a = 1;
    //nodel = Device->getSceneManager()->addLightSceneNode(0, core::vector3df(1050,750,750),  video::SColorf(a, a, a, a), 850.0f);


    //Irr->GetSceneManager()->saveScene("newscene.irr");




}

struct track* AGeometry::selectTrackByID (int ID, bool multi)
{
    if (!multi) //If we are not doing multiselection, the deselect anything selected
    {
        while (!selectedTracks.isEmpty())
        {
            HelixSceneNode *node=selectedTracks.back();
            node->deselect();
            selectedTracks.pop_back();
            emit trackDeselected(node->getTrack());
        }
    }

    if ( XmlEvt->EventComplete.tracks.size() >2 )
    {
        //Loop through all the tracks...
        for ( std::vector<track>::iterator go = XmlEvt->EventComplete.tracks.begin(); go!=XmlEvt->EventComplete.tracks.end(); go++ )
        {
            track* selectedTrack = &*go;
            if ( selectedTrack->trackID == ID ) //Found it
            {
                selectedTrack->node->select();
                selectedTracks.push_back(selectedTrack->node);
                emit trackSelected( selectedTrack );
                return selectedTrack;
            }
        }
    }
    return NULL;
}

struct track* AGeometry::deselectTrackByID (int ID)
{
    track* tr;
    for (int i=0;i<selectedTracks.size();i++)
    {
        tr=selectedTracks[i]->getTrack();
        if (tr->trackID == ID)
        {
            selectedTracks[i]->deselect();
            emit trackDeselected(tr);
            selectedTracks.removeAt(i);
            return tr;
        }
    }
    return NULL;
}

void AGeometry::grabControl()
{
  if ( active_viewport == AGeometry::Cam3D )
    {
      Device->getSceneManager()->getActiveCamera()->setInputReceiverEnabled ( true );
      setCursor( QCursor( Qt::BlankCursor ) );
      allowTrackSelection=false;
      setFocus();
    }
}

void AGeometry::releaseControl()
{
  if ( active_viewport == AGeometry::Cam3D )
    {
      Device->getSceneManager()->getActiveCamera()->setInputReceiverEnabled ( false );
      setCursor( QCursor( Qt::ArrowCursor ) );
      allowTrackSelection=(XmlEvt->EventComplete.tracks.size()>0); //Only allow track selection if event loaded, which is when number of tracks >0
    }
}

bool AGeometry::OnEvent ( const SEvent& event )
{
    if ( event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN  )
    {
        if ( active_viewport == AGeometry::Cam3D )
        {
            if (allowTrackSelection)
            {
                core::position2di posMouse = core::position2di(event.MouseInput.X,event.MouseInput.Y);
                HelixSceneNode *selected;

                //Do the actual selection
                selected=trackSelection(posMouse);

                //If shifty/ctrly no clicky, then we do not have a multi-track selection and so we deselect everything, but the clicked ray
				bool Shift =((QApplication::keyboardModifiers() & (Qt::ShiftModifier | Qt::ControlModifier)) > 0);
                if (!Shift)
                {
                    while (!selectedTracks.isEmpty())
                    {
                        HelixSceneNode *node=selectedTracks.back();
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
                }
            }

            //This next block is part of an offset test for the Irrlicht ray generator
            if ( offsetTest )
            {

                core::position2di posMouse = Device->getCursorControl()->getPosition();
                line3d<f32> ray = Device->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(posMouse, Device->getSceneManager()->getActiveCamera());
                cube->setPosition ( ray.start + ray.getVector().normalize() * 180.0 ); // put it some distance away
                cube->updateAbsolutePosition();
                cout << "cube X: " <<  cube->getPosition().X << "  cube Y: " <<  cube->getPosition().Y <<endl;
                cout << "pos2D X: " <<  posMouse.X << "  pos2D Y: " <<  posMouse.Y <<endl;
                cout << "RayStart X: " <<  ray.start.X << "  RayStart Y: " <<  ray.start.Y <<endl;
                cout << "RayEnd X: " <<  ray.end.X << "  RayEnd Y: " <<  ray.end.Y <<endl;
                cout << ""  <<endl;
                cout << ""  <<endl;
                cout << ""  <<endl;
                cout << "Width: " <<  width() << "  Heigth: " <<  height() <<endl;
            }
        }
    }


    if ( event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP  )
    {
        //If you clicked an FPS camera and it does not have input, give it input! This allows us to focus the camera by clicking on AGeometry
        if (!Device->getSceneManager()->getActiveCamera()->isInputReceiverEnabled() && !allowTrackSelection && !offsetTest)
        {
            grabControl();
        }
    }

    if ( event.EventType == EET_KEY_INPUT_EVENT )
    {
        if ( event.KeyInput.PressedDown == false )
        {
            switch ( event.KeyInput.Key )
            {

                //toggles moses mode
            case KEY_KEY_M:
                MosesMode = ! ( MosesMode ); //toggles moses mode
                if ( MosesMode ) MosesFreeCalm = false;
                mosesRestore = true;
                return true;
                break;

            case KEY_KEY_P:
                camera[0]->setPosition ( core::vector3df ( 0,0,-1200 ) );
                camera[0]->setTarget ( core::vector3df ( 0,0,0 ) );
                return true;
                break;

            case KEY_KEY_G:
                selectTrackByID ( 11 );
                return true;
                break;

                //toggles camera/cursor control
            case KEY_SPACE:
                if (Device->getSceneManager()->getActiveCamera()->isInputReceiverEnabled()) releaseControl();
                else grabControl();
                return true;
                break;

                //sets PtCutoff to 1GeV
            case KEY_KEY_C:
                XmlEvt->PtCutoff ( 1 );
                return true;
                break;

                //turns on/off slice mode
            case KEY_KEY_U:
                sliceMode = !sliceMode;
                return true;
                break;

                // Toggle detector system visibility

            case KEY_KEY_0:
                switchVisibility ( 0 );
                return true;
                break;

            case KEY_KEY_1:
                switchVisibility ( 1 );
                return true;
                break;


            case KEY_KEY_2:
                switchVisibility ( 2 );
                return true;
                break;


            case KEY_KEY_3:
                switchVisibility ( 3 );
                return true;
                break;

            case KEY_KEY_4:
                switchVisibility ( 4 );
                return true;
                break;

            case KEY_KEY_5:
                switchVisibility ( 5 );
                return true;
                break;

            case KEY_KEY_6:
                switchVisibility ( 6 );
                return true;
                break;

            case KEY_KEY_7:
                switchVisibility ( 7 );
                return true;
                break;
            case KEY_F1:
                qDebug() << "Active Camera " << active_viewport;
                qDebug() << "\tPosition: (" << Device->getSceneManager()->getActiveCamera()->getPosition().X << ","
                << Device->getSceneManager()->getActiveCamera()->getPosition().Y << ","
                << Device->getSceneManager()->getActiveCamera()->getPosition().Z << ")";
                qDebug() << "\tTarget: (" << Device->getSceneManager()->getActiveCamera()->getTarget().X << ","
                << Device->getSceneManager()->getActiveCamera()->getTarget().Y << ","
                << Device->getSceneManager()->getActiveCamera()->getTarget().Z << ")";
                return true;
                break;
            case KEY_F2:
                offsetTest=!offsetTest;
                qDebug() << "offsetTest =" << offsetTest;
                cube->setVisible(offsetTest);
                return true;
            case KEY_F4:
                if ( Device->getSceneManager()->getSceneNodeFromName ( "Atlas_Reference" ) )
                    Device->getSceneManager()->getSceneNodeFromName ( "Atlas_Reference" )->setVisible ( !Device->getSceneManager()->getSceneNodeFromName ( "Atlas_Reference" )->isVisible() );
                return true;
            case KEY_F5:
                setEnabled(!isEnabled());
                return true;
            default:
                //Don't do anything
                break;
            }
        }
    }
    return QIrrWidget::OnEvent(event);
}

//Switch the current camera, viewport clicked
void AGeometry::setViewport(int to)
{
  int camId;
  if(to==AGeometry::Cam3D) camId=active_cam;
  else camId=to;

  if (camId==active_viewport) return; //Already using it

  int from=active_viewport;

  Device->getSceneManager()->setActiveCamera (camera[camId]);

  setupView(to);

  emit viewportSwitched(from,to);
  active_viewport=to;
  renderViewport(from);

  qDebug() << "Active viewport switched from " << from << " to " << to;
}

void AGeometry::setCamera(int to)
{
  if (to==active_cam) return; //Already using it

  switch(to)
    {
    case AGeometry::FPS:
      actSphere->setChecked(false);
      actFPS->setChecked(true);
      sliceMode=false;
      break;
    case AGeometry::Maya:
      actFPS->setChecked(false);
      actSphere->setChecked(true);
      sliceMode=true;
      break;
    }
  camera[to]->setInputReceiverEnabled(camera[active_cam]->isInputReceiverEnabled());


  if(active_viewport==AGeometry::Cam3D)
    {
      cameraSwitcher->setTargetCamera(camera[to]);
    }

  active_cam=to;
  renderViewport(AGeometry::Cam3D);
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

    if ( Device->getSceneManager()->getSceneNodeFromName ( "Atlas_Reference" ) )
        Device->getSceneManager()->getSceneNodeFromName ( "Atlas_Reference" )->setVisible ( fps );

    if ( Device->getSceneManager()->getSceneNodeFromName ( "Pit_Reference" ) )
        Device->getSceneManager()->getSceneNodeFromName ( "Pit_Reference" )->setVisible ( fps );
}

void AGeometry::addCamAnimator (irr::core::array<vector3df> p)
{

    ISceneNodeAnimator *anim = Device->getSceneManager()->createFollowSplineAnimator (0, p, 1);

    //camera[0]->addAnimator (anim);
    cam_node->addAnimator (anim);
    anim->drop ();

    force_target = true;

    cout << "ANIMATOR" << endl;

}



void AGeometry::addTarAnimator (irr::core::array<vector3df> p)
{

    ISceneNodeAnimator *anim = Device->getSceneManager()->createFollowSplineAnimator (0, p, 1);

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
