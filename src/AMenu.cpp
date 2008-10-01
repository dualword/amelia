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

#include "AMenu.h"


AMenu::AMenu(QWidget* parent) : QIrrWidget(parent)
{

    //Initialize some menu variables


    timeCamera = 0;
    zero = core::vector3df ( 0,0,0 );
    disp_p = zero;
    disp_r = zero;
    nodePos_final = zero;
    nodeRot_final = zero;
    nodePos_init = zero;
    nodeRot_init = zero;
    nodeCurrentPos = zero;
    nodeCurrentRot = zero;
    nodeScale = core::vector3df ( 1,1,1 );
    CamPos_final = zero;
    TarPos_final = zero;
    CamPos_init = zero;
    TarPos_init = zero;
    TarPos = zero;
    CamPos = zero;
    ScreenRot = zero;
    ScreenRot_init = zero;
    ScreenRot_final = zero;

    executeOnce = true;
    executeOnceCamera = true;
    timerLogo = true;
    timerM = false;
    timerE = false;
    timerL = false;
    timerI = false;
    timerA = false;
    timerMenu = false;
    timerAway = false;
    timerCamera = true;
    menuSelectionEnabled = false;
    subMenuSelectionEnabled = false;
    menuAnimationOn = false;
    menuAnimationOff = false;
    selectMenuSwitch = false;
    TT_executeOnce = false;
    TT_lock = false;
    TT_isOnType = true;
    TTP_isOnType = true;
    TTAnim = true;
    TTPAnim = false;
    TT_Enabled = false;
    TT_Retract = false;
    TT_SwitchL1 = false;
    TT_Pos = 0;
    TT_Type = 0;
    currentMenuDown = 3;
    currentEnabledPanel = 0;
    SMTT_lock = false;
    SMTTAnim = false;
    SMTT_Enabled = false;
    iconSwitch = false;
    iconTimer = 40;

    menuEmissiveInit = video::SColor ( 255,255,255,255 );
}

AMenu::~AMenu()
{
    //dtor
}

void AMenu::load()
{

    //Set the Menu state pointers to NULL, just in case

    activatedMenu = NULL;
    currentlySelectedMenu = NULL;
    selectedSceneNode = NULL;
    lastSelectedSceneNode = NULL;
    selectedSubMenu = NULL;
    lastSelectedSubMenu = NULL;

    //Load the elements of the menu

    Device->getFileSystem()->addZipFileArchive ( "media/menu.aml" );
    Device->getSceneManager()->loadScene ( "menu01o.lvl" );
    skybox = Device->getSceneManager()->getSceneNodeFromName ( "skybox" );
    nodeLogo = Device->getSceneManager()->getSceneNodeFromName ( "ALogo" );
    nodeLight = Device->getSceneManager()->getSceneNodeFromName ( "Light" );
    nodeMenu01 = Device->getSceneManager()->getSceneNodeFromName ( "Menu01" );
    nodeMenu02 = Device->getSceneManager()->getSceneNodeFromName ( "Menu02" );
    nodeMenu03 = Device->getSceneManager()->getSceneNodeFromName ( "Menu03" );
    nodeA = Device->getSceneManager()->getSceneNodeFromName ( "A" );
    nodeM = Device->getSceneManager()->getSceneNodeFromName ( "M" );
    nodeE = Device->getSceneManager()->getSceneNodeFromName ( "E" );
    nodeI = Device->getSceneManager()->getSceneNodeFromName ( "I" );
    nodeL = Device->getSceneManager()->getSceneNodeFromName ( "L" );
    nodeScreen01 = Device->getSceneManager()->getSceneNodeFromName ( "Screen01" );
    nodeScreen02 = Device->getSceneManager()->getSceneNodeFromName ( "Screen02" );
    nodeScreen03 = Device->getSceneManager()->getSceneNodeFromName ( "Screen03" );

    s1m1 = Device->getSceneManager()->getSceneNodeFromName ( "s1m1" );
    s1m2 = Device->getSceneManager()->getSceneNodeFromName ( "s1m2" );
    s1m3 = Device->getSceneManager()->getSceneNodeFromName ( "s1m3" );
    s1m4 = Device->getSceneManager()->getSceneNodeFromName ( "s1m4" );
    s2m1 = Device->getSceneManager()->getSceneNodeFromName ( "s2m1" );
    s2m2 = Device->getSceneManager()->getSceneNodeFromName ( "s2m2" );
    s2m3 = Device->getSceneManager()->getSceneNodeFromName ( "s2m3" );
    s2m4 = Device->getSceneManager()->getSceneNodeFromName ( "s2m4" );
    s3m1 = Device->getSceneManager()->getSceneNodeFromName ( "s3m1" );
    s3m2 = Device->getSceneManager()->getSceneNodeFromName ( "s3m2" );
    s3m3 = Device->getSceneManager()->getSceneNodeFromName ( "s3m3" );
    s3m4 = Device->getSceneManager()->getSceneNodeFromName ( "s3m4" );


    GS_buttons = Device->getSceneManager()->getSceneNodeFromName ( "GS_buttons" );
    FO_buttons = Device->getSceneManager()->getSceneNodeFromName ( "FO_buttons" );
    TO_buttons = Device->getSceneManager()->getSceneNodeFromName ( "TO_buttons" );


    //initialize the vectors that hold animations for the three screens
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F1.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F2.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F3.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F4.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F5.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F6.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F7.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F8.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F9.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F10.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F11.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F12.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F13.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F14.png" ) );
    GS_animation.push_back ( Device->getVideoDriver()->getTexture ( "GS_F15.png" ) );

    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F1.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F2.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F3.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F4.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F5.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F6.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F7.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F8.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F9.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F10.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F11.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F12.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F13.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F14.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F15.png" ) );
    FO_animation.push_back ( Device->getVideoDriver()->getTexture ( "FO_F16.png" ) );

    TO_animation.push_back ( Device->getVideoDriver()->getTexture ( "TO_F1.png" ) );
    TO_animation.push_back ( Device->getVideoDriver()->getTexture ( "TO_F2.png" ) );
    TO_animation.push_back ( Device->getVideoDriver()->getTexture ( "TO_F3.png" ) );
    TO_animation.push_back ( Device->getVideoDriver()->getTexture ( "TO_F4.png" ) );
    TO_animation.push_back ( Device->getVideoDriver()->getTexture ( "TO_F5.png" ) );
    TO_animation.push_back ( Device->getVideoDriver()->getTexture ( "TO_F6.png" ) );

    //Initialize the vectors for the submenu buttons
    GS_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "GS_M0.png" ) );
    GS_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "GS_M1.png" ) );
    GS_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "GS_M2.png" ) );
    GS_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "GS_M3.png" ) );
    GS_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "GS_M4.png" ) );

    FO_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "FO_M0.png" ) );
    FO_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "FO_M1.png" ) );
    FO_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "FO_M2.png" ) );
    FO_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "FO_M3.png" ) );
    FO_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "FO_M4.png" ) );

    TO_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "TO_M0.png" ) );
    TO_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "TO_M1.png" ) );
    TO_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "TO_M2.png" ) );
    TO_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "TO_M3.png" ) );
    TO_buttonImages.push_back ( Device->getVideoDriver()->getTexture ( "TO_M4.png" ) );

    nodeTT = Device->getSceneManager()->getSceneNodeFromName ( "TT_Plane" );
    nodeTTPanel = Device->getSceneManager()->getSceneNodeFromName ( "TT_Panel" );
    nodeTT_SM = Device->getSceneManager()->getSceneNodeFromName ( "TT_SM" );

    nodeLogo -> setVisible ( false );
    nodeA -> setVisible ( false );
    nodeM -> setVisible ( false );
    nodeE -> setVisible ( false );
    nodeL -> setVisible ( false );
    nodeI -> setVisible ( false );
    nodeMenu01 -> setVisible ( false );
    nodeMenu02 -> setVisible ( false );
    nodeMenu03 -> setVisible ( false );
    nodeTT -> setVisible ( false );
    nodeTTPanel -> setVisible ( false );
    nodeTT_SM -> setVisible ( false );
    skybox -> setVisible ( true );
    //s1m4 -> setVisible(false);

    skybox->setID ( 0 );


    s1m1->setID ( 5 );
    s1m2->setID ( 5 );
    s1m3->setID ( 5 );
    s1m4->setID ( 5 );
    s2m1->setID ( 5 );
    s2m2->setID ( 5 );
    s2m3->setID ( 5 );
    s2m4->setID ( 5 );
    s3m1->setID ( 5 );
    s3m2->setID ( 5 );
    s3m3->setID ( 5 );
    s3m4->setID ( 5 );

    GS_buttons->setID ( 64 );
    FO_buttons->setID ( 64 );
    TO_buttons->setID ( 64 );

    s1m1->getMaterial ( 0 ).EmissiveColor = menuEmissiveInit;
    s1m2->getMaterial ( 0 ).EmissiveColor = menuEmissiveInit;
    s1m3->getMaterial ( 0 ).EmissiveColor = menuEmissiveInit;
    s1m4->getMaterial ( 0 ).EmissiveColor = menuEmissiveInit;
    s2m1->getMaterial ( 0 ).EmissiveColor = menuEmissiveInit;
    s2m2->getMaterial ( 0 ).EmissiveColor = menuEmissiveInit;
    s2m3->getMaterial ( 0 ).EmissiveColor = menuEmissiveInit;
    s2m4->getMaterial ( 0 ).EmissiveColor = menuEmissiveInit;
    s3m1->getMaterial ( 0 ).EmissiveColor = menuEmissiveInit;
    s3m2->getMaterial ( 0 ).EmissiveColor = menuEmissiveInit;
    s3m3->getMaterial ( 0 ).EmissiveColor = menuEmissiveInit;
    s3m4->getMaterial ( 0 ).EmissiveColor = menuEmissiveInit;

    nodeMenu01 -> setID ( 16 );
    nodeMenu02 -> setID ( 16 );
    nodeMenu03 -> setID ( 16 );
    nodeScreen01 -> setID ( 1 );
    nodeScreen02 -> setID ( 2 );
    nodeScreen03 -> setID ( 3 );

    nodeLogo -> setID ( 0 );

    //Create the dynamic camera and define some variables
    camera = Device->getSceneManager()->addCameraSceneNode();
    camera->setPosition ( core::vector3df ( -10,000,-200 ) );
    camera->setTarget ( core::vector3df ( 100,0,0 ) );
    camera->setFarValue ( 22000.0f );
    //camera[0]->setAspectRatio(0.8/0.6);
    camera->setID ( 0 );

    colorMenuOrig = nodeMenu01->getMaterial ( 0 ).SpecularColor;
    colorMenuSelect = video::SColor ( 0,255,128,0 );


    ISceneNodeAnimator* animBG=Device->getSceneManager()->createRotationAnimator ( vector3df ( 0,0.3,0 ) );
    skybox->addAnimator ( animBG );
    animBG->drop();

    levelChange = 0;

