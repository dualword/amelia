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


#ifndef TOUR_H
#define TOUR_H

#include "QIrrWidget.h"
#include "AGeometry.h"

using namespace std;
using namespace irr;
using namespace irr::core;
using namespace irr::io;
using namespace irr::scene;

class ATourBlock
{

	public:


		ATourBlock();

		irr::core::list <vector3df> *cam;
		irr::core::list <int> *camt;

		irr::core::list <vector3df> *tar;
		irr::core::list <int> *tart;

		irr::core::list <int> *act;
		irr::core::list <wchar_t*> *actv;
		irr::core::list <int> *actt;

		int stopat;

		void popAction();

};


class ATour
{

	public:

		ATour ( IXMLReader* reader );

		irr::core::list<ATourBlock*>::Iterator current();

		void close();

		irr::core::list<ATourBlock*> *blocks;

		const wchar_t* description;
		const wchar_t* title;

		static enum {AT_PLAYSOUND, AT_BUTTON, AT_LOADFILE, AT_CAMMODE, AT_PTCHANGE, AT_TRACKSEL} TOUR_ACTIONS;

	private:

		void loadTour ( IXMLReader* reader );
		void loadBlock ( IXMLReader* reader );
		void interpolate ( vector3df npos, int ntime, irr::core::list<vector3df>* plst, irr::core::list<int>* tlst );
		int loadKeyframe ( ATourBlock* block, IXMLReader* reader );

};

class ATourManager: public QObject
{
Q_OBJECT

	public:

		ATourManager (IFileSystem*, ITimer*);
		virtual ~ATourManager();

		bool isRunning();
		void load ( core::string<c16> file );
		void begin();
		void skip();
		void stop();
		const wchar_t* getDescription();
		const wchar_t* getTitle();

		vector3df getTargetVector();

                void listTours (c8*);
                bool addTourToList (IXMLReader*, int);

		core::string<c16>* tourFiles;
                core::string<c16>* tourNames;
                core::string<c16>* tourDescs;

                int numTours;

        public slots:
		void timerEvent(QTimerEvent *event);

        signals:

                void camAnimatorAdded (irr::core::array<vector3df>);
                void camAnimatorRemoved ();

                void tarAnimatorAdded (irr::core::array<vector3df>);
                void tarAnimatorRemoved ();

                void tour_playsound ();
                void tour_button (char *);
                void tour_loadfile (QString);
                void tour_ptchange (int);
                void tour_tracksel (int);
                void tour_camera (int);

                void tour_stopped ();


	private:

		void startCamera();
		void startTarget();

		void advanceAction();
		void advanceBlock();
		void stopBlock();

		void dumpactt();

		IFileSystem* fs;

		ITimer* timer;

		ATour* tour;

		bool running;
		bool advancing;

};

inline wchar_t* cstr_to_wstr ( char* cstr, int len )
{

	wchar_t* wstr = new wchar_t[len+1];

	int i;

	for ( i = 0; i < len; i++ )

		wstr[i] = ( wchar_t ) cstr[i];

        wstr [len] = '\0';

	return wstr;

}

inline char* wstr_to_cstr ( wchar_t* wstr, int len )
{

	char* cstr = new char[len+1];

	int i;

	for ( i = 0; i < len; i++ )

		cstr[i] = ( char ) wstr[i];

        cstr [len] = '\0';

	return cstr;

}
#endif  //TOUR_H
