#ifndef AMISET_H_
#define AMISET_H_

#include "ATrack.h"
#include "AEventManagerDefines.h"

//!Objects of this class represent Missing Energy
class AMisET : public ATrack
{
public:
  AMisET();
  ~AMisET();
  
  float etx;
  float ety;
  float et;
  
  void setMisETType(QString type);
  QString misETType();

private:
  QString _misETType;
};
#endif //AMISET_H_
