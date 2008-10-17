#ifndef ASTRACK_H_INCLUDED
#define ASTRACK_H_INCLUDED

#include "AXmlEvent.h"
#include "AGeometry.h"


/// A generic "track" of the event. Types: 0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
class ATrack
{

public:

    ATrack() {}
    ~ATrack() {}
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


    int trackID;
    int style; //default track style, 0 1 2 3 or 4

    int selectionID;
    bool isInList;

    std::string name;
    int q;
    float pt;

    /*class shower LAr_shower;
    class shower HEC_shower;
    shower TILE_shower;
    FCALshower FCAL_shower;*/
    class ATrack3DNode* node;


};

class ASTrack : public ATrack //Objects of this class represent simulated tracks in the event (STr)
{
public:
    ASTrack() {}
    ~ASTrack() {}

    int code;
    float eta;
    float phi;
    float phiVertex;
    float rhoVertex;
    float zVertex;
    float tL;
    float Mlv;

    float et;
    float etx;
    float ety;

    irr::core::vector3df start;
    irr::core::vector3df end;
    float maxAngle;

    video::SColor trackColor;
    class ASTrack3DNode* node;

};

class AJet : public ATrack //Objects of this class represent jet in the event
{
public:

    AJet() {}
    ~AJet() {}
    enum jType //Jet type
    {
        jKt4H1TopoJets,
        jCone4H1TopoJets,
        jKt4H1TowerJets,
        jCone4H1TowerJets
    };
    float eta;
    float phi;
    float et;
    int numCells;
    int id;
    jType type;
    class AJet3DNode* node;
};

class AMisET : public ATrack //Objects of this class represent Missing Energy
{
public:

    AMisET() {}
    ~AMisET() {}
    enum mType // type
    {
        mMET_Final,
        mMET_RefMuon,
        mMET_Calib,
        mMET_RefFinal,
        mMET_Truth
    };
    float etx;
    float ety;
    float et;

    int id;
    mType type;

    irr::core::vector3df start;
    irr::core::vector3df end;
    video::SColor trackColor;
    class AMisET3DNode* node;
};




class shower : public ATrack
{
public:
    float energy;
    float eta;
    int id;
    int layer;
    float phi;
    int sub;
};

class FCALshower : public ATrack
{
public:
    float dx;
    float dy;
    float energy;
    int id;
    int layer;
    int sub;
    float x;
    float y;
};




#endif // ASTRACK_H_INCLUDED
