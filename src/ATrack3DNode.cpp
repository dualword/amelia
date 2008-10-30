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




#include "ATrack3DNode.h"


static const float Length = 2500; // half-length
static const float Radius = 43; //radius of the tracking zone
static const float DetectorRadius = 1500; //radius of the detector
const float Pi = 3.14159265358979f;
const float RadDeg = 180.0f/Pi;
//float mField = 1; // Magnetic Field (Tesla)
int helixSections = 10; // Number of sections in each helix track
const float scaleEvent = 0.05; // Adjustment scaling factor


//First, let's define the methods of the parent class for 3D nodes in tracks

ATrack3DNode::ATrack3DNode ( scene::ISceneNode* parent, AGeometry* base,  s32 id )
        : scene::ISceneNode ( parent, base->GetSceneManager(), id )
{
    boxSizeAnim = new CRelativeScaleSceneNodeAnimator(base->GetSceneManager());
    Base = base;
    this->setName ( "Track3DNode" );
}


ATrack3DNode::~ATrack3DNode()
{
    boxSizeAnim->drop();
}


// Then the children classes, for each type of ATrack


ASTrack3DNode::ASTrack3DNode ( scene::ISceneNode* parent, AGeometry* base,  s32 id )
        : ATrack3DNode ( parent, base, id )
{
    boxSizeAnim = new CRelativeScaleSceneNodeAnimator(base->GetSceneManager());
    Base = base;
    this->setName ( "Track3DNode" );
}


ASTrack3DNode::~ASTrack3DNode()
{
    boxSizeAnim->drop();
}

int ASTrack3DNode::getTrackNumber()
{
    return this->trackNumber;
}

void ASTrack3DNode::setTrack ( ASTrack* track )
{
    this->trackPointer = track;
}

ATrack* ASTrack3DNode::getTrack()
{
    return this->trackPointer;
}


ATrack* ASTrack3DNode::getTrackById ( int id )
{
    for ( vector<ATrack*>::iterator iter = Base->XmlEvt->Event.Tracks.begin(); iter < Base->XmlEvt->Event.Tracks.end(); iter++ )
    {
        if ( (*iter)->trackID == trackID )
        {
            return *iter;
            break;
        }
    }
}

void ASTrack3DNode::setTrackStyle ( int style )
{

    if ( trackPointer->Type == 1 ) //if it's a particle track
    {
        //simple line track, no boxes
        if ( style == 0 )
        {
            if ( boxMode )
            {
                for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
                {
                    Base->GetSceneManager()->addToDeletionQueue ( *it ); //smoother
                    //(*it)->remove(); //faster
                }
                boxSegments.clear();
            }
            this->color = this->vividColor;

            this->boxMode = false;
        }

        //simple line visible in vivid color, boxes present for selection but invisible
        if ( style == 1 )
        {
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
                ( *it )->setMaterialTexture ( 0, Base->GetDriver()->getTexture ( "transparent.png" ) );
                ( *it )->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL );
                ( *it )->setDebugDataVisible ( EDS_OFF );
            }
        }

        //simple line visible, bounding boxes visible
        if ( style == 2 )
        {
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
                ( *it )->setMaterialTexture ( 0, Base->GetDriver()->getTexture ( "transparent.png" ) );
                ( *it )->setMaterialType ( video::EMT_TRANSPARENT_ALPHA_CHANNEL );
                ( *it )->setDebugDataVisible ( EDS_BBOX );
            }
        }

        //simple line invisible, boxes visible with original color
        if ( style == 3 )
        {
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
                ( *it )->setMaterialTexture ( 0, Base->GetDriver()->getTexture ( "" ) );
                //(*it)->getMaterial(0).EmissiveColor.set(this->color);
                video::SMaterial* m = & ( *it )->getMaterial ( 0 );
                m->EmissiveColor = vividColor ;
            }
        }

        //simple line invisible, boxes visible in grey
        if ( style == 4 )
        {
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
                ( *it )->setMaterialTexture ( 0, Base->GetDriver()->getTexture ( "" ) );
                video::SMaterial* m = & ( *it )->getMaterial ( 0 );
                m->EmissiveColor = video::SColor ( 0,122,122,122 );
            }
        }

        //simple line invisible, boxes invisible if present
        if ( style == 5 )
        {
            isLineVisible = false;
            if ( boxMode )
            {
                for ( vector<scene::ISceneNode*>::iterator it = boxSegments.begin() ; it < boxSegments.end(); it++ )
                {
                    ( *it )->setVisible ( false );
                }
            }

        }
    }// end of track styles

}

