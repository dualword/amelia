#ifndef AANIMATIONGUI_H_
#define AANIMATIONGUI_H_

#include <QObject>
#include <QPoint>
#include <QMap>
#include <QWidget>
#include <QTimeLine>

#include "ADefines.h"

class LIBAMELIA_EXPORT AAnimationGUI : public QObject
{
  Q_OBJECT

  public:
   AAnimationGUI(QWidget* w,QObject *parent=0);
   AAnimationGUI(QObject* parent=0);
   ~AAnimationGUI();

   void setPosAt(qreal time, QPoint point);

   void setTimeLine(QTimeLine*);
   QTimeLine* timeLine();

   void setWidget(QWidget*);
   QWidget* widget();

public slots:
	void setStep(qreal step);

 private:
   QWidget* _widget;
   QTimeLine* _timeLine;

   QMap<qreal,QPoint> positions;

   int findLessThan(int begin,int end,QList<qreal> haystack,qreal needle);
};


#endif // AANIMATIONGUI_H_
