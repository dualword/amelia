#include "AHistogramWidget.h"
#include <QPainter>
#include <QDebug>

#include <math.h>

uint qHash(const QColor &color)
{
  uint hash=color.red()*1000*1000
    +color.green()*1000
    +color.blue();

  return hash;
}

AHistogramWidget::AHistogramWidget(int bins,float min,float max,QWidget* parent)
  : QWidget(parent),_bins(bins),_min(min),_max(max)
{ }
  
void AHistogramWidget::addData(QColor color,float value)
{
  if(value>_max) return;

  if(!data.contains(color))
    {
      int *newhist=new int[_bins];
      for(int i=0;i<_bins;i++)
	newhist[i]=0;
      data.insert(color,newhist);
    }

  int *subhist=data[color];

  float binWidth=(_max-_min)/_bins;
  int binID=floor((value-_min)/binWidth);

  subhist[binID]++;

  repaint();
}

void AHistogramWidget::reset()
{
  QHash<QColor,int*>::const_iterator iter=data.begin();
  QHash<QColor,int*>::const_iterator iterE=data.end();
  for(;iter!=iterE;iter++)
    {
      int *binInfo=*iter;
      delete[] binInfo;
    }
  data.clear();
  repaint();
}

void AHistogramWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  painter.fillRect(QRect(QPoint(0,0),size()),Qt::white);

  int atheight[_bins];
  for(int i=0;i<_bins;i++)
    atheight[i]=0;

  QHash<QColor,int*>::const_iterator iter=data.begin();
  QHash<QColor,int*>::const_iterator iterE=data.end();
  for(;iter!=iterE;iter++)
    {
      int *binInfo=*iter;
      for(int i=0;i<_bins;i++)
	{
	  atheight[i]+=binInfo[i];
	}
    }

  int maxHeight=0;
  for(int i=0;i<_bins;i++)
    {
      if(atheight[i]>maxHeight)
	maxHeight=atheight[i];
      atheight[i]=0;
    }
  if(maxHeight==0) return;

  float scale=size().height()/maxHeight;

  float binWidth=(_max-_min)/_bins;
  iter=data.begin();
  iterE=data.end();
  for(;iter!=iterE;iter++)
    {
      int *binInfo=*iter;
      for(int i=0;i<_bins;i++)
	{
	  QPoint pos(i*binWidth,size().height()-binInfo[i]*scale-atheight[i]*scale);
	  QSize height(binWidth,binInfo[i]*scale);
	  painter.fillRect(QRect(pos,height),iter.key());
	  atheight[i]+=binInfo[i];
	}
    }
  
  /*int c=bars.size();
  int w=boundingRect().width()/c;
  int h=boundingRect().height();

  int max=0;
  for(int i=0;i<bars.size();i++)
    if(max<bars[i])
      max=bars[i];

  for(int i=0;i<bars.size();i++)
    {
      float curH=(float)bars[i]/(float)max*h;
      
      painter->drawRect(i*w,0,w,curH);
      }*/
}
