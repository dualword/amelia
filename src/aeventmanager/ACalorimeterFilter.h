#ifndef ACALORIMETERFILTER_H_
#define ACALORIMETERFILTER_H_

#include "AEventObjectFilter.h"
#include "AJet.h"
#include "AMisET.h"

class AEVENTMANAGER_EXPORT ACalorimeterFilter : public AEventObjectFilter
{
 public:
  ACalorimeterFilter(AEventObjectFilter *next=0);
  
  bool check(AEventObject* object);
  
  Q_OBJECT
};

#endif //ACALORIMETERFILTER_H_
