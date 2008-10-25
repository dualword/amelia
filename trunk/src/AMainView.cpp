#include "AMainView.h"

#include <QLayout>

AMainView::AMainView(QWidget *parent):QStackedWidget(parent)
{
	slideOut=0;
	slideIn=0;
}

AMainView::~AMainView()
{
	delete slideOut;
	delete slideIn;
}

void AMainView::setCurrentIndex(int idx)
{
	if((slideOut && slideOut->getStatus()==AAnimationGUI::Running) ||
		(slideIn && slideIn->getStatus()==AAnimationGUI::Running))
			return;
	if(!slideOut)
	{
		originalPosition=pos();
		slideOut=new AAnimationGUI(this);
		slideOut->setKeyframe(100,QPoint(-this->width()-20,this->y()));
		connect(slideOut,SIGNAL(animationFinished()),
				this,SLOT(setCurrentIndex1()));
	}

	this->idx=idx;
	currentWidget()->setDisabled(true);

	slideOut->reset();
	slideOut->play();
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
  parentWidget()->layout()->setEnabled(false);
  QStackedWidget::setCurrentIndex(idx);
  
	if(!slideIn)
	{
		slideIn=new AAnimationGUI(this);
		slideIn->setKeyframe(100,originalPosition);
		connect(slideIn,SIGNAL(animationFinished()),
				this,SLOT(setCurrentIndex2()));
	}
	slideIn->reset();
	slideIn->play();
}

void AMainView::setCurrentIndex2()
{
  //Enable parent layout
  currentWidget()->setDisabled(false);
  parentWidget()->layout()->setEnabled(true);
}
