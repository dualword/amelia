#ifndef ARTRACK_H_
#define ARTRACK_H_

#include "ATrack.h"

//Objects of this class represent reconstructedtracks in the event (Track)
class AEVENTMANAGER_EXPORT ARTrack : public ATrack
{
public:


    enum rType // type
    {
        ExtendedTracks,
        CombinedInDetTracks,
        ConvertedIPatTracks
        //There are a lot more types. The list should be completed

    };



    ARTrack();
    ~ARTrack() {}


    //basic parameters
    int barcode;
    float chi2;
    int id;
    float cotTheta;
    float d0;
    float pt;
    float phi0;
    float z0;

    void setCode(int code);
    int getIndex();

    bool isElectron();
    bool isMuon();
    bool isPhoton();
    bool isNeutrino();
    bool isChargedHadron();
    bool isNeutralHadron();
    bool isHadron();


    bool isInteresting();

    std::vector<irr::core::vector3df> polyLine;
    std::vector<irr::core::vector3df> getPolyline();
    rType RTrackType;

    void setRTrackType(rType type);
    void setRTrackType(QString type);

private:
    static const int codelist[];
    static const int chargelist[];
    static const QString namelist[];

    int code;
};
#endif //ARTRACK_H_
