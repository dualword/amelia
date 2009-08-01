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

#include "ARTrack3DNode.h"
#include <QDebug>


const float scaleEvent = 0.05f; // Adjustment scaling factor



ARTrack3DNode::ARTrack3DNode ( scene::ISceneNode* parent, ISceneManager* smgr,  s32 id ,ARTrack* track)
        : ATrack3DNode ( parent, smgr, id ,track),_blinkTimer(-1),_blinkCount(0)
{
    boxSizeAnim = new CRelativeScaleSceneNodeAnimator(smgr,1.5);
    this->setName ( "ARTrack3DNode" );

    calculateDimmedColors();


    createCurveVector();


    std::vector<core::vector3df>::const_iterator iter=curvePoints.begin();
    std::vector<core::vector3df>::const_iterator iterE=curvePoints.end();
    for (;iter!=iterE;iter++)
    {
        Box.addInternalPoint(*iter);
    }


    boxMode=false;
    setTrackStyle(Basic);
}


ARTrack3DNode::~ARTrack3DNode()
{
    boxSizeAnim->drop();
}

void ARTrack3DNode::setTrack ( ASTrack* track )
{
    ATrack3DNode::setTrack(track);
}

int ARTrack3DNode::getTrackNumber()
{
    return this->trackNumber;
}

void ARTrack3DNode::setTrackStyle ( Style style )
{
    ATrack3DNode::setTrackStyle(style);

    switch (style)
    {
    default:
    case Basic:
        //simple line track, no boxes
        if ( boxMode )
        {
            for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
            {
                (*it)->remove();
            }
            boxSegments.clear();
        }
        isLineVisible = true;
        boxMode = false;
        color = this->vividColor;
        addAnimator(boxSizeAnim);
        emit lookChanged();
        break;

    case Selectable:
        //simple line visible in vivid color, boxes present for selection but invisible
        if ( !boxMode )
        {
            createBoxes();
        }
        isLineVisible = true;
        boxMode = true;
        color = this->vividColor;
        removeAnimator(boxSizeAnim);
        setScale(vector3df(1,1,1));
        updateAbsolutePosition();
        for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
        {
            ( *it )->setVisible ( true );
            ( *it )->setMaterialTexture ( 0, SceneManager->getVideoDriver()->getTexture ( "transparent.png" ) );
            ( *it )->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL );
            ( *it )->setDebugDataVisible ( EDS_OFF );
            boxSizeAnim->animateNode((*it),0);
        }
        break;

        /*case Selected:
        //simple line visible, bounding boxes visible
        if ( !boxMode )
        {
        createBoxes();
        }
        this->isLineVisible = true;
        this->boxMode = true;
        this->color = this->vividColor;
        for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
        {
        ( *it )->setVisible ( true );
        ( *it )->setMaterialTexture ( 0, SceneManager->getVideoDriver()->getTexture ( "transparent.png" ) );
        ( *it )->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL );
        ( *it )->setDebugDataVisible ( EDS_BBOX );
        }*/
    case Selected:
        //simple line invisible, boxes visible with original color
        if ( !boxMode )
        {
            createBoxes();
        }
        boxMode = true;
        removeAnimator(boxSizeAnim);
        setScale(vector3df(1,1,1));
        updateAbsolutePosition();
        for ( vector<scene::ISceneNode*>::iterator it = boxSegments.begin() ; it < boxSegments.end(); it++ )
        {
            ( *it )->setVisible ( true );
            ( *it )->setDebugDataVisible ( EDS_OFF );
            ( *it )->setMaterialType ( video::EMT_SOLID );
            ( *it )->setMaterialTexture ( 0, SceneManager->getVideoDriver()->getTexture ( "" ) );
            //(*it)->getMaterial(0).EmissiveColor.set(this->color);
            video::SMaterial* m = & ( *it )->getMaterial ( 0 );
            m->EmissiveColor = vividColor ;
        }
        if (!getTrack()->isInteresting())
        {
            isLineVisible = true;
            if (_blinkTimer==-1)
            {
                _blinkCount=0;
                _blinkTimer=startTimer(100);
            }
        }
        else
        {
            isLineVisible = false;
        }
        emit lookChanged();
        break;
    case Disabled:
        //simple line invisible, boxes visible in grey
        if ( !boxMode )
        {
            createBoxes();
        }
        isLineVisible = false;
        boxMode = true;
        for ( vector<scene::ISceneNode*>::iterator it = boxSegments.begin() ; it < boxSegments.end(); it++ )
        {
            ( *it )->setVisible ( true );
            ( *it )->setDebugDataVisible ( EDS_OFF );
            ( *it )->setMaterialType ( video::EMT_SOLID );
            ( *it )->setMaterialTexture ( 0, SceneManager->getVideoDriver()->getTexture ( "" ) );
            video::SMaterial* m = & ( *it )->getMaterial ( 0 );
            m->EmissiveColor = video::SColor ( 0,122,122,122 );
        }
        emit lookChanged();
        break;

        //simple line invisible, boxes invisible if present
        /*if ( style == 5 )
        {
        isLineVisible = false;
        if ( boxMode )
        {
        for ( vector<scene::ISceneNode*>::iterator it = boxSegments.begin() ; it < boxSegments.end(); it++ )
          {
            ( *it )->setVisible ( false );
          }
        }

        }*/
    }
}