void ASTrack3DNode::calculateDimmedColors()
{
    //this->dimmedColor = this->vividColor.getInterpolated(video::SColor(0,0,0,0), 0.5);
    dimmedColor = vividColor;
    /*this->dimmedColor.setRed(this->vividColor.getRed()*0.75);
    this->dimmedColor.setGreen(this->color.getGreen()*0.75);
    this->dimmedColor.setBlue(this->color.getBlue()*0.75);
    this->dimmedColor.setAlpha(this->color.getAlpha()*0.75);*/
}

/*********************************************************

****************NEUTRAL PARTICLE**************************

//********************************************************/


std::vector<core::vector3df> ASTrack3DNode::getNeutralPath()

{
    std::vector<core::vector3df> StartEnd ( 2 );
    float c = 180/3.1415926;
    float sc = 0.001;
    float theta = 2*atan ( exp ( -(trackPointer->eta) ) );

    float X0 = trackPointer->rhoVertex * cos ( trackPointer->phiVertex ) *sc;
    float Y0 = trackPointer->rhoVertex * sin ( trackPointer->phiVertex ) *sc;
    float Z0 = trackPointer->zVertex*sc;
    float Xdir = sin ( trackPointer->phi );
    float Ydir = cos ( trackPointer->phi );
    float Zdir = 1/tan ( theta );
    //float Length_dir = sqrt(Xdir*Xdir + Ydir*Ydir + Zdir*Zdir);

    float Length_multiply = 0;
    while ( ( ( ( Xdir*Length_multiply+X0 ) * ( Xdir*Length_multiply+X0 ) + ( Ydir*Length_multiply+Y0 ) * ( Ydir*Length_multiply+Y0 ) ) <=Radius*Radius ) )
    {
        Length_multiply +=0.5;
    }

    if ( Length_multiply > Length )
    {
        Length_multiply = Length;
    }

    if ( ( X0*X0+Y0*Y0 ) <=Radius*Radius )
    {
        core::vector3df start0 = core::vector3df ( X0,Y0,Z0 );
        StartEnd.clear();
        StartEnd.push_back ( start0 );
        this->start = start0;
        core::vector3df direction = core::vector3df ( Xdir, Ydir, Zdir );
        //core::vector3df end0  = start + direction*Length_multiply;
        core::vector3df end0  = start0 + direction*Length_multiply;

        StartEnd.push_back ( end0 );

        this->end = end0;
    }

    return StartEnd;
}


void ASTrack3DNode::createBoxes()
{
    if ( trackPointer->q == 0 )
    {
        createBoxesNeutral();
    }
    else
    {
        createBoxesCharged();
    }
}



void ASTrack3DNode::createBoxesNeutral()
{
    core::vector3df vect0 = this->end - this->start;
    core::vector3df pos0 = ( vect0 ) /2 + this->start;
    core::vector3df rot = vect0.getHorizontalAngle();
    core::vector3df zero = core::vector3df ( 0,0,0 );

    scene::IAnimatedMesh* trackCube = Base->GetSceneManager()->getMesh ( "CubeUnit.X" );


    core::vector3df vect = vect0/10;
    core::vector3df scale = core::vector3df ( 0.2,0.2, vect.getLength() );
    for (int i = 0; i < 10; i++) // let's reate segments for the neutral tracks, to avoid perspective tappering
    {
        core::vector3df pos = vect*i + ( vect ) /2 + this->start;

        scene::ISceneNode* nodeBox = 0;
        nodeBox = Base->GetSceneManager()->addMeshSceneNode ( trackCube->getMesh ( 0 ) );
        nodeBox->setPosition ( pos );
        nodeBox->setRotation ( rot );
        nodeBox->setScale ( scale );
        //nodeBox = SceneManager->addCubeSceneNode(1.0f, 0, -1, pos, rot, scale );

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

        nodeBox->getTransformedBoundingBox();
        nodeBox->setParent ( this );
        //nodeBox->setDebugDataVisible(true);
        nodeBox->setID ( 16 );
        nodeBox->setVisible ( false );
        this->boxSegments.push_back ( nodeBox );
    }
}


