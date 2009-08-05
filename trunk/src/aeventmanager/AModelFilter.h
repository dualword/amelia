#ifndef AMODELFILTER_H_
#define AMODELFILTER_H_

#include "ATrackFilter.h"
#include "AJet.h"
#include "AMisET.h"

class AEVENTMANAGER_EXPORT AModelFilter : public ATrackFilter
{
Q_OBJECT

 private:
  QString _jetType;
  QString _misEtType;
  QString _trackType;
  
 public:
  AModelFilter(QString jetType,QString misEtType,QString trackType,
	       ATrackFilter *next=0);
  
  QString jetType();
  QString misEtType();
  QString trackType();
  
  bool checkTrack(ATrack* track);
				
 public slots:
  void setJetType(QString);
  void setMisEtType(QString);
  void setTrackType(QString);
};

#endif //AMODELFILTER_H_
