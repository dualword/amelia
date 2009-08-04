#ifndef AMODELFILTER_H_
#define AMODELFILTER_H_

#include "ATrackFilter.h"
#include "AJet.h"
#include "AMisET.h"

class AEVENTMANAGER_EXPORT AModelFilter : public ATrackFilter
{
  Q_OBJECT
    private:
  AJet::jType _jetType;
  AMisET::mType _misEtType;
  ATrack::eType _trackType;
  
 public:
  AModelFilter(AJet::jType jetType,AMisET::mType misEtType,ATrack::eType _trackType,
	       ATrackFilter *next=0);

  AJet::jType jetType();
  QString jetTypeString();

  AMisET::mType misEtType();
  QString misEtTypeString();

  ATrack::eType trackType();
  QString trackTypeString();

  bool checkTrack(ATrack* track);

 public slots:
  void setJetType(AJet::jType);
  void setJetType(QString);

  void setMisEtType(AMisET::mType);
  void setMisEtType(QString);

  void setTrackType(ATrack::eType);
  void setTrackType(QString);
};

#endif //AMODELFILTER_H_
