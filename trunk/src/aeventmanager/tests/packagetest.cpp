#include <aeventmanager/AXmlEvent.h>
#include <aeventmanager/AEventPackage.h>
#include <aeventmanager/AEventManager.h>

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>

#include "AShittyHistogram.h"


int main(int argc,char* argv[])
{
  AEventManager manager;
  manager.load();

  /*AXmlEvent event(argv[1]);
  event.LoadEvent();

  printf("Charged Hadrons: %d\n",event.numChargedHadrons);
  printf("Neutral Hadrons: %d\n",event.numNeutralHadrons);
  printf("Photons: %d\n",event.numPhotons);
  printf("Neutrinos: %d\n",event.numNeutrinos);
  printf("Muons: %d\n",event.numMuons);
  printf("Electrons: %d\n",event.numElectrons);
  printf("Showers: %d\n",event.numShowers);
  */
  QApplication app(argc,argv);
  
  QGraphicsScene scene;
  QGraphicsView view(&scene);
  view.resize(500,300);

  view.show();

  AShittyHistogram hist(50);
  scene.addItem(&hist);

  QStringList packages=manager.packageList();
  for(int i=0;i<packages.size();i++)
    {
      AEventPackage* package=manager.package(packages[i]);
      qDebug() << "Package " << packages[i] << " has " << package->eventCount() << " events.";
      for(int j=0;j<package->eventCount();j++)
	{
	  AEvent *event=package->event(j);
	  event->LoadEvent();
	  hist.addPoint(event->numElectrons);
	  qDebug() << "Event " << event->uid() << " has " << event->numElectrons << " electrons";
	}
    }
    
  return app.exec();
}
