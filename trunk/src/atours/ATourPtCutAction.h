#ifndef ATOURPTCUTACTION_H_
#define ATOURPTCUTACTION_H_

#include "ATourAction.h"

#include <QDoubleSync.h>

class ATourPtCutAction : public ATourAction
{
public:
  Q_INVOKABLE ATourPtCutAction();

  void loadFromXML(QDomElement actionElement);
  void doAction();
  void updateAction(double done);
  void undoAction();

private:
  QDoubleSync *ptFilter;

  double valueInitial;
  double value;

  Q_OBJECT
};

#endif // ATOURPTCUTACTION_H_
