#include "AEventPackage.h"

#include <QFile>
#include <QDebug>
#include <QtXml>
#include <QSetIterator>

AEventPackage::AEventPackage(QObject *parent):QObject(parent)
{
}

AEventPackage::~AEventPackage()
{
}

void AEventPackage::load(const QString& loc)
{
    location=loc;
    QDir dir(loc);
    QStringList elist=dir.entryList();
    for (int i=0;i<elist.size();i++)
    {
        int idx=elist[i].lastIndexOf(".xml");
        if (idx==elist[i].length()-4 && idx>=0)
        {
	  AEvent *event=new AXmlEvent(loc+"/"+elist[i]);
	  event->package=this;
	  event->filename=elist[i];
	  event->LoadEvent();
	  
	  events.append(event);
        }
    }

    setName(dir.dirName());

    /*
     * Load the metainfo
     */
    QFile fh(loc+"/.metainfo");
    QDomDocument doc("metainfo");
    if (!fh.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&fh))
    {
        fh.close();
        return;
    }
    fh.close();

    QDomNode package=doc.namedItem("package");

    //Check for names
    QDomNode nameNode=package.namedItem("name");
    QString nameText=nameNode.firstChild().toText().data();
    if (!nameText.isEmpty()) setName(nameText);

    /* END metainfo loading */

    /*
     * Load the already analyzed events
     */
    //QFile fh_logbook(loc+"/.logbook");
    fh.setFileName(loc+"/.logbook");
    doc=QDomDocument("logbooks");
    if (!fh.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&fh))
    {
        fh.close();
        return;
    }
    fh.close();

    //Load already analyzed events
    QDomNode eventsNode=doc.namedItem("events");
    QDomNodeList eventNodes=eventsNode.childNodes();
    for (int i=0;i<eventNodes.size();i++)
    {
        QDomNode eventNode=eventNodes.at(i);
	QDomElement event=eventNode.toElement();
        QString filename=event.attributes().namedItem("filename").toAttr().value();
        for (int j=0;j<events.size();j++)
        {
            if (events[j]->filename==filename)
            {
                qDebug() << "Filename " << events[j]->filename;

		// Load tags
                QDomNode tagsNode=event.firstChildElement("tags");
                QDomNodeList tagNodes=tagsNode.childNodes();
                for (int i=0;i<tagNodes.size();i++)
                {
		  QString tag=tagNodes.at(i).firstChild().toText().data();
		  qDebug() << "Tag " <<  tag;
		  events[j]->tags.insert(tag);
                }

		// Load analysis data
		QDomNodeList analysisNodes=event.elementsByTagName("analysis");
		for(int i=0;i<analysisNodes.size();i++)
		  { //START load analysis nodes
		    QDomElement analysisElement=analysisNodes.at(i).toElement();
		    QString moduleName=analysisElement.attribute("module");
		    if(moduleName.isEmpty())
		      {
			qDebug() << "ERROR: Empty module name!";
			continue;
		      }
		    
		    qDebug() << "Found analysis node " << moduleName;
		    AEventAnalysisData *data=new AEventAnalysisData(moduleName);
		    
		    QDomNodeList collectionNodes=analysisElement.elementsByTagName("collection");
		    for(int k=0;k<collectionNodes.size();k++)
		      { //START load collections
			QDomElement collectionElement=collectionNodes.at(k).toElement();
			QString collectionName=collectionElement.attribute("name");
			if(moduleName.isEmpty())
			  {
			    qDebug() << "ERROR: Empty collection name in module \""<< moduleName <<"\"!";
			    continue;
			  }

			qDebug() << "\tFound collection node " << collectionName;
			
			QList<ATrack*> collection=readTracksFromXmlElement(events[j],collectionElement,2);
						

			data->setCollection(collectionName,collection);
		      } //END load collections

		    events[j]->addAnalysisData(moduleName,data);
		  } //END load analysis nodes
            }
        }
    }

}

