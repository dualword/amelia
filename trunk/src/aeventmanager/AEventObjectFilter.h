#ifndef AEVENTOBJECTFILTER_H_
#define AEVENTOBJECTFILTER_H_

#include "AEventObject.h"

#include <QObject>

class AEventObjectFilter : public QObject
{
 Q_OBJECT

 private:
  AEventObjectFilter *_nextFilter;

 public:
  AEventObjectFilter(AEventObjectFilter *next=0);
  virtual bool check(AEventObject* object) =0;

  AEventObjectFilter* nextFilter();

 signals:
  void filterUpdated();
};

#endif //ATRACKFILTER_H_
