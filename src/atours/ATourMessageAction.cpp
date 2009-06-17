#include "ATourMessageAction.h"
#include <QDebug>

ATourMessageAction::ATourMessageAction()
  :ATourAction()
{ 
  geoplugin=(AGeoPlugin *)AMELIA::global->plugin("AGeometry");
  setStateless(true);
}

void ATourMessageAction::loadFromXML(QDomElement actionElement)
{
  QDomElement messageElement=actionElement.namedItem("message").toElement();
  message=messageElement.text();

  ATourAction::loadFromXML(actionElement);
}

void ATourMessageAction::act()
{
  ATourAction::act();
  geoplugin->displayMessage(message);
}

void ATourMessageAction::undo()
{
  ATourAction::undo();
  geoplugin->displayMessage(QString());
}

void ATourMessageAction::cleanup()
{
  ATourAction::cleanup();
  geoplugin->displayMessage(QString());
}
