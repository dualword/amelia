#include "AEventManagerTreeView.h"
#include <QBetterMimeData.h>
#include <QDebug>
#include <QMenu>

AEventManagerTreeView::AEventManagerTreeView(QWidget *parent):QTreeView(parent)
{
  setRootIsDecorated(false);
  setExpandsOnDoubleClick(false);
  setHeaderHidden(true);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setDragDropMode(QAbstractItemView::InternalMove);
  setDragEnabled(true);
  setAcceptDrops(true);
  setDropIndicatorShown(true);

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

      emit packageActivated(pkg);
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
  AEventPackage* pkg=model->activePackage();

  if(!pkg) return;
  
  if (pkg->eventCount()==0) return;

  int id=(e)?(pkg->indexOf(e)+1):0;
  if (id>=pkg->eventCount())
    id=0;
      
  AEvent *nextEvent=pkg->event(id);
  
  // Prevent the same event from being clicked
  // ei: there is only one event in a package
  if(nextEvent!=e)
    emit eventClicked(nextEvent);
}

void AEventManagerTreeView::contextMenuEvent(QContextMenuEvent *event)
{
  lastSelectedIndex=indexAt(event->pos());
  if(!lastSelectedIndex.isValid()) return;
  if(lastSelectedIndex.parent().isValid()) return; //Not a package
  
  QMenu menu(this);
  
  menu.addAction("Rename",this,SLOT(toggleRename()));

  menu.exec(event->globalPos());
}

void AEventManagerTreeView::toggleRename()
{
  if(!lastSelectedIndex.isValid()) return;

  setCurrentIndex(lastSelectedIndex);
  edit(lastSelectedIndex);
}
