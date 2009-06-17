#ifndef ATOURPLAYER_H_
#define ATOURPLAYER_H_

#include <QObject>
#include <QTimeLine>
#include <QLayout>
#include <QSignalMapper>
#include <QSlider>

#include <ASlidyManager.h>

#include <ageometry/AGeoPlugin.h>
#include <ageometry/AGeometry.h>

#include <abase/ABase.h>

#include <AMELIA.h>

#include "ATour.h"

class ATourPlayer : public QObject
{
public:
  ATourPlayer();

  void setupElements();

  QTimeLine::State state();

public slots:
  void play(ATour *tour);
  void playBlock(int idx);

  void toggle();
  void resume();
  void pause();
  void stop();
  void backward();
  void forward();
  void seek(int to);

  void tourFinished();

signals:
  void setCameraPosition(APoint3D pos);
  void setCameraTarget(APoint3D pos);

  void tour_started();
  void tour_stopped();

protected:
  bool eventFilter(QObject *obj, QEvent *event);

private:
  ATour *currentTour;
  ATourBlock *currentBlock;

  AGeometry *geo;
  AGeoPlugin *geoplugin;

  ASlidyManager *controls;
  QWidget *playerControls;
  QLayout *blocksLayout;
  QSlider *timeControl;
  QSpinBox *timeShow;
  
  QSignalMapper blocksMapper;

  QTimeLine timeLine;
  int lastFrame;

  bool hasParent(QObject *obj,QObject *parent);

  Q_OBJECT
};

#endif //ATOURPLAYER_H_
