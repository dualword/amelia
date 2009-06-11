#ifndef AJET3DNODE_H_
#define AJET3DNODE_H_

#include "ATrack3DNode.h"

class AJet3DNode : public ATrack3DNode
{
 public:
  AJet3DNode ( scene::ISceneNode* parent, ISceneManager* base,  s32 ID ,AJet* jet);
  virtual ~AJet3DNode();
  
    video::SColor color;
    video::SColor vividColor;
    core::vector3df start;
    core::vector3df end;
    float getTl();

    virtual void setBoxesVisibility ( bool boxVisibility );
    int trackNumber;

    virtual int getTrackNumber();
    virtual void setBoxesSelected ( bool boxesSelected );
    virtual void setTrackStyle( ATrack3DNode::Style style );
    void setTrack(AJet* track);

    void createJetPyramids(); //for the jets

    ISceneNode* Pyramid;

    virtual void OnRegisterSceneNode();

    virtual const core::aabbox3d<f32>& getBoundingBox() const;

    virtual video::SMaterial& getMaterial ( s32 i );
    virtual void render() {}

    void select();
    void deselect();
};
#endif // AJET3DNODE_H_
