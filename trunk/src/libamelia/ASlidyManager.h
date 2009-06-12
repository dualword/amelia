#ifndef ASLIDYMANAGER_H_
#define ASLIDYMANAGER_H_

#include <QWidget>
#include <QList>
#include <QEvent>
#include <QSignalMapper>
#include <QTimer>

#include "AAnimationGUI.h"
#include "QTimeLineProxy.h"
#include "ASlidyWidget.h"

#include "ADefines.h"

class LIBAMELIA_EXPORT ASlidyManager : public QObject
{
 public:
  ASlidyManager(QWidget *parent,Qt::Alignment align=Qt::AlignLeft);

  void setMaxSize(int maxSize);
  int addWidget(QWidget *wdg,QString title);
  
 public slots:
  void toggleWidget(int);
  void showWidgetTimed(int);
  void showWidget(int);
  void hideWidget(int);
  void hideWidget();
  
  void timerEvent();
  
 protected:
  bool eventFilter(QObject *watched,QEvent *event);
  
 private:
  QList<ASlidyWidget *> _flaps;
  
  QTimeLine mainTimeline;
  
  QTimeLineProxy showingTimeline;
  AAnimationGUI showingAnimator;
  
  QTimeLineProxy hidingTimeline;
  AAnimationGUI hidingAnimator;
  
  int _visibleId;
  int _lastVisibleId;
  int _maxSize;
  
  QPoint topCorner;
  QPoint moveIn;
  bool verticalSlide;
  QTimer timer;
  Qt::Alignment align;

  void recalculatePositions();
  void recalculateAlignment();
  
  void prepareShowAnimation();
  void prepareHideAnimation();
  
  Q_OBJECT
};

#endif //ASLIDYMANAGER_H_
