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


#include "ATourBuilder.h"

#define ACTION_INTERVAL 5000

ATourBuilder::ATourBuilder ( IFileSystem* fs_, ICameraSceneNode *cam_)
{

	recording = false;
	fs = fs_;
        cam = cam_;

	currentEventPath = 0;



}

void ATourBuilder::startRecording ( char* filename )
{

	fs->changeWorkingDirectoryTo ( "media/tours" );
	wf = fs->createAndWriteFile ( filename );
	fs->changeWorkingDirectoryTo ( "../.." );

	if ( !wf )
	{
//		cout << "oops" << endl;
		return;
	}
	else
	{
//		cout << "yeah!" << endl;
	}
	writer = fs->createXMLWriter ( wf );

	recording = true;
	time = 0;

	writer->writeXMLHeader();
	writer->writeLineBreak();
	writer->writeElement ( L"amtour", false, L"title", L"User", L"description", L"Sample user tour" );
	writer->writeLineBreak();
	writer->writeLineBreak();
	writer->writeElement ( L"block" );
	writer->writeLineBreak();
	writer->writeLineBreak();

	if ( currentEventPath )
	{

		markAction ( ATour::AT_LOADFILE, currentEventPath );
	}
	else
	{
		markCamera();
	}

}

void ATourBuilder::stopRecording()
{

	recording = false;

	writer->writeClosingTag ( L"block" );
	writer->writeLineBreak();
	writer->writeLineBreak();
	writer->writeClosingTag ( L"amtour" );
	writer->writeLineBreak();
	writer->writeLineBreak();

	writer->drop();
	wf->drop();

//	cout << "done" << endl;

}

void ATourBuilder::beginKeyframe()
{

	wchar_t* tnow = ( wchar_t* ) malloc ( sizeof ( wchar_t ) *10 );
	swprintf ( tnow, 10, L"%d", time );

	writer->writeElement ( L"keyframe", false, L"time", tnow );
	writer->writeLineBreak();
	writer->writeLineBreak();

	time += ACTION_INTERVAL;

}

void ATourBuilder::endKeyframe()
{

	writer->writeClosingTag ( L"keyframe" );
	writer->writeLineBreak();
	writer->writeLineBreak();

}

void ATourBuilder::writeCamPos()
{

	vector3df pos = cam->getPosition();


	int ix = pos.X;
	int iy = pos.Y;
	int iz = pos.Z;

	wchar_t* x = ( wchar_t* ) malloc ( 10*sizeof ( wchar_t ) );
	wchar_t* y = ( wchar_t* ) malloc ( 10*sizeof ( wchar_t ) );
	wchar_t* z = ( wchar_t* ) malloc ( 10*sizeof ( wchar_t ) );

	swprintf ( x, 10, L"%d", ix );
	swprintf ( y, 10, L"%d", iy );
	swprintf ( z, 10, L"%d", iz );

	writer->writeElement ( L"camera", true, L"x", x, L"y", y, L"z", z );
	writer->writeLineBreak();
	writer->writeLineBreak();

}

void ATourBuilder::writeTarPos()
{

	vector3df tar = cam->getTarget();
	vector3df pos = cam->getPosition();

	float dx = tar.X - pos.X;
	float dy = tar.Y - pos.Y;
	float dz = tar.Z - pos.Z;

        int ix = tar.X + 10*dx;
        int iy = tar.Y + 10*dy;
        int iz = tar.Z + 10*dz;

	wchar_t* x = ( wchar_t* ) malloc ( 10*sizeof ( wchar_t ) );
	wchar_t* y = ( wchar_t* ) malloc ( 10*sizeof ( wchar_t ) );
	wchar_t* z = ( wchar_t* ) malloc ( 10*sizeof ( wchar_t ) );

	swprintf ( x, 10, L"%d", ix );
	swprintf ( y, 10, L"%d", iy );
	swprintf ( z, 10, L"%d", iz );

	writer->writeElement ( L"target", true, L"x", x, L"y", y, L"z", z );
	writer->writeLineBreak();
	writer->writeLineBreak();

}

void ATourBuilder::markAction ( int type )
{

	markAction ( type, 0 );

}

void ATourBuilder::markAction ( int type, wchar_t* value )
{

	if ( !recording ) return;

	beginKeyframe();

	writeTarPos();
	writeCamPos();

	wchar_t* atype;

	switch ( type )
	{

		case ATour::AT_BUTTON    :
			atype = L"button";
			break;
		case ATour::AT_LOADFILE  :
			atype = L"loadfile";
			break;
		case ATour::AT_PLAYSOUND :
			atype = L"playsound";
			break;
		//default : /* NOTREACHED */
//			cout << ( "Error: invalid action type" ) << endl;

	}

	if ( value ) writer->writeElement ( L"action", true, L"type", atype, L"data", value );
	else writer->writeElement ( L"action", true, L"type", atype );
	writer->writeLineBreak();
	writer->writeLineBreak();

	endKeyframe();

}

void ATourBuilder::markCamera()
{

	if ( !recording ) return;

	beginKeyframe();
	writeCamPos();
	writeTarPos();
	endKeyframe();

}

void ATourBuilder::markLoadEvent ( wchar_t* path )
{

//        cout << "LOAD EVENT" << endl;

	if ( recording ) markAction ( ATour::AT_LOADFILE, path );

	else currentEventPath = path;

}


bool ATourBuilder::isRecording () {

        return recording;

}
