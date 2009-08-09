#ifndef AMODELFILTER_H_
#define AMODELFILTER_H_

#include "AEventObjectFilter.h"
#include "AJet.h"
#include "AMisET.h"

class AEVENTMANAGER_EXPORT AModelFilter : public AEventObjectFilter
{
Q_OBJECT

 private:
  QString _jetType;
  QString _misEtType;
  QString _trackType;
  
 public:
  AModelFilter(QString jetType,QString misEtType,QString trackType,
	       AEventObjectFilter *next=0);
  
  QString jetType();
  QString misEtType();
  QString trackType();
  
  bool check(AEventObject* object);
				
 public slots:
  void setJetType(QString);
  void setMisEtType(QString);
  void setTrackType(QString);
};

#endif //AMODELFILTER_H_
