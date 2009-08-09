#ifndef AMISET3DNODE_H_
#define AMISET3DNODE_H_

#include "AEventObject3DNode.h"

class AMisET3DNode : public AEventObject3DNode
{
public:
  AMisET3DNode ( scene::ISceneNode* parent, ISceneManager* base,  s32 ID , AMisET* miset);
  virtual ~AMisET3DNode();
  
  virtual void setStyle (Style style );
  void setTrack(AMisET* track);
  
  void createMisEtBoxes();
    
  //Irrlicht node stuff
  virtual void OnRegisterSceneNode();
  virtual void render();
  virtual const core::aabbox3d<f32>& getBoundingBox() const;
  virtual video::SMaterial& getMaterial ( s32 i );

private:
  video::SColor color;
  video::SColor dimmedColor;
  video::SColor vividColor;
  
  bool boxMode;
  float boxWidth;
  std::vector<scene::ISceneNode*> boxSegments;
  CRelativeScaleSceneNodeAnimator *boxSizeAnim;

  Q_OBJECT
};

#endif // AMISET3DNODE_H_
