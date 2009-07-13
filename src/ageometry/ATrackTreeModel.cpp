#include "ATrackTreeModel.h"

#include <QApplication>
#include <QInputDialog>
#include <QDebug>

#include "QAbstractTreeItem.h"

unsigned int ATrackTreeModel::selectionID=0;

ATrackTreeModel::ATrackTreeModel(QWidget* parent)
 :QAbstractItemModel(parent)
{
  analysisData=new ATrackCollection("AGeometry");

  //Use this for selecting tracks by clicking on the table
  selection=new QItemSelectionModel(this);//parent->selectionModel();
  connect(selection,SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
	  this,SLOT(handleSelectionChanged(const QItemSelection&,const QItemSelection&)));
}

ATrackTreeModel::~ATrackTreeModel()
{
  selection->deleteLater();
}

void ATrackTreeModel::handleNewEventLoaded(AEvent *event)
{
  // Unload the data loaded before, if any.
  if(analysisData)
    {
      disconnect(analysisData,SIGNAL(updated()),
		 this,SLOT(refresh()));
      analysisData=new ATrackCollection("AGeometry");

      clearInternalTree();
    }
  
  // Load new analysis data.
  if(event)
    {
      analysisData=event->getAnalysisData<ATrackCollection>("AGeometry");
      
      connect(analysisData,SIGNAL(updated()),
	      this,SLOT(refresh()));
      
      // Create the internal tree to store child->parent relationships
      createInternalTree();
    }
  refresh();
}

void ATrackTreeModel::setTracks(QList<ATrack *> tracks)
{
  analysisData->setCollection("bookmarked_tracks",tracks);
}

QList<ATrack*> ATrackTreeModel::tracks() const
{
  return analysisData->getCollection("bookmarked_tracks");
}

void ATrackTreeModel::addView(QAbstractItemView* view)
{
  view->setModel(this);
  view->setSelectionModel(selection);
  view->setSelectionBehavior(QAbstractItemView::SelectRows);
}

QModelIndex ATrackTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if(!hasIndex(row,column,parent))
    return QModelIndex();

  // The data of the tree item is stored as some data, parent tree item and the row of the parent
  QAbstractTreeItem *item=0;

  if(!parent.isValid())
    {  // Invalid parent (top level track), so we store 0 as the parent combination
      item=findTreeItem(tracks().at(row),0,0);
    }
  else
    { 
      // Determine the parent's tree item
      void *ptr=parent.internalPointer(); // Holds pointer to the parent
      QAbstractTreeItem *pItem=(QAbstractTreeItem*)ptr;
      // Determine the row of the parent combination by looking at parent intex's data
      int pRow=parent.row();
      
      //Determine the data to store in this item
      ATrack *data=0;
      ATrack *pTrack=qobject_cast<ATrack*>(pItem->data());
      ATrackCombination *combo=qobject_cast<ATrackCombination*>(pTrack);
      if(combo) // Parent is (and always should be) a track combination!
	data=combo->getTrack(row);
      else
	  return QModelIndex();
	 
      item=findTreeItem(data,pItem,pRow);
    }

  return createIndex(row,column,item);
}

QModelIndex ATrackTreeModel::parent(const QModelIndex &index) const
{
  if(!index.isValid())
    {
      return QModelIndex();
    }

  // Determining the parent index is simple.
  // 1) Check if the track is top level, if so, then return an invalid parent
  QAbstractTreeItem *pItem=(QAbstractTreeItem*)index.internalPointer();

  if(!pItem->parentItem())
      return QModelIndex();

  // 2) Otherwise return an index containing the parent's parent as data
  // The column is always 0 in a tree...
  return createIndex(pItem->parentRow(),0,pItem->parentItem());
}

int ATrackTreeModel::rowCount(const QModelIndex& root) const
{
  if(!root.isValid())
    { // Invalid root, means it wants the number of top level items
      return tracks().size();
    }
  
  QAbstractTreeItem *item=(QAbstractTreeItem*)root.internalPointer();
  ATrackCombination *combo=qobject_cast<ATrackCombination*>(item->data());
  
  if(combo)
    return combo->size();
  else
    return 0;
}

int ATrackTreeModel::columnCount(const QModelIndex& root) const
{
  return 1;
}

