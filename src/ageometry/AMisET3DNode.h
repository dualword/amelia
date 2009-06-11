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
  
  video::SColor color;
  video::SColor dimmedColor;
  video::SColor vividColor;
  core::vector3df start;
  core::vector3df end;
  
  bool boxMode;
  float boxWidth;
  std::vector<scene::ISceneNode*> boxSegments;
  virtual void setBoxesVisibility ( bool boxVisibility );
  int trackNumber;
  virtual int getTrackNumber();
  virtual void setBoxesSelected ( bool boxesSelected );
  virtual void setTrackStyle (Style style );
  void setTrack(AMisET* track);
  
  void createMisEtBoxes();
  
  
  virtual void constructNeutral();
  
  virtual void OnRegisterSceneNode();

  virtual void render() {};
  
  virtual const core::aabbox3d<f32>& getBoundingBox() const;
  
  virtual video::SMaterial& getMaterial ( s32 i );

private:
  CRelativeScaleSceneNodeAnimator *boxSizeAnim;
};

#endif // AMISET3DNODE_H_
