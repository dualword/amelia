#include "AEventPackage.h"

#include <aeventmanager/ATrackCombination.h>
#include <aeventmanager/AXmlEvent.h>

#include <QFile>
#include <QDebug>
#include <QtXml>
#include <QSetIterator>

AEventPackage::AEventPackage(const QString &location,QObject *parent)
  :QObject(parent),_location(location),_loaded(false)
{
  QDir dir(_location);
  _name=dir.dirName();
  loadMetaInfo();
}

AEventPackage::~AEventPackage()
{
}

void AEventPackage::load()
{
  QDir dir(_location);
    QStringList elist=dir.entryList();
    for (int i=0;i<elist.size();i++)
    {
        int idx=elist[i].lastIndexOf(".xml");
        if (idx==elist[i].length()-4 && idx>=0)
        {
	  AEvent *event=new AXmlEvent(_location+"/"+elist[i]);
	  event->setPackage(this);
	  event->filename=elist[i];
	  event->LoadEvent();

	  events.append(event);
        }
    }
  
  loadLogBook();

  _loaded=true;
}

QString AEventPackage::location()
{
  return _location;
}

void AEventPackage::loadMetaInfo()
{
  /*
   * Load the metainfo
   */
  QFile fh(_location+"/.metainfo");
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
  if (!nameText.isEmpty()) _name=nameText;
}

void AEventPackage::loadLogBook()
{
  /*
   * Load the already analyzed events
   */
  QFile fh(_location+"/.logbook");
  QDomDocument doc("logbooks");
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
		  QString type=analysisElement.attribute("type");
		  if(moduleName.isEmpty())
		    {
		      qDebug() << "ERROR: Empty module name!";
		      continue;
		    }
		  
		  if(type.isEmpty())
		    {
		      qDebug() << "ERROR: Empty analysis data type!";
		      continue;
		    }
		  
		  
		  AEventAnalysisData *data=AEventAnalysisData::newInstance(type,moduleName);
		  if(data==0)
		    {
		      qDebug() << "ERROR: Invalid analysis data type!";
		      continue;
		    }
		  
		  data->loadFromXML(analysisElement,events[j]);
		  
		  events[j]->addAnalysisData(moduleName,data);
		} //END load analysis nodes
            }
        }
    }
}

bool AEventPackage::isLoaded()
{
  return _loaded;
}

void AEventPackage::save()
{
    QFile metafile(_location+"/.metainfo");
    metafile.open(QIODevice::WriteOnly);
    QTextStream in(&metafile);
    in << "<?xml version=\"0.1\"?>" << endl;
    in << "<package>" << endl;
    if (!name().isEmpty()) in << "<name>" << name() << "</name>" << endl;
    in << "</package>";
    metafile.close();

    QFile logbook(_location+"/.logbook");
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
	QList<AEventAnalysisData*> modules=e->allAnalysisData();
	QListIterator<AEventAnalysisData*> moduleIter(modules);
	//Loop over modules
	while (moduleIter.hasNext())
	  {
	    AEventAnalysisData* data=moduleIter.next();
	    data->writeToFile(in);
	  }
	

	in << "\t</event>" << endl;
    }
    in << "</events>" << endl;

    logbook.close();
}

void AEventPackage::setName(QString name)
{
    _name=name;
    save();
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

void AEventPackage::addEvent(AEvent *e)
{
  if(indexOf(e)>=0) return;
  e->setPackage(this);
  events+=e;
  save();
}

void AEventPackage::removeEvent(AEvent *e)
{
  int index = indexOf(e);
  if(index>=0)
    events.removeAt(index);
  e->setPackage(0);
  save();
}

AEvent* AEventPackage::event(int idx)
{
    return events[idx];
}
