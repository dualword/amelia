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

#include "ASTrack3DNode.h"
#include <QDebug>

static const float Length = 2500.f; // half-length
static const float Radius = 43.f; //radius of the tracking zone
static const float DetectorRadius = 1500.f; //radius of the detector
const float Pi = 3.14159265358979f;
const float RadDeg = 180.0f/Pi;
//float mField = 1; // Magnetic Field (Tesla)
int helixSections = 10; // Number of sections in each helix track
const float scaleEvent = 0.05f; // Adjustment scaling factor

const video::SColor colorlist[52] =
  {
    video::SColor ( 0,0,0,0 ), video::SColor ( 0,255,255,0 ), video::SColor ( 0,55,55,55 ), video::SColor ( 0,0,0,255 ), video::SColor ( 0,55,55,55 ),
    video::SColor ( 0,255,255,0 ), video::SColor ( 0,55,55,55 ), video::SColor ( 0,0,255,0 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,255 ),
    video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,111,111,111 ), video::SColor ( 0,77,77,77 ), video::SColor ( 0,255,255,255 ),
    video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,0 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ),
    video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,255 ),video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,0 ), video::SColor ( 0,55,55,55 ), video::SColor ( 0,0,0,255 ), video::SColor ( 0,55,55,55 ),
    video::SColor ( 0,255,255,0 ), video::SColor ( 0,55,55,55 ), video::SColor ( 0,255,0,0 ),video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,255 ),
    video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,0,0 ),
    video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,0,0 )
  };

ASTrack3DNode::ASTrack3DNode ( scene::ISceneNode* parent, ISceneManager* smgr,  s32 id ,ASTrack* strack)
  : AHelix3DNode ( parent, smgr, id ,strack)
{
  this->setName ( "ASTrack3DNode" );

  vividColor=colorlist[strack->index()];  
  
  if( strack->charge() == 0 )
    {
      createNeutralVector(10);
    }
  else
    {
      maxAngle = getChargedMaxAngle();
      createCurveVector();
    }
  
  std::vector<core::vector3df>::const_iterator iter=curvePoints.begin();
  std::vector<core::vector3df>::const_iterator iterE=curvePoints.end();
  for(;iter!=iterE;iter++)
    {
      Box.addInternalPoint(*iter);
    }
  
  setStyle(Basic);
}


ASTrack3DNode::~ASTrack3DNode()
{ }

void ASTrack3DNode::setTrack ( ASTrack* track )
{
  AHelix3DNode::setTrack(track);
}

/*********************************************************
****************NEUTRAL PARTICLE**************************
*********************************************************/
void ASTrack3DNode::createNeutralVector(int nsegments)
{
  // Clear previous setting
  this->curvePoints.clear();

  // Some constants
  float c = 180.f/3.1415926f;
  float sc = 0.001f;

  // Start here
  float X0 = ((ASTrack*)track())->rhoVertex * cos ( ((ASTrack*)track())->phiVertex ) *sc;
  float Y0 = ((ASTrack*)track())->rhoVertex * sin ( ((ASTrack*)track())->phiVertex ) *sc;
  float Z0 = ((ASTrack*)track())->zVertex*sc;

  // Go in this direction
  float theta = 2*atan ( exp ( -(((ASTrack*)track())->eta()) ) );
  float Xdir = sin ( ((ASTrack*)track())->phi() );
  float Ydir = cos ( ((ASTrack*)track())->phi() );
  float Zdir = 1/tan ( theta );

  // Find the maxumum length of the track such that it does not exit the radius of the inner detector.
  // This is done by slowly increasing the length, until it gets bigger..
  // (Can't we just calculate the length by trigonometry (radius = projection of length onto phi plane + initial position in x/y)
  float Length_multiply = 0;
  while ( ( ( ( Xdir*Length_multiply+X0 ) * ( Xdir*Length_multiply+X0 ) + ( Ydir*Length_multiply+Y0 ) * ( Ydir*Length_multiply+Y0 ) ) <=Radius*Radius ) )
    {
      Length_multiply +=0.5;
    }
  
  // Set some limits on the maximum length..
  if ( Length_multiply > Length )
    {
      Length_multiply = Length;
    }
  
  if ( ( X0*X0+Y0*Y0 ) <=Radius*Radius ) // Sanity check. Track shouldn't start outside of the radius..
    {
      core::vector3df initial = core::vector3df ( X0,Y0,Z0 );
      
      core::vector3df direction = core::vector3df ( Xdir, Ydir, Zdir );
      direction*=Length_multiply/nsegments;
      
      // We need to loop over n+1 times, because we want to add in the initial point
      for(int i=0;i<=nsegments;i++)
	{
	  curvePoints.push_back(initial);
	  initial+=direction;
	}
    }
 
}

