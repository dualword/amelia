#ifndef AGEOPLUGIN_H
#define AGEOPLUGIN_H

#include <APlugin.h>
#include <AMELIA.h>

#include <QObject>
#include <QMainWindow>
#include <QMenu>
#include <QSignalMapper>
#include <QStackedWidget>

#include <aeventmanager/AEvent.h>

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
  void addMainViewWidget(QWidget* widget,QString title);

 public slots:
  void handleNewEventLoaded(AEvent*);

 signals:
  void eventLoaded(AEvent*);

 private:
  QMainWindow* geoWin;
  QStackedWidget* mainView;
  QMenu* menuMain_View;

  QMenu comboMenu;
  QSignalMapper mainViewMapper;
};

#endif // AGEOPLUGIN_H

