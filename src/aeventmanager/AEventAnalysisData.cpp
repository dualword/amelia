#include "AEventAnalysisData.h"

#include "AEvent.h"
#include <QDebug>

QMap<QString,QMetaObject> AEventAnalysisData::_listOfStructures;

AEventAnalysisData::AEventAnalysisData(QString modulename):_modulename(modulename)
{ }

AEventAnalysisData::~AEventAnalysisData()
{ }

QString AEventAnalysisData::moduleName()
{
  return _modulename;
}

void AEventAnalysisData::writeToFile(QTextStream& in)
{
  beginWriteToFile(in);
  endWriteToFile(in);
}

void AEventAnalysisData::beginWriteToFile(QTextStream& in)
{
  in << "<analysis"
     << " module=\"" << _modulename << "\""
     << " type=\"" << metaObject()->className() << "\""
     << ">" <<endl;
}

void AEventAnalysisData::endWriteToFile(QTextStream& in)
{
  in << "</analysis>"<<endl;
}

void AEventAnalysisData::loadFromXML(QDomElement ele,AEvent *)
{ }

void AEventAnalysisData::addStructure(QMetaObject metaobj)
{
  _listOfStructures[metaobj.className()]=metaobj;
}

AEventAnalysisData* AEventAnalysisData::newInstance(QString classname,QString module)
{
  if(!_listOfStructures.contains(classname))
    return 0;
  
  QObject* obj=_listOfStructures[classname].newInstance(Q_ARG(QString,module));
  return (AEventAnalysisData*)obj;
}
