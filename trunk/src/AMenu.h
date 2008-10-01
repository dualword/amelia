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

#ifndef AMENU_H
#define AMENU_H

#include "QIrrWidget.h"

using namespace irr;

class AMenu : public QIrrWidget
{
Q_OBJECT

	public:
		AMenu(QWidget* parent=0);
		~AMenu();

		//Animation of geometry elements
		void load();
		void animateMenuOff ( scene::ISceneNode* nodeMenu, scene::ICameraSceneNode* camera );
		void animateMenuOn ( scene::ISceneNode* nodeMenu, scene::ICameraSceneNode* camera );
		void animateMenuTooltipOn ( int pos, int type );
		void animateMenuTooltipOff();
		void animateMenuIcon();
		void selectMenu ( scene::ISceneNode* nodeMenu );
		void tooltipAction ( scene::ISceneNode* menuNode, bool enable );
		void panelAction ( scene::ISceneNode* menuNode );
		void panelAnimateOn();
		void panelAnimateOff();
		void initialAnimationP3();
		void initialAnimationP1();
		void shortcutAnimation();
		void menuSetupAnimation();
		void execute();
		int getMenuPos ( int menu );
		int getMenuType ( scene::ISceneNode* menuNode );
		void setSubMenuTexture ( scene::ISceneNode* subMenu );
		void subMenuTTAction ( scene::ISceneNode* subMenu, bool enable );
		void SMTTAnimateOn();
		void SMTTAnimateOff();

		void changeSubMenu ( std::string name );
		void skipIntro();

		bool OnEvent(const irr::SEvent&);

 signals:
		void changeToLevel(QString loadMe);

	protected:

		//Menu states
		scene::ISceneNode* activatedMenu;
		scene::ISceneNode* currentlySelectedMenu;
		scene::ISceneNode* selectedSceneNode;
		scene::ISceneNode* lastSelectedSceneNode;
		scene::ISceneNode* selectedSubMenu;
		scene::ISceneNode* lastSelectedSubMenu;

		//Menu elements
		scene::ISceneNode* skybox;
		scene::ISceneNode* nodeLogo;
		scene::ISceneNode* nodeM;
		scene::ISceneNode* nodeE;
		scene::ISceneNode* nodeI;
		scene::ISceneNode* nodeL;
		scene::ISceneNode* nodeA;
		scene::ISceneNode* nodeLight;
		scene::ISceneNode* nodeMenu01;
		scene::ISceneNode* nodeMenu02;
		scene::ISceneNode* nodeMenu03;
		scene::ISceneNode* nodeScreen01;
		scene::ISceneNode* nodeScreen02;
		scene::ISceneNode* nodeScreen03;

		scene::ISceneNode* s1m1;
		scene::ISceneNode* s1m2;
		scene::ISceneNode* s1m3;
		scene::ISceneNode* s1m4;
		scene::ISceneNode* s2m1;
		scene::ISceneNode* s2m2;
		scene::ISceneNode* s2m3;
		scene::ISceneNode* s2m4;
		scene::ISceneNode* s3m1;
		scene::ISceneNode* s3m2;
		scene::ISceneNode* s3m3;
		scene::ISceneNode* s3m4;

		scene::ISceneNode* GS_buttons;
		scene::ISceneNode* FO_buttons;
		scene::ISceneNode* TO_buttons;

		scene::ISceneNode* nodeTT;
		scene::ISceneNode* nodeTTPanel;
		scene::ISceneNode* nodeTT_SM;

		vector<video::ITexture*> GS_animation;
		vector<video::ITexture*> FO_animation;
		vector<video::ITexture*> TO_animation;

		vector<video::ITexture*> GS_buttonImages;
		vector<video::ITexture*> FO_buttonImages;
		vector<video::ITexture*> TO_buttonImages;

		vector<video::ITexture*>::iterator iter;
		vector<video::ITexture*> curAnimation;

	private:

