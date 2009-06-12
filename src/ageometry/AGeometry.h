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



#ifndef AGEOMETRY_H_
#define AGEOMETRY_H_

#include "QIrrWidget.h"
#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QSignalMapper>

#include <QBoolSync.h>

#include "CSceneNodeAnimatorCameraSwitch.h"
#include "AFPSControl.h"
#include "APoint3D.h"

#include "ASTrack3DNode.h"
#include "AJet3DNode.h"
#include "AMisET3DNode.h"

#include <aeventmanager/AFilteredEvent.h>

class AGeometry : public QIrrWidget
{
public:
  static const int FPS=0,Maya=3,Lock=4;
  static const int Cam3D=-1,Orthogonal=1,Projective=2;
  static const int NoneMode=0,WedgeMode=1,MosesMode=2;
  
  AGeometry(QWidget* parent=0);
  virtual ~AGeometry();
  
  void execute(); // The cycle function which will be called on every frame
  void load();
  
  AFilteredEvent* event();
  int viewport();
  int camera();
  
  void setComboMenu(QMenu*);
  void addToDetectorMenu(QString,QAction*);
  
  QBoolSync* detectorVisibility();
  QBoolSync* pixelsVisibility();
  QBoolSync* sctVisibility();
  QBoolSync* trtVisibility();
  QBoolSync* larVisibility();
  QBoolSync* tileVisibility();
  QBoolSync* muonSpecVisibility();
  QBoolSync* magnetsVisibility();
  QBoolSync* pitVisibility();
  
  QBoolSync* calorimetersVisibility();
  QBoolSync* trackerVisibility();
  
  APoint3D cameraPosition();
  APoint3D cameraTarget();
  
  scene::ISceneNode* cube;
  
  // Control variables for the dynamic hiding of parts of ATLAS
  bool isTC_on;
  bool isLAr_on;
  bool isSCT_on;
  bool isPix_on;
  int TC_switch;
  int LAr_switch;
  int SCT_switch;
  int Pix_switch;
  
  int jetIdCounter;
  
  
  bool generateCameraStats;
  bool displayFPS;
  
  
  //Switches for the different modes
  bool detectorMode;
  bool eventAnalysisMode;
  bool multiMediaMode;
  
  scene::ISceneNode* selectedTrackBox;  

  QAction *actFPS;
  QAction *actSphere;
		    
public slots:
  void setViewport(int to); //Switches the main screen to the specified camera
  void setCamera(int to,bool animate=true); //Switches the 3D camera
  void setCropMode(int to); //Switches to a new cropping mode...
  
  void setEvent(AFilteredEvent*); // Sets the current event
  void clearEvent();    
  void updateTracks();
  
  void switchVisibility(int modType); //Switches the visibility of the different components of the detector
  
  void setCameraPosition(APoint3D pos);
  void setCameraTarget(APoint3D tar);
  
  void renderViewport(int view);
  
  ATrack* selectTrackByID ( int ID ,bool multitrack=false);
  ATrack* deselectTrackByID ( int ID );
  void clearTrackSelection();
  
  //Functions to grab and release control of the camera
  void grabControl();
  void releaseControl();
  
signals:
  void cameraControlSwitched(bool grabbed);
  void viewportSwitched(int from,int to);
  void viewportUpdated(int cam,QImage newshot);
  void cropModeSwitched(int newCropMode);
  
  void trackSelected(ATrack*);
  void trackDeselected(ATrack*);
  
  void detectorPartSelected(const QString&);
  void emptySelection();
  
protected:
  void resizeEvent( QResizeEvent* event );
  void mouseClickEvent( QMouseEvent *event );
  void mouseDoubleClickEvent(QMouseEvent *event);
  void keyPressEvent( QKeyEvent* event );
  void contextMenuEvent( QContextMenuEvent *event );
  
  void setupView(int);
  
  core::vector3df pos;
  core::vector3df rot;
  core::vector3df scale;
  
  
private:
  //Tracks and the event variables
  AFilteredEvent* _event;
  QList<ATrack3DNode*> selectedTracks;
  QList<ATrack3DNode*> allTracks;
  QList<AJet3DNode*> allJets;
  
  //GUI stuff
  QMenu *_comboMenu;
  QMap<QString,QMenu*>  _detectorMenu;
  gui::IGUIButton* multiSelectButton,*zoomIn,*zoomOut;
  
  const bool isCrappyComputer;  //removes pit .obj and textures, to speed up rendering
  const bool generateDetectorGeometry;//enables or disables detector geometry for testing purposes

  // Important parts of the detector //
  scene::ISceneNode* Pit_Reference;
  scene::ISceneNode* Atlas_Reference;
  scene::ISceneNode* Calorimeters_Reference;
  scene::ISceneNode* Trackers_Reference;
  scene::ISceneNode* Muons_Reference;
  scene::ISceneNode* Magnets_Reference;
  scene::ISceneNode* TC_Reference;
  scene::ISceneNode* EMC_Reference;
  scene::ISceneNode* TRT_Reference;
  scene::ISceneNode* SCT_Reference;
  scene::ISceneNode* Pixels_Reference;
  scene::ISceneNode* background_node_f; //Used for flat view
  scene::ISceneNode* background_node_s; //Used for flat view
  scene::ISceneNode* CameraBB;
  core::matrix4 OrthoCameraFront;
  core::matrix4 OrthoCameraSide;
  CSceneNodeAnimatorCameraSwitch *cameraSwitcher; //Used for switching the camera modes in 3D
  f32 moduleDistanceFromCam;
  f32 moduleAngleFromCam;
  core::vector3df cameraLoc; //camera position for Moses Mode, initialized to zero
  core::vector3df DCamPos;
  
  //Cropping modes
  int _cropMode;
  vector<scene::ISceneNode*> allModules;
  
  
  // Dynamic FPS camera initial parameters
  unsigned int cameraZone;
  float camChangeDist1;
  float camChangeDist2;
  float BBscale;
  
  void dynamicCameraSpeed(core::vector3df camPos);  //Modifying camera speed based on proximity to detector
  void dynamicHidingOfModules(core::vector3df camPos);
  void executeMosesMode(core::vector3df camPos);
  void restoreMosesMode();

  float angleBetween ( scene::ISceneNode* module, core::vector3df cam );
  void createAtlasGeometry();
  void createFlatGeometry();
  
  ATrack3DNode* trackSelection ( core::position2di pos );
  QString detectorSelection( core::position2di pos );
  bool offsetTest;

  //Camera stuff
  ICameraSceneNode *cameras[5];
  int active_viewport;
  int active_cam;
  AFPSControl *fpsControl;
  
  //State values
  bool allowTrackSelection; //Whether to allow track selection
  bool firstShow;
  
  video::ITexture *rt;

  scene::ISceneNode *_logoNode;
  scene::ISceneNodeAnimator *_logoAnim;
  scene::ILightSceneNode *_logoLight;
  
  //Visibility stuff
  QSignalMapper visibilityMapper;
  QBoolSync _detectorVisibility;
  QBoolSync _calorimetersVisibility;
  QBoolSync _trackerVisibility;
  
  QBoolSync _pixelsVisibility;
  QBoolSync _sctVisibility;
  QBoolSync _trtVisibility;
  QBoolSync _larVisibility;
  QBoolSync _tileVisibility;
  QBoolSync _muonSpecVisibility;
  QBoolSync _magnetsVisibility;
  QBoolSync _pitVisibility;
  
  ATrack3DNode* createTrackNode(ATrack* track);
  
  Q_OBJECT
};

#endif // AGEOMETRY_H_
