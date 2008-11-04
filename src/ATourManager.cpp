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



#include "ATourManager.h"

ATour::ATour ( IXMLReader* reader )
{

    blocks = new irr::core::list<ATourBlock*>();

    loadTour ( reader );

}

void ATour::close()
{

    blocks->clear();

}

irr::core::list<ATourBlock*>::Iterator ATour::current()
{

    return blocks->begin();
}

void ATour::loadTour ( IXMLReader* reader )
{

    reader->read();
    reader->read();

    if ( wcscmp ( reader->getNodeName(), L"amtour" ) )
    {

        wcout << L"failure: " << reader->getNodeName() << endl;
        return;

    }

    description = reader->getAttributeValue ( L"description" );
    title = reader->getAttributeValue ( L"title" );

    do
    {

        loadBlock ( reader );

    }
    while ( wcscmp ( reader->getNodeName(), L"amtour" ) );

}

void ATour::loadBlock ( IXMLReader* reader )
{

    new int();
    int thistime;
    new int();
    int prevtime;

    reader->read();

    if ( wcscmp ( reader->getNodeName(), L"block" ) ) return;

    ATourBlock* block = new ATourBlock();

    do
    {

        thistime = loadKeyframe ( block, reader );

        if ( thistime == -1 )
        {
            block->stopat = prevtime;
        }
        else
            prevtime = thistime;

    }
    while ( wcscmp ( reader->getNodeName(), L"block" ) );

    //prevents "jump" at end of camera loop
    interpolate ( *block->tar->getLast (), block->stopat + 2000, block->tar, block->tart );
    interpolate ( *block->cam->getLast (), block->stopat + 2000, block->cam, block->camt );

    blocks->push_back ( block );

}

int ATour::loadKeyframe ( ATourBlock* block, IXMLReader* reader )
{

    reader->read();

    if ( wcscmp ( reader->getNodeName(), L"keyframe" ) ) return -1;

    int frametime = reader->getAttributeValueAsInt ( L"time" );

    //cout << "keyframe time is " << frametime << endl;

    reader->read();

    while ( wcscmp ( reader->getNodeName(), L"keyframe" ) )
    {

        if ( !wcscmp ( reader->getNodeName(), L"camera" ) )
        {

            vector3df newcam = vector3df ( reader->getAttributeValueAsFloat ( L"x" ),
                                           reader->getAttributeValueAsFloat ( L"y" ),
                                           reader->getAttributeValueAsFloat ( L"z" ) );

            //cout << newcam.X << " " << newcam.Y << " " << newcam.Z << endl;

            //cout << "about to die?" << endl;
            //cout << "interpolating between " << newcam.X << endl;
            //cout << " and " << (block->cam->begin())->X << endl;

            interpolate ( newcam, frametime, block->cam, block->camt );


        }
        else if ( !wcscmp ( reader->getNodeName(), L"target" ) )
        {

            vector3df newtar = vector3df ( reader->getAttributeValueAsFloat ( L"x" ),
                                           reader->getAttributeValueAsFloat ( L"y" ),
                                           reader->getAttributeValueAsFloat ( L"z" ) );

            interpolate ( newtar, frametime, block->tar, block->tart );

        }
        else if ( !wcscmp ( reader->getNodeName(), L"action" ) )
        {

            int type;
            wchar_t* stype = ( wchar_t* ) ( reader->getAttributeValue ( L"type" ) );

            if ( !wcscmp ( stype, L"playsound" ) )
            {
                type = AT_PLAYSOUND;
            }
            else if ( !wcscmp ( stype, L"button" ) )
            {
                type = AT_BUTTON;
            }
            else if ( !wcscmp ( stype, L"loadfile" ) )
            {
                type = AT_LOADFILE;
            }
            else if ( !wcscmp ( stype, L"cammode" ) )
            {
                type = AT_CAMMODE;
            }
            else if ( !wcscmp ( stype, L"ptchange" ) )
            {
                type = AT_PTCHANGE;
            }
            else if ( !wcscmp ( stype, L"tracksel" ) )
            {
                type = AT_TRACKSEL;
            }
            else
            {
            }

            block->act->push_back ( type );

            wchar_t* data = new wchar_t[wcslen ( reader->getAttributeValue ( L"data" ) ) *sizeof ( wchar_t ) ];
            wcscpy ( data, reader->getAttributeValue ( L"data" ) );
            block->actv->push_back ( data );

            block->actt->push_back ( frametime );


        }
        else
        {
        }

        reader->read();

    }

    return frametime;

}

