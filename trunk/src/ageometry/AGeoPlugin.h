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

#include <AMainView.h>
#include <AMainViewTmpWidget.h>

#include "AGeometryDefines.h"

class AGEOMETRY_EXPORT AGeoPlugin : public QObject, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

public:
  AGeoPlugin ( QObject *parent=0 );
  ~AGeoPlugin();
  
  void load();
  
  QWidget* findWidget(QString name);
  QMenu* addTrackComboMenu(QString text);
  int  addMainViewWidget(QWidget* widget,QString title="");
  AMainViewTmpWidget* addMainViewTmpWidget(QWidget* widget);
  void addToDetectorMenu(QString partName,QAction *action);

 public slots:
  void handleNewEventLoaded(AEvent*);
  void displayMessage(QString text,QString header="",QPixmap img=QPixmap());
  void switchToMainView(int idx);

 signals:
  void eventLoaded(AEvent*);

 private:
  QMainWindow* geoWin;
  AMainView* mainView;
  class AGeometry *geo;
  QMenu* menuMain_View;

  QMenu comboMenu;
  QSignalMapper mainViewMapper;
};

#endif // AGEOPLUGIN_H