QVariant ATrackTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  
 // Get the track data...
  QAbstractTreeItem *item=(QAbstractTreeItem*)index.internalPointer();
  ATrack *track=qobject_cast<ATrack*>(item->data());
  
  if (role == Qt::DisplayRole)
    {
      switch (index.column())
        {
        case 0:
	  {
	    return track->name();
	  }
	  /*case 1:
	  if (tracks().at(index.row())->type() == ATrack::eSTrack)
            {
	      ASTrack* STrack = static_cast<ASTrack*>(tracks().at(index.row()));
	      return QString::number(STrack->Pt());
            }
	  else if (tracks().at(index.row())->type() == ATrack::eJet)
            {
	      AJet* Jet = static_cast<AJet*>(tracks().at(index.row()));
	      return QString::number(Jet->et);
            }
	  else return QString("N/A");
        case 2:
	  if (tracks().at(index.row())->type() == ATrack::eSTrack)
            {
	      ASTrack* STrack = static_cast<ASTrack*>(tracks().at(index.row()));
	      return QString::number(STrack->Mlv);
            }
	    else return QString("N/A");*/
        }
    }
  return QVariant();
}

QVariant ATrackTreeModel::headerData (int section, Qt::Orientation orientation, int role) const
{
  if (role!=Qt::DisplayRole)
    return QVariant();
  
  if (orientation == Qt::Horizontal)
    {
      switch (section)
        {
        case 0:
	  return "Name";
        case 1:
	  return "pT";
	  /*case 2:
            return "M(lv)";*/
	}
    }
  
  if (orientation == Qt::Vertical)
    {
      QList<ATrack *> tracks=this->tracks();
      return QString::number(tracks[section]->trackID());
      //TODO Renable selection ID
      //selectionID());
    }
  return QVariant();
}

void ATrackTreeModel::sort(int column, Qt::SortOrder order)
{
  //Do a bubble sort... Switch to something faster later?
  /*QList<ATrack*> tracks=this->tracks();
  emit layoutAboutToBeChanged();
  for (int i=0;i<tracks.size()-1;i++)
    {
      for (int j=i;j<tracks.size()-1;j++)
        {
	  switch (column)
            {
            case 0:
	      if ( (tracks[j]->trackID()<tracks[j+1]->trackID() && order==Qt::AscendingOrder) ||
		   (tracks[j]->trackID()>tracks[j+1]->trackID() && order==Qt::DescendingOrder) )
                {
		  tracks.swap(i,j+1);
                }
	      break;
            case 1:
	      if ( (tracks[j]->name()<tracks[j+1]->name() && order==Qt::AscendingOrder) ||
		   (tracks[j]->name()>tracks[j+1]->name() && order==Qt::DescendingOrder) )
                {
		  tracks.swap(i,j+1);
                }
	      break;
            case 2:
	      if ( (tracks[j]->Pt()<tracks[j+1]->Pt() && order==Qt::AscendingOrder) ||
		   (tracks[j]->Pt()>tracks[j+1]->Pt() && order==Qt::DescendingOrder) )
                {
		  tracks.swap(i,j+1);
                }
	      break;
            default:
	      break;
            }
        }
    }
  setTracks(tracks);
  emit layoutChanged();*/
}

void ATrackTreeModel::deleteSelectedTracks()
{
  //Get rows. Since all colums should be selected, might as well select column 0.
  QModelIndexList rows=selection->selectedRows(0);
  //Go through each row...
  QList<ATrack *> tracks=this->tracks();
  for (int i=rows.size()-1;i>=0;i--)
    {
      QModelIndex index=rows[i];
      if(!index.parent().isValid())
	{ // Top level item
	  tracks.removeAt(index.row());
	}
      else
	{ // Inside a combination
	  QAbstractTreeItem *item=(QAbstractTreeItem*)rows[i].internalPointer();
	  ATrackCombination *combo=qobject_cast<ATrackCombination*>(item->parentItem()->data());
	  ATrack *track=qobject_cast<ATrack*>(item->data());
	  combo->deleteTrack(track);
	}

    }
  setTracks(tracks);
}

void ATrackTreeModel::combineSelectedTracks()
{
  ATrackCombination *combo=new ATrackCombination(); //hold the stuff
  
  //Get rows. Since all colums should be selected, might as well select column 0.
  QModelIndexList rows=selection->selectedRows();
  //Go through each row...
  for (int i=0;i<rows.size();i++)
    {
      QAbstractTreeItem *item=(QAbstractTreeItem*)rows[i].internalPointer();
      ATrack *track=qobject_cast<ATrack*>(item->data());
      combo->addTrack(track);
    }
  
  QList<ATrack *> tracks=this->tracks();
  tracks.append(combo);
  setTracks(tracks);
}

