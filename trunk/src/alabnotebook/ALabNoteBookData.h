#ifndef ALABNOTEBOOKDATA_H_
#define ALABNOTEBOOKDATA_H_

#include <aeventmanager/AEventAnalysisData.h>

#include "ALabNoteBookEntry.h"

class ALabNoteBookData : public AEventAnalysisData
{
 public:
  Q_INVOKABLE ALabNoteBookData(QString modulename);
  ~ALabNoteBookData();

  QList<ALabNoteBookEntry*> entries();
  void addEntry(ALabNoteBookEntry*);

  void writeToFile(QTextStream& in);
  void loadFromXML(QDomElement ele, class AEvent* e);
  
 public slots:
  void handleEntryUpdate();

 private:
  QList<ALabNoteBookEntry*> _entries;

  Q_OBJECT
};

#endif //ALABNOTEBOOKDATA_H_
