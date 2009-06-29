#ifndef AMISET3DNODE_H_
#define AMISET3DNODE_H_

#include "ATrack3DNode.h"

class AMisET3DNode : public ATrack3DNode
{
public:
  AMisET3DNode ( scene::ISceneNode* parent, ISceneManager* base,  s32 ID , AMisET* miset);
  virtual ~AMisET3DNode();
  
  void select();
  void deselect();
  
  virtual int getTrackNumber();
  virtual void setTrackStyle (Style style );
  void setTrack(AMisET* track);
  
  void createMisEtBoxes();
    
  //Irrlicht node stuff
  virtual void OnRegisterSceneNode();
  virtual void render();
  virtual const core::aabbox3d<f32>& getBoundingBox() const;
  virtual video::SMaterial& getMaterial ( s32 i );

private:
  CRelativeScaleSceneNodeAnimator *boxSizeAnim;

  video::SColor color;
  video::SColor dimmedColor;
  video::SColor vividColor;
  //core::vector3df start;
  //core::vector3df end;
  
  bool boxMode;
  float boxWidth;
  std::vector<scene::ISceneNode*> boxSegments;

  int trackNumber;
};

#endif // AMISET3DNODE_H_
