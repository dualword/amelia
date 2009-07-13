#include "ATrackCombination.h"

#include <math.h>

unsigned int ATrackCombination::_IDS=0;

ATrackCombination::ATrackCombination()
 : ATrack("",ATrack::eCombination)
{
  _IDS++;
  setTrackID(_IDS);
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
    }
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

QString ATrackCombination::name()
{
  return name(true);
}

QString ATrackCombination::name(bool generateDefault)
{
  QString _name=ATrack::name();
  if(!_name.isEmpty() || !generateDefault) return _name;

  if(tracks.size()==0) return "Empty Combination";

  //In case of empty name, generate one:
  // track1|track2|track3
  _name=QString::number(tracks[0]->trackID());
  for(int i=1;i<tracks.size();i++)
    _name+="|"+QString::number(tracks[i]->trackID());

  return _name;
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
  int q=0;
  for (int i=0;i<tracks.size();i++)
    {
      q+=tracks[i]->charge();
    }
  setCharge(q);

  calculateInvariantMass();
}

float ATrackCombination::calculateInvariantMass()
{
  float px_sum = 0;
  float py_sum = 0;
  float pz_sum = 0;
  float P_sum = 0;
  for (int i = 0; i < tracks.size(); i++)
    {
      if (tracks[i]->type() == ATrack::eSTrack)
        {
	  ASTrack* track = static_cast<ASTrack*>(tracks[i]);
	  
	  float px = fabs(track->Pt()) * cos(track->phi);
	  float py = fabs(track->Pt()) * sin(track->phi);
	  float pz = fabs(tracks[i]->Pt()) * track->getTl();
	  float P = sqrt( (px*px)+(py*py)+(pz*pz));
	  
	  
	  px_sum += px;
	  py_sum += py;
	  pz_sum += pz;
	  P_sum += P;
        }
      
      if (tracks[i]->type() == ATrack::eJet)
        {
	  AJet* track = static_cast<AJet*>(tracks[i]);
	  
	  float px = fabs(track->Pt()) * cos(track->phi);
	  float py = fabs(track->Pt()) * sin(track->phi);
	  float pz = fabs(tracks[i]->Pt()) * track->getTl();
	  float P = sqrt( (px*px)+(py*py)+(pz*pz));
	  
	  
	  px_sum += px;
	  py_sum += py;
	  pz_sum += pz;
	  P_sum += P;
        }
    }
  
  InvMass = sqrt(P_sum*P_sum - (px_sum*px_sum + py_sum*py_sum + pz_sum*pz_sum));
  return InvMass;
  
}

float ATrackCombination::getInvariantMass()
{
  return InvMass;
}