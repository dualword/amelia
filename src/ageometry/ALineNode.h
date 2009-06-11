#ifndef ALINENODE_H_
#define ALINENODE_H_

#include <irrlicht.h>
#include <iostream>

using namespace irr;

class ALineNode : public scene::ISceneNode
{
 private:
  core::aabbox3d<f32> Box;
  core::vector3df start;
  core::vector3df end;
  video::SMaterial mat; 

 public:
  ALineNode(core::vector3df,core::vector3df,scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);
  virtual void OnRegisterSceneNode();
  virtual void render();
  virtual const core::aabbox3d<f32>& getBoundingBox() const;
  virtual u32 getMaterialCount() const;
  virtual video::SMaterial& getMaterial(u32 i);
};

#endif //ALINENODE_H_
