#include "CRelativeScaleSceneNodeAnimator.h"

CRelativeScaleSceneNodeAnimator::CRelativeScaleSceneNodeAnimator(ISceneManager* mgr):smgr(mgr) { }

CRelativeScaleSceneNodeAnimator::~CRelativeScaleSceneNodeAnimator() { }

void CRelativeScaleSceneNodeAnimator::animateNode(ISceneNode *node,u32 timeMs)
{
    vector3df posNode=node->getPosition();
    vector3df posCam=smgr->getActiveCamera()->getPosition();

    vector3df diff=posNode-posCam;
    f32 scale=diff.getLength()*0.01;
    if(scale<=0.1) scale=0.1;

	f32 maxScale=posCam.getLength()*0.03;
	if(scale>=maxScale) scale=maxScale;
	vector3df old=node->getScale();
    node->setScale(vector3df(scale,scale,old.Z));
}
