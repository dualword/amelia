#include "AEventAnalysisData.h"
#include <QDebug>

AEventAnalysisData::AEventAnalysisData(QString modulename):_modulename(modulename)
{ }

AEventAnalysisData::~AEventAnalysisData()
{ }

QList<QString> AEventAnalysisData::listCollections()
{
  return _collections.keys();
}

QList<ATrack*> AEventAnalysisData::getCollection(QString name)
{
  if(!_collections.contains(name))
    {
      _collections[name]=QList<ATrack*>();
    }
  
  return _collections[name];
}

void AEventAnalysisData::setCollection(QString name,QList<ATrack*> collection)
{
  _collections[name]=collection;
  emit updated();  
}
