#include "ATrackCollection.h"

#include <QDebug>

#include "ATrackCombination.h"
#include "AEvent.h"

ATrackCollection::ATrackCollection(QString modulename):AEventAnalysisData(modulename)
{ }

ATrackCollection::~ATrackCollection()
{ }

QList<QString> ATrackCollection::listCollections()
{
  return _collections.keys();
}

QList<ATrack*> ATrackCollection::getCollection(QString name)
{
  if(!_collections.contains(name))
    {
      _collections[name]=QList<ATrack*>();
    }
  
  return _collections[name];
}

void ATrackCollection::setCollection(QString name,QList<ATrack*> collection)
{
  _collections[name]=collection;
  emit updated();  
}

void ATrackCollection::writeToFile(QTextStream& in)
{
  beginWriteToFile(in);
  
  QListIterator<QString> collectionIter(_collections.keys());
  //Loop over collections
  while (collectionIter.hasNext())
    {
      QString collectionName=collectionIter.next();
      in << "<collection name=\"" << collectionName << "\">"<<endl;
      
      QList<ATrack*> tracks=getCollection(collectionName);
      QListIterator<ATrack*> tracksIter(tracks);
      //Loop over tracks..
      while(tracksIter.hasNext())
	{
	  ATrack* track=tracksIter.next();
	  writeTrackToXmlFile(in,track);
	}
      
      in << "</collection>"<<endl;
    }
  
  endWriteToFile(in);
}

void ATrackCollection::loadFromXML(QDomElement analysisElement,AEvent* event)
{
  event->LoadEvent();
  QDomNodeList collectionNodes=analysisElement.elementsByTagName("collection");
  for(int k=0;k<collectionNodes.size();k++)
    { //START load collections
      QDomElement collectionElement=collectionNodes.at(k).toElement();
      QString collectionName=collectionElement.attribute("name");
      if(moduleName().isEmpty())
	{
	  qDebug() << "ERROR: Empty collection name in module \""<< moduleName() <<"\"!";
	  continue;
	}
      
      qDebug() << "Found collection node " << collectionName;
      
      QList<ATrack*> collection=readTracksFromXmlElement(event,collectionElement);
      
      
      setCollection(collectionName,collection);
    } //END load collections
}

QList<ATrack*> ATrackCollection::readTracksFromXmlElement(AEvent* event, const QDomElement& ele)
{
  QList<ATrack*> tracks;
  QDomNodeList childs=ele.childNodes();
  for(int l=0;l<childs.size();l++)
    {
      QDomElement node=childs.at(l).toElement();
      if(node.tagName()=="track")
	{
	  unsigned int id=node.attribute("id","0").toUInt();
	  qDebug() << "Found track " << id;
	  ATrack *track=event->getTrackById(id);
	  if(track==0)
	    {
	      qDebug() << "ERROR: Invalid track " << id;
	      continue;
	    }
	  tracks.append(track);
	}
      if(node.tagName()=="combination")
	{
	  QString name=node.attribute("name","Unknown Combination");
	  ATrackCombination *combo=new ATrackCombination();
	  combo->setName(name);
	  qDebug() << "Found combination " << name;
	  
	  QList<ATrack*> found=readTracksFromXmlElement(event,node);
	  for(int i=0;i<found.size();i++)
	    combo->addTrack(found[i]);

	  tracks.append(combo);
	}
    }
  
  return tracks;

}

void ATrackCollection::writeTrackToXmlFile(QTextStream& in,ATrack* track)
{
  if(track->type()==ATrack::eCombination)
    {
      ATrackCombination* combo=(ATrackCombination*)track;
      in << "<combination"
	 << " name=\"" << combo->name() << "\">" << endl;
      for(int i=0;i<combo->size();i++)
	{
	  writeTrackToXmlFile(in,combo->getTrack(i));
	}
      in << "</combination>"<<endl;
      
    }
  else
    {
      in << "<track"
	 << " id=\"" << track->trackID() << "\"/>"<<endl;
    }
  

}