//    AZilla* testZilla = new AZilla(Device->getVideoDriver(), Device);

}


void AMenu::animateMenuOff ( scene::ISceneNode* nodeMenu, scene::ICameraSceneNode* camera )
{
    nodeScale = nodeMenu->getScale();
    core::vector3df nodeScale_init = core::vector3df ( 1.6,1,1.6 );
    core::vector3df nodeScale_final = core::vector3df ( 1,1,1 );
    time1 = Device->getTimer()->getRealTime();
    core::list<scene::ISceneNode*>::ConstIterator ChildIt = nodeMenu->getChildren().begin();



    if ( executeOnce )
    {
        TTPanelEnable = false;
        panelAction ( nodeMenu );
        time0 = Device->getTimer()->getRealTime();
        nodePos_init = nodeMenu->getPosition();
        CamPos_init = camera->getPosition();
        CamPos_final = camera->getPosition() + core::vector3df ( 0,45,0 );
        CamPos = CamPos_init;
        TarPos_init = camera->getTarget();
        TarPos_final = TarPos_init + core::vector3df ( 0,45,0 );
        TarPos = TarPos_init;
        nodePos_final = core::vector3df ( 0,0,0 );
        nodeCurrentPos = nodePos_init;

        ScreenRot_init = ( *ChildIt )->getRotation();
        ScreenRot_final = ScreenRot_init - core::vector3df ( 0,0,180 );
        ScreenRot = ScreenRot_init;
        executeOnce = false;
        menuSelectionEnabled = false;
        subMenuSelectionEnabled = false;
    }

    timeLapse = 1000;
    timeDelay = 0;

    if ( time1 - time0 < timeLapse + timeDelay ) // && time1 - time0 > timeDelay )
    {
        nodeScale = nodeScale_init + ( nodeScale_final-nodeScale_init ) * ( time1 - time0 - timeDelay ) / ( timeLapse );
        nodeCurrentPos = nodePos_init + ( nodePos_final-nodePos_init ) * ( time1 - time0 - timeDelay ) / ( timeLapse );
        CamPos = CamPos_init + ( CamPos_final-CamPos_init ) * ( time1 - time0 - timeDelay ) / ( timeLapse );
        TarPos = TarPos_init + ( TarPos_final-TarPos_init ) * ( time1 - time0 - timeDelay ) / ( timeLapse );
        ScreenRot = ScreenRot_init + ( ScreenRot_final-ScreenRot_init ) * ( time1 - time0 - timeDelay ) / ( timeLapse );
    }


    if ( time1 - time0 > timeLapse + timeDelay )
    {


        nodeScale = nodeScale_final;
        nodeCurrentPos = nodePos_final;
        CamPos = CamPos_final;
        TarPos = TarPos_final;
        ScreenRot = ScreenRot_final;
        executeOnce = true;
        menuSelectionEnabled = true;
        menuAnimationOff = false;
        currentlySelectedMenu = NULL;
    }

    nodeMenu->setScale ( nodeScale );
    nodeMenu->setPosition ( nodeCurrentPos );
    camera->setPosition ( CamPos );
    camera->setTarget ( TarPos );
    ( *ChildIt )->setRotation ( ScreenRot );
}

