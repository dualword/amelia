#include "ManagerTestHelper.h"
#include <QDebug>

ManagerTestHelper::ManagerTestHelper(AShittyHistogram *_hist,AEventManagerScene *_model,QObject* parent):QObject(parent),hist(_hist),model(_model)
{
  
}

void ManagerTestHelper::handleNewEvent(AEvent *event)
{
  event->LoadEvent();
  hist->clear();

  for(int i=0;i<event->STracks.size();i++)
    {
      //if(event.STracks[i]->isElectron())
      hist->addPoint(event->STracks[i]->Pt()*1000);
    }

  model->setActiveEvent(event);
}
