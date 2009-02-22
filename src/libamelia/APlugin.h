#ifndef APLUGIN_H_
#define APLUGIN_H_

#include <QtPlugin>
#include <QList>
#include <QString>

#include "ADefines.h"

class AMELIA;

class LIBAMELIA_EXPORT APlugin
{
 public:
  virtual ~APlugin() {}
  virtual void load() =0;

  void setPluginBase(AMELIA *amelia);
  AMELIA* pluginBase();

 private:
  AMELIA *_AMELIA;
  QList<QString> dependencies;
};

Q_DECLARE_INTERFACE(APlugin,
		    "amelia.APlugin/1.0")
#endif
