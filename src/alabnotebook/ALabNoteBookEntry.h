#ifndef ALABNOTEBOOKENTRY_H_
#define ALABNOTEBOOKENTRY_H_

#include <QObject>
#include <QDateTime>
#include <QString>

class ALabNoteBookEntry : public QObject
{
 public:
  ALabNoteBookEntry(QObject* parent=0);
  ~ALabNoteBookEntry();

  void setTime(QDateTime);
  QDateTime time();
  
  void setText(QString);
  QString text();

 signals:
  void updated();
  
 private:
  QDateTime _time;
  QString _text;

  Q_OBJECT
};

#endif //ALABNOTEBOOKENTRY_H_
