#include "APlugin.h"

void APlugin::setPluginBase(AMELIA *am)
{
  _AMELIA=am;
}

AMELIA* APlugin::pluginBase()
{
  return _AMELIA;
}
