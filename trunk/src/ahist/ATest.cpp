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
  bins=findChild<QSpinBox*>("bins");
  minx=findChild<QDoubleSpinBox*>("minx");
  maxx=findChild<QDoubleSpinBox*>("maxx");
  
  AEventManagerTreeView *tree=findChild<AEventManagerTreeView*>("packageList");
  
  AEventManagerScene *model=new AEventManagerScene(manager);
  tree->setModel(model);
  
  connect(tree,SIGNAL(eventClicked(AEvent*)),
	  this,SLOT(loadEvent(AEvent*)));
  
  connect(bins,SIGNAL(valueChanged(int)),
	  this,SLOT(updateBins()));
  connect(minx,SIGNAL(valueChanged(double)),
	  this,SLOT(updateBins()));
  connect(maxx,SIGNAL(valueChanged(double)),
	  this,SLOT(updateBins()));
}

void ATest::loadEvent(AEvent* event)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  
  event->LoadEvent();
  QList<AEvent*> keys=hists.keys();
  
  QWidget *current;
  if(keys.indexOf(event)==-1)
    {
      TQtWidget *MyWidget= new TQtWidget(0,"MyWidget");
            
      TH1F *hist=new TH1F(event->filename.toLocal8Bit().data(),event->filename.toLocal8Bit().data(),
			  bins->value(),minx->value(),maxx->value());
      
      int size=event->Tracks.size();

      for(int i=0;i<size;i++)
	{
	  float pt=event->Tracks[i]->pt;
	  hist->Fill(pt);
	}
      
      hist->Draw();
      
      tabs->addTab(MyWidget,event->filename);

      hists[event]=hist;
      widgets[event]=MyWidget;
      current=MyWidget;
    }
  else
    {
      current=widgets[event];
    }
  tabs->setCurrentWidget(current);

  QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void ATest::updateBins()
{
  if(minx->value()>=maxx->value()) 
    {
      minx->setValue(maxx->value()-0.01);
    }

  QList<AEvent*> keys=hists.keys();
  for(int i=0;i<keys.size();i++)
    {
      AEvent* event=keys[i];

      hists[event]->Reset();
      hists[event]->SetBins(bins->value(),minx->value(),maxx->value());

      
      int size=event->Tracks.size();

      for(int i=0;i<size;i++)
	{
	  float pt=event->Tracks[i]->pt;
	  hists[event]->Fill(pt);
	}

      widgets[event]->GetCanvas()->cd();
      hists[event]->Draw();
      widgets[event]->GetCanvas()->Update();
    }
}
