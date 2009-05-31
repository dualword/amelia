#ifndef AANIMATIONLAYOUTGUI_H_
#define AANIMATIONLAYOUTGUI_H_

#include <QLayout>
#include <QLayoutItem>

#include "AAnimationGUI.h"

#include "ADefines.h"

class LIBAMELIA_EXPORT AAnimationLayoutGUI : public AAnimationGUI
{
  Q_OBJECT

  public:
   AAnimationLayoutGUI(QWidget* w,QObject *parent=0);
   AAnimationLayoutGUI(QObject* parent=0);
   ~AAnimationLayoutGUI();
   
   void setPosAt(qreal time, QPoint point);

   void setWidget(QWidget*);
   void setTimeLine(QTimeLine *timeLine);

 public slots:
   void handleFinished();
   void setStep(qreal step);

 private:
   QLayout *layout;

   QLayout *findLayout(QLayout *layout);
};


#endif // AANIMATIONLAYOUTGUI_H_
