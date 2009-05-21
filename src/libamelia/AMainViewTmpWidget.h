#ifndef AMAINVIEWTMPWIDGET_H_
#define AMAINVIEWTMPWIDGET_H_

#include <QWidget>

#include "AMainView.h"
#include "AHideAButton.h"

class AMainViewTmpWidget : public QWidget
{
public:
  AMainViewTmpWidget(QWidget *widget,AMainView *parent=0);
					 
public slots:
  void start();
  void stop();
  void close();

protected:
  void resizeEvent(QResizeEvent *event);

private:
  int internalId;
  int previousId;
  AHideAButton closeButton;

Q_OBJECT
};

#endif //AMAINVIEWTMPWIDGET_H_