QList<ATrack*> AEventPackage::readTracksFromXmlElement(AEvent* event,const QDomElement& ele,int level)
{
  QString tabs="";
  for(int i=0;i<level;i++)
    tabs+="\t";

  QList<ATrack*> tracks;
  QDomNodeList childs=ele.childNodes();
  for(int l=0;l<childs.size();l++)
    {
      QDomElement node=childs.at(l).toElement();
      if(node.tagName()=="track")
	{
	  unsigned int id=node.attribute("id","0").toUInt();
	  qDebug() << tabs << "Found track " << id;
	  ATrack *track=event->getTrackById(id);
	  if(track==0)
	    {
	      qDebug() << tabs << "ERROR: Invalid track " << id;
	      continue;
	    }
	  tracks.append(track);
	}
      if(node.tagName()=="combination")
	{
	  QString name=node.attribute("name","Unknown Combination");
	  ATrackCombination *combo=new ATrackCombination();
	  combo->setName(name);
	  qDebug() << tabs << "Found combination " << name;
	  
	  QList<ATrack*> found=readTracksFromXmlElement(event,node,level+1);
	  for(int i=0;i<found.size();i++)
	    combo->addTrack(found[i]);

	  tracks.append(combo);
	}
    }
  
  return tracks;
}


void AEventPackage::save()
{
    QFile metafile(location+"/.metainfo");
    metafile.open(QIODevice::WriteOnly);
    QTextStream in(&metafile);
    in << "<?xml version=\"0.1\"?>" << endl;
    in << "<package>" << endl;
    if (!name().isEmpty()) in << "<name>" << name() << "</name>" << endl;
    in << "</package>";
    metafile.close();

    QFile logbook(location+"/.logbook");
    logbook.open(QIODevice::WriteOnly);
    in.setDevice(&logbook);
    in << "<?xml version=\"0.1\"?>" << endl;
    in << "<events>" << endl;

    for (int i=0;i<events.size();i++)
    {
        AEvent *e=events[i];

	in << "\t<event filename=\"" << e->filename << "\">" << endl;
	// Save tags!!
	if (e->tags.size()>0)
	  {
	    in << "\t\t<tags>"<<endl;
	    QSetIterator<QString> iter(e->tags);
	    while (iter.hasNext())
	      {
		in << "\t\t\t<tag>"<<iter.next()<<"</tag>"<<endl;
	      }
	    in << "\t\t</tags>"<<endl;
	  }

	// Save the analysis!!
	QList<QString> modules=e->listAnalysisData();
	QListIterator<QString> moduleIter(modules);
	//Loop over modules
	while (moduleIter.hasNext())
	  {
	    QString moduleName=moduleIter.next();
	    in << "\t\t<analysis module=\"" << moduleName << "\">"<<endl;
	    
	    AEventAnalysisData *data=e->getAnalysisData(moduleName);
	    QList<QString> collections=data->listCollections();
	    QListIterator<QString> collectionIter(collections);
	    
	    //Loop over collections
	    while (collectionIter.hasNext())
	      {
		QString collectionName=collectionIter.next();
		in << "\t\t\t<collection name=\"" << collectionName << "\">"<<endl;
		
		QList<ATrack*> tracks=data->getCollection(collectionName);
		QListIterator<ATrack*> tracksIter(tracks);
		//Loop over tracks..
		while(tracksIter.hasNext())
		  {
		    ATrack* track=tracksIter.next();
		    writeTrackToXmlFile(in,track,4);
		  }
		
		in << "\t\t\t</collection>"<<endl;
	      }
	    in << "\t\t</analysis>"<<endl;
	  }
	

	in << "\t</event>" << endl;
    }
    in << "</events>" << endl;

    logbook.close();
}

void AEventPackage::writeTrackToXmlFile(QTextStream& in,ATrack* track,int level)
{
  QString tabs="";
  for(int i=0;i<level;i++)
    tabs+="\t";
  
  if(track->type()==ATrack::eCombination)
    {
      ATrackCombination* combo=(ATrackCombination*)track;
      in << tabs << "<combination"
	 << " name=\"" << combo->name() << "\">" << endl;
      for(int i=0;i<combo->size();i++)
	{
	  writeTrackToXmlFile(in,combo->getTrack(i),level+1);
	}
      in << tabs << "</combination>"<<endl;
      
    }
  else
    {
      in << tabs << "<track"
	 << " id=\"" << track->trackID() << "\"/>"<<endl;
    }
  
}

void AEventPackage::setName(QString name)
{
    _name=name;
}

QString AEventPackage::name()
{
    return _name;
}

int AEventPackage::eventCount()
{
    return events.size();
}

int AEventPackage::indexOf(AEvent* e)
{
  return events.indexOf(e);
}

AEvent* AEventPackage::event(int idx)
{
    return events[idx];
}
