#ifndef QWAITFORREADY_H_
#define QWAITFORREADY_H_

#include <QObject>
#include <QSet>

#include "ADefines.h"

class LIBAMELIA_EXPORT QWaitForReady : public QObject
{
 public:
  QWaitForReady(QObject *parent=0);

  void waitFor(QObject *object);

 public slots:
  void setReady();

 signals:
  void ready();

 private:
  QSet<QObject*> waitingFor;
Q_OBJECT
};

#endif //QWAITFORREADY_H_
