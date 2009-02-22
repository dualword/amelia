#ifndef NEWPLUGIN_H
#define NEWPLUGIN_H

#include <APlugin.h>

class NewPlugin : public QObject, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

public:
    NewPlugin ( QObject *parent=0 );
    ~NewPlugin();

    void load();
};

#endif // NEWPLUGIN_H

