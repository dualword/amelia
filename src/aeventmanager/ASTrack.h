#ifndef ASTRACK_H_
#define ASTRACK_H_

#include "ATrack.h"

//Objects of this class represent simulated tracks in the event (STr)
class AEVENTMANAGER_EXPORT ASTrack : public ATrack
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
  bool isNeutralHadron();
  bool isChargedHadron();

  float getTl();

  bool isInteresting();

 private:
  static const int codelist[];
  static const int chargelist[];
  static const QString namelist[];

  int code;
};
#endif //ASTRACK_H_
