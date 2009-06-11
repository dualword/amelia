#ifndef ATOURS_H
#define ATOURS_H

#include <APlugin.h>

#include <ageometry/AGeometry.h>
#include <ageometry/AGeoPlugin.h>

#include <QMenu>

#include "ATourManager.h"
#include "ATourBuilder.h"
#include "ATourPlayer.h"

class ATours : public QObject, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

public:
  ATours ( QObject *parent=0 );
  ~ATours();
  
  void load();

public slots:
  void startTour(int selIndex);

private:
  QMenu *toursMenu;
  QMenu *toursListMenu;
  
  ATourManager tourManager;
  ATourPlayer tourPlayer;
  ATourBuilder *tourBuilder;
  QSignalMapper tourMapper;

  AGeoPlugin *geoplugin;
  AGeometry *geo;


  void prepareTours();
};

#endif // ATOURS_H

