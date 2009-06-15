#ifndef ATOURMOUSECONNECTACTION_H_
#define ATOURMOUSECONNECTACTION_H_

#include "ATourMouseMoveAction.h"

class ATourMouseConnectAction : public ATourMouseMoveAction
{
public:
  Q_INVOKABLE ATourMouseConnectAction();
  Q_INVOKABLE ATourMouseConnectAction(QString wdg);

  void loadFromXML(QDomElement actionElement);
  
  void doAction();

private:
  QString _targetWidget;

  Q_OBJECT
};

#endif //ATOURMOUSECONNECTACTION_H_
