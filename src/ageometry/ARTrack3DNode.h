#ifndef ARTRACK3DNODE_H_
#define ARTRACK3DNODE_H_

#include "AHelix3DNode.h"



///This class represents the 3D helix of reconstructed tracks.
class ARTrack3DNode : public AHelix3DNode
{
public:

  ARTrack3DNode ( scene::ISceneNode* parent, ISceneManager* base,  s32 ID ,ARTrack* track);
  virtual ~ARTrack3DNode();

  void setTrack(ARTrack* track);

  Q_OBJECT
};

#endif // ARTRACK3DNODE_H_