void AMenu::execute()
{

    //menu level here

    if ( timerCamera )
    {
        if ( executeOnceCamera && timeCamera == 0 )

        {
            time0Camera = Device->getTimer()->getRealTime();
            CamPos_final = core::vector3df ( 170,0,-320 );
            TarPos_final = core::vector3df ( 170,0,0 );
            CamPos_init = core::vector3df ( -10,000,-200 );
            TarPos_init = core::vector3df ( 0,0,0 );
            CamPos = CamPos_init;
            TarPos = TarPos_init;

            timeLapseCamera = 4000;
            timeDelayCamera = 3000;

            executeOnceCamera = false;
        }

        timeCamera = Device->getTimer()->getRealTime();
        camera->setPosition ( CamPos );
        camera->setTarget ( TarPos );
        camera->updateAbsolutePosition();

        if ( timeCamera - time0Camera < timeLapseCamera + timeDelayCamera && timeCamera - time0Camera > timeDelayCamera )
        {
            CamPos = CamPos_init + ( CamPos_final - CamPos_init ) * ( timeCamera - time0Camera - timeDelayCamera ) / ( timeLapseCamera );
            TarPos = TarPos_init + ( TarPos_final - TarPos_init ) * ( timeCamera - time0Camera - timeDelayCamera ) / ( timeLapseCamera );
        }

        if ( timeCamera - time0Camera < 12000 )
        {
            initialAnimationP1();
        }

        if ( startMenu )
        {
            menuSetupAnimation();
        }

        if ( timerMenu )
        {
            initialAnimationP3();
        }

    }

    core::position2di pos = Device->getCursorControl()->getPosition();
    pos.Y = pos.Y + 27;

    if ( subMenuSelectionEnabled )
    {
        selectedSubMenu = Device->getSceneManager()->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB ( pos, 5 );

        if ( selectedSubMenu == 0 || selectedSubMenu->getID() != 5 )
        {
            changeSubMenu ( "" );
        }

        if ( lastSelectedSubMenu )
        {
            menuSelectionEnabled = true;
            subMenuTTAction ( selectedSubMenu, false );
        }

        if ( selectedSubMenu && selectedSubMenu->getParent()->getParent() == activatedMenu )
        {
            lastSelectedSubMenu = selectedSubMenu;
            changeSubMenu ( lastSelectedSubMenu->getName() );
            menuSelectionEnabled = false;
            subMenuTTAction ( selectedSubMenu, true );

            /*			if ( Base->receiver->Keys[ KEY_LBUTTON ] )
            {
            	if ( lastSelectedSubMenu == s2m4 )
            	{
            		levelChange = 2;
            	}
            	}*/
        }



        /*		if ( selectedSceneNode && selectedSceneNode->getID() == 16 && Base->receiver->Keys[ KEY_LBUTTON ] )
        {
        	activatedMenu = selectedSceneNode;
        	selectMenuSwitch = true;
        	}*/

    }

    if ( menuSelectionEnabled )
    {
        selectedSceneNode = Device->getSceneManager()->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB ( pos, 16 );


        if ( lastSelectedSceneNode && lastSelectedSceneNode->getID() == 16 )
        {
            lastSelectedSceneNode->getMaterial ( 0 ).SpecularColor = colorMenuOrig;
            core::list<scene::ISceneNode*>::ConstIterator ChildIt = lastSelectedSceneNode->getChildren().begin();
            ( *ChildIt )->getMaterial ( 0 ).TextureLayer[0] = ( *ChildIt )->getMaterial ( 0 ).TextureLayer[1]; //Change texture

            if ( TT_Enabled && TT_SwitchL2 ) tooltipAction ( lastSelectedSceneNode, false );
            TT_SwitchL1 = true;
        }


        if ( selectedSceneNode && selectedSceneNode->getID() == 16 && selectedSceneNode != currentlySelectedMenu )
        {
            if ( lastSelectedSceneNode != selectedSceneNode )
            {
                newNode = true;
            }
            else
            {
                newNode = false;
            }

            lastSelectedSceneNode = selectedSceneNode;
            selectedSceneNode->getMaterial ( 0 ).SpecularColor = colorMenuSelect;

            animateMenuIcon();

            TT_SwitchL1 = false;

            if ( !TT_Enabled ) tooltipAction ( selectedSceneNode, true );
            TT_Retract = false;
        }

        if ( TT_SwitchL1 ) TT_SwitchL2 = true;

        /*		if ( selectedSceneNode && selectedSceneNode->getID() == 16 && Base->receiver->Keys[ KEY_LBUTTON ] )
        {
        	activatedMenu = selectedSceneNode;
        	selectMenuSwitch = true;
        	tooltipAction ( activatedMenu, false );
        	}*/

    }

    if ( currentlySelectedMenu && !menuAnimationOff && selectMenuSwitch )
    {
        menuAnimationOff = true;
    }

    if ( selectMenuSwitch && !menuAnimationOff )
    {
        selectMenu ( activatedMenu );
    }

    if ( menuAnimationOff )
    {
        animateMenuOff ( currentlySelectedMenu, camera );
    }

    if ( menuAnimationOn && ( currentlySelectedMenu != lastSelectedSceneNode ) )
    {
        animateMenuOn ( activatedMenu, camera );
    }

    if ( TTAnim )
    {
        if ( TT_isOnType )
        {
            animateMenuTooltipOn ( TT_Pos, TT_Type );
        }
        else
        {
            animateMenuTooltipOff();
        }
    }

    if ( TTPAnim )
    {
        if ( TTP_isOnType )
        {
            panelAnimateOn();
        }
        else
        {
            panelAnimateOff();
        }
    }

    if ( SMTTAnim )
    {
        if ( SMTT_isOnType )
        {
            SMTTAnimateOn();
        }
        else
        {
            SMTTAnimateOff();
        }
    }


}

void AMenu::animateMenuIcon()
{
    timeIcon = Device->getTimer()->getRealTime();
    core::list<scene::ISceneNode*>::ConstIterator ChildIt = selectedSceneNode->getChildren().begin();
    if ( iconSwitch == true )
    {
        timeIcon0 = Device->getTimer()->getRealTime();
        iconSwitch = false;
    }

    if ( newNode )
    {
        if ( ( *ChildIt )->getID() == 1 ) //pointing to "Getting Started"
        {
            curAnimation = GS_animation;
            iconTimer = 40;
        }
        else if ( ( *ChildIt )->getID() == 2 ) //pointing to "Figure_it_out"
        {
            curAnimation = FO_animation;
            iconTimer = 40;
        }
        else
        {
            curAnimation = TO_animation;
            iconTimer = 40;
        }
        iter = curAnimation.begin();
    }

    if ( iter == curAnimation.end() )
    {
        iter = curAnimation.begin();
    }

    ( *ChildIt )->getMaterial ( 0 ).setTexture ( 0, *iter );

    if ( timeIcon - timeIcon0 >= iconTimer )
    {
        iter++;
        iconSwitch = true;
    }

}

void AMenu::changeSubMenu ( const std::string name )
{

    // Get Started SubMenu
    if ( name == "s1m1" )
    {
        GS_buttons->getMaterial ( 0 ).setTexture ( 0, GS_buttonImages.at ( 1 ) );
    }
    else if ( name == "s1m2" )
    {
        GS_buttons->getMaterial ( 0 ).setTexture ( 0, GS_buttonImages.at ( 2 ) );
    }
    else if ( name == "s1m3" )
    {
        GS_buttons->getMaterial ( 0 ).setTexture ( 0, GS_buttonImages.at ( 3 ) );
    }
    else if ( name == "s1m4" )
    {
        GS_buttons->getMaterial ( 0 ).setTexture ( 0, GS_buttonImages.at ( 4 ) );
    }

    // Figure It Out SubMenu
    else if ( name == "s2m1" )
    {
        FO_buttons->getMaterial ( 0 ).setTexture ( 0, FO_buttonImages.at ( 1 ) );
    }
    else if ( name == "s2m2" )
    {
        FO_buttons->getMaterial ( 0 ).setTexture ( 0, FO_buttonImages.at ( 2 ) );
    }
    else if ( name == "s2m3" )
    {
        FO_buttons->getMaterial ( 0 ).setTexture ( 0, FO_buttonImages.at ( 3 ) );
    }
    else if ( name == "s2m4" )
    {
        FO_buttons->getMaterial ( 0 ).setTexture ( 0, FO_buttonImages.at ( 4 ) );
    }

    // Tell Others SubMenu
    else if ( name == "s3m1" )
    {
        TO_buttons->getMaterial ( 0 ).setTexture ( 0, TO_buttonImages.at ( 1 ) );
    }
    else if ( name == "s3m2" )
    {
        TO_buttons->getMaterial ( 0 ).setTexture ( 0, TO_buttonImages.at ( 2 ) );
    }
    else if ( name == "s3m3" )
    {
        TO_buttons->getMaterial ( 0 ).setTexture ( 0, TO_buttonImages.at ( 3 ) );
    }
    else if ( name == "s3m4" )
    {
        TO_buttons->getMaterial ( 0 ).setTexture ( 0, TO_buttonImages.at ( 4 ) );
    }

    // All other cases - no menu selected
    else
    {
        GS_buttons->getMaterial ( 0 ).setTexture ( 0, GS_buttonImages.at ( 0 ) );
        FO_buttons->getMaterial ( 0 ).setTexture ( 0, FO_buttonImages.at ( 0 ) );
        TO_buttons->getMaterial ( 0 ).setTexture ( 0, TO_buttonImages.at ( 0 ) );
    }
}


