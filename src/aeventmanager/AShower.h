#ifndef ASHOWER_H_
#define ASHOWER_H_

#include "ATrack.h"

#include <QString>

class AShower : public ATrack
{
 public:
  AShower();

  float energy;
  float eta;
  int layer;
  float phi;
  int sub;
  
  QString calometer;
};

#endif //ASHOWER_H_
