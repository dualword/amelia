#ifndef AFCALSHOWER_H_
#define AFCALSHOWER_H_

#include "AEventObject.h"

class AEVENTMANAGER_EXPORT AFCALShower : public AEventObject
{
 public:
  AFCALShower();

  float dx;
  float dy;
  float energy;
  int layer;
  int sub;
  float x;
  float y;
};

#endif //AFCALSHOWER_H_
