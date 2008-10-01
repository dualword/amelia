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
	setDisabled(true);

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
	parentWidget()->layout()->removeWidget(this);
	QPoint p=pos();
	hide();
	QStackedWidget::setCurrentIndex(idx);
	show();
	move(p);

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
	setDisabled(false);
}