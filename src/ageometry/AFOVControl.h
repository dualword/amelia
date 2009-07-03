#ifndef AFOVCONTROL_H_
#define AFOVCONTROL_H_

#include "ACameraControl.h"

class AFOVControl : public ACameraControl
{
 public:
  AFOVControl(ICameraSceneNode *camera,ISceneManager *scenemanager, IGUIEnvironment *environment, IGUIElement *parent, s32 id,rect<s32> rectangle);
  
  virtual void OnAnimate(u32 timeDeltaMs);
  bool OnEvent(const SEvent& event);

  void setVisible(bool);

 private:
  IGUIScrollBar *_fov;
};

#endif //AFOVCONTROL_H_
