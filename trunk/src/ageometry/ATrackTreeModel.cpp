#include "ATrackTreeModel.h"

#include <QApplication>
#include <QInputDialog>
#include <QDebug>

#include <QBetterMimeData.h>

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
      //disconnect(analysisData,SIGNAL(updated()),
      //this,SLOT(refresh()));
      disconnect(analysisData,SIGNAL(trackInserted(int)),
		 this,SLOT(handleTrackInserted(int)));
      disconnect(analysisData,SIGNAL(trackRemoved(int)),
		 this,SLOT(handleTrackRemoved(int)));

      analysisData=new ATrackCollection("AGeometry");

      clearInternalTree();
    }
  
  // Load new analysis data.
  if(event)
    {
      analysisData=event->getAnalysisData<ATrackCollection>("AGeometry");
      
      //connect(analysisData,SIGNAL(updated()),
      //this,SLOT(refresh()));
      connect(analysisData,SIGNAL(trackInserted(int)),
	      this,SLOT(handleTrackInserted(int)),
	      Qt::DirectConnection);
      connect(analysisData,SIGNAL(trackRemoved(int)),
	      this,SLOT(handleTrackRemoved(int)),
	      Qt::DirectConnection);
      
      // Create the internal tree to store child->parent relationships
      createInternalTree();
    }
  refresh();
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
      item=findTreeItem(analysisData->getTrack(row),row,0);
    }
  else
    { 
      // Determine the parent's tree item
      void *ptr=parent.internalPointer(); // Holds pointer to the parent
      QAbstractTreeItem *pItem=(QAbstractTreeItem*)ptr;
      
      //Determine the data to store in this item
      ATrack *data=0;
      ATrack *pTrack=qobject_cast<ATrack*>(pItem->data());
      ATrackCombination *combo=qobject_cast<ATrackCombination*>(pTrack);
      if(combo) // Parent is (and always should be) a track combination!
	data=combo->getTrack(row);
      else
	return QModelIndex();
	 
      item=findTreeItem(data,row,pItem);
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
  return createIndex(pItem->parentItem()->row(),0,pItem->parentItem());
}

int ATrackTreeModel::rowCount(const QModelIndex& root) const
{
  if(!root.isValid())
    { // Invalid root, means it wants the number of top level items
      return analysisData->size();
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
  return 2;
}

QVariant ATrackTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  
 // Get the track data...
  QAbstractTreeItem *item=(QAbstractTreeItem*)index.internalPointer();
  ATrack *track=qobject_cast<ATrack*>(item->data());
  ATrackCombination *combo=qobject_cast<ATrackCombination*>(track);
  
  if (role == Qt::DisplayRole)
    {
      switch (index.column())
        {
	  /*case 0:
	  {
	    if(track->type()==ATrack::eCombination)
	      return combo->trackIDString();
	    else
	      return QString::number(track->trackID());
	      }*/
        case 0:
	  {
	    if(track->type()==ATrack::eCombination)
	      {
		if(combo->name(false).isEmpty())
		  return "Combo\nID: "+combo->trackIDString();
		else
		  return combo->name()+"\nID: "+combo->trackIDString();
	      }
	    else
	      return track->name()+"\nID: "+QString::number(track->trackID());
	  }
	case 1:
	  if (track->type() == ATrack::eSTrack)
            {
	      ASTrack* STrack = static_cast<ASTrack*>(track);
	      return "Pt: "+QString::number(STrack->Pt());
            }
	  else if (track->type() == ATrack::eJet)
            {
	      AJet* Jet = static_cast<AJet*>(track);
	      return "Et: "+QString::number(Jet->et);
            }
	  else if(track->type() == ATrack::eCombination)
	    {
	      ATrackCombination* combo = static_cast<ATrackCombination*>(track);
	      return "IM: "+QString::number(combo->getInvariantMass());
	    }
	  else if (track->type() == ATrack::eMissingEt)
            {
	      AMisET* met = static_cast<AMisET*>(track);
	      return "Et: "+QString::number(met->et);
            }
	  else return QString("N/A");
        }
    }
  else if(role == Qt::ForegroundRole)
    {
      if(track->type()==ATrack::eCombination)
	return Qt::blue;
    }
  else if(role == Qt::BackgroundRole)
    {
      if(track->type()==ATrack::eCombination)
	return QColor(255,255,220);
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
	  return "Et/Pt/IM (GeV)";
	}
    }
  
  if (orientation == Qt::Vertical)
    {
      return QString::number(analysisData->getTrack(section)->trackID());
      //TODO Renable selection ID
      //selectionID());
    }
  return QVariant();
}

