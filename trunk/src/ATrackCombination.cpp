#include "ATrackCombination.h"

#include "ATrackTableModel.h"

unsigned int ATrackCombination::_IDS=0;

ATrackCombination::ATrackCombination():netCharge(0)
{
    id=++_IDS;
    name="Combo #"+QString::number(id);
}
ATrackCombination::~ATrackCombination() { }

void ATrackCombination::addTrack(ATrack* tr)
{
    tracks.append(tr);
    recalculate();
}

bool ATrackCombination::deleteTrack(ATrack* tr)
{
    int idx=tracks.indexOf(tr);
    if (idx>=0) //Found it
    {
        tracks.removeAt(idx);
        recalculate();
        return true;
    }
    return false;
}

int ATrackCombination::size()
{
    return tracks.size();
}

int ATrackCombination::getID()
{
    return id;
}

QString ATrackCombination::getName()
{
    return name;
}

void ATrackCombination::setName(QString _name)
{
    name=_name;
}

float ATrackCombination::getNetCharge()
{
    return netCharge;
}

ATrack* ATrackCombination::operator[](unsigned int idx)
{
    return tracks[idx];
}

void ATrackCombination::recalculate()
{
    netCharge=0;
    for (int i=0;i<tracks.size();i++)
    {
        netCharge+=tracks[i]->q;
    }
    calculateInvariantMass();
}

void ATrackCombination::fixIDs()
{
    for (int i=0;i<tracks.size();i++)
        if (tracks[i]->selectionID==0)
            tracks[i]->selectionID=(++ATrackTableModel::selectionID);
}

float ATrackCombination::calculateInvariantMass()
{
    float px_sum = 0;
    float py_sum = 0;
    float pz_sum = 0;
    float P_sum = 0;
    for (int i = 0; i < tracks.size(); i++)
    {
        float px = fabs(tracks[i]->pt) * cos(tracks[i]->phi);
        float py = fabs(tracks[i]->pt) * sin(tracks[i]->phi);
        float pz = fabs(tracks[i]->pt) * tracks[i]->tL;
        float P = sqrt( (px*px)+(py*py)+(pz*pz));


        px_sum += px;
        py_sum += py;
        pz_sum += pz;
        P_sum += P;
    }

    InvMass = sqrt(P_sum*P_sum - (px_sum*px_sum + py_sum*py_sum + pz_sum*pz_sum));
    return InvMass;

}

float ATrackCombination::getInvariantMass()
{
    return InvMass;
}
