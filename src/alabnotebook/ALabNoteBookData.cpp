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
	  this,SLOT(handleEntryUpdate()));

  emit updated();
  for(int i=0;i<_entries.size();i++)
    {
      if(newEntry->time()>_entries[i]->time())
	{
	  _entries.insert(i,newEntry);
	  return;
	}
    }
  //Not inserted because everything come before..
  _entries.append(newEntry);
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
      in << entry->text();
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
      QString text=entryElement.text().trimmed();
      QString timeString=entryElement.attribute("time");
      
      ALabNoteBookEntry *entry=new ALabNoteBookEntry();
      entry->setTime(QDateTime::fromString(timeString,"yyyyMMddHHmmss"));
      entry->setText(text);

      addEntry(entry);
    }
}