Qt::ItemFlags ATrackTreeModel::flags(const QModelIndex& index) const
{
  if(!index.isValid()) return QAbstractItemModel::flags(index);

  // All items can be dragged, selected
  Qt::ItemFlags flags = Qt::ItemIsEnabled;
  flags|=Qt::ItemIsSelectable;
  flags|=Qt::ItemIsDragEnabled;

  // Only combinations can be dropped on and edited
  QAbstractTreeItem *item=(QAbstractTreeItem*)index.internalPointer();
  ATrack *track=qobject_cast<ATrack*>(item->data());
  if(track->type()==ATrack::eCombination)
    {
      flags|=Qt::ItemIsDropEnabled;
      if(index.column()==0) flags|=Qt::ItemIsEditable;
    } 
  
  return flags;
}

bool ATrackTreeModel::setData(const QModelIndex& index,const QVariant& value,int role)
{
  if(!index.isValid()) return false;
  
  if(role==Qt::EditRole)
    {
      QAbstractTreeItem *item=(QAbstractTreeItem*)index.internalPointer();
      ATrack *track=qobject_cast<ATrack*>(item->data());
      
      QString newName=value.toString();
      if(newName.isEmpty()) return false; //No empties allowed
      
      track->setName(newName);
      emit dataChanged(index,index);
      return true;
    }
  return QAbstractItemModel::setData(index,value,role);
}

Qt::DropActions ATrackTreeModel::supportedDropActions() const
{
  return Qt::CopyAction | Qt::MoveAction;
}

bool ATrackTreeModel::dropMimeData(const QMimeData *data,Qt::DropAction action,int row,int column,const QModelIndex &parent)
{
  QAbstractTreeItem *item=(QAbstractTreeItem*)parent.internalPointer();
  ATrackCombination *combo=qobject_cast<ATrackCombination*>(item->data());
  if(!combo) return false;
  
  const QBetterMimeData *data1=qobject_cast<const QBetterMimeData*>(data);
  if(!data1) return false;

  ATrack *track=qobject_cast<ATrack*>(data1->data("amelia/x-track"));
  combo->addTrack(track);

  refresh();

  return true;
}


QStringList ATrackTreeModel::mimeTypes() const
{
  QStringList mimeTypes;
  mimeTypes << "amelia/x-track";
  return mimeTypes;
}

