#include "ATrackTreeModel.h"

#include <QApplication>
#include <QInputDialog>
#include <QSortFilterProxyModel>
#include <QDebug>

#include <QBetterMimeData.h>

#include "QAbstractTreeItem.h"

ATrackTreeModel::ATrackTreeModel(QWidget* parent)
  :QAbstractItemModelWithContextMenu(parent),topItem(0)
{
  analysisData=new AEventObjectCollection("AGeometry");
}

ATrackTreeModel::~ATrackTreeModel()
{ }

void ATrackTreeModel::handleNewEventLoaded(AEvent *event)
{
  // Unload the data loaded before, if any.
  if(analysisData)
    {
      disconnect(analysisData,SIGNAL(updated()),
		 this,SLOT(refresh()));

      analysisData=new AEventObjectCollection("AGeometry");

      clearInternalTree();
    }
  
  // Load new analysis data.
  if(event)
    {
      analysisData=event->getAnalysisData<AEventObjectCollection>("AGeometry");
      
      connect(analysisData,SIGNAL(updated()),
	      this,SLOT(refresh()));
      
      // Create the internal tree to store child->parent relationships
      topItem=createInternalTree();
    }
  reset();
}

void ATrackTreeModel::addView(QAbstractItemView* view)
{
  //Sort filter proxy ... for sorting..
  QSortFilterProxyModel *sortProxy=new QSortFilterProxyModel(view);
  sortProxy->setSourceModel(this);
  view->setModel(sortProxy);

  view->setSelectionBehavior(QAbstractItemView::SelectRows);
}

QModelIndex ATrackTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if(!hasIndex(row,column,parent))
    return QModelIndex();

  // The data of the tree item is stored as some data, parent tree item and the row of the parent
  QAbstractTreeItem *item=0;

  if(!parent.isValid())
    {  // Invalid parent (top level track), so we retrieve the data from topItem
      item=topItem->getChildItem(row);
    }
  else
    { 
      // Determine the parent's tree item
      void *ptr=parent.internalPointer(); // Holds pointer to the parent
      QAbstractTreeItem *pItem=(QAbstractTreeItem*)ptr;
      
      item=pItem->getChildItem(row);
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
  QAbstractTreeItem *item=(QAbstractTreeItem*)index.internalPointer();

  if(item->parentItem()==topItem)
      return QModelIndex();

  // 2) Otherwise return an index containing the parent's parent as data
  // The column is always 0 in a tree...
  return createIndex(item->parentItem()->row(),0,item->parentItem());
}

int ATrackTreeModel::rowCount(const QModelIndex& root) const
{
  if(!root.isValid())
    { // Invalid root, means it wants the number of top level items
      return (topItem)?topItem->numChildItems():0;
    }
  
  QAbstractTreeItem *item=(QAbstractTreeItem*)root.internalPointer();
  return item->numChildItems();;
}

int ATrackTreeModel::columnCount(const QModelIndex& root) const
{
  return 5;
}

