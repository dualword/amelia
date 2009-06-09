#ifndef ASLIDYWIDGET_H_
#define ASLIDYWIDGET_H_

#define _USE_MATH_DEFINES

#include <QWidget>
#include <QList>
#include <QEvent>
#include <QSignalMapper>
#include <QTimer>

#include "AAnimationGUI.h"
#include "QTimeLineProxy.h"
#include "ASlidyFlap.h"
#include "ADefines.h"

class LIBAMELIA_EXPORT ASlidyWidget : public QObject
{
 public:
  ASlidyWidget(QWidget *parent);

  int addWidget(QWidget *wdg,QString title);

 public slots:
  void toggleWidget(int);
  void showWidgetTimed(int);
  void showWidget(int);
  void hideWidget(int);

  void raiseFlap(qreal);
  void timerEvent();

 protected:
  bool eventFilter(QObject *watched,QEvent *event);

 private:
  QList<QWidget *> _pages;
  QList<ASlidyFlap *> _flaps;

  QSignalMapper flapMapper; //flapper

  QTimeLine mainTimeline;

  QTimeLineProxy showingTimeline;
  AAnimationGUI showingAnimator;

  QTimeLineProxy hidingTimeline;
  AAnimationGUI hidingAnimator;

  int _visibleId;
  int _lastVisibleId;
  int _maxWidth;

  QPoint topCorner;
  QTimer timer;

  void recalculatePositions();

  void prepareShowAnimation(ASlidyFlap *flap);
  void prepareHideAnimation(ASlidyFlap *flap);

  Q_OBJECT
};

#endif //ASLIDYWIDGET_H_
