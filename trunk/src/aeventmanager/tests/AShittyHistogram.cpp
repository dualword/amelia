#include "AShittyHistogram.h"

#include <QDebug>

AShittyHistogram::AShittyHistogram(int _binWidth,QGraphicsItem* parent):QGraphicsItem(parent),binWidth(_binWidth)
{

}
  
QRectF AShittyHistogram::boundingRect() const
{
  return QRectF(0,0,400,200);
}

void AShittyHistogram::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
  int c=bars.size();
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
    }
}

void AShittyHistogram::addPoint(int value)
{
  int bin=value/binWidth;

  if(bin>=boundingRect().width()/2) return;

  for(int i=bars.size();i<=bin;i++)
    bars.insert(i,0);

  bars[bin]++;

  if(isVisible())
    update();
}

void AShittyHistogram::clear()
{
  bars.clear();

  if(isVisible())
    update();  
}
