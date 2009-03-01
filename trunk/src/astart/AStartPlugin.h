#ifndef ASTARTPLUGIN_H
#define ASTARTPLUGIN_H

#include <QAction>
#include <phonon>

#include <APlugin.h>

class AStartPlugin : public QObject, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

 public:
    AStartPlugin ( QObject *parent=0 );
    ~AStartPlugin();

    void load();

 public slots:
    void goBack();

 private:
    Phonon::VideoPlayer *player;
    QWidget* oldWidget;

    QAction* skipAction;
};

#endif // ASTARTPLUGIN_H
