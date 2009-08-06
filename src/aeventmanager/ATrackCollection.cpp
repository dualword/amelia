#include "ATrackCollection.h"

#include <QDebug>

#include "ATrackCombination.h"
#include "AEvent.h"

ATrackCollection::ATrackCollection(QString modulename)
 : AEventAnalysisData(modulename)
{ }

ATrackCollection::~ATrackCollection()
{ }

int ATrackCollection::size()
{
  return _tracks.size();
}

void ATrackCollection::addTrack(AEventObject *track)
{
  if(!containsTrack(track))
    {
      _tracks.push_back(track);
      connect(track,SIGNAL(updated()),
	      this,SLOT(handleTrackUpdated()),
	      Qt::DirectConnection);
      emit trackInserted(_tracks.size()-1);
      emit updated();
    }
}

AEventObject* ATrackCollection::getTrack(int idx)
{
  return _tracks[idx];
}

void ATrackCollection::removeTrack(int idx)
{
  disconnect(_tracks.at(idx),SIGNAL(updated()),
	     this,SLOT(handleTrackUpdated()));
  _tracks.removeAt(idx);
  emit trackRemoved(idx);
  emit updated();
}

bool ATrackCollection::containsTrack(AEventObject *track)
{
  return _tracks.contains(track);
}

void ATrackCollection::writeToFile(QTextStream& in)
{
  beginWriteToFile(in);
  
  QListIterator<AEventObject*> tracksIter(_tracks);
  //Loop over tracks..
  while(tracksIter.hasNext())
    {
      AEventObject* track=tracksIter.next();
      writeTrackToXmlFile(in,track);
    }
  
  endWriteToFile(in);
}

void ATrackCollection::loadFromXML(QDomElement analysisElement,AEvent* event)
{
  event->LoadEvent();
  
  QList<AEventObject*> tracks=readTracksFromXmlElement(event,analysisElement);
  for(int i=0;i<tracks.size();i++)
    {
      addTrack(tracks[i]);
    }
}

QList<AEventObject*> ATrackCollection::readTracksFromXmlElement(AEvent* event, const QDomElement& ele)
{
  QList<AEventObject*> tracks;
  QDomNodeList childs=ele.childNodes();
  for(int l=0;l<childs.size();l++)
    {
      QDomElement node=childs.at(l).toElement();
      if(node.tagName()=="track")
	{
	  unsigned int id=node.attribute("id","0").toUInt();
	  qDebug() << "Found track " << id;
	  AEventObject *track=event->getTrackById(id);
	  if(track==0)
	    {
	      qDebug() << "ERROR: Invalid track " << id;
	      continue;
	    }
	  tracks.append(track);
	}
      else if(node.tagName()=="combination")
	{
	  QString name=node.attribute("name","Unknown Combination");
	  ATrackCombination *combo=new ATrackCombination();
	  combo->setName(name);
	  qDebug() << "Found combination " << name;
	  
	  QList<AEventObject*> found=readTracksFromXmlElement(event,node);
	  for(int i=0;i<found.size();i++)
	    combo->addTrack(found[i]);

	  tracks.append(combo);
	}
    }
  
  return tracks;

}

void ATrackCollection::writeTrackToXmlFile(QTextStream& in,AEventObject* track)
{
  if(track->type()==AEventObject::eCombination)
    {
      ATrackCombination* combo=(ATrackCombination*)track;
      in << "<combination"
	 << " name=\"" << combo->name(false) << "\">" << endl;
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

void ATrackCollection::handleTrackUpdated()
{
  emit updated();
}