/*********************************************************
****************CHARGED PARTICLE**************************
*********************************************************/

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
  ASTrack *strack=(ASTrack*)track();
  float C = 1000.f/0.6f;
  float sc = 0.001f;
  float theta = 2*atan ( exp ( - strack->eta() ));
  float X0 = strack->rhoVertex * cos ( strack->phiVertex ) *sc; // The X coordinate of the vertex
  float Y0 = strack->rhoVertex * sin ( strack->phiVertex ) *sc; // The Y coordinate of the vertex
  //float Z0 = v_z;                       // The Z coordinate of the vertex
  float R = strack->pt()*C;
  float X_CH = X0 + strack->charge() * R*cos ( strack->phi() ); //The X coordinate of the center of the helix
  float Y_CH = Y0 + strack->charge() * R*sin ( strack->phi() ); //The Y coordinate of the center of the helix
  float E = exp ( strack->eta() );
  float tL = strack->getTl(); //dip of track = Pz/pTTrack, constant along the helix
  //float startPhi = 90 - phi0*RadDeg + RadDeg*atan(Y_CH/X_CH); //phi0 and the projection angle on the helix are out of phase
  //float startPhi = phi0+adjPhi;
  float Z_CH = strack->zVertex*sc; //- R * startPhi*tL;                     //The Z coordinate of the center of the helix
  float a=0.05/RadDeg;
  
  for ( int w=0; w<=5000; w++ )
    {
      if ( ( x_helix ( w*a, X_CH, R, strack->phi(), strack->charge() ) *x_helix ( w*a, X_CH, R, strack->phi(), strack->charge() ) + y_helix ( w*a, Y_CH, R, strack->phi(), strack->charge() ) *y_helix ( w*a, Y_CH, R, strack->phi(), strack->charge() ) ) >= Radius*Radius/ ( scaleEvent*scaleEvent ) || ( z_helix ( w*a, Z_CH, theta, R ) *z_helix ( w*a, Z_CH, theta, R ) >=Length*Length ) )
        {
	  return w*a;
	  break;
        }
    }
	
  //TODO: Return a sensible value
  return 0;
}

void ASTrack3DNode::createCurveVector()
{
  ASTrack *strack=(ASTrack*)track();

  float pi = 3.1415926f;
  float phiTrans = -(strack->phi()) + pi;
  float C = 1000.f/0.6f;
  float theta = 2*atan ( exp ( - ( strack->eta() ) ) );
  float X0 = strack->rhoVertex * cos ( strack->phiVertex ); // The X coordinate of the vertex
  float Y0 = strack->rhoVertex * sin ( strack->phiVertex ); // The Y coordinate of the vertex
  float Z0 = strack->zVertex;                       // The Z coordinate of the vertex
  float R = strack->pt()*C;
  float X_CH = X0 + strack->charge() * R*cos ( phiTrans ); //The X coordinate of the center of the helix
  float Y_CH = Y0 + strack->charge() * R*sin ( phiTrans ); //The Y coordinate of the center of the helix
  float E = exp ( strack->eta() );
  float tL = 0.5 * ( exp (strack->eta()) - exp ( - (strack->eta()) ) ); //dip of track = Pz/pTTrack, constant along the helix
  //float startPhi = 90 - phi0*RadDeg + RadDeg*atan(Y_CH/X_CH); //phi0 and the projection angle on the helix are out of phase
  //float startPhi = phi0+adjPhi;
  float Z_CH = strack->zVertex; //- R * startPhi*tL;                     //The Z coordinate of the center of the helix
  
  
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
      point = scaleEvent*core::vector3df ( x_helix ( w*angularStep, X_CH, R, phiTrans, strack->charge() ) , y_helix ( w*angularStep, Y_CH, R, phiTrans, strack->charge() ) , z_helix ( w*angularStep, Z_CH, theta, R ) );
      this->curvePoints.push_back ( point );
    }
}

