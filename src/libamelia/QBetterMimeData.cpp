#include "QBetterMimeData.h"
#include <QDebug>

QBetterMimeData::QBetterMimeData()
  :QMimeData()
{ }

void QBetterMimeData::setData(const QString& mimeType, QObject *data)
{
  _data[mimeType]=data;
}

QObject *QBetterMimeData::data(const QString& mimeType) const
{
  return _data[mimeType];
}

QStringList QBetterMimeData::formats() const
{
  QStringList formats=QMimeData::formats();
  formats+=_data.keys();
  return formats;
}