QVariant ATrackTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  
 // Get the track data...
  QAbstractTreeItem *item=(QAbstractTreeItem*)index.internalPointer();
  AEventObject *track=qobject_cast<AEventObject*>(item->data());
  ATrackCombination *combo=qobject_cast<ATrackCombination*>(track);
  
  if (role == Qt::DisplayRole)
    {
      switch (index.column())
        {
        case 0:
	  {
	    if(track->type()==AEventObject::eCombination)
	      {
		if(combo->name(false).isEmpty())
		  return "Combo\nID: "+combo->IDString();
		else
		  return combo->name()+"\nID: "+combo->IDString();
	      }
	    else
	      return track->name()+"\nID: "+QString::number(track->ID());
	  }
	case 1:
	  {
	    if (track->type() == AEventObject::eTrack)
	      return "Pt: "+QString::number(track->pt());
	    else if (track->type() == AEventObject::eJet || track->type() == AEventObject::eMissingEt)
	      return "Et: "+QString::number(track->et());
	    else if(track->type() == AEventObject::eCombination)
	      {
		ATrackCombination* combo = static_cast<ATrackCombination*>(track);
		return "IM: "+QString::number(combo->getInvariantMass());
	      }
	    else return QString("N/A");
	  }
	case 2:
	  {
	    if(track->type()==AEventObject::eCombination)
	      return combo->IDString();
	    else
	      return QString::number(track->ID());
	  }
        case 3:
	  {
	    if(track->type()==AEventObject::eCombination && combo->name(false).isEmpty())
	      return "Combo";
	    else
	      return track->name();
	  }
	case 4:
	  {
	    if (track->type() == AEventObject::eTrack)
	      return QString::number(track->pt());
	    else if (track->type() == AEventObject::eJet || track->type() == AEventObject::eMissingEt)
	      return QString::number(track->et());
	    else if(track->type() == AEventObject::eCombination)
	      {
		ATrackCombination* combo = static_cast<ATrackCombination*>(track);
		return QString::number(combo->getInvariantMass());
	      }
	    else return QString("N/A");
	  }
        }
    }
  else if(role == Qt::ForegroundRole)
    {
      if(track->type()==AEventObject::eCombination)
	return Qt::blue;
    }
  else if(role == Qt::BackgroundRole)
    {
      if(track->type()==AEventObject::eCombination)
	return QColor(255,255,220);
    }
  else if(role == QAbstractItemModelWithContextMenu::MenuDataRole)
    {
      return QVariant::fromValue<QObject *>(track);
    }

  return QAbstractItemModelWithContextMenu::data(index,role);
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
	case 2:
	  return "ID";
	case 3:
	  return "Name";
	case 4:
	  return "Et/Pt/IM (GeV)";
	}
    }
  
  if (orientation == Qt::Vertical)
    {
      return QString::number(analysisData->get(section)->ID());
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
  AEventObject *track=qobject_cast<AEventObject*>(item->data());
  if(track->type()==AEventObject::eCombination)
    {
      flags|=Qt::ItemIsDropEnabled;
      if(index.column()==0 || index.column()==3) flags|=Qt::ItemIsEditable;
    } 
  
  return flags;
}

bool ATrackTreeModel::setData(const QModelIndex& index,const QVariant& value,int role)
{
  if(!index.isValid()) return false;
  
  if(role==Qt::EditRole)
    {
      QAbstractTreeItem *item=(QAbstractTreeItem*)index.internalPointer();
      // We can only rename combinations
      ATrackCombination *combo=qobject_cast<ATrackCombination*>(item->data());
      if(!combo) return false; // Not a combination!
      
      QString newName=value.toString();
      if(newName.isEmpty()) return false; //No empties allowed
      
      combo->setName(newName);
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

  AEventObject *track=qobject_cast<AEventObject*>(data1->data("amelia/x-track"));
  combo->addTrack(track);

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
      AEventObject *track=qobject_cast<AEventObject*>(item->data());

      if(track)
	{
	  mimeData->setData("amelia/x-track",track);
	}
    }
  return mimeData;
}

bool ATrackTreeModel::removeRows (int row, int count , const QModelIndex& parent )
{
  for (int i=0;i<count;i++)
    {
      if(!parent.isValid())
	{ // Top level item
	  analysisData->remove(row);
	}
      else
	{ // Inside a combination
	  QAbstractTreeItem *item=(QAbstractTreeItem*)parent.internalPointer();
	  ATrackCombination *combo=qobject_cast<ATrackCombination*>(item->data());
	  AEventObject *track=combo->getTrack(row);
	  combo->deleteTrack(track);
	}
    }
  return true;
}

void ATrackTreeModel::refresh()
{
  QAbstractTreeItem *newTopItem=createInternalTree();
  synchronizeItems(newTopItem,topItem,QModelIndex());
  newTopItem->deleteLater();
}

void ATrackTreeModel::clear()
{
  analysisData=new AEventObjectCollection("AGeometry");
  clearInternalTree();
  reset();
}

void ATrackTreeModel::clearInternalTree()
{
  topItem->deleteLater();
  topItem=0;
}

