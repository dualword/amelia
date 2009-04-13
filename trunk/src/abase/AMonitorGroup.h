#ifndef AMONITORGROUP_H_
#define AMONITORGROUP_H_

#include <QTimeLine>
#include <QGraphicsItemAnimation>

#include "QGraphicsItemLayout.h"
#include "AMonitor.h"

class AMonitorGroup : public QGraphicsItemLayout
{
  Q_OBJECT
  
public:
  AMonitorGroup(QGraphicsItem *parent=0);

  AMonitor* monitor(QString name);
  QMap<QString,AMonitor*> monitors();
  
  QPointF calculateScaledWidgetGroupPosition();  
						     
public slots:
  void addMonitor(QString name,AMonitor* display,Qt::Alignment align=Qt::AlignHCenter);
  void show();
  void hide();
  void setSelected(const QString& selected);

protected:
  void updateAnimatorPositions();
  QTransform calculateTransformationForItem(int idx);

private:
  QString _selected;

  QMap<QString,AMonitor*> _monitors;
  QTimeLine pullUpTime,dropDownTime;
  QGraphicsItemAnimation pullUp,dropDown;
};

#endif //AMONITORGROUP_H_
