#include "QWaitForReady.h"

QWaitForReady::QWaitForReady(QObject *parent)
  :QObject(parent)
{ }

void QWaitForReady::waitFor(QObject *object)
{
  waitingFor.insert(object);
}

void QWaitForReady::setReady()
{
  QObject *check=sender();
  waitingFor.remove(check);;
  if(waitingFor.empty())
    emit ready();
}
