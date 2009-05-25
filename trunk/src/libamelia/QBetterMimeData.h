#ifndef QBETTERMIMEDATA_H_
#define QBETTERMIMEDATA_H_

#include <QMimeData>
#include <QStringList>

class QBetterMimeData : public QMimeData
{
 public:
  QBetterMimeData();

  void setData(const QString& mimeType,QObject *data);
  QObject *data(const QString& mimeType) const;

  QStringList formats() const;

 private:
  QMap<QString,QObject*> _data;

Q_OBJECT
};

#endif //QBETTERMIMEDATA_H_
