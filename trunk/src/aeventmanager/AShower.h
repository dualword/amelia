#ifndef ASHOWER_H_
#define ASHOWER_H_

#include "AEventObject.h"

#include <QString>

class AShower : public AEventObject
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
