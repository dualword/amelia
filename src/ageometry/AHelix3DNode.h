#ifndef AHELIX3DNODE_H_
#define AHELIX3DNODE_H_

#include "AEventObject3DNode.h"

///This class represent and draw a 3D helix of particle tracks.
class AHelix3DNode : public AEventObject3DNode
{
public:
  /// Destructor
  virtual ~AHelix3DNode();

  /// Set the track style
  /*
   * \param style The style you want to set.
   */
  virtual void setStyle( Style style );

  /* Irrlicht node functions */
  /// Register for rendering
  virtual void OnRegisterSceneNode();
  /// Draws everything
  virtual void render(); 
  /// Get the bounding box of the helix
  /*!
   * \return Box containing all of the helix points
   */
  virtual const core::aabbox3d<f32>& getBoundingBox() const;
  /// Get material
  virtual video::SMaterial& getMaterial( s32 i );

protected:
  /* Variables that hold information in order to display this
     helix. These options are set by any subclass */
  /// In-order list of points connected by this helix
  /*!
   * \brief In-order list of points connected by this helix
   *
   * Should be set by the subclass
   */
  std::vector<core::vector3df> curvePoints; 
  /// Vivid/normal color of the track
  video::SColor vividColor;
  /// Dimmed color of the track
  video::SColor dimmedColor;

  /* Constructor */
  AHelix3DNode ( scene::ISceneNode* parent, ISceneManager* base, s32 ID, AEventObject* track);

  void timerEvent(QTimerEvent*);

private:
  CRelativeScaleSceneNodeAnimator *boxSizeAnim;
  // ID of the blink timer
  int _blinkTimer;
  // Number of blinks the track already went through
  int _blinkCount;

  /*!
   * \brief Simple lines visible
   * Flag to indicate whether the polyline should be draw.
   */
  bool isLineVisible;

  /// Current color of the track
  video::SColor color;

  /// Holds the thick boxes
  std::vector<scene::ISceneNode*> boxSegments;
  
  /*!
   * Draws the lines between the curvePoints
   */
  void drawPolyline();

  /*!
   * \brief Creates boxes.
   * Loops over the lines in the helix, and creates a thick box for each.
   */
  void createBoxes();

  /*!
   * \brief Delete boxes.
   * Loops over the boxes representing the lines of the helix, and deletes them.
   */
  void deleteBoxes();

  /*! 
   * \param boxVisibility
   *
   * Loops over all of the boxes and toggles their visibility to boxVisibility
   */
  void setBoxesVisibility ( bool boxVisibility );

  Q_OBJECT
};

#endif // AHELIX3DNODE_H_
