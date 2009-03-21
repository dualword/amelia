#include "AllHistograms.h"

#include "ui_allhistograms.h"

#include <QDebug>
#include <QInputDialog>

#include "AComboHistogram.h"
#include <ageometry/ATrackCombination.h>

AllHistograms::AllHistograms(QMenu* menu,QWidget* parent):QMainWindow(parent),_menu(menu)
{ 
  Ui::AllHistograms ui;
  ui.setupUi(this);

  tabWidget=findChild<QTabWidget*>("tabWidget");

  QAction *actionNew_Histogram=findChild<QAction*>("actionNew_Histogram");
  connect(actionNew_Histogram,SIGNAL(triggered()),
	  this,SLOT(newHistorgamDlg()));
  
  _menu->addSeparator();
  _menu->addAction("New Histogram",this,SLOT(newHistorgamDlg()));
}

QMenu* AllHistograms::menu()
{
  return _menu;
}

void AllHistograms::newHistorgamDlg()
{
  QObject *fromObj=sender();

  QString name=QInputDialog::getText(0,"New Histogram","Name of the histogram:");
  if(name.isEmpty()) return;

  AComboHistogram *hist;
  if(!histograms.contains(name))
    {
      hist=new AComboHistogram(name,this);
      histograms[name]=hist;
      
      QAction *newMenuAction=new QAction(name,_menu);
      connect(newMenuAction,SIGNAL(triggered()),
	      this,SLOT(addToHistogram()));
      _menu->insertAction(_sep,newMenuAction);
      tabWidget->addTab(hist,name);
    }
  else
    {
      hist=histograms[name];
    }
  
  if(fromObj->inherits("QAction"))
    {
      QAction *fromAct=(QAction*)fromObj;
      QVariant data=fromAct->data();
      QObject* ptr=data.value<QObject*>();
      ATrackCombination *combo=(ATrackCombination*)ptr;
      hist->addData(combo->getInvariantMass());
    }

}

void AllHistograms::addToHistogram()
{
  QObject *fromObj=sender();

  if(fromObj->inherits("QAction"))
    {
      QAction *fromAct=(QAction*)fromObj;
      QVariant data=fromAct->data();
      QObject* ptr=data.value<QObject*>();
      ATrackCombination *combo=(ATrackCombination*)ptr;
      QString name=fromAct->text();

      histograms[name]->addData(combo->getInvariantMass());
    }
}