QAbstractTreeItem* ATrackTreeModel::createInternalTree()
{
  // For each toplevel track, add it to the list
  // and perform recursive add.
  QAbstractTreeItem *topItem=new QAbstractTreeItem(0,0,0);
  for(int i=0;i<analysisData->size();i++)
    {
      AEventObject *track=analysisData->get(i);
      QAbstractTreeItem *item=new QAbstractTreeItem(track,i,topItem);
      topItem->addChildItem(item);

      createInternalTree(track,item); // Recursive add
    }
  return topItem;
}

void ATrackTreeModel::createInternalTree(AEventObject *track,QAbstractTreeItem *parent)
{
  // Loop through the combination's tracks and add them to the list
  if(track->type()==AEventObject::eCombination)
    {
      ATrackCombination *combo=qobject_cast<ATrackCombination*>(track);
      for(int i=0;i<combo->size();i++)
	{
	  AEventObject *childTrack=combo->getTrack(i);
	  QAbstractTreeItem *item=new QAbstractTreeItem(childTrack,i,parent);
	  parent->addChildItem(item);
      
	  createInternalTree(childTrack,item);
	}
    }
}

void ATrackTreeModel::synchronizeItems(QAbstractTreeItem *newItem,QAbstractTreeItem *oldItem,QModelIndex index)
{
  // To syncronize two branches, we do the following
  // 1) Create a temporary list of the new children
  // 2) Remove items from the old branch that are not found in the temporary list. If a copy is found, then remove it from the temporary list. This handles duplicate entries.
  // 3) Whatever is left over in the temporary list is not found in the old list. Thus insert it all at the end.
  // 4) Perform transversal. Note, we use a temporary list again because of possible duplicates.

  // Prepare the temporary list of items to check against
  QList<QAbstractTreeItem*> checkQueue;
  for(int i=0;i<newItem->numChildItems();i++)
    {
      checkQueue.push_back(newItem->getChildItem(i));
    }

  // Perform removals
  for(int i=oldItem->numChildItems()-1;i>=0;i--)
    {
      QAbstractTreeItem *oldChildItem=oldItem->getChildItem(i);
      bool foundInNew=false;
      for(int j=0;j<checkQueue.size();j++)
	{
	  QAbstractTreeItem *newChildItem=checkQueue.at(j);
	  if(oldChildItem->data()==newChildItem->data())
	    {
	      foundInNew=true;
	      checkQueue.removeAt(j);
	      break;
	    }
	}
      
      if(!foundInNew)
	{
	  emit beginRemoveRows(index,i,i);
	  oldItem->removeChildItem(i);
	  oldChildItem->deleteLater();
	  emit endRemoveRows();
	}
    }

  // Perform insertions
  for(int i=0;i<checkQueue.size();i++)
    {
      QAbstractTreeItem *newChildItem=checkQueue.at(i);
      
      emit beginInsertRows(index,oldItem->numChildItems(),oldItem->numChildItems());
      newChildItem->setParentItem(oldItem);
      newChildItem->setRow(oldItem->numChildItems());
      oldItem->addChildItem(newChildItem);
      emit endInsertRows();
    }

  // Perform the transversal to update the childdren
  // Remake the temporary list of items to check against
  checkQueue.clear();
  for(int i=0;i<newItem->numChildItems();i++)
    {
      checkQueue.push_back(newItem->getChildItem(i));
    }

  // The actual tansversal!
  for(int i=0;i<oldItem->numChildItems();i++)
    {
      QAbstractTreeItem *oldChildItem=oldItem->getChildItem(i);
      oldChildItem->setRow(i);
      for(int j=0;j<checkQueue.size();j++)
	{
	  QAbstractTreeItem *newChildItem=checkQueue.at(j);
	  if(oldChildItem->data()==newChildItem->data())
	    {
	      checkQueue.removeAt(j);
	      synchronizeItems(newChildItem,oldChildItem,this->index(i,0,index));
	      break;
	    }
	}
    }
	      
  return;
}
