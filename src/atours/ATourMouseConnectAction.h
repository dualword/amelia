#ifndef ATOURMOUSECONNECTACTION_H_
#define ATOURMOUSECONNECTACTION_H_

#include "ATourMouseMoveAction.h"

#include <abase/ABase.h>

class ATourMouseConnectAction : public ATourMouseMoveAction
{
public:
  Q_INVOKABLE ATourMouseConnectAction();

  void connectTo(ATourAction *act);

  QPoint target();
  
private:
  ATourAction *action;
  ABase *base;
  
  Q_OBJECT
};

#endif //ATOURMOUSECONNECTACTION_H_