QMimeData* ATrackTreeModel::mimeData(const QModelIndexList &indexes) const
{
  QBetterMimeData *mimeData=new QBetterMimeData();

  foreach(QModelIndex index,indexes)
    {
      if(!index.isValid() || index.column()!=0) continue;
      QAbstractTreeItem *item=(QAbstractTreeItem*)index.internalPointer();
      ATrack *track=qobject_cast<ATrack*>(item->data());

      if(track)
	{
	  mimeData->setData("amelia/x-track",track);
	}
    }
  return mimeData;
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
  for (int i=rows.size()-1;i>=0;i--)
    {
      QModelIndex index=rows[i];
      if(!index.parent().isValid())
	{ // Top level item
	  analysisData->removeTrack(index.row());
	}
      else
	{ // Inside a combination
	  QAbstractTreeItem *item=(QAbstractTreeItem*)rows[i].internalPointer();
	  ATrackCombination *combo=qobject_cast<ATrackCombination*>(item->parentItem()->data());
	  ATrack *track=qobject_cast<ATrack*>(item->data());
	  combo->deleteTrack(track);
	  refresh();
	}
    }
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
  
  analysisData->addTrack(combo);
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

void ATrackTreeModel::handleTrackRemoved(int idx)
{
  clearInternalTree(treeItems[idx]);
  emit beginRemoveRows(QModelIndex(),idx,idx);
  emit endRemoveRows();
}

void ATrackTreeModel::handleTrackInserted(int idx)
{
  emit beginInsertRows(QModelIndex(),idx,idx);
  ATrack *track=analysisData->getTrack(idx);
  QAbstractTreeItem *item=new QAbstractTreeItem(track,idx,0);
  treeItems.insert(idx,item);
  for(int i=idx+1;i<treeItems.size() && treeItems[i]->parentItem()==0;i++)
    {
      treeItems[i]->setRow(i);
    }
  createInternalTree(track,item);
  emit endInsertRows();
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

QAbstractTreeItem* ATrackTreeModel::findTreeItem(QObject *data,int row,QAbstractTreeItem* parentItem) const
{
  QAbstractTreeItem *item=0;

  // Find where the item is cached.
  QList<QAbstractTreeItem*>::const_iterator iter=treeItems.begin();
  QList<QAbstractTreeItem*>::const_iterator iterE=treeItems.end();
  for(;iter!=iterE;iter++)
    {
      QAbstractTreeItem *test=(*iter);
      if(test->data()==data &&
	 test->row() == row &&
	 test->parentItem() == parentItem)
	{
	  item=test;
	  break;
	}
    }

  if(!item)
    {
      qDebug() << "Non existent " << data << " " << parentItem;
      item=new QAbstractTreeItem(data,row,parentItem);
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

void ATrackTreeModel::clearInternalTree(QAbstractTreeItem *item)
{
  // Remove an item from the internal tree by looping over
  // all of the items, and modifying each as required.
  // - Remove the item, when found.
  // - Is a child of an item
  // - Is a sibling of the item, but comes after. The row will have to be shifted up.
  for(int i=0;i<treeItems.size();i++)
    {
      QAbstractTreeItem *test=treeItems[i];
      if(test==item)
	{ // Found it
	  treeItems.removeAt(i);
	  test->setData(0);
	  test->deleteLater();
	  i--; // The list is one shorter, so we have to go over this index again
	}
      else if(test->parentItem()==item)
	{ // Remove child
	  clearInternalTree(test);
	  i=0; // Don't know how many items the recursive function removed, so just start again..
	}
      else if(item->parentItem()==test->parentItem() && item->row() < test->row())
	{ // Same parent, but one row after. So shift down
	  test->setRow(test->row()-1);
	}
    }
}

void ATrackTreeModel::createInternalTree()
{
  // For each toplevel track, add it to the list
  // and perform recursive add.
  for(int i=0;i<analysisData->size();i++)
    {
      ATrack *track=analysisData->getTrack(i);
      QAbstractTreeItem *item=new QAbstractTreeItem(track,i,0);
      treeItems.insert(i,item);

      createInternalTree(track,item); // Recursive add
    }
}

void ATrackTreeModel::createInternalTree(ATrack *track,QAbstractTreeItem *parent)
{
  // Loop through the combination's tracks and add them to the list
  if(track->type()==ATrack::eCombination)
    {
      ATrackCombination *combo=qobject_cast<ATrackCombination*>(track);
      for(int i=0;i<combo->size();i++)
	{
	  ATrack *childTrack=combo->getTrack(i);
	  QAbstractTreeItem *item=new QAbstractTreeItem(childTrack,i,parent);
	  treeItems.push_back(item);
      
	  createInternalTree(childTrack,item);
	}
    }
}
