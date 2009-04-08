#ifndef ADROPDOWNMENU_H_
#define ADROPDOWNMENU_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QAction>
#include <QPushButton>

#include <AAnimationGUI.h>

#include "AWebDefines.h"

class AWEB_EXPORT ADropDownMenu : public QWidget
{
 public:
  ADropDownMenu(QWidget* parent=0);

  void addAction(QAction* action);

 public slots:
  void toggle();
  void showMe();
  void hideMe();

 protected:
  virtual void moveEvent(QMoveEvent* event);

 private:
  QVBoxLayout layout;
  QPushButton showhideButton;

  QTimeLine *dropDownTime;
  AAnimationGUI dropDown;
  QTimeLine *pullUpTime;
  AAnimationGUI pullUp;

  QPoint posInit;

  Q_OBJECT
};

#endif //ADROPDOWNMENU_H_
