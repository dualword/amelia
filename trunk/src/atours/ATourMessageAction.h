#ifndef ATOURMESSAGEACTION_H_
#define ATOURMESSAGEACTION_H_

#include "ATourAction.h"

#include <ageometry/APoint3D.h>
#include <ageometry/AGeoPlugin.h>

#include <AMELIA.h>

class ATourMessageAction : public ATourAction
{
public:
  Q_INVOKABLE ATourMessageAction();

  void loadFromXML(QDomElement actionElement);
  
  void doAction();
  void endAction();

private:
  QString message;

  AGeoPlugin *geoplugin;

  Q_OBJECT
};

#endif //ATOURMESSAGEACTION_H_
