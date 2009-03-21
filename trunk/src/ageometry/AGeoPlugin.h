#ifndef AGEOPLUGIN_H
#define AGEOPLUGIN_H

#include <APlugin.h>
#include <AMELIA.h>

#include <QObject>
#include <QMainWindow>
#include <QMenu>

class AGeoPlugin : public QObject, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

public:
  AGeoPlugin ( QObject *parent=0 );
  ~AGeoPlugin();
  
  void load();
  
  QWidget* findWidget(QString name);
  QMenu* addTrackComboMenu(QString text);

 private:
  QMainWindow *geoWin;
  QMenu comboMenu;
};

#endif // AGEOPLUGIN_H

