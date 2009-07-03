#ifndef ASPHERECONTROL_H_
#define ASPHERECONTROL_H_

#include "ACameraControl.h"

class ASphereControl : public ACameraControl
{
 public:
  ASphereControl(ICameraSceneNode *camera,ISceneManager *scenemanager, IGUIEnvironment *environment, IGUIElement *parent, s32 id,rect<s32> rectangle);
  
  virtual void OnAnimate(u32 timeDeltaMs);

 private:
  IGUIButton *_zoomIn;
  IGUIButton *_zoomOut;
};

#endif //ASPHERECONTROL_H_
