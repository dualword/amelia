#include "AComboHistogram.h"

#include <AMELIA.h>
#include <aeventmanager/AEventManagerTreeView.h>

#include "ui_acombohistogram.h"


AComboHistogram::AComboHistogram(QString _name,QWidget* parent):QWidget(parent),name(_name)
{
  Ui::AComboHistogram ui;
  ui.setupUi(this);

  bins=findChild<QSpinBox*>("bins");
  minx=findChild<QDoubleSpinBox*>("minx");
  maxx=findChild<QDoubleSpinBox*>("maxx");
  
  widget=findChild<TQtWidget*>("GraphWidget");
  hist=new TH1F(_name.toLocal8Bit().data(),
		_name.toLocal8Bit().data(),
		bins->value(),minx->value(),maxx->value());
  
  connect(bins,SIGNAL(valueChanged(int)),
	  this,SLOT(updateBins()));
  connect(minx,SIGNAL(valueChanged(double)),
	  this,SLOT(updateBins()));
  connect(maxx,SIGNAL(valueChanged(double)),
	  this,SLOT(updateBins()));
}

void AComboHistogram::addData(int point)
{
  data.push_back(point);
  hist->Fill(point);
  hist->Draw();
}

void AComboHistogram::updateBins()
{
  if(minx->value()>=maxx->value()) 
    {
      minx->setValue(maxx->value()-0.01);
    }

  hist->Reset();
  hist->SetBins(bins->value(),minx->value(),maxx->value());
      
  for(int i=0;i<data.size();i++)
    {
      hist->Fill(data[i]);
    }
  
  widget->GetCanvas()->cd();
  hist->Draw();
  widget->GetCanvas()->Update();
}