void ARTrack3DNode::select()
{
    setTrackStyle(Selected);
}

void ARTrack3DNode::deselect()
{
    setTrackStyle(Basic);
}

void ARTrack3DNode::calculateDimmedColors()
{

    //this->dimmedColor = this->vividColor.getInterpolated(video::SColor(0,0,0,0), 0.5);
    //vividColor=colorlist[((ARTrack*)getTrack())->getIndex()];
    //dimmedColor = vividColor;
    /*this->dimmedColor.setRed(this->vividColor.getRed()*0.75);
      this->dimmedColor.setGreen(this->color.getGreen()*0.75);
      this->dimmedColor.setBlue(this->color.getBlue()*0.75);
      this->dimmedColor.setAlpha(this->color.getAlpha()*0.75);*/
}


void ARTrack3DNode::createBoxes()
{

     core::vector3df previous = this->curvePoints.at ( 0 );
    vector<core::vector3df>::iterator it;
    scene::IAnimatedMesh* trackCube = SceneManager->getMesh ( "CubeUnit.X" );
    for ( vector<core::vector3df>::iterator it = this->curvePoints.begin() ; it < this->curvePoints.end(); it++ )
    {
        core::vector3df vect = *it - previous;
        core::vector3df pos = ( *it - previous ) /2 + previous;
        core::vector3df rot = vect.getHorizontalAngle();
        core::vector3df scale = core::vector3df ( 0.2f,0.2f,vect.getLength() );

        scene::ISceneNode* nodeBox = 0;
        //nodeBox = SceneManager->addCubeSceneNode(1.0f, 0, -1, pos, rot, scale );

        nodeBox = SceneManager->addMeshSceneNode ( trackCube );
        nodeBox->setPosition ( pos );
        nodeBox->setRotation ( rot );
        nodeBox->setScale ( scale );
        video::SMaterial* m = &nodeBox->getMaterial ( 0 );
        nodeBox->setMaterialType ( video::EMT_SOLID );
        nodeBox->setMaterialFlag ( video::EMF_GOURAUD_SHADING , false );
        nodeBox->setMaterialFlag ( video::EMF_LIGHTING , true );
        nodeBox->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
        nodeBox->setAutomaticCulling ( EAC_OFF );
        nodeBox->addAnimator(boxSizeAnim);
        m->EmissiveColor = this->color ;
        m->DiffuseColor = video::SColor ( 0,0,0,0 );
        m->AmbientColor = video::SColor ( 0,0,0,0 );
        m->Shininess = 128 ;

        nodeBox->setParent ( this );
        nodeBox->getTransformedBoundingBox();
        //nodeBox->setDebugDataVisible(true);
        nodeBox->setID ( 16 );
        nodeBox->setVisible ( false );
        this->boxSegments.push_back ( nodeBox );


        previous = *it;
    }
}




void ARTrack3DNode::createCurveVector()
{
    curvePoints.clear();
    curvePoints = ((ARTrack*)getTrack())->getPolyline();
}

void ARTrack3DNode::drawLines()
{
    video::SMaterial m;
    core::vector3df previous = this->curvePoints.at ( 0 );
    vector<core::vector3df>::iterator it;
    for ( vector<core::vector3df>::iterator it = this->curvePoints.begin() ; it < this->curvePoints.end(); it++ )
    {
        m.EmissiveColor = this->color ;
        SceneManager->getVideoDriver()->setMaterial ( m );

        SceneManager->getVideoDriver()->setTransform ( video::ETS_WORLD, core::matrix4() );
        SceneManager->getVideoDriver()->draw3DLine ( previous, *it );
        previous = *it;
    }
}


void ARTrack3DNode::setBoxesVisibility ( bool boxVisibility )
{
    for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
    {
        ( *it )->setVisible ( boxVisibility );
    }
}

void ARTrack3DNode::setBoxesSelected ( bool boxesSelected )
{
    for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
    {
        video::SMaterial* m = & ( *it )->getMaterial ( 0 );

        boxesSelected ? m->EmissiveColor = video::SColor ( 0,122,122,122 ) : m->EmissiveColor = this->color ;

    }
}


void ARTrack3DNode::Helix()
{
    drawLines();
}


void ARTrack3DNode::OnRegisterSceneNode()
{
    if ( IsVisible )
        SceneManager->registerNodeForRendering ( this );


    ISceneNode::OnRegisterSceneNode();
}

void ARTrack3DNode::render()
{
    if ( this->isLineVisible )
    {
        Helix();
    }
}


const core::aabbox3d<f32>& ARTrack3DNode::getBoundingBox() const
{
    return Box;
}



video::SMaterial& ARTrack3DNode::getMaterial ( s32 i )
{
    return Material;
}

void ARTrack3DNode::timerEvent(QTimerEvent *event)
{
    for ( vector<scene::ISceneNode*>::iterator it = boxSegments.begin() ; it < boxSegments.end(); it++ )
    {
        ( *it )->setVisible ( !( *it )->isVisible() );
    }
    _blinkCount++;
    if (_blinkCount>10)
    {
        killTimer(_blinkTimer);
        _blinkTimer=-1;
        deselect();
    }

    emit lookChanged();
}
