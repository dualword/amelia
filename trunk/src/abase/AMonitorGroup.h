#ifndef AMONITORGROUP_H_
#define AMONITORGROUP_H_

#include <QTimeLine>
#include <QGraphicsItemAnimation>

#include "QGraphicsItemLayout.h"
#include "AMonitor.h"

class AMonitorGroup : public QObject,public QGraphicsItemLayout
{
  Q_OBJECT
  
public:
  AMonitorGroup(QString name,QGraphicsItem* parent=0);

  AMonitor* monitor(QString name);
  QMap<QString,AMonitor*> monitors();
  
  QPointF calculateScaledWidgetGroupPosition();  
						     
public slots:
  void addMonitor(QString name,AMonitor* display,Qt::Alignment align);
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
