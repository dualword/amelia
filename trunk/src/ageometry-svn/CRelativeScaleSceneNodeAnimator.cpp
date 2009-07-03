#include "CRelativeScaleSceneNodeAnimator.h"

CRelativeScaleSceneNodeAnimator::CRelativeScaleSceneNodeAnimator(ISceneManager* mgr,f32 maxScale)
  :smgr(mgr), _maxScale(maxScale)
{ }

CRelativeScaleSceneNodeAnimator::~CRelativeScaleSceneNodeAnimator() { }

void CRelativeScaleSceneNodeAnimator::animateNode(ISceneNode *node,u32 timeMs)
{
  vector3df posNode=node->getPosition();
  vector3df posCam=smgr->getActiveCamera()->getPosition();
  
  float fov=smgr->getActiveCamera()->getFOV();

  vector3df diff=posNode-posCam;
  f32 scale=diff.getLength()*0.01f;
  if(scale<=0.1f) scale=0.1f;

  if(scale>=_maxScale) scale=_maxScale;

  vector3df old=node->getScale();
  node->setScale(vector3df(scale,scale,old.Z));
  node->updateAbsolutePosition();
}
