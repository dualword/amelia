#ifndef AANIMATIONGUI_H_
#define AANIMATIONGUI_H_

#include <QObject>
#include <QPoint>
#include <QMap>
#include <QWidget>
#include <QTime>

class AAnimationGUI : public QObject
{
    Q_OBJECT

	Q_ENUMS(Status)
public:
	enum Status {Running=0, NotRunning=1};

    AAnimationGUI(QWidget* w=0);
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
	QMap<int, QPoint> keyframes;
    void calculateCurrentInterval();
    QPoint positionInterval[2];
    int timeInterval[2];
    int duration;
    int timeStep;
    QTime time;
    curveShape shape;

	AAnimationGUI::Status status;

};


#endif // AANIMATIONGUI_H_
