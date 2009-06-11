#ifndef ATOURMONITORACTION_H_
#define ATOURMONITORACTION_H_

#include "ATourAction.h"

#include <abase/ABase.h>

#include <AMELIA.h>

class ATourMonitorAction : public ATourAction
{
public:
  Q_INVOKABLE ATourMonitorAction();

  void loadFromXML(QDomElement actionElement);
  
  void doAction();
  void undoAction();

private:
  QString monitor;
  QString monitorInitial;

  ABase *base;

  Q_OBJECT
};

#endif //ATOURMONITORACTION_H_
