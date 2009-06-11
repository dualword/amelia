#ifndef ATOURACTIONGROUP_H_
#define ATOURACTIONGROUP_H_

#include <QObject>
#include <QDomElement>
#include <QList>

#include "ATourAction.h"

class ATourActionGroup : public ATourAction
{
public:
  Q_INVOKABLE ATourActionGroup();

  void doAction();
  void updateAction(double done);
  
  void loadFromXML(QDomElement actionElement);

private:


  Q_OBJECT
};

#endif //ATOURACTIONGROUP_H_
