#ifndef ARTRACK_H_
#define ARTRACK_H_

#include "ATrack.h"

//Objects of this class represent reconstructedtracks in the event (Track)
class AEVENTMANAGER_EXPORT ARTrack : public ATrack
{
public:
  ARTrack();
  ~ARTrack() {}
  
  
  //basic parameters
  int barcode;
  float chi2;
  int id;
  float cotTheta;
  float d0;
  float pt;
  float phi0;
  float z0;
  
  void setCode(int code);
  int getIndex();
  
  bool isElectron();
  bool isMuon();
  bool isPhoton();
  bool isNeutrino();
  bool isChargedHadron();
  bool isNeutralHadron();
  bool isHadron();
  
  
  bool isInteresting();
  
  std::vector<irr::core::vector3df> polyLine;
  std::vector<irr::core::vector3df> getPolyline();

  void setRTrackType(QString type);
  QString RTrackType();
  
 private:
  static const int codelist[];
  static const int chargelist[];
  static const QString namelist[];
  
  int code;

  QString _RTrackType;
};
#endif //ARTRACK_H_
