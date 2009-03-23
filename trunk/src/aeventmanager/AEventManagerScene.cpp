#include "AEventManagerScene.h"

#include <QDir>
#include <QDebug>
#include <QFont>

#include <iostream>
using namespace std;

AEventManagerScene::AEventManagerScene(AEventManager* _manager,QString module):manager(_manager),_module(module)
{ }

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
      index=createIndex(row,column,pkg->event(row));
    }
  return index;
}

QModelIndex AEventManagerScene::parent(const QModelIndex& index) const
{
  if (!index.isValid()) return QModelIndex();
  
  AEventPackage* pkg=getPackage(index);
  if(pkg)
    return QModelIndex();
  else
    {
      QStringList packages=manager->packageList();
      AEvent* e=getEvent(index);

      return createIndex(packages.indexOf(e->package->name()),0,e->package);
    }
}

int AEventManagerScene::rowCount(const QModelIndex &index) const
{
  if (!index.isValid())  return manager->packageList().size();  
  
AEventPackage* pkg=getPackage(index);
 if(pkg)
   return pkg->eventCount();
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
  
  int id=index.internalId();
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
      else
        { // Package Name
	  font.setBold(true);
	  
	  if (_activePackage==pkg)
	    font.setItalic(true);
	  
	  font.setPointSize(14);
        }
      
      return font;
      
    case Qt::DisplayRole:
      if (pkg)
        {
	  if (index.column()==0)
            {
	      return pkg->name();
            }
        }
      else
        {
	  if (index.column()==0)
            {
	      return e->filename;
            }
	  else
            {
	      QString tagString="";
	      for (QSetIterator<QString> iter(e->tags);iter.hasNext();)
		tagString+=iter.next()+" ";
	      return tagString;
	    }
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
	    "<html><b>Tracks:</b> \t\t"+QString::number(e->numTracks)+"<br/>"
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

AEventPackage* AEventManagerScene::getPackage(const QModelIndex& index) const
{
  QObject *obj=(QObject*)index.internalPointer();
  if(obj->inherits("AEventPackage"))
    return (AEventPackage*)obj;
  else
    return 0;
}

AEvent* AEventManagerScene::getEvent(const QModelIndex& index) const
{
  QObject *obj=(QObject*)index.internalPointer();
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
  if(event->package==0) return QModelIndex();

  int row=event->package->indexOf(event);
  QModelIndex parent=index(event->package,0);

  return index(col,row,parent);
}

QModelIndex AEventManagerScene::index(AEventPackage* pkg,int col) const
{
  QStringList packages=manager->packageList();
  int row=packages.indexOf(pkg->name());

  return index(col,row,QModelIndex());
}
