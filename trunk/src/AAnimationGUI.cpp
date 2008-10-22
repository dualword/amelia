#include "AAnimationGUI.h"

#include <QTimerEvent>


AAnimationGUI::AAnimationGUI(QWidget* w)
        : QObject(0)
{
    widget = w;
    setDuration(0);
    timeStep = 20;
    timeInterval[0]=timeInterval[1]=0;
    if(w) positionInterval[0]=positionInterval[1] = w->pos();

    shape = linear;
	status=AAnimationGUI::NotRunning;
}

AAnimationGUI::~AAnimationGUI()
{
}

void AAnimationGUI::setCurveShape(curveShape s)
{
    shape = s;
}

void AAnimationGUI::setWidget(QWidget* w)
{
    this->widget = w;
}

void AAnimationGUI::setDuration(int dur)
{
    this->duration = dur;
}

void AAnimationGUI::setKeyframe(int time, QPoint point)
{
	if(duration<time) duration=time;
    keyframes[time] = point;
}

void AAnimationGUI::reset()
{
	if(status==AAnimationGUI::Running) return;
}

void AAnimationGUI::play() // After we set up all the elements of the animation, we start it
{
  setKeyframe(0,widget->pos());
  time.start();
  startTimer(timeStep);
	status=AAnimationGUI::Running;
}

void AAnimationGUI::animate()
{
    widget->move(getCurrentPosition());
    widget->update();
}


QPoint AAnimationGUI::getCurrentPosition()
{
    calculateCurrentInterval();

    int time = getCurrentTime();
    float timeFactor = ((float)time - (float)timeInterval[0])/(timeInterval[1]-timeInterval[0]);

    QPoint pos = QPoint(0,0);
    int x0 = positionInterval[0].x();
    int x1 = positionInterval[1].x();
    int y0 = positionInterval[0].y();
    int y1 = positionInterval[1].y();
    int posX = 0;
    int posY = 0;


    switch (shape)
    {
    case linear:
    posX = x0 + (x1 - x0)*(timeFactor);
    posY = y0 + (y1 - y0)*(timeFactor);
    break;

    case easeIn:
    break;

    case easeOut:
    break;

    case easeInOut:
    break;
    }



    pos.setX(posX);
    pos.setY(posY);
    //cout <<"Time interval:  "<< timeInterval[0] << "  --  "<< timeInterval[1] << endl;
    //cout <<"Position interval:  "<< x0 << " , "<< y0<<"  ----   " << x1 << " , "<< y1<< endl;
    //cout <<"duration:  "<< duration << endl;
    //cout <<"Pos:  "<< pos.x() << " , "<< pos.y()<< endl;
    //cout <<"currentTime:  "<< time << endl;
    //cout <<"timeFactor:  "<< timeFactor << endl;
    return pos;

}

void AAnimationGUI::calculateCurrentInterval()
{
    int time = getCurrentTime();
    bool control = true;
  for ( QMap<int, QPoint>::iterator iter = keyframes.begin(); iter != keyframes.end(); iter++)
    {
      if (iter.key()<= time)
        {
	  timeInterval[0]=(iter.key());
	  positionInterval[0]=(iter.value());
        }

      if ((iter.key()>= time) && control)
        {
	  timeInterval[1]=(iter.key());
	  positionInterval[1]=(iter.value());
            control = false;
            emit keyframe();
        }
      setDuration(iter.key());
    }
}

void AAnimationGUI::timerEvent(QTimerEvent * event)
{
    //cout <<"numIterations:  "<< numIterations << endl;
    if (getCurrentTime() >= duration)
    {
      killTimer(event->timerId());
        widget->move(positionInterval[1]);
		widget->setAttribute(Qt::WA_Moved);
        //cout <<"killed:  "<< endl;
		status=AAnimationGUI::NotRunning;
        emit animationFinished();
    }
    else
    {
        animate();
    }

}

int AAnimationGUI::getCurrentTime()
{
  return time.elapsed();
}

AAnimationGUI::Status AAnimationGUI::getStatus()
{
	return status;
}

