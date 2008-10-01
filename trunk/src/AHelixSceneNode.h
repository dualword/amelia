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


#include "AGeometry.h"
#include "CRelativeScaleSceneNodeAnimator.h"


using namespace irr;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::core;
using namespace io;
using namespace std;

class AGeometry;

class HelixSceneNode : public scene::ISceneNode
{
	private:

		video::SMaterial Material;
		void generateBoxes();
		core::aabbox3d<f32>& getBoundingBox();
		//wxIrrlicht* Irr;
		CRelativeScaleSceneNodeAnimator *boxSizeAnim;

	public:

		core::aabbox3d<f32> Box;


		HelixSceneNode ( scene::ISceneNode* parent, AGeometry* base,  s32 ID );


		virtual ~HelixSceneNode();

		AGeometry* Base;
		struct track* trackPointer;
		int charge;
		float eta;
		float phi;
		float pt;
		float v_phi;
		float v_rho;
		float v_z;
		float tL;
		int trackID;
		float maxAngle;
		float Mlv; //Invariant Mass
		video::SColor color;
		video::SColor dimmedColor;
		video::SColor vividColor;
		core::vector3df start;
		core::vector3df end;
		float etx;
		float ety;

		int getCharge();
		float getEta();
		float getPhi();
		float getPt();
		float getEt();
		float getEtx();
		float getEty();
		float getV_phi();
		float getV_rho();
		float getV_z();

		bool isLineVisible;
		bool boxMode;
		float boxWidth;
		std::vector<scene::ISceneNode*> boxSegments;
		std::vector<core::vector3df> curvePoints;
		virtual void setBoxesVisibility ( bool boxVisibility );
		int trackNumber;
		struct track* getTrackById ( int id );
		virtual int getTrackNumber();
		virtual void setBoxesSelected ( bool boxesSelected );
		virtual void setTrackStyle ( int style );
		virtual void calculateDimmedColors();
		int style;
		virtual void setTrack ( track* track );
		virtual track* getTrack();
		int type; /// Types: 0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy

		void createBoxes();
		void createJetPyramids(); //for the jets
		void createMisEtBoxes(); //for Missing Et
		ISceneNode* Pyramid;

		void calculateMlv(); //Invariant Mass for a single particle

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




#endif // AHELIXSCENENODE_H


