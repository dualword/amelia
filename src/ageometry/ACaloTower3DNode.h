#ifndef ACALOTOWER3DNODE_H_
#define ACALOTOWER3DNODE_H_

#include "AEventObject3DNode.h"

#include <aeventmanager/AShower.h>

//! Represents calorimeter towers in Irrlicht
class ACaloTower3DNode : public AEventObject3DNode
{
public:
  /*!
   * \brief Constructor
   *
   * \param parent Irrlicht parent of this node
   * \param smgr The Irrlicht scene manager
   * \param ID The Irrlicht ID of the node
   * \param shower The information about the shower represented by this node
   */
  ACaloTower3DNode ( scene::ISceneNode* parent, ISceneManager* base,  s32 ID ,AShower* shower);
  //! Destructor
  virtual ~ACaloTower3DNode();
  
  /*!
   * \brief Set the track style
   * \param style The style you want to set.
   */
  virtual void setStyle( AEventObject3DNode::Style style );

  /*!
   * \brief Set the tower data
   *
   * This function should not be called outside of the constructor! Calling this won't recalculate the scene node..
   *
   * \param The information about the calorimeter represented by this node.
   */
  void setTrack(AShower* track);
  
  /* Irrlicht node functions */
  /// Register for rendering
  virtual void OnRegisterSceneNode();
  /// Draws everything
  virtual void render(); 
  /// Get the bounding box of the tower
  virtual const core::aabbox3d<f32>& getBoundingBox() const;
  /// Get material
  virtual video::SMaterial& getMaterial( s32 i );
  
private:  
  video::SColor color;
  video::SColor vividColor;
  
  //! Bounding box
  core::aabbox3d<f32> Box;
  //! Vertices
  video::S3DVertex Vertices[8];

  Q_OBJECT
};
#endif // ACALOTOWER3DNODE_H_
