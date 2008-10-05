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

#include "AGeometry.h"

#include <QDomNode>
#include <QDomDocument>

struct shower
{
    float energy;
    float eta;
    int id;
    int layer;
    float phi;
    int sub;
};

struct FCALshower
{
    float dx;
    float dy;
    float energy;
    int id;
    int layer;
    int sub;
    float x;
    float y;
};

/// A generic "track" of the event. Types: 0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
struct track
{

public:

    enum eType //element type
    {
        eUndefined,
        eSTrack,
        eJet,
        eShower,
        eMissingEt,
        eCombination
    };

    eType Type;

    int code;
    float eta;
    int trackID;
    float phi;
    float phiVertex;
    float pt;
    float rhoVertex;
    float zVertex;
    float tL;
    float Mlv;
    int q;
    float et;
    float etx;
    float ety;
    core::vector3df start;
    core::vector3df end;
    float maxAngle;
    std::string name;
    video::SColor trackColor;

    shower LAr_shower;
    shower HEC_shower;
    shower TILE_shower;
    FCALshower FCAL_shower;

    class HelixSceneNode* node;
    int style; //default track style, 0 1 2 3 or 4

    int selectionID;
    bool isInList;
};

struct jet
{
    float eta;
    float phi;
    float pt;
    std::string name;

};

struct Aevent
{
  QString filename;

  QString location;

  bool read;

  QSet<QString> tags;

  std::vector<track> tracks;
  std::vector<jet> jets;
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



class XmlEvent : public QObject
{

Q_OBJECT

public:
    static int ptcut;
    static XmlEvent* CachedEvent(QString file);

    XmlEvent();
    virtual ~XmlEvent();
    class AGeometry* Base;
    struct Aevent Event;
    struct Aevent EventComplete;
    struct Aevent GetEventFromFile ( const char* filename );
    void HideAllTracks();
    struct Aevent DisplayParticles ( vector<bool>states, struct Aevent &ievent );
    void LoadEvent ( const c8* file );
    void UnloadEvent ( );
    void DisplayEvent(AGeometry* device);
    vector<bool> P_checkbox_states;

public slots:
    void PtCutoff ( int PtCut );

signals:
    // Emitted whenever the displayed event changes shape (ei: during pT cutoff)
    void eventChanged();

private:
    static QMap<QString,XmlEvent*> cache;

    std::vector<int> getDataInt ( QDomNode xml );
    std::vector<float> getDataFloat ( QDomNode xml );
    std::vector <track> GetTracksFromDOM ( QDomDocument doc );
    std::vector <shower> GetShowersFromDOM ( QDomDocument doc, char* calo );
    std::vector <FCALshower> GetFCALShowersFromDOM ( QDomDocument doc );
    

};



#endif //AXMLEVENT_H
