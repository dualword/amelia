#include <aeventmanager/AXmlEvent.h>
#include <aeventmanager/AFilteredEvent.h>
#include <aeventmanager/APtFilter.h>

#include <aeventmanager/ATrackCombination.h>
#include <aeventmanager/ASTrack.h>

#include <QDebug>

#include <math.h>

int main(int argc,char* argv[])
{
  if(argc!=2)
    {
      printf("usage: eventtest event.xml\n");
      return 0;
    }

  AXmlEvent event(argv[1]);
  event.LoadEvent();

  printf("Charged Hadrons: %d\n",event.numChargedHadrons);
  printf("Neutral Hadrons: %d\n",event.numNeutralHadrons);
  printf("Photons: %d\n",event.numPhotons);
  printf("Neutrinos: %d\n",event.numNeutrinos);
  printf("Muons: %d\n",event.numMuons);
  printf("Electrons: %d\n",event.numElectrons);
  printf("Showers: %d\n",event.numShowers);

  for(int i=0;i<event.STracks.size()-1;i++)
    {
      ASTrack *t1=event.STracks[i];
      if(!t1->isPhoton()) continue;

      double p1x=fabs(t1->Pt()) * cos(t1->phi);
      double p1y=fabs(t1->Pt()) * sin(t1->phi);
      double p1z=fabs(t1->Pt()) * t1->getTl();

      for(int j=i+1;j<event.STracks.size();j++)
	{
	  ASTrack *t2=event.STracks[j];
	  if(!t2->isPhoton()) continue;

	  double p2x=fabs(t2->Pt()) * cos(t2->phi);
	  double p2y=fabs(t2->Pt()) * sin(t2->phi);
	  double p2z=fabs(t2->Pt()) * t2->getTl();

	  qDebug() << "=============================";
	  ATrackCombination combo;
	  combo.addTrack(t1);
	  combo.addTrack(t2);
	  qDebug() << combo.getInvariantMass();
	}
    }

  return 0;
}
