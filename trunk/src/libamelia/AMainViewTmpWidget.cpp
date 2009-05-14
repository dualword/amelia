#include "AMainViewTmpWidget.h"
#include <QVBoxLayout>

AMainViewTmpWidget::AMainViewTmpWidget(QWidget *widget,AMainView *parent)
  :QWidget(parent)
{
  QLayout *layout=new QVBoxLayout(this);
  layout->addWidget(widget);

  internalId=parent->addWidget(widget);
  previousId=parent->currentIndex();

  parent->setCurrentIndex(internalId);

  connect(parent,SIGNAL(beforeCurrentIndexChanged(int)),
	  this,SLOT(stop()));
  connect(parent,SIGNAL(afterCurrentIndexChanged(int)),
	  this,SLOT(start()));
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
  
  setEnabled(true);
}
  
  void AMainViewTmpWidget::close()
  {
    AMainView *mainView=(AMainView*)parentWidget();
    mainView->setCurrentIndex(previousId);
  }
