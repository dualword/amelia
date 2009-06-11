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
  IGUIButton *_1Button;
  IGUIButton *_2Button;
  IGUIButton *_3Button;
  IGUIButton *_4Button;
  IGUIButton *_5Button;
  IGUIButton *_6Button;
  IGUIButton *_7Button;
  IGUIButton *_8Button;

  IGUIButton *forwardButton;
  IGUIButton *backButton;

  ICameraSceneNode *camera;
  ISceneNodeAnimatorCameraFPS *anim;

  ISceneManager *SceneManager;

  u32 lastUpdate;
};

#endif //AFPSCONTROL_H_
