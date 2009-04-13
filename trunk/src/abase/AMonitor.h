#ifndef AMONITOR_H_
#define AMONITOR_H_

#include <QGraphicsProxyWidget>
#include <QGraphicsSimpleTextItem>
#include <QObject>
#include <QTimeLine>
#include <QGraphicsItemAnimation>

class AMonitor : public QGraphicsProxyWidget
{ 
Q_OBJECT

 public:
  AMonitor(QString name,QString description,QWidget* w=0,QGraphicsItem* parent=0);

  void setWidget(QWidget *);
  
  void setWidgetEnabled(bool status);

  void storeWidget();
  void restoreWidget();

 signals:
  void clicked();

 protected:
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

 private:
  QString name;
  QString description;

  QGraphicsSimpleTextItem descItem;
  QTimeLine descTimeline;
  QGraphicsItemAnimation descAnimator;

  //These variables are used to store information about the monitor widget
  // when the widget is currently being displayed by ABase, and thus
  // does not belong to this proxy.
  //Use (re)storeWidget() method to use these variables.
  QRect geo;
  QWidget* tmpwidget;
};

#endif //AMONITOR_H_
