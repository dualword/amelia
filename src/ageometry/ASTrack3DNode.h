#ifndef ASTRACK3DNODE_H_
#define ASTRACK3DNODE_H_

#include "AHelix3DNode.h"

/// This class calculates the helix of the track based on kinematic variables.
class ASTrack3DNode : public AHelix3DNode
{
public:
  /*!
   * \brief Constructor
   *
   * \param parent Irrlicht parent of this node
   * \param smgr The Irrlicht scene manager
   * \param ID The Irrlicht ID of the node
   * \param track The information about the track represented by this node
   */
  ASTrack3DNode ( scene::ISceneNode* parent, ISceneManager* smgr,  s32 ID ,ASTrack* track);

  /// Destructor
  virtual ~ASTrack3DNode();
 
private:
  /// Maximum angle
  float maxAngle;
 
  /*!
   * \brief Set the track data
   *
   * This function should not be called outside of the constructor! Calling this won't recalculate the helix (yet)
   *
   * \param The information about the track represented by this node.
   */
  void setTrack(ASTrack* track);

  /********************************************************
   ****************NEUTRAL PARTICLE************************
   ********************************************************/
  /*!
   * \brief Create the helix for a neutral particle
   * This is a simplified version of createCurveVector(), because it does not 
   * need to bend the helix. It just creates a straight line.
   *
   * \param nsegments The number of segments to break the line into.
   */
  void createNeutralVector(int nsegments);
  
  /********************************************************
   ****************CHARGED PARTICLE************************
   ********************************************************/
  virtual float x_helix ( float w, float X_CH, float R, float phi, float charge );
  virtual float y_helix ( float w, float Y_CH, float R, float phi, float charge );
  virtual float z_helix ( float w, float Z_CH, float theta, float R );
  virtual float getChargedMaxAngle ();

  /*!
   * \brief Create the helix for a charged particle
   *
   * \param nsegments The number of segments to break the line into.
   */
  void createCurveVector();

  
  Q_OBJECT
};

#endif // ASTRACK3DNODE_H_
