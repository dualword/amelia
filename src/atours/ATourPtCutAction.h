#ifndef ATOURPTCUTACTION_H_
#define ATOURPTCUTACTION_H_

#include "ATourAction.h"

#include <QDoubleSync.h>

class ATourPtCutAction : public ATourAction
{
public:
  Q_INVOKABLE ATourPtCutAction();

  void loadFromXML(QDomElement actionElement);

  void update(double done);

  void prepare();

  QString widgetOfInterest();

private:
  QDoubleSync *ptFilter;

  double value;

  Q_OBJECT
};

#endif // ATOURPTCUTACTION_H_
