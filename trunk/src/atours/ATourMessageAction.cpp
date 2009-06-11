#include "ATourMessageAction.h"
#include <QDebug>

ATourMessageAction::ATourMessageAction()
  :ATourAction()
{ 
  geoplugin=(AGeoPlugin *)AMELIA::global->plugin("AGeometry");
}

void ATourMessageAction::loadFromXML(QDomElement actionElement)
{
  ATourAction::loadFromXML(actionElement);

  QDomElement messageElement=actionElement.namedItem("message").toElement();
  
  message=messageElement.text();
}

void ATourMessageAction::doAction()
{
  geoplugin->displayMessage(message);
}

void ATourMessageAction::endAction()
{
  geoplugin->displayMessage(QString());
}