void AMenu::animateMenuOn ( scene::ISceneNode* nodeMenu, scene::ICameraSceneNode* camera )
{
    nodeScale = nodeMenu->getScale();
    core::vector3df nodeScale_final = core::vector3df ( 1.6,1,1.6 );
    core::vector3df nodeScale_init = core::vector3df ( 1,1,1 );
    time1 = Device->getTimer()->getRealTime();
    core::list<scene::ISceneNode*>::ConstIterator ChildIt = nodeMenu->getChildren().begin();


    if ( executeOnce )
    {
        time0 = Device->getTimer()->getRealTime();
        nodePos_init = nodeMenu01->getPosition();
        CamPos_init = camera->getPosition();
        CamPos_final = camera->getPosition() - core::vector3df ( 0,45,0 );
        CamPos = CamPos_init;
        TarPos_init = camera->getTarget();
        TarPos_final = TarPos_init - core::vector3df ( 0,45,0 );
        TarPos = TarPos_init;
        nodePos_final = nodePos_init + core::vector3df ( 0,25,0 );
        nodeCurrentPos = nodePos_init;

        ScreenRot_init = ( *ChildIt )->getRotation();
        ScreenRot_final = ScreenRot_init + core::vector3df ( 0,0,180 );
        ScreenRot = ScreenRot_init;
        executeOnce = false;
        menuSelectionEnabled = false;
    }

    timeLapse = 1000;
    timeDelay = 0;

    if ( time1 - time0 < timeLapse + timeDelay ) // && time1 - time0 > timeDelay )
    {
        nodeScale = nodeScale_init + ( nodeScale_final-nodeScale_init ) * ( time1 - time0 - timeDelay ) / ( timeLapse );
        nodeCurrentPos = nodePos_init + ( nodePos_final-nodePos_init ) * ( time1 - time0 - timeDelay ) / ( timeLapse );
        CamPos = CamPos_init + ( CamPos_final-CamPos_init ) * ( time1 - time0 - timeDelay ) / ( timeLapse );
        TarPos = TarPos_init + ( TarPos_final-TarPos_init ) * ( time1 - time0 - timeDelay ) / ( timeLapse );
        ScreenRot = ScreenRot_init + ( ScreenRot_final-ScreenRot_init ) * ( time1 - time0 - timeDelay ) / ( timeLapse );
    }

    if ( time1 - time0 > timeLapse + timeDelay )
    {
        //activatedMenu = nodeMenu;
        currentlySelectedMenu = nodeMenu;
        nodeScale = nodeScale_final;
        nodeCurrentPos = nodePos_final;
        CamPos = CamPos_final;
        TarPos = TarPos_final;
        ScreenRot = ScreenRot_final;
        executeOnce = true;
        menuSelectionEnabled = true;
        subMenuSelectionEnabled = true;
        //nodeLogo->setVisible(false);
        selectMenuSwitch = false;
        menuAnimationOn = false;

    }
    nodeMenu->setScale ( nodeScale );
    nodeMenu->setPosition ( nodeCurrentPos );
    camera->setPosition ( CamPos );
    camera->setTarget ( TarPos );
    ( *ChildIt )->setRotation ( ScreenRot );
}

void AMenu::selectMenu ( scene::ISceneNode* nodeMenu )
{
    nodeRot_init = nodeMenu->getRotation();
    core::vector3df menu01Rot_init = nodeMenu01->getRotation();
    core::vector3df menu02Rot_init = nodeMenu02->getRotation();
    core::vector3df menu03Rot_init = nodeMenu03->getRotation();
    nodeRot_final = core::vector3df ( -90,0,0 );
    time1 = Device->getTimer()->getRealTime();
    if ( nodeRot_init != nodeRot_final )
    {
        if ( executeOnce )
        {
            time0 = Device->getTimer()->getRealTime();
            executeOnce = false;
            menuSelectionEnabled = false;

            TTPanelEnable = true;
            panelAction ( nodeMenu );
        }

        timeLapse = 1000;
        timeDelay = 0;

        if ( time1 - time0 < timeLapse + timeDelay ) // && time1 - time0 > timeDelay )
        {
            nodeCurrentRot = nodeRot_init + ( nodeRot_final-nodeRot_init ) * ( time1 - time0 - timeDelay ) / ( timeLapse+3500 );
        }

    }
    if ( time1 - time0 > timeLapse + timeDelay )
    {
        nodeCurrentRot = nodeRot_final;
        currentMenuDown = getMenuType ( nodeMenu );
        executeOnce = true;
        //menuSelectionEnabled = true;
        selectMenuSwitch = false;
        menuAnimationOn = true;
    }
    nodeMenu01->setRotation ( menu01Rot_init + nodeCurrentRot - nodeRot_init );
    nodeMenu02->setRotation ( menu02Rot_init + nodeCurrentRot - nodeRot_init );
    nodeMenu03->setRotation ( menu03Rot_init + nodeCurrentRot - nodeRot_init );
}

