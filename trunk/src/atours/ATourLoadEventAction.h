#ifndef ATOURLOADEVENTACTION_H_
#define ATOURLOADEVENTACTION_H_

#include "ATourAction.h"

#include <ageometry/AGeoPlugin.h>

class ATourLoadEventAction : public ATourAction
{
public:
  Q_INVOKABLE ATourLoadEventAction();

  void loadFromXML(QDomElement actionElement);

  void doAction();
  void undoAction();

private:
  QString eventFile;
  QString eventFileIninial;

  AGeoPlugin *geo;

  Q_OBJECT
};

#endif //ATOURLOADEVENTACTION_H_
