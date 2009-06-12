#ifndef ASNAPSHOTTOOL_H_
#define ASNAPSHOTTOOL_H_

#include <QObject>

#include <QFaderWidget.h>

#include "AGeometry.h"

class ASnapshotTool : public QObject
{
public:
  ASnapshotTool(QObject *parent=0);

  void setTarget(QWidget*);
  QWidget* target();

public slots:
  void snapshot();

  void finish();

private:
  QWidget *_target;
  AGeometry *_geotarget;

  int currentCam;

  QFaderWidget fader;

  QPixmap lastSnapshot;

Q_OBJECT
};

#endif //ASNAPSHOTTOOL_H_
