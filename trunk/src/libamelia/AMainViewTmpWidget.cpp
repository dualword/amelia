#include "AMainViewTmpWidget.h"
#include <QVBoxLayout>

AMainViewTmpWidget::AMainViewTmpWidget(QWidget *widget,AMainView *parent)
  :QWidget(parent)
{
  QLayout *layout=new QVBoxLayout(this);
  layout->addWidget(widget);

  internalId=parent->addWidget(this);
  previousId=parent->currentIndex();

  parent->setCurrentIndex(internalId);

  connect(parent,SIGNAL(beforeCurrentIndexChanged(int)),
	  this,SLOT(stop()));
  connect(parent,SIGNAL(afterCurrentIndexChanged(int)),
	  this,SLOT(start()));

  closeButton.setParent(this);
  closeButton.resize(15,15);

  QPoint pos(width()-15,0);
  closeButton.move(pos);
  
  closeButton.setPixmap(QPixmap(":/media/stop.png"));

  connect(&closeButton,SIGNAL(clicked()),
	  this,SLOT(close()));
}
					 
void AMainViewTmpWidget::stop()
{
  setEnabled(false);
}

void AMainViewTmpWidget::start()
{
  AMainView *mainView=(AMainView*)parentWidget();
  if(mainView->currentIndex()==internalId)
    {
      setEnabled(true);
    }
  else //Switched away from the widget, delete it
    {
      deleteLater();
      mainView->removeWidget(this);
    }
}
  
void AMainViewTmpWidget::close()
{
  AMainView *mainView=(AMainView*)parentWidget();
  mainView->setCurrentIndex(previousId);
}

void AMainViewTmpWidget::resizeEvent(QResizeEvent *event)
{
  QPoint pos(event->size().width()-15,0);
  closeButton.move(pos);
}
