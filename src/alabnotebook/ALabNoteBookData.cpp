#include "ALabNoteBookData.h"
#include <QDebug>

ALabNoteBookData::ALabNoteBookData(QString moduleName):AEventAnalysisData(moduleName)
{ }

ALabNoteBookData::~ALabNoteBookData()
{ }

QList<ALabNoteBookEntry*> ALabNoteBookData::entries()
{
  return _entries;
}

void ALabNoteBookData::addEntry(ALabNoteBookEntry* newEntry)
{
  connect(newEntry,SIGNAL(updated()),
	  this,SLOT(handleEntryUpdate()),
		Qt::DirectConnection);

  bool inserted=false;
  for(int i=0;i<_entries.size();i++)
    {
      if(newEntry->time()>_entries[i]->time())
	{
	  _entries.insert(i,newEntry);
	  inserted=true;
	  break;
	}
    }
  //Not inserted because everything come before..
  if(!inserted)
    _entries.append(newEntry);

  emit updated();
}

void ALabNoteBookData::handleEntryUpdate()
{
  emit updated();
}

void ALabNoteBookData::writeToFile(QTextStream& in)
{
  beginWriteToFile(in);
  
  //Loop over collections
  for(unsigned int i=0;i<_entries.size();i++)
    {
      ALabNoteBookEntry* entry=_entries[i];
      in << "<entry"
	 << " time=\"" << entry->time().toString("yyyyMMddHHmmss") << "\""
	 << ">" <<endl;
      in << "<![CDATA[" << endl;
      // Replace some entities, so the user won't be able to corrupt a 
      // file by putting in ]]>. The <![CDATA[ ]]> will take care of the rest.
      in << entry->text().replace("&","&amp;").replace("<","&lt;").replace(">","&gt;");
      in << "]]>" << endl;
      in << "</entry>"<<endl;
    }

  endWriteToFile(in);
}

void ALabNoteBookData::loadFromXML(QDomElement element,AEvent* event)
{
  QDomNodeList entryNodes=element.elementsByTagName("entry");
  for(int k=0;k<entryNodes.size();k++)
    {
      QDomElement entryElement=entryNodes.at(k).toElement();
      QString text=entryElement.text().trimmed().replace("&lt;","<").replace("&gt;",">").replace("&amp;","&");
      QString timeString=entryElement.attribute("time");
      
      ALabNoteBookEntry *entry=new ALabNoteBookEntry();
      entry->setTime(QDateTime::fromString(timeString,"yyyyMMddHHmmss"));
      entry->setText(text);

      addEntry(entry);
    }
}

