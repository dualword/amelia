#include "ATrackCombination.h"

#include "ASTrack.h"
#include "AJet.h"
#include "AMisET.h"

#include <math.h>
#include <QDebug>

ATrackCombination::ATrackCombination()
  : ATrack("",ATrack::eCombination)
{
  setTrackID(0);
}

ATrackCombination::ATrackCombination(const ATrackCombination& o)
 : ATrack(o)
{
  this->tracks=o.tracks;
  recalculate();
}

ATrackCombination::~ATrackCombination() { }

void ATrackCombination::addTrack(ATrack* tr)
{
  if(!tracks.contains(tr))
    {
      tracks.append(tr);
      recalculate();
      connect(tr,SIGNAL(updated()),
	      this,SLOT(handleTrackUpdated()));

      emit updated();
    }
}

bool ATrackCombination::deleteTrack(ATrack* tr)
{
  int idx=tracks.indexOf(tr);
  if (idx>=0) //Found it
    {
      disconnect(tracks.at(idx),SIGNAL(updated()),
		 this,SLOT(handleTrackUpdated()));
      tracks.removeAt(idx);
      recalculate();

      emit updated();
      return true;
    }
  return false;
}

QString ATrackCombination::trackIDString()
{
  QString name;
  if(tracks.size()==0) return "";

  //In case of empty name, generate one:
  // track1|track2|track3
  if(tracks[0]->type()==ATrack::eCombination)
    name+="("+qobject_cast<ATrackCombination*>(tracks[0])->trackIDString()+")";
  else
    name+=QString::number(tracks[0]->trackID());
  for(int i=1;i<tracks.size();i++)
    {
      if(tracks[i]->type()==ATrack::eCombination)
	name+="-("+qobject_cast<ATrackCombination*>(tracks[i])->trackIDString()+")";
      else
	name+="|"+QString::number(tracks[i]->trackID());
    }
  
  return name;
}

QString ATrackCombination::name()
{
  return name(true);
}

QString ATrackCombination::name(bool generateDefault)
{
  QString _name=ATrack::name();
  if(!_name.isEmpty() || !generateDefault) return _name;

  return trackIDString();
}

int ATrackCombination::size()
{
  return tracks.size();
}

ATrack* ATrackCombination::getTrack(unsigned int idx)
{
  return tracks[idx];
}

bool ATrackCombination::operator==(ATrackCombination& o)
{
  for(int i=0;i<o.size();i++)
    {
      //Oh oh, founda mismatch!
      if(!tracks.contains(o[i]))
	return false;
    }
  
  // No mismatches from other found in this
  // But it is possible that in this exist tracks not present in other
  // This check sizes...
  return o.size()==size(); 
}

ATrack* ATrackCombination::operator[](unsigned int idx)
{
  return tracks[idx];
}

void ATrackCombination::recalculate()
{
  /* Reset variables */
  setCharge(0);
  Px=0;
  Py=0;
  Pz=0;
  E=0;

  /* Calculate variables by looping over all of the tracks */
  for (int i=0;i<tracks.size();i++)
    {
      recalculate(tracks[i]);
    }
  
  /* M^2 = E^2 - P^2 */
  InvMass = sqrt(E*E - (Px*Px + Py*Py + Pz*Pz));

  /* Might as well save Pt (because we can) */
  setPt(sqrt(Px*Px + Py*Py));
}

void ATrackCombination::recalculate(ATrack *trk)
{
  // The momentum variables for the current track (notice the lower case)
  float px=0;
  float py=0;
  float pz=0;

  if(trk->type() == ATrack::eSTrack)
    { // Tracks contain momentum stored as PtPhiEta
      ASTrack* track = static_cast<ASTrack*>(trk);
      
      px = fabs(track->Pt()) * cos(track->phi);
      py = fabs(track->Pt()) * sin(track->phi);
      pz = fabs(track->Pt()) * track->getTl();
      setCharge(charge() + track->charge()); // Tracks can be charged
    }
  else if (trk->type() == ATrack::eJet)
    { // Jets contain momentum stored as PtPhiEta
      AJet* jet = static_cast<AJet*>(trk);
      
      px = fabs(jet->Pt()) * cos(jet->phi);
      py = fabs(jet->Pt()) * sin(jet->phi);
      pz = fabs(jet->Pt()) * jet->getTl();
    }
  else if (trk->type() == ATrack::eMissingEt)
    { // Missing Energy is stored as EtxEty and no Z component
      AMisET* met = static_cast<AMisET*>(trk);
      
      px = met->etx;
      py = met->ety;
      pz = 0;
    }
  else if (trk->type() == ATrack::eCombination)
    { // Track combination does not have any variables, we have to loop over its children
      ATrackCombination* combo = static_cast<ATrackCombination*>(trk);
      
      for(int i=0;i<combo->size();i++)
	{
	  recalculate(combo->getTrack(i));
	}
    }
  
  // We assume that all tracks are massless (E = P)
  E  += sqrt( (px*px)+(py*py)+(pz*pz));
  Px += px;
  Py += py;
  Pz += pz;	  
}

float ATrackCombination::getInvariantMass()
{
  return InvMass;
}

void ATrackCombination::handleTrackUpdated()
{
  recalculate();
  emit updated();
}
