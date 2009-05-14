#include "AMainView.h"

#include <QLayout>

AMainView::AMainView(QWidget *parent):QStackedWidget(parent)
{
  slide=new AAnimationGUI(this);
  slideTime=new QTimeLine(1000);
  slide->setTimeLine(slideTime);

  connect(slideTime,SIGNAL(finished()),
	  this,SLOT(setCurrentIndex1()));
}

AMainView::~AMainView()
{ }

void AMainView::setCurrentIndex(int idx)
{
  if(slideTime->state()==QTimeLine::Running)
    return;

  if(!isEnabled() || !isVisible()) return QStackedWidget::setCurrentIndex(idx);
  
  originalPosition=pos();
  slide->setPosAt(0,pos());
  slide->setPosAt(1,QPoint(-this->width()-20,this->y()));
  
  this->idx=idx;
  setDisabled(true);

  emit beforeCurrentIndexChanged(idx);
  slideTime->start();
}

void AMainView::toggle()
{
  int next=(currentIndex()+1)%count();
  setCurrentIndex(next);
}

void AMainView::setCurrentIndex1()
{
  //By disabling the parent layout, we ensure that when we call setCurrentIndex(), we won't see quick flicker.
  // as the index of the parent stacked widget is updated. The problem is that if we add widgets in the meantime,
  // then they won't be "properly" layed out until we enabled the layout again at the end of the animation. But
  // then, how often that happens?
  //In conclusion, if you have problem with the layouts while an AMainView transition is in effect, then this is 
  // probably the cause....
  if(slideTime->direction()==QTimeLine::Forward)
    { // Animation step 1
      parentWidget()->layout()->setEnabled(false);
      QStackedWidget::setCurrentIndex(idx);
      
      slideTime->setDirection(QTimeLine::Backward);
      slideTime->start();
    }
  else
    { // Animation step 2
      slideTime->setDirection(QTimeLine::Forward);
      setDisabled(false);
      parentWidget()->layout()->setEnabled(true);
      emit afterCurrentIndexChanged(idx);
    }
}