void ATrackTreeModel::handleSelectionChanged(const QItemSelection& selected,const QItemSelection& deselected)
{
  // PERFORM DESELECTION
  //Deselection should be performed first, because in the case of singletrack selection, everything will be deselected anyways. Even though in theory
  //there shouldn't be any problems, might as well do this to keep safe.
  QModelIndexList idxs=deselected.indexes();
  for (int i=0;i<idxs.size();i++)
    {
      QAbstractTreeItem *item=(QAbstractTreeItem*)idxs[i].internalPointer();
      ATrack *track=qobject_cast<ATrack*>(item->data());
      performDeselection(track);
    }
  
  // PERFORM SELECTION
  idxs=selected.indexes();
  
  bool multi = ((QApplication::keyboardModifiers() & (Qt::ShiftModifier | Qt::ControlModifier)) > 0);
  for (int i=0;i<idxs.size();i++)
    {
      QAbstractTreeItem *item=(QAbstractTreeItem*)idxs[i].internalPointer();
      ATrack *track=qobject_cast<ATrack*>(item->data());
      performSelection(track,multi);
    }

  // The combine button should be enabled, because there are tracks selected
  emit combineButtonEnabled(selection->selectedRows().size()>0);
}

void ATrackTreeModel::performSelection(ATrack *track,bool multi)
{
  // Perform a recursive selection if this is a combination..
  if(track->type()==ATrack::eCombination)
    {
      multi=true; // Set multi to true, because we want to select all track in a combination...
      ATrackCombination *combo=qobject_cast<ATrackCombination*>(track);
      for(int i=0;i<combo->size();i++)
	{
	  performSelection(combo->getTrack(i),multi);
	}
    }
  else
    {
      int id=track->trackID();
      emit entrySelected(id,multi);
    }
}

void ATrackTreeModel::performDeselection(ATrack *track)
{
  // Perform a recursive deselection if this is a combination..
  if(track->type()==ATrack::eCombination)
    {
      ATrackCombination *combo=qobject_cast<ATrackCombination*>(track);
      for(int i=0;i<combo->size();i++)
	{
	  performDeselection(combo->getTrack(i));
	}
    }
  else
    {
      int id=track->trackID();
      emit entryDeselected(id);
    }
}

void ATrackTreeModel::refresh()
{
  clearInternalTree();
  createInternalTree();
  reset();
}

void ATrackTreeModel::clear()
{
  analysisData=new ATrackCollection("AGeometry");
  refresh();
}

QAbstractTreeItem* ATrackTreeModel::findTreeItem(QObject *data,QAbstractTreeItem* parentItem,int parentRow) const
{
  QAbstractTreeItem *item=0;

  // Find where the item is cached.
  QList<QAbstractTreeItem*>::const_iterator iter=treeItems.begin();
  QList<QAbstractTreeItem*>::const_iterator iterE=treeItems.end();
  for(;iter!=iterE;iter++)
    {
      QAbstractTreeItem *test=(*iter);
      if(test->data()==data &&
	 test->parentItem() == parentItem &&
	 test->parentRow() == parentRow)
	{
	  item=test;
	  break;
	}
    }

  if(!item)
    {
      item=new QAbstractTreeItem(data,parentItem,parentRow);
    }

  return item;
}

void ATrackTreeModel::clearInternalTree()
{
  // For each item, delete
  QList<QAbstractTreeItem*>::const_iterator iter=treeItems.begin();
  QList<QAbstractTreeItem*>::const_iterator iterE=treeItems.end();
  for(;iter!=iterE;iter++)
    {
      QAbstractTreeItem *test=(*iter);
      test->setData(0);
      test->deleteLater();
    }
  treeItems.clear();
}

void ATrackTreeModel::createInternalTree()
{
  // For each toplevel track, add it to the list
  // and perform recursive add.
  QList<ATrack*> tracks=this->tracks();
  for(int i=0;i<tracks.size();i++)
    {
      ATrack *track=tracks[i];
      QAbstractTreeItem *item=new QAbstractTreeItem(track,0,0);
      treeItems.push_back(item);

      createInternalTree(track,item,i); // Recursive add
    }
}

void ATrackTreeModel::createInternalTree(ATrack *track,QAbstractTreeItem *parent,int row)
{
  // Loop through the combination's tracks and add them to the list
  if(track->type()==ATrack::eCombination)
    {
      ATrackCombination *combo=qobject_cast<ATrackCombination*>(track);
      for(int i=0;i<combo->size();i++)
	{
	  ATrack *childTrack=combo->getTrack(i);
	  QAbstractTreeItem *item=new QAbstractTreeItem(childTrack,parent,row);
	  treeItems.push_back(item);
      
	  createInternalTree(childTrack,item,i);
	}
    }
}
