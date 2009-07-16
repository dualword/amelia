#ifndef ASTRACK3DNODE_H_
#define ASTRACK3DNODE_H_

#include "ATrack3DNode.h"

class ASTrack3DNode : public ATrack3DNode
{
public:
  
  ASTrack3DNode ( scene::ISceneNode* parent, ISceneManager* base,  s32 ID ,ASTrack* track);
  virtual ~ASTrack3DNode();
  
  float tL;
  float maxAngle;
  video::SColor color;
  video::SColor dimmedColor;
  video::SColor vividColor;
  core::vector3df start;
  core::vector3df end;
  float getTl();
  
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
  
  /********************************************************
   ****************NEUTRAL PARTICLE************************
   ********************************************************/
  
  
  virtual std::vector<core::vector3df> getNeutralPath();
  
  virtual void constructNeutral();
  
  virtual void createBoxesNeutral();
  
  virtual void Helix();
  
  /********************************************************
   ****************CHARGED PARTICLE************************
   ********************************************************/
  
  virtual float x_helix ( float w, float X_CH, float R, float phi, float charge );
  
  virtual float y_helix ( float w, float Y_CH, float R, float phi, float charge );
  
  virtual float z_helix ( float w, float Z_CH, float theta, float R );
  
  
  virtual float getChargedMaxAngle ();
  
  virtual void constructCharged();
  
  virtual void createBoxesCharged();
  
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

#endif // ASTRACK3DNODE_H_