ATourBlock::ATourBlock()
{

    cam = new irr::core::list <vector3df> ();
    camt = new irr::core::list <int> ();

    tar = new irr::core::list <vector3df> ();
    tart = new irr::core::list <int> ();

    act = new irr::core::list <int> ();
    actv = new irr::core::list <wchar_t*> ();
    actt = new irr::core::list <int> ();

    stopat = 0;

}

void ATourBlock::popAction()
{

    irr::core::list<int>::Iterator a0 = act->begin();
    irr::core::list<wchar_t*>::Iterator av0 = actv->begin();
    irr::core::list<int>::Iterator at0 = actt->begin();

    act->erase ( a0 );
    actv->erase ( av0 );
    actt->erase ( at0 );

}

void ATour::interpolate ( vector3df npos, int ntime, irr::core::list<vector3df>* plst, irr::core::list<int>* tlst )
{

    if ( plst->empty() )  // there is nothing on the list yet
    {

        plst->push_back ( npos );
        tlst->push_back ( ntime );

        //printf("started with <%g,%g,%g> @ %d\n", npos.X, npos.Y, npos.Z, ntime);

    }
    else
    {

        irr::core::list<vector3df>::Iterator p0 = plst->getLast();
        irr::core::list<int>::Iterator t0 = tlst->getLast();

        //printf("between <%g,%g,%g> @ %d and <%g,%g,%g> @ %d\n", npos.X, npos.Y, npos.Z, ntime, p0->X, p0->Y, p0->Z, *t0);

        float dt = ntime - *t0;
        float dx = ( ( float ) ( npos.X - p0->X ) ) / dt;
        float dy = ( ( float ) ( npos.Y - p0->Y ) ) / dt;
        float dz = ( ( float ) ( npos.Z - p0->Z ) ) / dt;

        //cout << "dX is " << dx << endl;

        //IF ERROR "new float"s here

        new int();
        int i = 1000;

        while ( dt - i + 1000 > 0 )  // add a new point to the string spaced at 1-second intervals
        {

            plst->push_back ( vector3df ( p0->X + dx*i,
                                          p0->Y + dy*i,
                                          p0->Z + dz*i ) );

            //printf("pushed <%g %g %g>\n", p0->X + dx*i, p0->Y + dy*i, p0->Z + dz*i);

            i += 1000;

        }

        tlst->push_back ( ntime );

    }

}


ATourManager::ATourManager (IFileSystem *fs_, ITimer *timer_)
{

    fs = fs_;

    running = false;
    advancing = false;
    timer = timer_;
    tour = 0;

    startTimer(0);
}

ATourManager::~ATourManager()
{}

void ATourManager::listTours (c8* dir)
{
  fs->changeWorkingDirectoryTo (dir);
  IFileList *fl = fs->createFileList ();

  int filecount = fl->getFileCount ();

    tourFiles = new char* [filecount];
    tourNames = new char* [filecount];
    tourDescs = new char* [filecount];
    IXMLReader *reader;

    int ai = 0;


    for (int i = 0; i < filecount; i++)
    {

        reader = fs->createXMLReader (fl->getFileName(i));
        if (reader)
        {
            if (addTourToList (fs->createXMLReader (fl->getFileName (i)), ai))
            {
                tourFiles [ai] = (char*) fl->getFileName (i);
                ai++;
            }

        }
    }

    numTours = ai;

    fs->changeWorkingDirectoryTo ("../..");
}

bool ATourManager::addTourToList (IXMLReader *reader, int i)
{

    reader->read ();
    reader->read ();

    if ( wcscmp ( reader->getNodeName(), L"amtour" ) )
    {

        return false;

    }

    //wcout << reader->getAttributeValue (L"title");

    wchar_t* wName=(wchar_t*)reader->getAttributeValue (L"title");
    wchar_t* wDesc=(wchar_t*)reader->getAttributeValue (L"description");
    tourNames [i] = wstr_to_cstr (wName, wcslen(wName));
    tourDescs [i] = wstr_to_cstr (wDesc, wcslen(wDesc));
    //MAGIC NUMBER

    reader->drop ();

    //cout << "done!" << endl;
    return true;

}



void ATourManager::load ( char* file )
{

  //fs->changeWorkingDirectoryTo ( "media/tours" );

    cout << "loading " /*<< parent->directoryBase*/ << file << endl;

    tour = new ATour ( fs->createXMLReader ( file ) );

    //fs->changeWorkingDirectoryTo ("../..");

}

void ATourManager::begin()
{

    if ( !tour )
    {
        return;
    }

    advanceBlock();

}

