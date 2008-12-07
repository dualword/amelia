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



#ifndef AXMLEVENT_H
#define AXMLEVENT_H

#include "ATrack.h"
#include "AGeometry.h"
#include "ATrack3DNode.h"

#include <QDomNode>
#include <QDomDocument>


class ATrack;
class ASTrack;
class AJet;
class AMisET;
class shower;
class FCALshower;

class AEvent
{
public:

    AEvent() {};
    ~AEvent() {};
    QString filename;

    QString location;

    bool read;

    QSet<QString> tags;

    std::vector<ATrack*> Tracks; // Tracks pointers are stored here
    std::vector<ASTrack*> STracks; // Pointers to simulated tracks within "EventTracks" are stored here
    std::vector<AJet*> Jets;  // Pointers to Jets within "EventTracks" are stored here
    std::vector<AMisET*> MisET;  // Pointers to Jets within "EventTracks" are stored here
    int numTracks;
    int numChargedHadrons;
    int numPhotons;
    int numNeutralHadrons;
    int numNeutrinos;
    int numMuons;
    int numElectrons;
    std::vector<shower> LArshowers;
    std::vector<FCALshower> FCALshowers;
    std::vector<shower> HECshowers;
    std::vector<shower> TILEshowers;
    int numShowers;
    float ETMis;
    core::vector2df ETMisVec;
    float CaloETMis;
    core::vector2df CaloETMisVec;
};



class AXmlEvent : public QObject
{

    Q_OBJECT

public:

    AXmlEvent();
    virtual ~AXmlEvent();

    static int ptcut;
    static AXmlEvent* CachedEvent(QString file);


    class AGeometry* Base;
    AEvent Event;
    AEvent EventComplete;
    AEvent GetEventFromFile ( QString filename );
    void HideAllTracks();
    void LoadEvent ( QString file );
    void UnloadEvent ( );
    void DisplayEvent(AGeometry* device);
    vector<bool> P_checkbox_states;
    void setCurrentJetModel(QString jetType);
    void setCurrentTrackModel(QString trackType);
    void setCurrentMisEtModel(QString metType);
    int currentJetType; //defines the current Jet reconstruction model in use
    int currentMisEtType; //defines the current Missing Et reconstruction model in use
    int highestTrackID;

public slots:
    void PtCutoff ( int PtCut );
    AEvent DisplayParticles ();

signals:
    // Emitted whenever the displayed event changes shape (ei: during pT cutoff)
    void eventChanged();

private:
    static QMap<QString,AXmlEvent*> cache;

    std::vector<int> getDataInt ( QDomNode xml );
    std::vector<float> getDataFloat ( QDomNode xml );
    std::vector <ASTrack*> GetSTracksFromDOM ( QDomDocument doc , AEvent* event);
    std::vector <AJet*> GetJetsFromDOM ( QDomDocument doc , AEvent* event);
    std::vector <AMisET*> GetMisETFromDOM ( QDomDocument doc , AEvent* event);
    std::vector <shower> GetShowersFromDOM ( QDomDocument doc, char* calo );
    std::vector <FCALshower> GetFCALShowersFromDOM ( QDomDocument doc );


};



#endif //AXMLEVENT_H
