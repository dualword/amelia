#ifndef ATOURSELECTTRACKACTION_H_
#define ATOURSELECTTRACKACTION_H_

#include "ATourAction.h"

#include <ageometry/AGeometry.h>

class ATourSelectTrackAction : public ATourAction
{
public:
  Q_INVOKABLE ATourSelectTrackAction();
  Q_INVOKABLE ATourSelectTrackAction(QString msg);

  QString type();

  QPoint cursor();

  void loadFromXML(QDomElement actionElement);
  
  void act();
  void undo();

  void prepare();

private:
  AGeometry *geo;

  int trackid;
  bool state;
  bool multi;

  Q_OBJECT
};

#endif //ATOURSELECTTRACKACTION_H_
