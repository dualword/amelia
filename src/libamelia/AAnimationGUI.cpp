#include "AAnimationGUI.h"

#include <QTimerEvent>
#include <QDebug>

AAnimationGUI::AAnimationGUI(QObject* parent)
  : QObject(parent),_widget(0),_timeLine(0)
{ }

AAnimationGUI::AAnimationGUI(QWidget* w,QObject* parent)
  : QObject(parent),_widget(w),_timeLine(0)
{ }

AAnimationGUI::~AAnimationGUI()
{
}

void AAnimationGUI::setTimeLine(QTimeLine *t)
{
  if(_timeLine)
    { //Disconnect the last timeline
      disconnect(_timeLine,SIGNAL(valueChanged(qreal)),
		 this,SLOT(setStep(qreal)));
    }
  _timeLine=t;
  if(_timeLine)
    connect(_timeLine,SIGNAL(valueChanged(qreal)),
	    this,SLOT(setStep(qreal)));
}

QTimeLine* AAnimationGUI::timeLine()
{
  return _timeLine;
}

void AAnimationGUI::setWidget(QWidget* w)
{
  _widget=w;

  //We need us some default step 0 position!
  if(!positions.contains(0) && w)
    positions[0]=w->pos();
}

QWidget* AAnimationGUI::widget()
{
  return _widget;
}

void AAnimationGUI::setPosAt(qreal time,QPoint p)
{
  positions[time]=p;
}

QPoint AAnimationGUI::posAt(qreal step)
{
  if(positions.contains(step)) return positions[step];

  QList<qreal> knownPosSteps=positions.keys();
  qSort(knownPosSteps.begin(),knownPosSteps.end());
  
  int lessThanId=findLessThan(0,knownPosSteps.size()-1,knownPosSteps,step);
  int nextId=lessThanId+1;
  
  qreal prev=knownPosSteps[lessThanId];
  
  QPoint pos;
  if(nextId>=knownPosSteps.size())
    {//We are at the end, so just set position to lessThas's
      pos=positions[prev];
    }
  else
    {
      qreal next=knownPosSteps[nextId];
      QPoint prevPos=positions[prev];
      QPoint nextPos=positions[next];
      QPoint slope=(nextPos-prevPos)/(next-prev);
      
      QPoint intercept=nextPos-slope*next;
      pos=slope*step+intercept;
    }
  
  return pos;
}

void AAnimationGUI::setStep(qreal step)
{
  if(!_widget) return; //Don't do anything if not associated with a widget
  _widget->move(posAt(step));
}

//Basically a modified merge search.
int AAnimationGUI::findLessThan(int begin,int end,QList<qreal> haystack,qreal needle)
{
  if(begin==end)
    { // This is make or break time
      if(haystack[begin]>needle) return 0;
      else return begin;
    }
  
  int middleL=(end+begin)/2;
  int middleU=middleL+1;
  
  if(haystack[middleU]<=needle)
    return findLessThan(middleU,end,haystack,needle);
  if(haystack[middleL]>needle)
    return findLessThan(begin,middleL,haystack,needle);
  
  return middleL;
}
