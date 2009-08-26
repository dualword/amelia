#include "ATrackCombination.h"

#include "ASTrack.h"
#include "AJet.h"
#include "AMisET.h"

#include <math.h>
#include <QDebug>

ATrackCombination::ATrackCombination()
  : AEventObject("",AEventObject::eCombination)
{ }

ATrackCombination::ATrackCombination(const ATrackCombination& o)
 : AEventObject(o)
{
  this->objects=o.objects;
  recalculate();
}

ATrackCombination::~ATrackCombination() { }

void ATrackCombination::addTrack(AEventObject* tr)
{
  if(!objects.contains(tr))
    {
      objects.append(tr);
      recalculate();
      connect(tr,SIGNAL(updated()),
	      this,SLOT(handleUpdates()));

      emit updated();
    }
}

bool ATrackCombination::deleteTrack(AEventObject* tr)
{
  int idx=objects.indexOf(tr);
  if (idx>=0) //Found it
    {
      disconnect(objects.at(idx),SIGNAL(updated()),
		 this,SLOT(handleUpdates()));
      objects.removeAt(idx);
      recalculate();

      emit updated();
      return true;
    }
  return false;
}

QString ATrackCombination::IDString()
{
  QString name;
  if(objects.size()==0) return "";

  //In case of empty name, generate one:
  // track1|track2|track3
  if(objects[0]->type()==AEventObject::eCombination)
    name+="("+qobject_cast<ATrackCombination*>(objects[0])->IDString()+")";
  else
    name+=QString::number(objects[0]->ID());
  for(int i=1;i<objects.size();i++)
    {
      if(objects[i]->type()==AEventObject::eCombination)
	name+="-("+qobject_cast<ATrackCombination*>(objects[i])->IDString()+")";
      else
	name+="|"+QString::number(objects[i]->ID());
    }
  
  return name;
}

void ATrackCombination::setName(QString name)
{
  AEventObject::setName(name);
}

QString ATrackCombination::name()
{
  return name(true);
}

QString ATrackCombination::name(bool generateDefault)
{
  QString _name=AEventObject::name();
  if(!_name.isEmpty() || !generateDefault) return _name;

  return IDString();
}

int ATrackCombination::size()
{
  return objects.size();
}

AEventObject* ATrackCombination::getTrack(unsigned int idx)
{
  return objects[idx];
}

bool ATrackCombination::operator==(ATrackCombination& o)
{
  for(int i=0;i<o.size();i++)
    {
      //Oh oh, founda mismatch!
      if(!objects.contains(o[i]))
	return false;
    }
  
  // No mismatches from other found in this
  // But it is possible that in this exist objects not present in other
  // This check sizes...
  return o.size()==size(); 
}

AEventObject* ATrackCombination::operator[](unsigned int idx)
{
  return objects[idx];
}

void ATrackCombination::recalculate()
{
  /* Reset variables */
  _charge=0;
  Px=0;
  Py=0;
  Pz=0;
  E=0;

  /* Calculate variables by looping over all of the objects */
  for (int i=0;i<objects.size();i++)
    {
      recalculate(objects[i]);
    }
  
  double Psq=Px*Px + Py*Py + Pz*Pz;

  /* M^2 = E^2 - P^2 */
  if(E*E < Psq) //Check for massless particles with a rounding error
    InvMass = 0;
  else
    InvMass = sqrt(E*E - Psq);
  
  /* Update other kinematic variables */
  _pt=sqrt(Px*Px + Py*Py);
  _phi=atan2(Py,Px);
  _theta=atan2(static_cast<double>(_pt),Pz); // I think this uses the right axis
  _eta=-logf(tan(_theta/2)); //Pseudo-rapidity is not additive, right?
  _et=E*sin(_theta);
}

void ATrackCombination::recalculate(AEventObject *trk)
{
  // The momentum variables for the current track (notice the lower case)
  float px=0;
  float py=0;
  float pz=0;

  if(trk->type() == AEventObject::eTrack)
    { // Tracks contain momentum stored as PtPhiEta
      ATrack* track = static_cast<ATrack*>(trk);
      
      px = fabs(track->pt()) * cos(track->phi());
      py = fabs(track->pt()) * sin(track->phi());
      pz = fabs(track->pt()) * track->getTl();
      _charge += track->charge(); // Tracks can be charged
    }
  else if (trk->type() == AEventObject::eJet)
    { // Jets contain momentum stored as EtPhiEta
      AJet* jet = static_cast<AJet*>(trk);
      
      px = fabs(jet->et()) * cos(jet->phi());
      py = fabs(jet->et()) * sin(jet->phi());
      pz = fabs(jet->et()) * jet->getTl();
    }
  else if (trk->type() == AEventObject::eMissingEt)
    { // Missing Energy is stored as EtxEty and no Z component
      AMisET* met = static_cast<AMisET*>(trk);
      
      px = met->etx();
      py = met->ety();
      pz = 0;
    }
  else if (trk->type() == AEventObject::eCombination)
    { // Track combination does not have any variables, we have to loop over its children
      ATrackCombination* combo = static_cast<ATrackCombination*>(trk);
      
      for(int i=0;i<combo->size();i++)
	{
	  recalculate(combo->getTrack(i));
	}
    }
  
  // We assume that all objects are massless (E = P)
  E  += sqrt( (px*px)+(py*py)+(pz*pz));
  Px += px;
  Py += py;
  Pz += pz;	  
}

float ATrackCombination::getInvariantMass()
{
  return InvMass;
}

int ATrackCombination::charge()
{
  return _charge;
}

void ATrackCombination::handleUpdates()
{
  recalculate();
  emit updated();
}
