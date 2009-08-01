#ifndef ARTRACK3DNODE_H_
#define ARTRACK3DNODE_H_

#include "ATrack3DNode.h"



///This class represents the 3D helix of reconstructed tracks.
class ARTrack3DNode : public ATrack3DNode
{
public:

  ARTrack3DNode ( scene::ISceneNode* parent, ISceneManager* base,  s32 ID ,ARTrack* track);
  virtual ~ARTrack3DNode();


  video::SColor color;
  video::SColor dimmedColor;
  video::SColor vividColor;

  bool isLineVisible;
  bool boxMode;
  float boxWidth;
  std::vector<scene::ISceneNode*> boxSegments;
  std::vector<core::vector3df> curvePoints;
  virtual void setBoxesVisibility ( bool boxVisibility );
  int trackNumber;

  virtual int getTrackNumber();
  virtual void setBoxesSelected ( bool boxesSelected );
  virtual void setTrackStyle( Style style );
  virtual void calculateDimmedColors();
  void setTrack(ASTrack* track);

  void createBoxes();
  void select();
  void deselect();


  virtual void Helix();

  virtual void drawLines();

  void createCurveVector();

  virtual void OnRegisterSceneNode();

  virtual void render();

  virtual const core::aabbox3d<f32>& getBoundingBox() const;

  virtual video::SMaterial& getMaterial ( s32 i );

protected:
  void timerEvent(QTimerEvent*);

private:
  CRelativeScaleSceneNodeAnimator *boxSizeAnim;
  int _blinkTimer;
  int _blinkCount;

  Q_OBJECT
};

#endif // ARTRACK3DNODE_H_
