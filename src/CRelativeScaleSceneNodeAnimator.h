#ifndef CRELATIVECUBESCENENODE_H_INCLUDED
#define CRELATIVECUBESCENENODE_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CRelativeScaleSceneNodeAnimator : public ISceneNodeAnimator
{
public:
    CRelativeScaleSceneNodeAnimator(ISceneManager* mgr);
    ~CRelativeScaleSceneNodeAnimator();

    void animateNode(ISceneNode *node,u32 timeMs);

private:
    ISceneManager* smgr;
};

#endif // CRELATIVECUBESCENENODE_H_INCLUDED