void ASTrack3DNode::constructNeutral()
{

    video::SMaterial m;
    m.EmissiveColor = this->color ;
    Base->GetDriver()->setMaterial ( m );

    Base->GetDriver()->setTransform ( video::ETS_WORLD, core::matrix4() );
    Base->GetDriver()->draw3DLine ( this->start, this->end ,this->color);

}

/*********************************************************

****************CHARGED PARTICLE**************************

//********************************************************/

float ASTrack3DNode::x_helix ( float w, float X_CH, float R, float phi, float charge )
{
    return ( X_CH - charge * R*cos ( phi + charge*w ) );
}



float ASTrack3DNode::y_helix ( float w, float Y_CH, float R, float phi, float charge )
{
    return ( Y_CH - charge * R*sin ( phi + charge*w ) );
}

float ASTrack3DNode::z_helix ( float w, float Z_CH, float theta, float R )
{
    return ( Z_CH + w*R* ( 1/tan ( theta ) ) );
}


float ASTrack3DNode::getChargedMaxAngle ()

{


    float C = 1000./0.6;
    float sc = 0.001;
    float theta = 2*atan ( exp ( -(trackPointer->eta) ) );
    float X0 = trackPointer->rhoVertex * cos ( trackPointer->phiVertex ) *sc; // The X coordinate of the vertex
    float Y0 = trackPointer->rhoVertex * sin ( trackPointer->phiVertex ) *sc; // The Y coordinate of the vertex
    //float Z0 = v_z;                       // The Z coordinate of the vertex
    float R = trackPointer->pt*C;
    float X_CH = X0 + trackPointer->q * R*cos ( trackPointer->phi ); //The X coordinate of the center of the helix
    float Y_CH = Y0 + trackPointer->q * R*sin ( trackPointer->phi ); //The Y coordinate of the center of the helix
    float E = exp ( trackPointer->eta );
    float tL = 0.5 * ( exp ( trackPointer->eta ) - exp ( -trackPointer->eta ) ); //dip of track = Pz/pTTrack, constant along the helix
    //float startPhi = 90 - phi0*RadDeg + RadDeg*atan(Y_CH/X_CH); //phi0 and the projection angle on the helix are out of phase
    //float startPhi = phi0+adjPhi;
    float Z_CH = trackPointer->zVertex*sc; //- R * startPhi*tL;                     //The Z coordinate of the center of the helix
    float a=0.05/RadDeg;

    for ( int w=0; w<=5000; w++ )
    {
        if ( ( x_helix ( w*a, X_CH, R, trackPointer->phi, trackPointer->q ) *x_helix ( w*a, X_CH, R, trackPointer->phi, trackPointer->q ) + y_helix ( w*a, Y_CH, R, trackPointer->phi, trackPointer->q ) *y_helix ( w*a, Y_CH, R, trackPointer->phi, trackPointer->q ) ) >= Radius*Radius/ ( scaleEvent*scaleEvent ) || ( z_helix ( w*a, Z_CH, theta, R ) *z_helix ( w*a, Z_CH, theta, R ) >=Length*Length ) )
        {
            return w*a;
            break;
        }


    }
}


