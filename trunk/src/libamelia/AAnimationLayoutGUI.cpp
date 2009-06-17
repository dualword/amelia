#include "AAnimationLayoutGUI.h"

#include <QDebug>
#include <QApplication>

AAnimationLayoutGUI::AAnimationLayoutGUI(QObject* parent)
  : AAnimationGUI(parent),layout(0)
{ }

AAnimationLayoutGUI::AAnimationLayoutGUI(QWidget* w,QObject* parent)
  : AAnimationGUI(parent),layout(0)
{ 
  setWidget(w);
}

AAnimationLayoutGUI::~AAnimationLayoutGUI()
{
}

void AAnimationLayoutGUI::setWidget(QWidget* w)
{
  AAnimationGUI::setWidget(w);

  //Calculate the final position of the widget based on its layed out
  QWidget *parent=w->parentWidget();
  layout=parent->layout();
  layout=findLayout(layout);
  AAnimationGUI::setPosAt(1,w->pos());
}

void AAnimationLayoutGUI::setTimeLine(QTimeLine *timeLine)
{
  if(this->timeLine()!=0)
    disconnect(this->timeLine(),SIGNAL(finished()),
	       this,SLOT(handleFinished()));

  AAnimationGUI::setTimeLine(timeLine);

  if(this->timeLine()!=0)
    connect(this->timeLine(),SIGNAL(finished()),
	    this,SLOT(handleFinished()));
}

void AAnimationLayoutGUI::setPosAt(qreal time,QPoint p)
{
  if(time==1) return; //Final position is dictated by the layout
  AAnimationGUI::setPosAt(time,p);
}


QLayout* AAnimationLayoutGUI::findLayout(QLayout *layout)
{
  if(layout->indexOf(widget())!=-1) return layout;
  
  for(int i=0;i<layout->count();i++)
    {
      QLayoutItem *item=layout->itemAt(i);
      QLayout *sublayout=item->layout();
      if(sublayout)
	{
	  QLayout *found=findLayout(sublayout);
	  if(found) return found;
	}
    }
}

void AAnimationLayoutGUI::handleFinished()
{
  if(timeLine()->direction()==QTimeLine::Backward)
    widget()->hide();

  layout->addWidget(widget());
}

void AAnimationLayoutGUI::setStep(qreal step)
{
  AAnimationGUI::setStep(step);
  if(!widget()->isVisible())
    {
      layout->removeWidget(widget());
      widget()->show();
    }

}
