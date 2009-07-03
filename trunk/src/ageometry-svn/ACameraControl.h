#ifndef ACAMERACONTROL_H_
#define ACAMERACONTROL_H_

#include <irrlicht.h>
using namespace irr;
using namespace gui;
using namespace video;
using namespace core;
using namespace scene;

#include <QObject>

class ACameraControl : public QObject, public IGUIElement
{
public:
  ACameraControl(ICameraSceneNode *camera,ISceneManager *scenemanager, IGUIEnvironment *environment, IGUIElement *parent, s32 id,rect<s32> rectangle);
  
  ICameraSceneNode* camera();

  void setOnlyIfEnabled(bool);
  bool onlyIfEnabled();

  void OnPostRender (u32 timeMs);
  virtual void OnAnimate(u32 timeDeltaMs);

public slots:
  void handleNewActiveCamera(ICameraSceneNode*);
  
private:
  ICameraSceneNode *_camera;

  ISceneManager *SceneManager;

  u32 lastUpdate;
  bool _onlyIfEnabled;

Q_OBJECT
};

#endif //ACAMERACONTROL_H_
