#ifndef ATOURPTCUTACTION_H_
#define ATOURPTCUTACTION_H_

#include "ATourAction.h"

#include <aeventmanager/APtFilter.h>

class ATourPtCutAction : public ATourAction
{
public:
  Q_INVOKABLE ATourPtCutAction();

  void loadFromXML(QDomElement actionElement);

  void update(double done);

  void prepare();

 private:
  APtFilter *ptFilter;

  double value;

  Q_OBJECT
};

#endif // ATOURPTCUTACTION_H_
