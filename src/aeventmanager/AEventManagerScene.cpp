#include "AEventManagerScene.h"

#include <QBetterMimeData.h>

#include <QDir>
#include <QDebug>
#include <QFont>

#include <iostream>
using namespace std;

AEventManagerScene::AEventManagerScene(AEventManager* _manager,QString module)
  :QAbstractItemModel(_manager),manager(_manager),_activePackage(0),_activeEvent(0),_module(module)
{
  connect(_manager,SIGNAL(packageLoaded(const QString&)),
	  this,SLOT(handlePackageUpdate(const QString&)));

  connect(_manager,SIGNAL(packageAdded(const QString&)),
	  this,SLOT(handlePackageAdded(const QString&)));
}

QModelIndex AEventManagerScene::index(int row, int column, const QModelIndex& parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();
  
  QModelIndex index;

  QStringList packages=manager->packageList();
  if (!parent.isValid())
    {
      index=createIndex(row,column,manager->package(packages[row]));
    }
  else
    {
      AEventPackage* pkg=getPackage(parent);
      if(pkg->isLoaded())
	index=createIndex(row,column,pkg->event(row));
      else
	index=createIndex(row,column,new QObject(pkg));
    }
  return index;
}

QModelIndex AEventManagerScene::parent(const QModelIndex& index) const
{
  if (!index.isValid()) return QModelIndex();
  
  AEventPackage* pkg=getPackage(index);
  AEvent* e=getEvent(index);

  if(pkg) // Packages have no parents
    return QModelIndex();
  else if(e) // Events have their package as a parent
    {
      return this->index(e->package(),0);
    }
  else // It is possible that we are talking about the unloaded node of a package...
    {
      QObject *obj=(QObject*)index.internalPointer();
      if(obj->parent()->inherits("AEventPackage"))
	return this->index((AEventPackage*)obj->parent(),0);
    }
  return QModelIndex();
}

int AEventManagerScene::rowCount(const QModelIndex &index) const
{
  if (!index.isValid())  return manager->packageList().size();  
  
  AEventPackage* pkg=getPackage(index);

  if(pkg)
    {
      if(pkg->isLoaded())
	return pkg->eventCount();
      else
	return 1;
    }
  else
    return 0;
}

int AEventManagerScene::columnCount(const QModelIndex &index) const
{
  return 2;
}

QVariant AEventManagerScene::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();
  
  AEventPackage* pkg=getPackage(index);
  AEvent *e=getEvent(index);

  QFont font;

  switch (role)
    {
      case Qt::FontRole:
      if (e)
        { // Event Name
	  bool read=(_module=="")?true:e->listAnalysisData().contains(_module);
	  if (!read)
	    font.setBold(true);
	  
	  if (_activeEvent==e)
            {
	      font.setItalic(true);
            }
        }
      else if(pkg)
        { // Package Name
	  font.setBold(true);
	  
	  if (_activePackage==pkg)
	    font.setItalic(true);
	  
	  font.setPointSize(14);
        }
      
      return font;
      
    case Qt::DisplayRole:
      if (e)
        {
	  if (index.column()==0)
            {
	      
	      return e->name();
            }
	  else
            {
	      QString tagString="";
	      QSet<QString> tags=e->tags();
	      for (QSetIterator<QString> iter(tags);iter.hasNext();)
		tagString+=iter.next()+" ";
	      return tagString;
	    }
	}
      else if(pkg)
	{
	  if (index.column()==0)
            {
	      return pkg->name();
            }
	}
      else
	{
	  if(index.column()==0)
	    return "Loading..";
	}
      
      case Qt::ForegroundRole:
      if (e)
        {
	  if (_activeEvent==e)
	    return Qt::red;
	}
    case Qt::ToolTipRole:
      if (e)
        {
	  QString toolTip=
	    "<html><b>Run Number:</b> \t\t"+QString::number(e->runNumber)+"<br/>"+
	    "<b>Event Number:</b> \t\t"+QString::number(e->eventNumber)+"<br/><br/>"+
	    "<b>Tracks:</b> \t\t"+QString::number(e->numTracks)+"<br/>"+
	    "<b>Neutral Hadrons:</b> \t"+QString::number(e->numNeutralHadrons)+"<br/>"
	    "<b>Charged Hadrons:</b> \t"+QString::number(e->numChargedHadrons)+"<br/>"
	    "<b>Photons:</b> \t\t"+QString::number(e->numPhotons)+"<br/>"
	    "<b>Muons:</b> \t\t"+QString::number(e->numMuons)+"<br/>"
	    "<b>Electrons:</b> \t\t"+QString::number(e->numElectrons)+
	    "</html>";
	  
	  return toolTip;
	}
    }
  /*else if(role==Qt::DecorationRole)
    {
      if(id!=-1)
	{
	  if(index.column()==1)
	    {
	      QList<QVariant> iconList;
	      iconList.append(QIcon(":/media/higgs.png"));
	      iconList.append(QIcon(":/media/blackhole.png"));
	      return iconList;
	    }
	}
    }*/
    return QVariant();
}

