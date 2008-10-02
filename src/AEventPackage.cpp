#include "AEventPackage.h"

#include <QFile>
#include <QDebug>
#include <QtXml>
#include <QSetIterator>

AEventPackage::AEventPackage()
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
  for(int i=0;i<elist.size();i++)
    {
      int idx=elist[i].lastIndexOf(".xml");
      if(idx==elist[i].length()-4 && idx>=0)
	{
	  Aevent *event=new Aevent;
	  event->filename=elist[i];
	  event->location=loc+"/"+elist[i];
	  event->read=false;
	  
	  events.append(event);
	}
    }
  
  setName(dir.dirName());


  QString metainfo=loc+"/.metainfo";

  QFile fh(loc+"/.metainfo");

  QDomDocument doc("metainfo");
  if (!fh.open(QIODevice::ReadOnly))
    return;
  if (!doc.setContent(&fh)) {
    fh.close();
    return;
  }
  fh.close();

  QDomNode package=doc.namedItem("package");

  //Check for names
  QDomNode nameNode=package.namedItem("name");
  QString nameText=nameNode.firstChild().toText().data();
  if(!nameText.isEmpty()) setName(nameText);

  //Load already analyzed events
  QDomNode analyzedNode=package.namedItem("analyzed");
  QDomNodeList eventNodes=analyzedNode.childNodes();
  for(int i=0;i<eventNodes.size();i++)
    {
      QDomNode event=eventNodes.at(i);
      QString filename=event.attributes().namedItem("filename").toAttr().value();
      for(int j=0;j<events.size();j++)
	{
	  if(events[j]->filename==filename)
	    {
	      events[j]->read=true;

	      qDebug() << "Filename " << events[j]->filename;

	      QDomNode tagsNode=event.firstChildElement("tags");
	      QDomNodeList tagNodes=tagsNode.childNodes();
	      for(int i=0;i<tagNodes.size();i++)
		{
		  QString tag=tagNodes.at(i).firstChild().toText().data();
		  qDebug() << "Tag " <<  tag;
		  events[j]->tags.insert(tag);
		}
	    }
	}
    }

}

void AEventPackage::save()
{
  QFile metafile(location+"/.metainfo");
  metafile.open(QIODevice::WriteOnly);
  QTextStream in(&metafile);
  in << "<?xml version=\"0.1\"?>" << endl;
  in << "<package>" << endl;
  if(!name().isEmpty()) in << "<name>" << name() << "</name>" << endl;
  in << "<analyzed>" << endl;

  for(int i=0;i<events.size();i++)
    {
      Aevent *e=events[i];
      
      if(e->read)
	{
	  in << "\t<event filename=\"" << e->filename << "\">" << endl;
	  if(e->tags.size()>0)
	    {
	      in << "\t\t<tags>"<<endl;
	      QSetIterator<QString> iter(e->tags);
	      while (iter.hasNext())
		{
		  in << "\t\t\t<tag>"<<iter.next()<<"</tag>"<<endl;
		}
	     in << "\t\t</tags>"<<endl;
}
	  in << "\t</event>" << endl;
	}
    }
  in << "</analyzed>" << endl;
  in << "</package>" << endl;
  metafile.close();
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

Aevent* AEventPackage::event(int idx)
{
  return events[idx];
}
