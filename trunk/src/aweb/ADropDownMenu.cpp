#include "ADropDownMenu.h"

#include <math.h>
#include <QDebug>

ADropDownMenu::ADropDownMenu(QWidget *parent):QWidget(parent),isPosInit(false)
{
  showhideButton.setText("Show");
  showhideButton.setParent(this);

  setLayout(&layout);
  layout.addWidget(&showhideButton,0,Qt::AlignBottom);

  connect(&showhideButton,SIGNAL(clicked()),
	  this,SLOT(toggle()));

  dropDownTime=new QTimeLine(1000,this);
  dropDownTime->setCurveShape(QTimeLine::LinearCurve);
  dropDown.setWidget(this);
  dropDown.setTimeLine(dropDownTime);

  pullUpTime=new QTimeLine(1000,this);
  pullUpTime->setCurveShape(QTimeLine::LinearCurve);
  pullUp.setWidget(this);
  pullUp.setTimeLine(pullUpTime);
}

void ADropDownMenu::addAction(QAction* action)
{
  QPushButton *b1=new QPushButton(action->icon(),action->text(),this);

  connect(b1,SIGNAL(clicked()),
	  action,SLOT(trigger()));
  
  //Add the button to the layout,
  // while ensuring that the show/hide button
  // is always on the bottom
  layout.removeWidget(&showhideButton);
  layout.addWidget(b1);
  layout.addWidget(&showhideButton);
}

void ADropDownMenu::moveEvent(QMoveEvent* event)
{
  if(!isPosInit)
    {
      posInit=pos();
      int h=showhideButton.pos().y();
      move(posInit.x(),-h);
      isPosInit=true;

      //Create the forward timer...
      for(qreal i=0;i<=1000;i++)
        {
	  qreal t=i/1000;
	  qreal xDown=1-exp(-t/0.2)*cos(2*3.14*2.25*t);
	  qreal xUp=exp(-t/0.2)*cos(2*3.14*.75*t);
	  xDown*=h;
	  xUp*=h;
	  dropDown.setPosAt(t,QPoint(posInit.x(),(int)xDown-h));
	  pullUp.setPosAt(t,QPoint(posInit.x(),(int)xUp-h));
        }
    }

  QWidget::moveEvent(event);
}

void ADropDownMenu::toggle()
{
  if(showhideButton.text()=="Show")
    showMe();
  else
    hideMe();
}

void ADropDownMenu::showMe()
{
  showhideButton.setText("Hide");
  pullUpTime->stop();
  dropDownTime->start();
}

void ADropDownMenu::hideMe()
{
  showhideButton.setText("Show");
  dropDownTime->stop();
  pullUpTime->start();
}
