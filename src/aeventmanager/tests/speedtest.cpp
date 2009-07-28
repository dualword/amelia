#include <aeventmanager/AEventManager.h>
#include <aeventmanager/AEvent.h>

//#include <QApplication>
#include <QTime>
#include <QStringList>
#include <QDebug>

int main(int argc,char* argv[])
{
  AEventManager manager;
  manager.load();

  float total=0;
  int count=0;

  QStringList packages=manager.packageList();
  for(int i=0;i<packages.size();i++)
    {
      AEventPackage* pkg=manager.package(packages[i]);
      for(int j=0;j<pkg->eventCount();j++)
	{
	  AEvent* event=pkg->event(j);
	  QTime time;
	  time.start();
	  event->LoadEvent();
	  int elapsed=time.elapsed();
	  qDebug() << "Loaded event " << event->uid();
	  qDebug() << "\tNumber of tracks:\t" << event->Tracks.size() << " tracks";
	  qDebug() << "\tTotal time: \t\t" << elapsed << "ms";
	  qDebug() << "\tPer-track time: \t" << elapsed/event->Tracks.size() << "ms";
	  
	  total+=elapsed;
	  count++;
	}
    }

  qDebug() << "\nAverage Time: " << total/count << " ms";
  return 0;
}
