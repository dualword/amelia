#ifndef AJET_H_
#define AJET_H_

#include "AEventObject.h"

//! Objects of this class represent jet in the event
class AEVENTMANAGER_EXPORT AJet : public AEventObject
{
public:
  AJet();
  ~AJet() {}

  float eta;
  float phi;
  float et;
  int numCells;
  
  void setJetType(QString type);
  QString jetType();

  float getTl();
  float theta();

 private:
  QString _jetType;
};

#endif //AJET_H_