		u32 time0;
		u32 time0Camera;
		u32 time1;
		u32 timeLapse;
		u32 timeLapseTT;
		u32 timeLapseSMTT;
		u32 timeLapseTTP;
		u32 timeDelay;
		u32 timeDelayTT;
		u32 timeDelaySMTT;
		u32 timeDelayTTP;
		u32 timeLeft;
		u32 timeCamera;
		u32 timeLapseCamera;
		u32 timeDelayCamera;
		u32 timeTT0;
		u32 timeTT;
		u32 timeTTP0;
		u32 timeTTP;
		u32 timeSMTT;
		u32 timeSMTT0;
		u32 timeIcon;
		u32 timeIcon0;
		core::vector3df zero;
		core::vector3df disp_p;
		core::vector3df disp_r;
		core::vector3df nodePos_final;
		core::vector3df nodeRot_final;
		core::vector3df nodePos_init;
		core::vector3df nodeRot_init;
		core::vector3df nodeCurrentPos;
		core::vector3df nodeCurrentRot;

		core::vector3df nodeLogoPos_init;
		core::vector3df nodeMPos_init;
		core::vector3df nodeEPos_init;
		core::vector3df nodeLPos_init;
		core::vector3df nodeIPos_init;
		core::vector3df nodeAPos_init;

		core::vector3df nodeLogoPos_final;
		core::vector3df nodeMPos_final;
		core::vector3df nodeEPos_final;
		core::vector3df nodeLPos_final;
		core::vector3df nodeIPos_final;
		core::vector3df nodeAPos_final;

		core::vector3df nodeLogoRot_init;
		core::vector3df nodeMRot_init;
		core::vector3df nodeERot_init;
		core::vector3df nodeLRot_init;
		core::vector3df nodeIRot_init;
		core::vector3df nodeARot_init;

		core::vector3df nodeLogoRot_final;
		core::vector3df nodeMRot_final;
		core::vector3df nodeERot_final;
		core::vector3df nodeLRot_final;
		core::vector3df nodeIRot_final;
		core::vector3df nodeARot_final;

		core::vector3df nodeScale;
		core::vector3df CamPos_final;
		core::vector3df TarPos_final;
		core::vector3df CamPos_init;
		core::vector3df TarPos_init;
		core::vector3df TarPos;
		core::vector3df CamPos;
		core::vector3df ScreenRot;
		core::vector3df ScreenRot_init;
		core::vector3df ScreenRot_final;
		core::vector3df TTPos_init;
		core::vector3df TTPos_final;
		core::vector3df TTPos_current;
		core::vector3df TTPanelPos_init;
		core::vector3df TTPanelPos_final;
		core::vector3df TTPanelScale_initial;
		core::vector3df TTPanelScale_final;
		core::vector3df TTPanelPos_current;
		core::vector3df SMTTPos_init;
		core::vector3df SMTTPos_final;
		core::vector3df SMTTPos_current;
		bool executeOnce;
		bool executeOnceCamera;
		bool timerLogo;
		bool timerM;
		bool timerE;
		bool timerL;
		bool timerI;
		bool timerA;
		bool timerMenu;
		bool timerAway;
		bool timerCamera;
		bool menuSelectionEnabled;
		bool subMenuSelectionEnabled;
		bool menuAnimationOn;
		bool menuAnimationOff;
		bool selectMenuSwitch;
		bool TT_lock;
		bool TTP_lock;
		bool TT_isOnType;
		bool TTP_isOnType;
		bool TTAnim;
		bool TTPAnim;
		bool TT_Enabled;
		bool SMTT_Enabled;
		bool TTP_Enabled;
		bool TT_executeOnce;
		bool TTP_executeOnce;
		bool TTPanelEnable;
		int TT_Pos;
		int TT_Type;
		bool TT_Retract;
		bool TT_SwitchL1;
		bool TT_SwitchL2;
		int currentMenuDown;
		int currentEnabledPanel;
		bool SMTT_lock;
		bool SMTTAnim;
		bool SMTT_isOnType;
		bool SMTT_executeOnce;
		float panelScaleAux;
		video::SColor colorMenuOrig;
		video::SColor colorMenuSelect;
		video::SColor menuEmissiveInit;
		ICameraSceneNode *camera;

		bool newNode;
		bool menuShortcut;
		bool startMenu;
		bool menuShortcutAni;
		int iconTimer;
		int screens_per_frame;
		int levelChange;
		bool iconSwitch;
};




#endif // AMENU_H
