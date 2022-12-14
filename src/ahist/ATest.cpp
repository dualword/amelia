#include "ATest.h"

#include <AMELIA.h>
#include <aeventmanager/AEventManagerTreeView.h>

#include "ui_histograms.h"


ATest::ATest(QWidget* parent):QWidget(parent)
{
  AEventManager *manager=(AEventManager *)AMELIA::global->plugin("AEventManager");
  
  Ui::Form ui;
  ui.setupUi(this);
  
  tabs=findChild<QTabWidget*>("tabs");
  
  AEventManagerTreeView *tree=findChild<AEventManagerTreeView*>("packageList");
  
  AEventManagerScene *model=new AEventManagerScene(manager);
  tree->setModel(model);
  
  connect(tree,SIGNAL(eventClicked(AEvent*)),
	  this,SLOT(loadEvent(AEvent*)));
}

void ATest::loadEvent(AEvent* event)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  
  event->LoadEvent();
  QList<AEvent*> keys=histograms.keys();
  
  QWidget *current;
  if(keys.indexOf(event)==-1)
    {
      int size=event->Tracks.size();
      AComboHistogram* hist=new AComboHistogram(event->name(),this);
      TH1F *h=hist->histogram();
      h->GetXaxis()->SetTitle("pT (GeV)");

      for(int i=0;i<size;i++)
	{
	  if(event->Tracks[i]->type()==ATrack::eTrack)
	    {
	      float pt=qobject_cast<ATrack*>(event->Tracks[i])->pt();
	      hist->addData(pt*1000);
	    }
	}
      
      tabs->addTab(hist,event->name());

      histograms[event]=hist;
      current=hist;
    }
  else
    {
      current=histograms[event];
    }
  tabs->setCurrentWidget(current);

  QApplication::setOverrideCursor(Qt::ArrowCursor);
}
