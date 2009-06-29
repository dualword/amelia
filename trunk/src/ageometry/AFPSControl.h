#ifndef AFPSCONTROL_H_
#define AFPSCONTROL_H_

#include "ACameraControl.h"

class AFPSControl : public ACameraControl
{
 public:
  AFPSControl(ICameraSceneNode *camera,ISceneManager *scenemanager, IGUIEnvironment *environment, IGUIElement *parent, s32 id,rect<s32> rectangle);
  
  virtual void OnAnimate(u32 timeDeltaMs);

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

  ISceneNodeAnimatorCameraFPS *anim;
};

#endif //AFPSCONTROL_H_
