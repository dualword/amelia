#ifndef AWEBPLUGIN_H
#define AWEBPlugin_H

#include <QList>
#include <QObject>
#include <QAction>
#include <QSignalMapper>

#include <APlugin.h>
#include <AMELIA.h>

class AWebPlugin : public QObject, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

public:
    AWebPlugin ( QObject *parent=0 );
    ~AWebPlugin();

    void load();

};

#endif // AWEBVIEW_H

