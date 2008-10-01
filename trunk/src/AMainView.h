#ifndef AMAINVIEW_H
#define AMAINVIEW_H

#include <QWidget>
#include <QStackedWidget>
#include <QTimerEvent>

#include "AAnimationGUI.h"

class AMainView : public QStackedWidget
{
	Q_OBJECT
public:
	AMainView(QWidget *parent=0);
	~AMainView();

public slots:
	void toggle();
	void setCurrentIndex(int idx);
	void setCurrentIndex1();
	void setCurrentIndex2();

private:
	int idx;
	AAnimationGUI* slideOut;
	AAnimationGUI* slideIn;

	QPoint originalPosition;
};
#endif