const wchar_t* ATourManager::getDescription()
{

    return tour->description;

}

const wchar_t* ATourManager::getTitle()
{

    return tour->title;

}

void ATourManager::advanceBlock()
{

    if ( !tour->blocks->empty() )
    {

        running = true;
        //advancing = false;

        startCamera();
        startTarget();

        timer->stop();
        timer->setTime ( 0 );
        timer->start();

    }
    else
    {

        cout << "stopping" << endl;
        stop();
        tour_stopped ();

    }

}

void ATourManager::skip()
{

    stopBlock();

}

void ATourManager::stopBlock()
{

    running = false;

    camAnimatorRemoved ();
    tarAnimatorRemoved ();

    irr::core::list<ATourBlock*>::Iterator it = tour->blocks->begin();
    tour->blocks->erase ( it );

    advanceBlock();


}


void ATourManager::stop()
{

    if ( !running ) return;

    running = false;

    tour->close();

    camAnimatorRemoved ();
    tarAnimatorRemoved ();

}

bool ATourManager::isRunning()
{

    return running;

}


void ATourManager::startCamera()
{

    if ( ( *tour->current() )->cam->empty() ) return;

    array<vector3df> p = array<vector3df> ( ( *tour->current() )->cam->getSize() );

    irr::core::list<vector3df>::Iterator iter = ( *tour->current() )->cam->begin();

    unsigned int i;
    for ( i = 0; i < ( *tour->current() )->cam->getSize(); i++ )
    {
        //cout << ( *iter ).X << " " << ( *iter ).Y << " " << ( *iter ).Z << endl;
        p.push_back ( *iter );
        iter++;
    }

    camAnimatorAdded (p);

}

void ATourManager::startTarget()
{

    if ( ( *tour->current() )->tar->empty() ) return;

    array<vector3df> p = array<vector3df> ( ( *tour->current() )->tar->getSize() );

    irr::core::list<vector3df>::Iterator iter = ( *tour->current() )->tar->begin();

    unsigned int i;
    for ( i = 0; i < ( *tour->current() )->tar->getSize(); i++ )
    {
        p.push_back ( *iter );
        iter++;
    }

    tarAnimatorAdded (p);
}

void ATourManager::advanceAction()
{

    advancing = true;

    int i = * ( ( *tour->current() )->act->begin() );
    wchar_t* v = * ( ( *tour->current() )->actv->begin() );

    switch ( i )
    {

    case ATour::AT_PLAYSOUND :
        emit tour_playsound ();

    case ATour::AT_BUTTON:

    {
        cout << "button" << endl;
        emit tour_button (wstr_to_cstr (v, wcslen (v)));
    }

    break;

    case ATour::AT_LOADFILE :

        emit tour_loadfile ( wstr_to_cstr ( v, wcslen ( v )  ) );

        break;

    case ATour::AT_CAMMODE :

        if ( !wcscmp ( v, L"isometric" ) )
        {
            emit tour_camera (AGeometry::FPS);
        }
        else if ( !wcscmp ( v, L"orthogonal" ) )
        {

            emit tour_camera (AGeometry::Maya);

        }
        else if (!wcscmp (v, L"lengthwise"))
        {
            emit tour_camera (AGeometry::Maya);
        }

        break;

    case ATour::AT_PTCHANGE :

        emit tour_ptchange ( wcstol ( v, 0, 10 ) * 1000 );

        break;

    case ATour::AT_TRACKSEL :

        wcout << endl << "TRACKSEL: " << v << endl << endl;

        emit tour_tracksel ( wcstol ( v, 0, 10 ) );

        break;

    }

    //cout << * ( ( *tour->current() )->act->begin() ) << endl;
    //cout << * ( ( *tour->current() )->actt->begin() ) << endl;
    //wcout << * ( ( *tour->current() )->actv->begin() ) << endl;

    ( *tour->current() )->popAction();

    advancing = false;

}

vector3df ATourManager::getTargetVector()
{

    //if(!target) return vector3df(0,0,0);

    /*else*/
    //return target->getPosition() - camera->getPosition();

    return vector3df(0,0,0);

}



void ATourManager::timerEvent(QTimerEvent *event)
{

    if ( !running || advancing ) return;

    int t = timer->getTime();

    while ( ! ( *tour->current() )->actt->empty() && ( * ( ( *tour->current() )->actt->begin() ) <= t ) )
    {
        advanceAction();
    }

    if ( ( *tour->current() )->stopat <= t )
    {
        stopBlock();
    }
}


