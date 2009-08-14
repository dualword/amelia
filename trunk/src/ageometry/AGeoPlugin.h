#ifndef AGEOPLUGIN_H
#define AGEOPLUGIN_H

#include <APlugin.h>
#include <AMELIA.h>

#include <QObject>
#include <QMainWindow>
#include <QMenu>
#include <QSignalMapper>
#include <QStackedWidget>

#include <AMainView.h>
#include <AMainViewTmpWidget.h>
#include <QDoubleSync.h>

#include <aeventmanager/AFilteredEvent.h>
#include <aeventmanager/AParticleFilter.h>
#include <aeventmanager/APtFilter.h>

#include "AGeometryDefines.h"

class AEvent;

class AGEOMETRY_EXPORT AGeoPlugin : public QObject, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

public:
  AGeoPlugin ( QObject *parent=0 );
  ~AGeoPlugin();
  
  void load();
  
  /*!
   * Quick access to subwidgets of the main geometry monitor
   *
   * \param name Name of the widget
   * \return The widget with the name widget matching type T. 0 if not such thing exists..
   */
  template<class T> T findWidget(QString name)
    {
      return geoWin->findChild<T>(name);
    }

  QMenu* addTrackComboMenu(QString text);
  int  addMainViewWidget(QWidget* widget,QString title="");
  AMainViewTmpWidget* addMainViewTmpWidget(QWidget* widget);
  void addToDetectorMenu(QString partName,QAction *action);

  APtFilter* ptFilter();
  AParticleFilter *particleFilter();

 public slots:
  void handleNewEventLoaded(AEvent*);
  void displayMessage(QString text,QString header="",QPixmap img=QPixmap());
  void switchToMainView(int idx);

  void loadEvent(QString);
  void loadEvent(AEvent*);

  void unloadEvent();

 signals:
  void eventLoaded(AEvent*);
  void eventLoaded(AEvent*,AFilteredEvent*);

 private:
  QMainWindow* geoWin;
  AMainView* mainView;
  class AGeometry *geo;
  class ALayerGUI *layerGUI;
  QMenu* menuMain_View;

  QMenu comboMenu;
  QSignalMapper mainViewMapper;
};

#endif // AGEOPLUGIN_H

