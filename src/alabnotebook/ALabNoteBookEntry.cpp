#include "ALabNoteBookEntry.h"

ALabNoteBookEntry::ALabNoteBookEntry(QObject* parent)
  :QObject(parent)
{
  _time=QDateTime::currentDateTime();
}

ALabNoteBookEntry::~ALabNoteBookEntry()
{ }

void ALabNoteBookEntry::setText(QString text)
{
  _text=text;
  emit updated();
}

QString ALabNoteBookEntry::text()
{
  return _text;
}

void ALabNoteBookEntry::setTime(QDateTime time)
{
  _time=time;
  emit updated();
}

QDateTime ALabNoteBookEntry::time()
{
  return _time;
}
