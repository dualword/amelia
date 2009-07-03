#include "ALineNode.h"

ALineNode::ALineNode(core::vector3df start,core::vector3df end,scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
  : scene::ISceneNode(parent,mgr,id),start(start),end(end)
{ 
  Box.reset(start);
  Box.addInternalPoint(end);
}

void ALineNode::OnRegisterSceneNode()
{
  if (IsVisible)
    SceneManager->registerNodeForRendering(this);
  
  ISceneNode::OnRegisterSceneNode();
}

void ALineNode::render()
{
  video::SColor color(255,255,0,0);
  video::IVideoDriver *driver = SceneManager->getVideoDriver();
  driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
  driver->draw3DLine(start,end,color);
}

const core::aabbox3d<f32>& ALineNode::getBoundingBox() const
{
  return Box;
}

u32 ALineNode::getMaterialCount() const
{
  return 0;
}

video::SMaterial& ALineNode::getMaterial(u32 i)
{
  return mat;
}

