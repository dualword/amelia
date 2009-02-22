/**************** License agreement ******************

AMELIA Copyright (c) 2008, The Regents of the University of California,
through Lawrence Berkeley National Laboratory.  All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
U.S. Dept. of Energy nor the names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches, or upgrades to the features,
functionality or performance of the source code ("Enhancements") to anyone; however, if you choose to
make your Enhancements available either publicly, or directly to Lawrence Berkeley National Laboratory,
without imposing a separate written license agreement for such Enhancements, then you hereby grant the
following license: a  non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software, distribute,
and sublicense such enhancements or derivative works thereof, in binary and source code form.
******************************************************/



#ifndef AHELIXSCENENODE_H
#define AHELIXSCENENODE_H


#include <aeventmanager/ATrack.h>
#include <aeventmanager/ASTrack.h>
#include <aeventmanager/AMisET.h>
#include <aeventmanager/AJet.h>

#include <irrlicht.h>
#include <vector>
#include "CRelativeScaleSceneNodeAnimator.h"


using namespace irr;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::core;
using namespace io;
using namespace std;

class ATrack3DNode : public scene::ISceneNode
{
private:
  
 protected:
  video::SMaterial Material;
  void generateBoxes();
  core::aabbox3d<f32>& getBoundingBox();
  CRelativeScaleSceneNodeAnimator *boxSizeAnim;
  
  void setTrack(ATrack* track);
  
 public:
  
  ATrack3DNode ( scene::ISceneNode* parent, ISceneManager* base,  s32 ID ,ATrack* track=0);
  virtual ~ATrack3DNode();
  
  core::aabbox3d<f32> Box;
  ATrack* trackPointer;
  int style;
  int trackID;
  virtual ATrack* getTrack();
  virtual void setTrackStyle(int style) {}
  
  virtual void select();
  virtual void deselect();
};



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
    virtual void setTrackStyle ( int style );
    virtual void calculateDimmedColors();
    void setTrack(ASTrack* track);

    void createBoxes();
    void select();
    void deselect();

    /*********************************************************

    ****************NEUTRAL PARTICLE**************************

    //********************************************************/


    virtual std::vector<core::vector3df> getNeutralPath();

    virtual void constructNeutral();

    virtual void createBoxesNeutral();

    virtual void Helix();

    /*********************************************************

    ****************CHARGED PARTICLE**************************

    //********************************************************/

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

};


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
    virtual void setTrackStyle ( int style );
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
    virtual void setTrackStyle ( int style );
    void setTrack(AMisET* track);

    void createMisEtBoxes();


    virtual void constructNeutral();

    virtual void OnRegisterSceneNode();

    virtual void render() {};

    virtual const core::aabbox3d<f32>& getBoundingBox() const;

    virtual video::SMaterial& getMaterial ( s32 i );
};



#endif // AHELIXSCENENODE_H


