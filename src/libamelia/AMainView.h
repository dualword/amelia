#ifndef AMAINVIEW_H
#define AMAINVIEW_H

#include <QWidget>
#include <QStackedWidget>
#include <QTimerEvent>

#include "AAnimationGUI.h"

class LIBAMELIA_EXPORT AMainView : public QStackedWidget
{
 public:
public:
	AMainView(QWidget *parent=0);
	~AMainView();
      
public slots:
	void toggle();
	void setCurrentIndex(int idx);
	void setCurrentIndex1();
  
 signals:
  void beforeCurrentIndexChanged(int);
  void afterCurrentIndexChanged(int);
  
private:
	int idx;
	QTimeLine *slideTime;
	AAnimationGUI* slide;

	QPoint originalPosition;
  
  Q_OBJECT

};
#endif
