#include <aeventmanager/AXmlEvent.h>
#include <aeventmanager/AFilteredEvent.h>
#include <aeventmanager/APtFilter.h>

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>

#include "AShittyHistogram.h"


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

  /*QApplication app(argc,argv);
  
  QGraphicsScene scene;
  QGraphicsView view(&scene);
  view.resize(300,300);

  view.show();

  AShittyHistogram hist(1);
  scene.addItem(&hist);

  AFilteredEvent filtered(&event,new APtFilter(1));
  for(int i=0;i<filtered.STracks.size();i++)
    {
      //if(filtered->STracks[i]->isElectron())
      hist.addPoint(filtered.STracks[i]->Pt()*1000);
    }
  
    return app.exec();*/
  return 0;
}
