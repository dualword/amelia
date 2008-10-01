#ifndef AANIMATIONGUI_H_INCLUDED
#define AANIMATIONGUI_H_INCLUDED

#include <QFrame>
#include <QWidget>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsItemAnimation>
#include <QObject>
#include <map>
#include <iostream>


using namespace std;
class AAnimationGUI : public QObject
{
    Q_OBJECT

	Q_ENUMS(Status)
public:
	enum Status {Running=0, NotRunning=1};

    AAnimationGUI(QWidget* w);
    ~AAnimationGUI();

    enum curveShape {linear=0, easeOut=1, easeIn=2, easeInOut=3};
    void setCurveShape(curveShape s);

	void reset();
    void setWidget(QWidget* w);
    void setKeyframe(int time, QPoint point);
    void play();
    QPoint getCurrentPosition();
    void setDuration(int dur);
    int getCurrentTime();
	AAnimationGUI::Status getStatus();


public slots:
    void animate();

signals:
    void animationFinished();
    void keyframe();


protected:
    void timerEvent(QTimerEvent * event);

private:
    QWidget* widget;
    std::map<int, QPoint> keyframes;
    void calculateCurrentInterval();
    QPoint positionInterval[2];
    int timeInterval[2];
    int duration;
    int timeStep;
    int numIterations;
    int id;
    curveShape shape;

	AAnimationGUI::Status status;

};


#endif // AANIMATIONGUI_H_INCLUDED
