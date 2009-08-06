#ifndef AEVENTANALYSISDATA_H_
#define AEVENTANALYSISDATA_H_

#include <QList>
#include <QMap>
#include <QString>
#include <QObject>
#include <QTextStream>
#include <QDomElement>

#include "AEventManagerDefines.h"

class AEVENTMANAGER_EXPORT AEventAnalysisData : public QObject
{
 public:
  AEventAnalysisData(QString modulename);
  ~AEventAnalysisData();

  QString moduleName();

  virtual void writeToFile(QTextStream& in);
  virtual void loadFromXML(QDomElement ele,class AEvent* e);

  static void addStructure(QMetaObject);
  static AEventAnalysisData* newInstance(QString classname,QString module);

 public slots:
  void forceUpdate();

 signals:
  void updated();

 protected:
  void beginWriteToFile(QTextStream& in);
  void endWriteToFile(QTextStream& in);

 private:
  QString _modulename;

  static QMap<QString,QMetaObject> _listOfStructures;

  Q_OBJECT
};

#endif //AEVENTANALYSISDATA_H_
