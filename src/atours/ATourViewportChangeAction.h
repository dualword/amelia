#ifndef ATOURVIEWPORTCHANGEACTION_H_
#define ATOURVIEWPORTCHANGEACTION_H_

#include "ATourAction.h"

#include <ageometry/AGeometry.h>
#include <ageometry/AGeoPlugin.h>

class ATourViewportChangeAction : public ATourAction
{
public:
  Q_INVOKABLE ATourViewportChangeAction();

  QString widgetOfInterest();

  void loadFromXML(QDomElement actionElement);
  
  void doAction();
  void undoAction();
  
private:
  int viewport;
  int viewportInitial;

  AGeoPlugin *geoplugin;
  AGeometry *geo;

  Q_OBJECT
};

#endif //ATOURVIEWPORTCHANGEACTION_H_
