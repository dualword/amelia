#include "ATourBookmarkAction.h"

#include <QDebug>

#include <irrlicht.h>
using namespace irr;

#include <AMELIA.h>

#include <ageometry/AGeoPlugin.h>

ATourBookmarkAction::ATourBookmarkAction()
  :ATourAction()
{ 
  AGeoPlugin *geoplugin=(AGeoPlugin*)AMELIA::global->plugin("AGeometry");
  layerGUI=geoplugin->findWidget<ALayerGUI*>("LayerGUI");
}

Q_INVOKABLE ATourBookmarkAction::ATourBookmarkAction(QString msg)
{ 
  AGeoPlugin *geoplugin=(AGeoPlugin*)AMELIA::global->plugin("AGeometry");
  layerGUI=geoplugin->findWidget<ALayerGUI*>("LayerGUI");
}

QString ATourBookmarkAction::type()
{
  return ATourAction::type()+"_"+QString(trackid);
}

QPoint ATourBookmarkAction::cursor()
{
  return QPoint();
}

void ATourBookmarkAction::loadFromXML(QDomElement actionElement)
{
  ATourAction::loadFromXML(actionElement);

  trackid=actionElement.attribute("id").toInt();
  state=(actionElement.attribute("state","on")=="on")?true:false;
}

void ATourBookmarkAction::act()
{
  if(state)
    addToBookmarks();
  else
    removeFromBookmarks();
}

void ATourBookmarkAction::undo()
{ 
  if(state)
    removeFromBookmarks();
  else
    addToBookmarks();
}

void ATourBookmarkAction::removeFromBookmarks()
{
  AEventObject *object=objectOfInterest();
  AEventObjectCollection *collection=bookmarkCollection();

  if(!object || !collection) return;
  
  int id=collection->find(object);
  if(id>0)
    collection->remove(id);
}

void ATourBookmarkAction::addToBookmarks()
{
  AEventObject *object=objectOfInterest();
  AEventObjectCollection *collection=bookmarkCollection();
  if(collection && object)
      collection->add(object);
}

AEventObjectCollection* ATourBookmarkAction::bookmarkCollection()
{
  // We use the complete event, because the wanted track might be filetered out..
  AEvent *event=layerGUI->completeEvent();

  if(event)
    {
      modifiedEvents.insert(event);
      return event->getAnalysisData<AEventObjectCollection>("ATourBookmarks");
    }
  else
    return 0;
}

AEventObject* ATourBookmarkAction::objectOfInterest()
{
  // We use the complete event, because the wanted track might be filetered out..
  AEvent *event=layerGUI->completeEvent();
  
  if(event)
    return event->getObjectById(trackid);
  else
    return 0;
}

void ATourBookmarkAction::prepare()
{ 
  // Make sure the collection is empty at the beggining
  AEventObjectCollection *collection=bookmarkCollection();
  if(collection)
    collection->clear();

  // Set to the tour bookmark list
  ATrackTreeModel *bookmarkModel=layerGUI->bookmarkModel();
  bookmarkModel->setCollectionName("ATourBookmarks");
}

void ATourBookmarkAction::cleanup()
{
  QSet<AEvent *> allModifiedEvents;

  // Get list of all modified events by this chain
  ATourBookmarkAction *action=this;
  while(action)
    {
      allModifiedEvents+=action->modifiedEvents;
      action->modifiedEvents.clear(); // Don't forget to reset for the next run!

      action=qobject_cast<ATourBookmarkAction*>(action->nextAction());
    }

  // Loop over the modified events and clear them!
  QSet<AEvent*>::iterator iter=allModifiedEvents.begin();
  QSet<AEvent*>::iterator iterE=allModifiedEvents.end();
  for(;iter!=iterE;iter++)
    {
      AEvent *event=*iter;
      AEventObjectCollection *tourcoll=event->getAnalysisData<AEventObjectCollection>("ATourBookmarks");
      AEventObjectCollection *geocoll=event->getAnalysisData<AEventObjectCollection>("AGeometry");
      tourcoll->clear();
    }

  ATrackTreeModel *bookmarkModel=layerGUI->bookmarkModel();
  bookmarkModel->setCollectionName("AGeometry");  
}
