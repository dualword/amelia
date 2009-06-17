#ifndef ATOURMOUSEMOVEACTION_H_
#define ATOURMOUSEMOVEACTION_H_

#include "ATourAction.h"

#include <abase/ABase.h>

#include <AMELIA.h>

class ATourMouseMoveAction : public ATourAction
{
public:
  Q_INVOKABLE ATourMouseMoveAction();

  virtual QPoint target();
  void setTarget(QPoint);

  void loadFromXML(QDomElement actionElement);
  
  void update(double done);
  void cleanup();

  QWidget* mouseCursor();
  QPoint cursor();

private:
  QPoint _target;

  ABase *base;

  QPoint interpolate(QPoint start,QPoint end,double time);

  Q_OBJECT
};

#endif //ATOURMOUSEMOVEACTION_H_
