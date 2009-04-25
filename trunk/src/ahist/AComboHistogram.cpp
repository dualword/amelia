#include "AComboHistogram.h"

#include "ui_acombohistogram.h"

#include <QDebug>

AComboHistogram::AComboHistogram(QString _name,QWidget* parent):QWidget(parent),name(_name)
{
  Ui::AComboHistogram ui;
  ui.setupUi(this);
  
  // Setup the bin controls
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

  // Setup automatic binning
  QPushButton *automaticSettings=findChild<QPushButton*>("automaticSettings");
  connect(automaticSettings,SIGNAL(clicked()),
	  this,SLOT(automaticRebin()));
}

TH1F *AComboHistogram::histogram()
{
  return hist;
}

void AComboHistogram::addData(float point)
{
  // Insert the element such that data is organized
  // from least to greatest 
  int insertIdx=0;
  for(insertIdx=0;insertIdx<=data.size();insertIdx++)
    {
      if(insertIdx<data.size()) //We go 1 past the end, in case the point needs to be inserted at the end
	if(point<data[insertIdx])
	  break;
    }
  data.insert(insertIdx,point);

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

void AComboHistogram::automaticRebin()
{
  //TODO Figure out better conditions
  //Range: min to max value
  //Bins: Half the size of the set
  minx->setValue(data[0]);
  maxx->setValue(data[data.size()-1]);
  bins->setValue(data.size()/2);
}
