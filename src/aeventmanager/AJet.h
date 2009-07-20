#ifndef AJET_H_
#define AJET_H_

#include "ATrack.h"

class AEVENTMANAGER_EXPORT AJet : public ATrack //Objects of this class represent jet in the event
{
public:
  enum jType //Jet type
  {
    jKt4H1TopoJets,
    jCone4H1TopoJets,
    jKt4H1TowerJets,
    jCone4H1TowerJets
  };

  AJet();
  ~AJet() {}

  float eta;
  float phi;
  float et;
  int numCells;
  jType JetType;

  void setJetType(jType type);
  void setJetType(QString type);

  float getTl();
  float theta();
};

#endif //AJET_H_