void AMenu::menuSetupAnimation()
{
    if ( executeOnce )

    {
        //Reposition the camera
        time0Camera = Device->getTimer()->getRealTime();
        CamPos_final = core::vector3df ( 0,000,-220 );
        TarPos_final = core::vector3df ( 0,0,0 );
        CamPos_init = camera->getPosition();
        TarPos_init = camera->getTarget();
        CamPos = CamPos_init;
        TarPos = TarPos_init;

        timeLapseCamera = 800;
        if ( menuShortcut )
        {
            timeDelayCamera = 0;
        }
        else
        {
            timeDelayCamera = 500;
        }
        executeOnceCamera = true;

        //Info for leaving letters
        time0 = Device->getTimer()->getRealTime();
        disp_p = core::vector3df ( 0,0,-150 );
        disp_r = core::vector3df ( 0,0,0 );
        nodePos_final = core::vector3df ( 100,0,0 );
        nodeRot_final = core::vector3df ( 0,0,0 );

        //Info for logo
        nodeLogoPos_init = nodeLogo->getPosition();
        nodeLogoRot_init = nodeLogo->getRotation();
        nodeLogoPos_final = core::vector3df ( 0,0,0 );
        nodeLogoRot_final = core::vector3df ( -90,0,0 );

        executeOnce = false;
    }

    time1 = Device->getTimer()->getRealTime();
    core::vector3df unit = core::vector3df ( 1,0,0 );

    timeLapse = 800;

    if ( menuShortcut )
    {
        timeDelay = 0;
    }
    else
    {
        timeDelay = 500;
    }

    nodeM -> setPosition ( nodeM->getPosition() + disp_r );
    nodeE -> setPosition ( nodeE->getPosition() + disp_r );
    nodeL -> setPosition ( nodeL->getPosition() + disp_r );
    nodeI -> setPosition ( nodeI->getPosition() + disp_r );
    nodeA -> setPosition ( nodeA->getPosition() + disp_r );



    if ( time1 - time0 < timeLapse + timeDelay && time1 - time0 > timeDelay )
    {
        disp_r = nodePos_final*unit* ( time1 - time0 - timeDelay ) /timeLapse;

        nodeLogo -> setPosition ( nodeLogoPos_init + ( nodeLogoPos_final - nodeLogoPos_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
        nodeLogo -> setRotation ( nodeLogoRot_init + ( nodeLogoRot_final - nodeLogoRot_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
    }

    else if ( time1 - time0 > timeLapse + timeDelay )
    {
        nodeLogo -> setPosition ( core::vector3df ( 0,0,0 ) );
        nodeLogo -> setRotation ( core::vector3df ( -90,0,0 ) );

        timerMenu = true;
        timerAway = false;
        startMenu = false;
        executeOnce = true;
    }
}

void AMenu::initialAnimationP3()
{
    if ( executeOnce )
    {
        // Set correct position of menu logo, letters, and camera, just
        // in case we want to start here
        nodeLogo -> setPosition ( core::vector3df ( 0,0,0 ) );
        nodeLogo -> setRotation ( core::vector3df ( -90,0,0 ) );
        nodeM -> setVisible ( false );
        nodeE -> setVisible ( false );
        nodeL -> setVisible ( false );
        nodeI -> setVisible ( false );
        nodeA ->setVisible ( false );
        camera -> setPosition ( core::vector3df ( 0,000,-220 ) );
        camera -> setTarget ( core::vector3df ( 0,0,0 ) );

        time0 = Device->getTimer()->getRealTime();
        disp_p = core::vector3df ( 0,0,-220 );
        disp_r = core::vector3df ( 0,0,722 );
        nodePos_final = nodeMenu01->getPosition();
        nodeRot_final = nodeMenu01->getRotation();
        nodePos_init = nodePos_final + disp_p;
        nodeRot_init = nodeRot_final + disp_r;
        nodeCurrentPos = nodePos_init;
        nodeCurrentRot = nodePos_init;

        nodeMenu01 -> setPosition ( nodePos_init );
        nodeMenu01 -> setRotation ( nodeRot_init );
        nodeMenu02 -> setPosition ( nodePos_init );
        nodeMenu02 -> setRotation ( nodeRot_init );
        nodeMenu03 -> setPosition ( nodePos_init );
        nodeMenu03 -> setRotation ( nodeRot_init );

        nodeMenu01 -> updateAbsolutePosition();
        nodeMenu02 -> updateAbsolutePosition();
        nodeMenu03 -> updateAbsolutePosition();
        nodeScreen01 -> updateAbsolutePosition();
        nodeScreen02 -> updateAbsolutePosition();
        nodeScreen03 -> updateAbsolutePosition();

        s1m1 -> updateAbsolutePosition();
        s1m2 -> updateAbsolutePosition();
        s1m3 -> updateAbsolutePosition();
        s1m4 -> updateAbsolutePosition();
        s2m1 -> updateAbsolutePosition();
        s2m2 -> updateAbsolutePosition();
        s2m3 -> updateAbsolutePosition();
        s2m4 -> updateAbsolutePosition();
        s3m1 -> updateAbsolutePosition();
        s3m2 -> updateAbsolutePosition();
        s3m3 -> updateAbsolutePosition();
        s3m4 -> updateAbsolutePosition();

        nodeMenu01 -> setVisible ( true );
        nodeMenu02 -> setVisible ( true );
        nodeMenu03 -> setVisible ( true );

        executeOnce = false;
    }

    time1 = Device->getTimer()->getRealTime();
    timeLapse = 1500;
    timeDelay = 0;

    if ( time1 - time0 < timeLapse + timeDelay && time1 - time0 > timeDelay )
    {
        nodeCurrentPos = nodePos_init + ( nodePos_final-nodePos_init ) * ( time1 - time0 - timeDelay ) /timeLapse;
        nodeCurrentRot = nodeRot_init + ( nodeRot_final-nodeRot_init ) * ( time1 - time0 - timeDelay ) /timeLapse;

    }
    else if ( time1 - time0 > timeLapse + timeDelay )
    {
        nodeCurrentPos = nodePos_final;
        nodeCurrentRot = nodeRot_final;
        menuSelectionEnabled = true;
        timerMenu = false;
        executeOnce = true;
    }

    nodeMenu01->setPosition ( nodeCurrentPos );
    nodeMenu02->setPosition ( nodeCurrentPos );
    nodeMenu03->setPosition ( nodeCurrentPos );

    nodeMenu01->setRotation ( nodeCurrentRot );
    nodeMenu02->setRotation ( nodeCurrentRot - core::vector3df ( 0,0,120 ) );
    nodeMenu03->setRotation ( nodeCurrentRot + core::vector3df ( 0,0,120 ) );

    //nodeMenu01->setDebugDataVisible(true);
    //nodeMenu02->setDebugDataVisible(true);
    //nodeMenu03->setDebugDataVisible(true);
    //nodeLogo->setDebugDataVisible(true);

}

void AMenu::initialAnimationP1()
{
    //initial Logo animation
    if ( timerLogo )
    {
        if ( executeOnce )

        {
            time0 = Device->getTimer()->getRealTime();
            disp_p = core::vector3df ( 0,0,-250 );
            disp_r = core::vector3df ( 322,322,322 );
            nodePos_final = nodeLogo->getPosition();
            nodeRot_final = nodeLogo->getRotation();
            nodePos_init = nodePos_final + disp_p;
            nodeRot_init = nodeRot_final + disp_r;

            nodeLogo -> setPosition ( nodePos_init );
            nodeLogo -> setRotation ( nodeRot_init );
            nodeLogo -> updateAbsolutePosition();
            nodeLogo -> setVisible ( true );

            executeOnce = false;

        }
        time1 = Device->getTimer()->getRealTime();

        timeLapse = 1500; //2000
        timeDelay = 1000; //1000
        timeLeft = timeLapse - ( Device->getTimer()->getRealTime() - time0 );
        if ( time1 - time0 < timeLapse + timeDelay && time1 - time0 > timeDelay )
        {
            nodeLogo -> setPosition ( nodePos_init + ( nodePos_final - nodePos_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
            nodeLogo -> setRotation ( nodeRot_init + ( nodeRot_final - nodeRot_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
        }
        else if ( time1 - time0 > timeLapse + timeDelay )
        {
            timerM = true;
            timerLogo = false;
            executeOnce = true;
            nodeLogo -> setPosition ( nodePos_final );
            nodeLogo -> setRotation ( nodeRot_final );
        }
    }

    //initial M animation

    if ( timerM )
    {
        if ( executeOnce )

        {
            time0 = Device->getTimer()->getRealTime();
            disp_p = core::vector3df ( 0,0,-250 );
            disp_r = core::vector3df ( 622,15,122 );
            nodePos_final = nodeM->getPosition();
            nodeRot_final = nodeM->getRotation();
            nodePos_init = nodePos_final + disp_p;
            nodeRot_init = nodeRot_final + disp_r;

            nodeM -> setPosition ( nodePos_init );
            nodeM -> setRotation ( nodeRot_init );
            nodeM -> updateAbsolutePosition();
            nodeM -> setVisible ( true );

            executeOnce = false;

        }
        time1 = Device->getTimer()->getRealTime();

        timeLapse = 800; //2000
        timeDelay = 0;
        timeLeft = timeLapse - ( Device->getTimer()->getRealTime() - time0 );
        if ( time1 - time0 < timeLapse + timeDelay && time1 - time0 > timeDelay )
        {
            nodeM -> setPosition ( nodePos_init + ( nodePos_final - nodePos_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
            nodeM -> setRotation ( nodeRot_init + ( nodeRot_final - nodeRot_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
        }
        else if ( time1 - time0 > timeLapse + timeDelay )
        {
            nodeM -> setPosition ( nodePos_final );
            nodeM -> setRotation ( nodeRot_final );
            timerM = false;
            timerE = true;
            executeOnce = true;
        }
    }

    //initial E animation

    if ( timerE )
    {
        if ( executeOnce )

        {
            time0 = Device->getTimer()->getRealTime();
            disp_p = core::vector3df ( 0,0,-250 );
            disp_r = core::vector3df ( 722,92,182 );
            nodePos_final = nodeE->getPosition();
            nodeRot_final = nodeE->getRotation();
            nodePos_init = nodePos_final + disp_p;
            nodeRot_init = nodeRot_final + disp_r;

            nodeE -> setPosition ( nodePos_init );
            nodeE -> setRotation ( nodeRot_init );
            nodeE -> updateAbsolutePosition();
            nodeE -> setVisible ( true );

            executeOnce = false;

        }
        time1 = Device->getTimer()->getRealTime();

        timeLapse = 800; //1500
        timeDelay = 0;
        timeLeft = timeLapse - ( Device->getTimer()->getRealTime() - time0 );
        if ( time1 - time0 < timeLapse + timeDelay && time1 - time0 > timeDelay )
        {
            nodeE -> setPosition ( nodePos_init + ( nodePos_final - nodePos_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
            nodeE -> setRotation ( nodeRot_init + ( nodeRot_final - nodeRot_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
        }
        else if ( time1 - time0 > timeLapse + timeDelay )
        {
            nodeE -> setPosition ( nodePos_final );
            nodeE -> setRotation ( nodeRot_final );
            timerE = false;
            timerL = true;
            executeOnce = true;
        }
    }

    //initial L animation

    if ( timerL )
    {
        if ( executeOnce )

        {
            time0 = Device->getTimer()->getRealTime();
            disp_p = core::vector3df ( 0,0,-250 );
            disp_r = core::vector3df ( -722,-92,182 );
            nodePos_final = nodeE->getPosition();
            nodeRot_final = nodeE->getRotation();
            nodePos_init = nodePos_final + disp_p;
            nodeRot_init = nodeRot_final + disp_r;

            nodeL -> setPosition ( nodePos_init );
            nodeL -> setRotation ( nodeRot_init );
            nodeL -> updateAbsolutePosition();
            nodeL -> setVisible ( true );

            executeOnce = false;

        }
        time1 = Device->getTimer()->getRealTime();

        timeLapse = 800; //1500
        timeDelay = 0;
        timeLeft = timeLapse - ( Device->getTimer()->getRealTime() - time0 );
        if ( time1 - time0 < timeLapse + timeDelay && time1 - time0 > timeDelay )
        {
            nodeL -> setPosition ( nodePos_init + ( nodePos_final - nodePos_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
            nodeL -> setRotation ( nodeRot_init + ( nodeRot_final - nodeRot_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
        }
        else if ( time1 - time0 > timeLapse + timeDelay )
        {
            nodeL -> setPosition ( nodePos_final );
            nodeL -> setRotation ( nodeRot_final );
            timerL = false;
            timerI = true;
            executeOnce = true;
        }
    }



    //initial I animation

    if ( timerI )
    {
        if ( executeOnce )

        {
            time0 = Device->getTimer()->getRealTime();
            disp_p = core::vector3df ( 0,-300,-250 );
            disp_r = core::vector3df ( 722,-100,182 );
            nodePos_final = nodeI->getPosition();
            nodeRot_final = nodeI->getRotation();
            nodePos_init = nodePos_final + disp_p;
            nodeRot_init = nodeRot_final + disp_r;

            nodeI -> setPosition ( nodePos_init );
            nodeI -> setRotation ( nodeRot_init );
            nodeI -> updateAbsolutePosition();
            nodeI -> setVisible ( true );

            executeOnce = false;

        }
        time1 = Device->getTimer()->getRealTime();

        timeLapse = 800; //1500
        timeDelay = 0;
        timeLeft = timeLapse - ( Device->getTimer()->getRealTime() - time0 );
        if ( time1 - time0 < timeLapse + timeDelay && time1 - time0 > timeDelay )
        {
            nodeI -> setPosition ( nodePos_init + ( nodePos_final - nodePos_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
            nodeI -> setRotation ( nodeRot_init + ( nodeRot_final - nodeRot_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
        }
        else if ( time1 - time0 > timeLapse + timeDelay )
        {
            nodeI -> setPosition ( nodePos_final );
            nodeI -> setRotation ( nodeRot_final );
            timerI = false;
            timerA = true;
            executeOnce = true;
        }
    }


    //initial A animation

    if ( timerA )
    {
        if ( executeOnce )

        {
            time0 = Device->getTimer()->getRealTime();
            disp_p = core::vector3df ( 230,-650,-250 );
            disp_r = core::vector3df ( 22,-72,322 );
            nodePos_final = nodeA->getPosition();
            nodeRot_final = nodeA->getRotation();
            nodePos_init = nodePos_final + disp_p;
            nodeRot_init = nodeRot_final + disp_r;

            nodeA -> setPosition ( nodePos_init );
            nodeA -> setRotation ( nodeRot_init );
            nodeA -> updateAbsolutePosition();
            nodeA -> setVisible ( true );

            executeOnce = false;

        }
        time1 = Device->getTimer()->getRealTime();

        timeLapse = 800; //1500
        timeDelay = 0;
        timeLeft = timeLapse - ( Device->getTimer()->getRealTime() - time0 );
        if ( time1 - time0 < timeLapse + timeDelay && time1 - time0 > timeDelay )
        {
            nodeA -> setPosition ( nodePos_init + ( nodePos_final - nodePos_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
            nodeA -> setRotation ( nodeRot_init + ( nodeRot_final - nodeRot_init ) * ( time1 - time0 - timeDelay ) /timeLapse );
        }
        else if ( time1 - time0 > timeLapse + timeDelay )
        {
            nodeA -> setPosition ( nodePos_final );
            nodeA -> setRotation ( nodeRot_final );
            timerA = false;
            //timerAway = true;
            startMenu = true;
            executeOnce = true;
        }
    }



}

void AMenu::animateMenuTooltipOn ( int pos, int type )
{
    if ( !TT_Enabled )
    {

        // initial settings, according to the arguments of the function
        if ( TT_executeOnce && !TT_lock )
        {

            // first we set the position of the tooltip according to the position of the menu

            switch ( pos )
            {
            case 1:
                TTPos_init = core::vector3df ( -300,7,0 );
                TTPos_final = core::vector3df ( -137.000000, 7.000000, 0.000000 );
                break;

            case 2:
                TTPos_init = core::vector3df ( 300,7,0 );
                TTPos_final = core::vector3df ( 137.000000, 7.000000, 0.000000 );
                break;

            case 3:
                TTPos_init = core::vector3df ( -300.000000, -87.000000, 0.000000 );
                TTPos_final = core::vector3df ( -90.000000, -87.000000, 0.000000 );
                break;

            case 4:
                /*TTPos_init = core::vector3df(-1000,-50,0);
                TTPos_final = core::vector3df(-20,-50,0);*/
                break;
            }


            // then we set the respective texture according to the position and type
            switch ( type )
            {
            case 1: // Getting Started
                switch ( pos )
                {
                case 1:
                    nodeTT->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_GS_P1.png" ) );
                    break;

                case 2:
                    nodeTT->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_GS_P2.png" ) );
                    break;

                case 3:
                    nodeTT->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_GS_P3.png" ) );
                    break;

                case 4:
                    nodeTTPanel->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "panel_GS.png" ) );
                    break;
                }
                break;

            case 2: // Figure It Out
                switch ( pos )
                {
                case 1:
                    nodeTT->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_FO_P1.png" ) );
                    break;

                case 2:
                    nodeTT->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_FO_P2.png" ) );
                    break;

                case 3:
                    nodeTT->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_FO_P3.png" ) );
                    break;

                case 4:
                    nodeTTPanel->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "panel_FO.png" ) );
                    break;
                }
                break;

            case 3: // Tell Others
                switch ( pos )
                {
                case 1:
                    nodeTT->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_TO_P1.png" ) );
                    break;

                case 2:
                    nodeTT->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_TO_P2.png" ) );
                    break;

                case 3:
                    nodeTT->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_TO_P3.png" ) );
                    break;

                case 4:
                    nodeTTPanel->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "panel_TO.png" ) );
                    break;
                }
                break;

            }

            nodeTT->setVisible ( true );
            TTPos_current = TTPos_init;
            nodeTT->setPosition ( TTPos_init );
            timeTT0 = Device->getTimer()->getRealTime();
            timeTT = Device->getTimer()->getRealTime();
            TT_lock = true;
            TT_executeOnce = false;
            TT_SwitchL2 = false;
        }
        timeTT = Device->getTimer()->getRealTime();


        // now the animation
        if ( TT_lock )
        {


            //if (TTPos_init != TTPos_final)
            {

                timeLapseTT = 500;
                timeDelayTT = 0;
                TTPos_init = nodeTT->getPosition();

                if ( timeTT - timeTT0 < timeLapseTT + timeDelayTT )
                {
                    TTPos_current = TTPos_init + ( TTPos_final-TTPos_init ) * ( timeTT - timeTT0 - timeDelayTT ) / ( timeLapseTT+200 );
                }

            }
            if ( ( timeTT - timeTT0 > timeLapseTT + timeDelayTT ) && TT_lock )
            {
                TTPos_current = TTPos_final;
                nodeTT->setPosition ( TTPos_final );
                //TTSwitch = false;
                TT_lock = false;
                TTAnim = false;
                TT_executeOnce = true;
                TT_Enabled = true;
            }
            nodeTT->setPosition ( TTPos_current );


        }
    }
}


void AMenu::animateMenuTooltipOff()
{
    if ( TT_Enabled )
    {

        timeTT = Device->getTimer()->getRealTime();
        if ( TT_executeOnce )
        {
            TTPos_init = nodeTT->getPosition();

            TTPos_final = TTPos_init + core::vector3df ( 300,0,0 );
            if ( nodeTT->getPosition().X < 0.0 )
            {
                TTPos_final = TTPos_init + core::vector3df ( -300,0,0 );
            }

            timeTT0 = Device->getTimer()->getRealTime();
            TT_lock = true;
            TT_executeOnce = false;
        }

        timeLapseTT = 300;
        timeDelayTT = 0;
        TTPos_init = nodeTT->getPosition();

        if ( timeTT - timeTT0 < timeLapseTT + timeDelayTT )
        {
            TTPos_current = TTPos_init + ( TTPos_final-TTPos_init ) * ( timeTT - timeTT0 - timeDelayTT ) / ( timeLapseTT+800 );


        }


        if ( timeTT - timeTT0 > timeLapseTT + timeDelayTT )
        {
            nodeTT->setPosition ( TTPos_final );
            TT_lock = false;
            TTAnim = false;
            TT_Enabled = false;
        }
        nodeTT->setPosition ( TTPos_current );
    }
}


void AMenu::panelAnimateOn()
{
    if ( !TTP_Enabled )
    {

        // initial settings
        if ( TTP_executeOnce )
        {

            TTPanelPos_init = core::vector3df ( -300, -112, 0 );
            TTPanelPos_final = core::vector3df ( -118, -112, 0 );
            TTPanelScale_initial = core::vector3df ( 0.7, 1, 0.7 );
            TTPanelScale_final = core::vector3df ( 1.3, 1, 1.3 );
            panelScaleAux = TTPanelScale_initial.X;

            // then we set the respective texture according to the enabled panel
            switch ( currentEnabledPanel )
            {
            case 1:
                nodeTTPanel->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "panel_GS.png" ) );
                break;

            case 2:
                nodeTTPanel->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "panel_FO.png" ) );
                break;

            case 3:
                nodeTTPanel->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "panel_TO.png" ) );
                break;
            }

            nodeTTPanel->setVisible ( true );
            TTPanelPos_current = TTPanelPos_init;
            nodeTTPanel->setPosition ( TTPanelPos_init );
            nodeTTPanel->setScale ( TTPanelScale_initial );
            timeTTP0 = Device->getTimer()->getRealTime();
            timeTTP = Device->getTimer()->getRealTime();
            TTP_lock = true;
            TTP_executeOnce = false;
        }
        timeTTP = Device->getTimer()->getRealTime();


        // now the animation
        if ( TTP_lock )
        {


            if ( TTPanelPos_init != TTPanelPos_final )
            {

                timeLapseTTP = 800;
                timeDelayTTP = 1000;
                TTPanelPos_init = nodeTTPanel->getPosition();
                TTPanelScale_initial.X = panelScaleAux;

                if ( ( timeTTP - timeTTP0 < timeLapseTTP + timeDelayTTP ) && ( timeTTP - timeTTP0 > timeDelayTTP ) )
                {
                    TTPanelPos_current = TTPanelPos_init + ( TTPanelPos_final-TTPanelPos_init ) * ( timeTTP - timeTTP0 - timeDelayTTP ) / ( timeLapseTTP + 1700 );
                    panelScaleAux = TTPanelScale_initial.X + ( TTPanelScale_final.X - TTPanelScale_initial.X ) * ( timeTTP - timeTTP0 - timeDelayTTP ) / ( timeLapseTTP + 1700 );
                }

            }
            if ( ( timeTTP - timeTTP0 > timeLapseTTP + timeDelayTTP ) && TTP_lock )
            {
                TTPanelPos_current = TTPanelPos_final;
                panelScaleAux = TTPanelScale_final.X;
                nodeTTPanel->setPosition ( TTPanelPos_final );
                nodeTTPanel->setScale ( TTPanelScale_final );
                //TTSwitch = false;
                TTP_lock = false;
                TTPAnim = false;
                TTP_executeOnce = true;
                TTP_Enabled = true;
            }
            nodeTTPanel->setPosition ( TTPanelPos_current );
            nodeTTPanel->setScale ( core::vector3df ( panelScaleAux, 1, panelScaleAux ) );


        }
    }
}

void AMenu::panelAnimateOff()
{

    if ( TTP_Enabled )
    {

        // initial settings, according to the arguments of the function
        if ( TTP_executeOnce )
        {
            TTPanelPos_init = core::vector3df ( -118, -112, 0 );
            TTPanelPos_final = core::vector3df ( -300, -112, 0 );



            currentEnabledPanel = 0;
            nodeTTPanel->setVisible ( true );
            TTPanelPos_current = TTPanelPos_init;
            nodeTTPanel->setPosition ( TTPanelPos_init );
            timeTTP0 = Device->getTimer()->getRealTime();
            timeTTP = Device->getTimer()->getRealTime();
            TTP_lock = true;
            TTP_executeOnce = false;
        }
        timeTTP = Device->getTimer()->getRealTime();


        // now the animation
        if ( TTP_lock )
        {


            if ( TTPanelPos_init != TTPanelPos_final )
            {

                timeLapseTTP = 500;
                timeDelayTTP = 0;
                TTPanelPos_init = nodeTTPanel->getPosition();

                if ( timeTTP - timeTTP0 < timeLapseTTP + timeDelayTTP )
                {
                    TTPanelPos_current = TTPanelPos_init + ( TTPanelPos_final-TTPanelPos_init ) * ( timeTTP - timeTTP0 - timeDelayTTP ) / ( timeLapseTTP+200 );
                }

            }
            if ( ( timeTTP - timeTTP0 > timeLapseTTP + timeDelayTTP ) && TTP_lock )
            {
                TTPanelPos_current = TTPanelPos_final;
                nodeTTPanel->setPosition ( TTPanelPos_final );
                //TTSwitch = false;
                TTP_lock = false;
                TTPAnim = false;
                TTP_executeOnce = true;
                TTP_Enabled = false;
            }
            nodeTTPanel->setPosition ( TTPanelPos_current );


        }
    }
}


void AMenu::panelAction ( scene::ISceneNode* menuNode )
{

    if ( !TTP_lock )
    {
        // determine the panel to change
        currentEnabledPanel = getMenuType ( menuNode );


        if ( TTPanelEnable )
        {
            TTP_isOnType = true;
        }
        else TTP_isOnType = false;



        TTPAnim = true;

        TTP_executeOnce = true;

    }

}




void AMenu::tooltipAction ( scene::ISceneNode* menuNode, bool enable )
{


    if ( !TT_lock )
    {
        // determine the type of the selected menu
        int type = getMenuType ( menuNode );

        // determine the position of the selected menu
        int pos = getMenuPos ( type );


        if ( menuNode == activatedMenu ) pos = 4;


        if ( enable )
        {
            TT_isOnType = true;
        }
        else TT_isOnType = false;

        TT_Pos = pos;
        TT_Type = type;

        TT_executeOnce = true;
        TTAnim = true;


    }

}

int AMenu::getMenuType ( scene::ISceneNode* menuNode )
{
    if ( menuNode == nodeMenu01 ) return 1;
    if ( menuNode == nodeMenu02 ) return 2;
    if ( menuNode == nodeMenu03 ) return 3;
}


int AMenu::getMenuPos ( int menuType )
{
    switch ( menuType )
    {
    case 1:
        if ( currentMenuDown == 1 ) return 3;
        if ( currentMenuDown == 2 ) return 2;
        if ( currentMenuDown == 3 ) return 1;
        break;

    case 2:
        if ( currentMenuDown == 1 ) return 1;
        if ( currentMenuDown == 2 ) return 3;
        if ( currentMenuDown == 3 ) return 2;
        break;

    case 3:
        if ( currentMenuDown == 1 ) return 2;
        if ( currentMenuDown == 2 ) return 1;
        if ( currentMenuDown == 3 ) return 3;
        break;
    }

}

void AMenu::setSubMenuTexture ( scene::ISceneNode* subMenu )
{
    if ( subMenu == s1m1 )
    {
        nodeTT_SM->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_s1m1.png" ) );
    }

    if ( subMenu == s1m2 )
    {
        nodeTT_SM->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_s1m2.png" ) );
    }

    if ( subMenu == s1m3 )
    {
        nodeTT_SM->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_s1m3.png" ) );
    }

    if ( subMenu == s1m4 )
    {
        nodeTT_SM->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_s1m4.png" ) );
    }

    if ( subMenu == s2m1 )
    {
        nodeTT_SM->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_s2m1.png" ) );
    }

    if ( subMenu == s2m2 )
    {
        nodeTT_SM->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_s2m2.png" ) );
    }

    if ( subMenu == s2m3 )
    {
        nodeTT_SM->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_s2m3.png" ) );
    }

    if ( subMenu == s2m4 )
    {
        nodeTT_SM->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_s2m4.png" ) );
    }

    if ( subMenu == s3m1 )
    {
        nodeTT_SM->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_s3m1.png" ) );
    }

    if ( subMenu == s3m2 )
    {
        nodeTT_SM->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_s3m2.png" ) );
    }

    if ( subMenu == s3m3 )
    {
        nodeTT_SM->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_s3m3.png" ) );
    }

    if ( subMenu == s3m4 )
    {
        nodeTT_SM->setMaterialTexture ( 0, Device->getVideoDriver()->getTexture ( "tt_s3m4.png" ) );
    }

}

/** Activate or deactivate submenu tooltips */
void AMenu::subMenuTTAction ( scene::ISceneNode* subMenu, bool enable )
{
    if ( !SMTT_lock )
    {
        if ( enable )
        {
            {
                SMTT_isOnType = true;
                setSubMenuTexture ( subMenu );
            }
        }
        else SMTT_isOnType = false;

        SMTTAnim = true;
        SMTT_executeOnce = true;

    }
}

void AMenu::SMTTAnimateOn()
{
    if ( !SMTT_Enabled )
    {
        // initial settings
        if ( SMTT_executeOnce && !SMTT_lock )
        {

            // setting of the initial and final positions
            SMTTPos_init = core::vector3df ( 319, -112, 0 );
            SMTTPos_final = core::vector3df ( 119, -112, 0 );

            nodeTT_SM->setVisible ( true );
            SMTTPos_current = SMTTPos_init;
            nodeTT_SM->setPosition ( SMTTPos_init );
            timeSMTT0 = Device->getTimer()->getRealTime();
            timeSMTT = Device->getTimer()->getRealTime();
            SMTT_lock = true;
            SMTT_executeOnce = false;
        }
        timeSMTT = Device->getTimer()->getRealTime();


        // now the animation
        if ( SMTT_lock )
        {
            timeLapseSMTT = 500;
            timeDelaySMTT = 0;
            SMTTPos_init = nodeTT_SM->getPosition();

            if ( timeSMTT - timeSMTT0 < timeLapseSMTT + timeDelaySMTT )
            {
                SMTTPos_current = SMTTPos_init + ( SMTTPos_final-SMTTPos_init ) * ( timeSMTT - timeSMTT0 - timeDelaySMTT ) / ( timeLapseSMTT+200 );
            }


            if ( ( timeSMTT - timeSMTT0 > timeLapseSMTT + timeDelaySMTT ) && SMTT_lock )
            {
                SMTTPos_current = SMTTPos_final;
                nodeTT_SM->setPosition ( SMTTPos_final );
                //TTSwitch = false;
                SMTT_lock = false;
                SMTTAnim = false;
                SMTT_executeOnce = true;
                SMTT_Enabled = true;
            }
            nodeTT_SM->setPosition ( SMTTPos_current );
        }
    }
}

void AMenu::skipIntro()
{
    if ( !menuShortcut && !timerMenu )
    {
        timerLogo = false;
        timerM = false;
        timerE = false;
        timerL = false;
        timerI = false;
        timerA = false;
        executeOnce = true;
        menuShortcut = true;
        startMenu = true;
    }
}


void AMenu::SMTTAnimateOff()
{

    if ( SMTT_Enabled )
    {

        // initial settings, according to the arguments of the function
        if ( SMTT_executeOnce )
        {
            SMTTPos_init = core::vector3df ( 119, -112, 0 );
            SMTTPos_final = core::vector3df ( 319, -112, 0 );


            nodeTT_SM->setVisible ( true );
            SMTTPos_current = SMTTPos_init;
            nodeTT_SM->setPosition ( SMTTPos_init );
            timeSMTT0 = Device->getTimer()->getRealTime();
            timeSMTT = Device->getTimer()->getRealTime();
            SMTT_lock = true;
            SMTT_executeOnce = false;
        }
        timeSMTT = Device->getTimer()->getRealTime();


        // now the animation
        if ( SMTT_lock )
        {



            if ( SMTTPos_init != SMTTPos_final )
            {

                timeLapseSMTT = 500;
                timeDelaySMTT = 0;
                SMTTPos_init = nodeTT_SM->getPosition();

                if ( timeSMTT - timeSMTT0 < timeLapseSMTT + timeDelaySMTT )
                {
                    SMTTPos_current = SMTTPos_init + ( SMTTPos_final - SMTTPos_init ) * ( timeSMTT - timeSMTT0 - timeDelaySMTT ) / ( timeLapseSMTT+200 );
                }

            }
            if ( ( timeSMTT - timeSMTT0 > timeLapseSMTT + timeDelaySMTT ) && SMTT_lock )
            {
                SMTTPos_current = SMTTPos_final;
                nodeTT_SM->setPosition ( SMTTPos_final );
                //TTSwitch = false;
                SMTT_lock = false;
                SMTTAnim = false;
                SMTT_executeOnce = true;
                SMTT_Enabled = false;
            }
            nodeTT_SM->setPosition ( SMTTPos_current );

        }
    }
}







bool AMenu::OnEvent(const SEvent& event)
{
    if ( event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown == false  )
    {
        switch (event.KeyInput.Key)
        {
        case KEY_SPACE:
            skipIntro();
            return true;
            break;
        case KEY_KEY_M:
            timerMenu=true;
            executeOnce=true;
            menuSelectionEnabled=false;
            return true;
            break;
        }
    }

    if (event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN)
    {
        if (lastSelectedSubMenu == s2m4)
        {
            emit changeToLevel("geometry.ui");
            return true;
        }

        if (selectedSceneNode && selectedSceneNode->getID() == 16)
        {
            activatedMenu = selectedSceneNode;
            selectMenuSwitch = true;
            return true;
        }
    }


    /*if (selectedSceneNode && selectedSceneNode->getID() == 16 && Base->receiver->Keys[ KEY_LBUTTON ])
      {
      activatedMenu = selectedSceneNode;
              selectMenuSwitch = true;
              tooltipAction(activatedMenu, false);
              }*/
    return false;
}
