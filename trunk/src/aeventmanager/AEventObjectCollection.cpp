#include "AEventObjectCollection.h"

#include <QDebug>

#include "ATrackCombination.h"
#include "AEvent.h"

AEventObjectCollection::AEventObjectCollection(QString modulename)
 : AEventAnalysisData(modulename)
{ }

AEventObjectCollection::~AEventObjectCollection()
{ }

int AEventObjectCollection::size()
{
  return _objects.size();
}

void AEventObjectCollection::add(AEventObject *object)
{
  if(!contains(object)) // No duplicates!
    {
      _objects.push_back(object);
      connect(object,SIGNAL(updated()),
	      this,SLOT(handleUpdates()),
	      Qt::DirectConnection);

      emit updated(); // Resave..
    }
}

AEventObject* AEventObjectCollection::get(int idx)
{
  return _objects[idx];
}

void AEventObjectCollection::remove(int idx)
{
  disconnect(_objects.at(idx),SIGNAL(updated()),
	     this,SLOT(handleUpdates()));
  _objects.removeAt(idx);

  emit updated();
}

bool AEventObjectCollection::contains(AEventObject *track)
{
  return _objects.contains(track);
}

void AEventObjectCollection::writeToFile(QTextStream& in)
{
  beginWriteToFile(in);
  
  QListIterator<AEventObject*> objectsIter(_objects);
  //Loop over objects..
  while(objectsIter.hasNext())
    {
      AEventObject* object=objectsIter.next();
      writeObjectToXmlFile(in,object);
    }
  
  endWriteToFile(in);
}

void AEventObjectCollection::loadFromXML(QDomElement analysisElement,AEvent* event)
{
  event->LoadEvent(); //Make sure event is loaded
  
  QList<AEventObject*> objects=readObjectsFromXmlElement(event,analysisElement); //Recursive parse
  for(int i=0;i<objects.size();i++)
    {
      add(objects[i]);
    }
}

QList<AEventObject*> AEventObjectCollection::readObjectsFromXmlElement(AEvent* event, const QDomElement& ele)
{
  QList<AEventObject*> objects;
  QDomNodeList childs=ele.childNodes();
  for(int l=0;l<childs.size();l++)
    {
      QDomElement node=childs.at(l).toElement();
      if(node.tagName()=="track")
	{
	  unsigned int id=node.attribute("id","0").toUInt();
	  qDebug() << "Found track " << id;
	  AEventObject *track=event->getObjectById(id);
	  if(track==0)
	    {
	      qDebug() << "ERROR: Invalid track " << id;
	      continue;
	    }
	  objects.append(track);
	}
      else if(node.tagName()=="combination")
	{
	  QString name=node.attribute("name","Unknown Combination");
	  ATrackCombination *combo=new ATrackCombination();
	  combo->setName(name);
	  qDebug() << "Found combination " << name;
	  
	  // Combinations need to be recused so their constituent tracks are loaded
	  QList<AEventObject*> found=readObjectsFromXmlElement(event,node);
	  for(int i=0;i<found.size();i++)
	    combo->addTrack(found[i]);

	  objects.append(combo);
	}
    }
  
  return objects;
}

void AEventObjectCollection::writeObjectToXmlFile(QTextStream& in,AEventObject* object)
{
  if(object->type()==AEventObject::eCombination)
    {
      ATrackCombination* combo=(ATrackCombination*)object;
      in << "<combination"
	 << " name=\"" << combo->name(false) << "\">" << endl;
      for(int i=0;i<combo->size();i++)
	{
	  writeObjectToXmlFile(in,combo->getTrack(i));
	}
      in << "</combination>"<<endl;
      
    }
  else
    {
      in << "<track"
	 << " id=\"" << object->ID() << "\"/>"<<endl;
    }
}

void AEventObjectCollection::handleUpdates()
{
  emit updated();
}
