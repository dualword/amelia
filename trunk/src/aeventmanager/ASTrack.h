#ifndef ASTRACK_H_
#define ASTRACK_H_

#include "AEventObject.h"

//Objects of this class represent simulated tracks in the event (STr)
class AEVENTMANAGER_EXPORT ASTrack : public AEventObject
{
public:
  ASTrack();
  ~ASTrack() {}

  float eta;
  float phi;
  float phiVertex;
  float rhoVertex;
  float zVertex;
  float tL;
  float Mlv;

  float maxAngle;

  void setCode(int code);
  int getIndex();

  bool isElectron();
  bool isMuon();
  bool isPhoton();
  bool isNeutrino();
  bool isChargedHadron();
  bool isNeutralHadron();
  bool isHadron();

  float getTl();

  float theta();

  bool isInteresting();

 private:
  static const int codelist[];
  static const int chargelist[];
  static const QString namelist[];

  int code;
};
#endif //ASTRACK_H_