void ASTrack3DNode::createCurveVector()
{
    float pi = 3.1415926;
    float phiTrans = -(trackPointer->phi) + pi;
    float C = 1000/0.6;
    float theta = 2*atan ( exp ( - ( trackPointer->eta ) ) );
    float X0 = trackPointer->rhoVertex * cos ( trackPointer->phiVertex ); // The X coordinate of the vertex
    float Y0 = trackPointer->rhoVertex * sin ( trackPointer->phiVertex ); // The Y coordinate of the vertex
    float Z0 = trackPointer->zVertex;                       // The Z coordinate of the vertex
    float R = trackPointer->pt*C;
    float X_CH = X0 + trackPointer->q * R*cos ( phiTrans ); //The X coordinate of the center of the helix
    float Y_CH = Y0 + trackPointer->q * R*sin ( phiTrans ); //The Y coordinate of the center of the helix
    float E = exp ( trackPointer->eta );
    float tL = 0.5 * ( exp (trackPointer->eta) - exp ( - (trackPointer->eta) ) ); //dip of track = Pz/pTTrack, constant along the helix
    //float startPhi = 90 - phi0*RadDeg + RadDeg*atan(Y_CH/X_CH); //phi0 and the projection angle on the helix are out of phase
    //float startPhi = phi0+adjPhi;
    float Z_CH = trackPointer->zVertex; //- R * startPhi*tL;                     //The Z coordinate of the center of the helix


    // Output:
    // tL = dip of track = Pz/pTTrack =0.5 * ( E - 1/E ) : constant along the helix
    // eta = ln( Pz/pTTrack + SQRT[(Pz/pTTrack)**2 +1 ]
    // d0 = distance between circle and (0,0) at point of closest approach PCA
    // Fs = angle between (0,0) and vertex as seen from the circle center [degrees]
    // Z0 = Z of circle at PCA
    // F0 = phi0 of track at PCA [degrees]
    // Pq = charge*pTTrack
    // For helices starting at PCA: Fs=0. this is the case for reconstructed
    // helices, which are not yet associated to a secondary vertex or are
    // drawn to PCA.
    // calculation:
    // rC = C*pTTrack                            C = 100./0.6
    // rC = C*pTTrack
    // xC = rhoVertex*cos(phiVertex) + rC*sin(phiTrack)
    // yC = rhoVertex*sin(phiVertex) - rC*cos(phiTrack)
    // tl = Pz/pTTrack = 0.5 * ( E - 1/E )
    // d0 = rC - sqrt(xC*xC + yC*yC)
    // startPhi = pi/2 - phiTrack + atan2(yC,xC)    modify startPhi to: -pi/2 < startPhi < pi/2
    // z0 = zVertex - rC*startPhi*tl
    // phi0 = phiTrack + startPhi
    // pCharge = charge*pTTrack
    // Change startPhi and phi0 from radians to degrees.
    // ///

    this->curvePoints.clear();
    core::vector3df point;
    float angularStep = ( ( this->maxAngle ) /helixSections );

    for ( int w=0; w<=helixSections; w++ )
    {
        point = scaleEvent*core::vector3df ( x_helix ( w*angularStep, X_CH, R, phiTrans, trackPointer->q ) , y_helix ( w*angularStep, Y_CH, R, phiTrans, trackPointer->q ) , z_helix ( w*angularStep, Z_CH, theta, R ) );
        this->curvePoints.push_back ( point );
    }
}

void ASTrack3DNode::constructCharged()

{
    video::SMaterial m;
    core::vector3df previous = this->curvePoints.at ( 0 );
    vector<core::vector3df>::iterator it;
    for ( vector<core::vector3df>::iterator it = this->curvePoints.begin() ; it < this->curvePoints.end(); it++ )
    {
        m.EmissiveColor = this->color ;
        Base->GetDriver()->setMaterial ( m );

        Base->GetDriver()->setTransform ( video::ETS_WORLD, core::matrix4() );
        Base->GetDriver()->draw3DLine ( previous, *it );
        previous = *it;
    }

}

