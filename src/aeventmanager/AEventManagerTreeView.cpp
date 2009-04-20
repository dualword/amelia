#include "AEventManagerTreeView.h"
#include <QDebug>

AEventManagerTreeView::AEventManagerTreeView(QWidget *parent):QTreeView(parent)
{
  setRootIsDecorated(false);
  setExpandsOnDoubleClick(false);
  setHeaderHidden(true);
  
  connect(this,SIGNAL(doubleClicked(const QModelIndex&)),
	  this,SLOT(activate(const QModelIndex&)));
}

void AEventManagerTreeView::activate(const QModelIndex& index)
{
  AEventManagerScene *model=(AEventManagerScene*)this->model();
  
  AEventPackage *pkg=model->getPackage(index);
  AEvent *e=model->getEvent(index);

  if(pkg)
    {
      model->setActivePackage(pkg);
      
      int count=model->rowCount(QModelIndex());
      for (int i=0;i<count;i++)
	{
	  QModelIndex idx=model->index(i,0,QModelIndex());
	  if (i==index.row())
	    expand(idx);
	  else
	    collapse(idx);
        }
    }
  else if(e)
    {
      emit eventClicked(e);
    }
}

void AEventManagerTreeView::clickNextEvent()
{
  AEventManagerScene *model=(AEventManagerScene*)this->model();
  
  AEvent* e=model->activeEvent();

  if(!e->package) return;
  
  if (e->package->eventCount()<=1) return;

  int id=e->package->indexOf(e)+1;
  if (id>=e->package->eventCount())
    id=0;
      
  
  emit eventClicked(e->package->event(id));
}