Qt::ItemFlags AEventManagerScene::flags(const QModelIndex& index) const
{
  AEventPackage *pkg=getPackage(index);
  AEvent *e=getEvent(index);

  Qt::ItemFlags flags = Qt::ItemIsEnabled;
  if(pkg)
    {
      flags|=Qt::ItemIsEditable;
      flags|=Qt::ItemIsDropEnabled;
    } 

  if(e)
    {
      flags|=Qt::ItemIsDragEnabled;
      flags|=Qt::ItemIsSelectable;
    }
  
  return flags;
}

Qt::DropActions AEventManagerScene::supportedDropActions() const
{
  return Qt::CopyAction | Qt::MoveAction;
}

bool AEventManagerScene::dropMimeData(const QMimeData *data,Qt::DropAction action,int row,int column,const QModelIndex &parent)
{
  AEventPackage *pkg=getPackage(parent);
  if(!pkg) return false;
  
  const QBetterMimeData *data1=qobject_cast<const QBetterMimeData*>(data);
  if(!data1) return false;

  AEvent *e=qobject_cast<AEvent*>(data1->data("amelia/x-event"));
  pkg->addEvent(e);

  emit layoutAboutToBeChanged();
  emit layoutChanged();

  return true;
}


QStringList AEventManagerScene::mimeTypes() const
{
  QStringList mimeTypes;
  mimeTypes << "amelia/x-event";
  return mimeTypes;
}

QMimeData *AEventManagerScene::mimeData(const QModelIndexList &indexes) const
{
  QBetterMimeData *mimeData=new QBetterMimeData();

  foreach(QModelIndex index,indexes)
    {
      if(!index.isValid() || index.column()!=0) continue;
      AEvent *e=getEvent(index);
      if(e)
	{
	  mimeData->setData("amelia/x-event",e);
	}
    }
  return mimeData;
} 

bool AEventManagerScene::setData(const QModelIndex& index,const QVariant& value,int role)
{
  if(!index.isValid()) return false;
  
  if(role==Qt::EditRole)
    {
      AEventPackage *pkg=getPackage(index);
      if(!pkg) return setData(index,value,role);

      QString newName=value.toString();
      if(newName.isEmpty()) return false; //No empties allows
      if(manager->package(newName)) return false; //No duplicates allowed
      
      pkg->setName(newName);
      emit dataChanged(index,index);
      return true;
    }
  return QAbstractItemModel::setData(index,value,role);
}

AEventPackage* AEventManagerScene::getPackage(const QModelIndex& index) const
{
  void *pointer=index.internalPointer();
  if(pointer==0) return 0;

  QObject *obj=(QObject*)pointer;
  if(obj->inherits("AEventPackage"))
      return (AEventPackage*)obj;
  else
    return 0;
}

AEvent* AEventManagerScene::getEvent(const QModelIndex& index) const
{
  void *pointer=index.internalPointer();
  if(pointer==0) return 0;
  
  QObject *obj=(QObject*)pointer;
  if(obj->inherits("AEvent"))
    return (AEvent*)obj;
  else
    return 0;
}

void AEventManagerScene::setActivePackage(AEventPackage* newActivePackage)
{
  QModelIndex start=index(newActivePackage,0);
  QModelIndex end=index(newActivePackage,0);
  _activePackage=newActivePackage;
  emit dataChanged(start,end);
}

void AEventManagerScene::setActiveEvent(AEvent* newActiveEvent)
{
  QModelIndex start=index(newActiveEvent,0);
  QModelIndex end=index(newActiveEvent,1);
  _activeEvent=newActiveEvent;
  emit dataChanged(start,end);
}

AEventPackage* AEventManagerScene::activePackage()
{
  return _activePackage;
}

AEvent* AEventManagerScene::activeEvent()
{
  return _activeEvent;
}

QModelIndex AEventManagerScene::index(AEvent* event,int col) const
{
  if(event==0) return QModelIndex();
  if(event->package()==0) return QModelIndex();

  int row=event->package()->indexOf(event);
  QModelIndex parent=index(event->package(),0);

  return index(row,col,parent);
}

QModelIndex AEventManagerScene::index(AEventPackage* pkg,int col) const
{
  QStringList packages=manager->packageList();
  int row=packages.indexOf(pkg->name());

  return index(row,col,QModelIndex());
}

void AEventManagerScene::handlePackageUpdate(const QString &name)
{
  AEventPackage *pkg=manager->package(name);
  emit dataChanged(index(pkg,0),index(pkg,0));

  //Remove the "Loading" node
  emit beginRemoveRows(index(pkg,0),0,0);
  emit endRemoveRows();

  //Insert the nodes for each of the events
  emit beginInsertRows(index(pkg,0),0,pkg->eventCount()-1);
  emit endInsertRows();

}

void AEventManagerScene::handlePackageAdded(const QString &name)
{
  int row=manager->packageList().indexOf(name);
  
  //Insert the nodes for each of the events
  emit beginInsertRows(QModelIndex(),row,row);
  emit endInsertRows();

}
