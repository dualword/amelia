#ifndef QTIMELINEPROXY_H_
#define QTIMELINEPROXY_H_

#include <QTimeLine>
#include "ADefines.h"

class LIBAMELIA_EXPORT QTimeLineProxy : public QTimeLine
{
public:
  QTimeLineProxy(QTimeLine *original=0,QObject *parent=0);

  void setTimeLine(QTimeLine *);
  QTimeLine *timeLine();

  State state() const;

public slots:
  void handleValueChanged(qreal);
  
private:
  QTimeLine *_timeLine;

  Q_OBJECT
};

#endif //QTIMELINEPROXY_H_
