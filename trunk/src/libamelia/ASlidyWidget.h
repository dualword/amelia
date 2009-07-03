#ifndef ASLIDYWIDGET_H_
#define ASLIDYWIDGET_H_

#define _USE_MATH_DEFINES

#include <QFrame>
#include <QLabel>

#include "ADefines.h"
#include "AHideAButton.h"

class ASlidyManager;

class LIBAMELIA_EXPORT ASlidyWidget : public QFrame
{
 public:
  ASlidyWidget(ASlidyManager *mngr,const QString title=QString(),QWidget *parent=0,bool controlable=true);
  
  void addWidget(QWidget *wdg);
  
 private:
  QLabel *titleLabel;
  AHideAButton *closeButton;
  ASlidyManager *_mngr;

 Q_OBJECT
};

#endif //ASLIDYWIDGET_H_
