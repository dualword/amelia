#ifndef AFPSCONTROL_H_
#define AFPSCONTROL_H_

#include <irrlicht.h>
using namespace irr;
using namespace gui;
using namespace video;
using namespace core;
using namespace scene;

class AFPSControl : public IGUIElement
{
 public:
  AFPSControl(ICameraSceneNode *camera,ISceneManager *scenemanager, IGUIEnvironment *environment, IGUIElement *parent, s32 id,rect<s32> rectangle);
  
  bool OnEvent (const SEvent &event);

  void OnPostRender (u32 timeMs);

 private:
  IGUIButton *upButton;
  IGUIButton *rightButton;
  IGUIButton *downButton;
  IGUIButton *leftButton;

  IGUIButton *forwardButton;
  IGUIButton *backButton;

  IGUIButton *grabButton;

  ICameraSceneNode *camera;
  ISceneNodeAnimatorCameraFPS *anim;

  ISceneManager *SceneManager;

  u32 lastUpdate;
};

#endif //AFPSCONTROL_H_
