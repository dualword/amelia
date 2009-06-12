#ifndef QFADERWIDGET_H_
#define QFADERWIDGET_H_

#include <QWidget>
#include <QTimeLine>

#include "ADefines.h"

class LIBAMELIA_EXPORT QFaderWidget : public QWidget
{
public:
  QFaderWidget(QWidget *parent=0);

  void setParent(QWidget *parent);

public slots:
  void setOpacity(qreal);
  void finalize();

signals:
  void fadeFinished();

protected:
  void paintEvent(QPaintEvent *event);
  void showEvent(QShowEvent *event);
  bool eventFilter(QObject *sender,QEvent *event);

private:
  QTimeLine timeLine;
  double opacity;

  void recalculateSize();
  
  Q_OBJECT
};

#endif //QFADERWIDGET_H_
