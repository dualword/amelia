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

    //! We need to define this method because it's a pure virtual function on ISceneNodeAnimator
    virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0) { return 0; };

private:
    ISceneManager* smgr;
};

#endif // CRELATIVECUBESCENENODE_H_INCLUDED
