#ifndef AMONITORGROUP_H_
#define AMONITORGROUP_H_

#include <QGraphicsItemGroup>
#include <QTimeLine>
#include <QGraphicsItemAnimation>

#include "AMonitor.h"

class AMonitorGroup : public QObject,public QGraphicsItemGroup
{
  Q_OBJECT
  
public:
  AMonitorGroup(QString name,QGraphicsItem* parent=0);

  AMonitor* monitor(QString name);
  QMap<QString,AMonitor*> monitors();
  
  QPointF calculateScaledWidgetGroupPosition();  
						     
public slots:
  void addMonitor(QString name,AMonitor* display);
  void show();
  void hide();

protected:
  void updateAnimatorPositions();

private:
  QString name;

  QMap<QString,AMonitor*> _monitors;
  QTimeLine pullUpTime,dropDownTime;
  QGraphicsItemAnimation pullUp,dropDown;
};

#endif //AMONITORGROUP_H_
