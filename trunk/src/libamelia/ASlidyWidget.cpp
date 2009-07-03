#include "ASlidyWidget.h"

#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <QMoveEvent>

#include "ASlidyManager.h"

ASlidyWidget::ASlidyWidget(ASlidyManager *mngr,const QString title, QWidget *parent, bool controlable)
  :QFrame(parent),_mngr(mngr)
{ 
  setLayout(new QVBoxLayout());

  if(controlable)
    {
      closeButton=new AHideAButton(this);
      closeButton->move(0,0);
      closeButton->resize(15,15);
      closeButton->setPixmap(QPixmap(":/media/stop.png"));
      connect(closeButton,SIGNAL(clicked()),
	      mngr,SLOT(hideWidget()));
    }

  if(!title.isEmpty())
    {
      titleLabel=new QLabel(this);
      titleLabel->setText(QString("<b>")+title+QString("</b>"));
      titleLabel->setAlignment(Qt::AlignHCenter);

      layout()->addWidget(titleLabel);
    }  
}

void ASlidyWidget::addWidget(QWidget *wdg)
{
  resize(wdg->size());
  layout()->addWidget(wdg);
}