void ASTrack3DNode::createBoxesCharged()
{
    core::vector3df previous = this->curvePoints.at ( 0 );
    vector<core::vector3df>::iterator it;
    scene::IAnimatedMesh* trackCube = Base->GetSceneManager()->getMesh ( "CubeUnit.X" );
    for ( vector<core::vector3df>::iterator it = this->curvePoints.begin() ; it < this->curvePoints.end(); it++ )
    {

        Base->GetDriver()->draw3DLine ( previous, *it );

        core::vector3df vect = *it - previous;
        core::vector3df pos = ( *it - previous ) /2 + previous;
        core::vector3df rot = vect.getHorizontalAngle();
        core::vector3df scale = core::vector3df ( 0.2,0.2,vect.getLength() );

        scene::ISceneNode* nodeBox = 0;
        //nodeBox = SceneManager->addCubeSceneNode(1.0f, 0, -1, pos, rot, scale );

        nodeBox = Base->GetSceneManager()->addMeshSceneNode ( trackCube->getMesh ( 0 ) );
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

void ASTrack3DNode::setBoxesVisibility ( bool boxVisibility )
{
    for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
    {
        ( *it )->setVisible ( boxVisibility );
    }
}

void ASTrack3DNode::setBoxesSelected ( bool boxesSelected )
{
    for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
    {
        video::SMaterial* m = & ( *it )->getMaterial ( 0 );

        boxesSelected ? m->EmissiveColor = video::SColor ( 0,122,122,122 ) : m->EmissiveColor = this->color ;

    }
}


void ASTrack3DNode::Helix()
{

    if ( trackPointer->q == 0 )
    {
        constructNeutral();
    }
    else
    {
        constructCharged();
    }
    tL = 0.5 * ( exp (trackPointer->eta) - exp (-(trackPointer->eta)));
}

float ASTrack3DNode::getTl()
{
    return tL = 0.5 * ( exp (trackPointer->eta) - exp (-(trackPointer->eta)));
}


void ASTrack3DNode::OnRegisterSceneNode()
{
    if ( IsVisible )
        Base->GetSceneManager()->registerNodeForRendering ( this );


    ISceneNode::OnRegisterSceneNode();
}

void ASTrack3DNode::render()
{
    if ( this->isLineVisible )
    {
        Helix();
    }
}


const core::aabbox3d<f32>& ASTrack3DNode::getBoundingBox() const
{
    return Box;
}



video::SMaterial& ASTrack3DNode::getMaterial ( s32 i )
{
    return Material;
}



// Now the Jets


AJet3DNode::AJet3DNode ( scene::ISceneNode* parent, AGeometry* base,  s32 id )
        : ATrack3DNode ( parent, base, id )
{
    boxSizeAnim = new CRelativeScaleSceneNodeAnimator(base->GetSceneManager());
    Base = base;
    this->setName ( "Track3DNode" );
    Pyramid = NULL;
}


AJet3DNode::~AJet3DNode()
{
    boxSizeAnim->drop();
}

int AJet3DNode::getTrackNumber()
{
    return this->trackNumber;
}

void AJet3DNode::setTrack ( AJet* track )
{
    this->trackPointer = track;
}
ATrack* AJet3DNode::getTrack()
{
    return this->trackPointer;
}

ATrack* AJet3DNode::getTrackById ( int id )
{
    for ( vector<ATrack*>::iterator iter = Base->XmlEvt->Event.Tracks.begin(); iter < Base->XmlEvt->Event.Tracks.end(); iter++ )
    {
        if ( (*iter)->trackID == trackID )
        {
            return *iter;
            break;
        }
    }
}

void AJet3DNode::setTrackStyle ( int style )
{

    if ( trackPointer->Type == 2 ) //if it's a jet
    {
        //selected jet style
        if ( style == 6 )
        {
            Pyramid->setVisible(true);
            video::SMaterial* m = &Pyramid->getMaterial ( 0 );
            m->EmissiveColor = video::SColor ( 0,255,0,0 );
        }

        //unselected jet style
        if ( style == 7 )
        {
            Pyramid->setVisible(true);
            video::SMaterial* m = &Pyramid->getMaterial ( 0 );
            m->EmissiveColor = video::SColor ( 0,100,100,100 );
        }

        //invisible jet style
        if ( style == 8 )
        {
            Pyramid->setVisible(false);
        }

        //restore the jet visibility
        if ( style == 9 )
        {
            Pyramid->setVisible ( true );
        }
    }


}

void AJet3DNode::createJetPyramids()
{
    if ( type == 2 ) //jet
    {
        float pi = 3.1415926;
        float c = 180/pi;
        float theta = 2*atan ( exp ( -(trackPointer->eta )) );
        core::vector3df zero = core::vector3df ( 0,0,0 );
        core::vector3df scale = core::vector3df ( 0.5,0.5,1 );
        //core::vector3df rot = core::vector3df(phi * c - 90, 90, theta * c);
        core::vector3df rot = core::vector3df ( -theta * c, 0, -trackPointer->phi * c ); //

        scene::IAnimatedMesh* pyramid = Base->GetSceneManager()->getMesh ( "jet.X" );
        scene::ISceneNode* nodeBox = 0;
        nodeBox = Base->GetSceneManager()->addMeshSceneNode ( pyramid->getMesh ( 0 ) );
        nodeBox->setPosition ( zero );
        nodeBox->setRotation ( rot );
        nodeBox->updateAbsolutePosition();
        nodeBox->setScale ( scale );
        nodeBox->getTransformedBoundingBox();
        nodeBox->setID ( 18 );

        scene::ITriangleSelector* selector = 0;
        selector = Base->GetSceneManager()->createOctTreeTriangleSelector ( pyramid->getMesh ( 0 ), nodeBox, 128 );
        nodeBox->setTriangleSelector ( selector );
        selector->drop();

        video::SMaterial* m = &nodeBox->getMaterial ( 0 );
        nodeBox->setMaterialType ( video::EMT_TRANSPARENT_ADD_COLOR );
        nodeBox->setMaterialFlag ( video::EMF_GOURAUD_SHADING , false );
        nodeBox->setMaterialFlag ( video::EMF_NORMALIZE_NORMALS, true );
        nodeBox->setMaterialFlag ( video::EMF_LIGHTING , true );
        nodeBox->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
        nodeBox->setAutomaticCulling ( EAC_OFF );
        m->EmissiveColor = video::SColor ( 0,100,100,100 );
        m->DiffuseColor = video::SColor ( 0,0,0,0 );
        m->AmbientColor = video::SColor ( 0,0,0,0 );
        m->Shininess = 0 ;

        nodeBox->getTransformedBoundingBox();
        nodeBox->setParent ( this );
        //nodeBox->setDebugDataVisible(EDS_BBOX);
        //nodeBox->setVisible(false);
        Pyramid = nodeBox;

    }
}


void AJet3DNode::setBoxesVisibility ( bool boxVisibility )
{
    Pyramid->setVisible ( boxVisibility );
}

void AJet3DNode::setBoxesSelected ( bool boxesSelected )
{
    video::SMaterial* m = &Pyramid->getMaterial ( 0 );
    boxesSelected ? m->EmissiveColor = video::SColor ( 0,122,122,122 ) : m->EmissiveColor = this->color ;
}


void AJet3DNode::OnRegisterSceneNode()
{
    if ( IsVisible )
        Base->GetSceneManager()->registerNodeForRendering ( this );


    ISceneNode::OnRegisterSceneNode();
}


const core::aabbox3d<f32>& AJet3DNode::getBoundingBox() const
{
    return Box;
}



video::SMaterial& AJet3DNode::getMaterial ( s32 i )
{
    return Material;
}


float AJet3DNode::getTl()
{
    float tL = 0.5 * ( exp (trackPointer->eta) - exp (-(trackPointer->eta)));
    return tL;
}



AMisET3DNode::AMisET3DNode ( scene::ISceneNode* parent, AGeometry* base,  s32 id )
        : ATrack3DNode ( parent, base, id )
{
    boxSizeAnim = new CRelativeScaleSceneNodeAnimator(base->GetSceneManager());
    Base = base;
    this->setName ( "Track3DNode" );
}


AMisET3DNode::~AMisET3DNode()
{
    boxSizeAnim->drop();
}

int AMisET3DNode::getTrackNumber()
{
    return this->trackNumber;
}

void AMisET3DNode::setTrack ( AMisET* track )
{
    this->trackPointer = track;
}

ATrack* AMisET3DNode::getTrack()
{
    return this->trackPointer;
}


ATrack* AMisET3DNode::getTrackById ( int id )
{
    for ( vector<ATrack*>::iterator iter = Base->XmlEvt->Event.Tracks.begin(); iter < Base->XmlEvt->Event.Tracks.end(); iter++ )
    {
        if ( (*iter)->trackID == trackID )
        {
            return *iter;
            break;
        }
    }
}

void AMisET3DNode::setTrackStyle ( int style )
{

    if ( trackPointer->Type == 4 ) //if it's Missing Et
    {
        //selected Missing Et style
        if ( style == 10 )
        {
            for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
            {

                ( *it )->setVisible ( true );
                ( *it )->setDebugDataVisible ( EDS_OFF );
                ( *it )->setMaterialType ( video::EMT_SOLID );
                ( *it )->setMaterialTexture ( 0, Base->GetDriver()->getTexture ( "" ) );
                video::SMaterial* m = & ( *it )->getMaterial ( 0 );
                m->EmissiveColor = video::SColor ( 0,255,255,122 );



                /* video::SMaterial* m = & ( *it )->getMaterial ( 0 );
                 m->EmissiveColor = video::SColor ( 0,255,0,0 );*/
            }
        }

        //unselected Missing Et style
        if ( style == 11 )
        {
            for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
            {
                video::SMaterial* m = & ( *it )->getMaterial ( 0 );
                m->EmissiveColor = video::SColor ( 0,0,0,0 );
            }
        }

        //invisible Missing Et style
        if ( style == 12 )
        {
            for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
            {
                ( *it )->setVisible ( false );
            }
        }

        //restore the Missing Et visibility
        if ( style == 13 )
        {
            for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
            {
                ( *it )->setVisible ( true );
            }
        }
    }



}


void AMisET3DNode::createMisEtBoxes() //for Missing Et
{
    core::vector3df zero = core::vector3df ( 0,0,0 );
    end = core::vector3df ( trackPointer->etx, trackPointer->ety,0 );

    core::vector3df rot = end.getHorizontalAngle();
    core::vector3df scale = core::vector3df ( 5,5, end.getLength() );

    scene::IAnimatedMesh* trackCube = Base->GetSceneManager()->getMesh ( "CubeUnit.X" );
    scene::ISceneNode* nodeBox = 0;
    nodeBox = Base->GetSceneManager()->addMeshSceneNode ( trackCube->getMesh ( 0 ) );
    nodeBox->setPosition ( end/2 );
    nodeBox->setRotation ( rot );
    nodeBox->setScale ( scale );

    video::SMaterial* m = &nodeBox->getMaterial ( 0 );
    nodeBox->setMaterialType ( video::EMT_SOLID );
    nodeBox->setMaterialFlag ( video::EMF_GOURAUD_SHADING , false );
    nodeBox->setMaterialFlag ( video::EMF_LIGHTING , true );
    nodeBox->setMaterialFlag ( video::EMF_BACK_FACE_CULLING, false );
    nodeBox->setAutomaticCulling ( EAC_OFF );
    nodeBox->addAnimator(boxSizeAnim);
    color = video::SColor ( 0,0,0,0 );
    m->EmissiveColor = this->color ;
    m->DiffuseColor = video::SColor ( 0,0,0,0 );
    m->AmbientColor = video::SColor ( 0,0,0,0 );
    m->Shininess = 128 ;

    nodeBox->getTransformedBoundingBox();
    nodeBox->setParent ( this );
    //nodeBox->setDebugDataVisible(true);
    nodeBox->setID ( 16 );
    nodeBox->setVisible ( true );
    this->boxSegments.push_back ( nodeBox );


}


void AMisET3DNode::constructNeutral()
{

    video::SMaterial m;
    m.EmissiveColor = this->color ;
    Base->GetDriver()->setMaterial ( m );

    Base->GetDriver()->setTransform ( video::ETS_WORLD, core::matrix4() );
    Base->GetDriver()->draw3DLine ( this->start, this->end ,this->color);

}


void AMisET3DNode::setBoxesVisibility ( bool boxVisibility )
{
    for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
    {
        ( *it )->setVisible ( boxVisibility );
    }
}

void AMisET3DNode::setBoxesSelected ( bool boxesSelected )
{
    for ( vector<scene::ISceneNode*>::iterator it = this->boxSegments.begin() ; it < this->boxSegments.end(); it++ )
    {
        video::SMaterial* m = & ( *it )->getMaterial ( 0 );

        boxesSelected ? m->EmissiveColor = video::SColor ( 0,122,122,122 ) : m->EmissiveColor = this->color ;

    }
}




void AMisET3DNode::OnRegisterSceneNode()
{
    if ( IsVisible )
        Base->GetSceneManager()->registerNodeForRendering ( this );


    ISceneNode::OnRegisterSceneNode();
}


const core::aabbox3d<f32>& AMisET3DNode::getBoundingBox() const
{
    return Box;
}



video::SMaterial& AMisET3DNode::getMaterial ( s32 i )
{
    return Material;
}










