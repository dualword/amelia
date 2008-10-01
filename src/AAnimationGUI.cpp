#include "AAnimationGUI.h"




AAnimationGUI::AAnimationGUI(QWidget* w)
        : QObject(0)
{
    widget = w;
    setDuration(0);
    numIterations = 0;
    timeStep = 20;
    timeInterval[0]=timeInterval[1]=0;
    positionInterval[0]=positionInterval[1] = w->pos();
    id = 0;
    shape = linear;
	status=AAnimationGUI::NotRunning;
    //setKeyframe(0, w->pos());
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
	numIterations=0;
}

void AAnimationGUI::play() // After we set up all the elements of the animation, we start it
{

//    keyframes.sort();
    id = startTimer(timeStep);
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
    for ( map<int, QPoint>::iterator iter = keyframes.begin(); iter != keyframes.end(); iter++)
    {
        if (iter->first<= time)
        {
            timeInterval[0]=(iter->first);
            positionInterval[0]=(iter->second);
        }


        if ((iter->first>= time) && control)
        {
            timeInterval[1]=(iter->first);
            positionInterval[1]=(iter->second);
            control = false;
            emit keyframe();
        }
        setDuration(iter->first);

    }
}

void AAnimationGUI::timerEvent(QTimerEvent * event)
{
    numIterations++;
    //cout <<"numIterations:  "<< numIterations << endl;
    if (getCurrentTime() >= duration)
    {
        killTimer(id);
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
    return (numIterations*timeStep);
}

AAnimationGUI::Status AAnimationGUI::getStatus()
{
	return status;
}

