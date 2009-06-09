#ifndef QBOOLSYNC_H_
#define QBOOLSYNC_H_

#include <QAbstractButton>
#include <QGroupBox>
#include "ADefines.h"

class LIBAMELIA_EXPORT QBoolSync : public QObject
{
 public:
  QBoolSync();
  QBoolSync(bool value);
  
  void syncButton(QAbstractButton *button);
  void syncGroupBox(QGroupBox *groupBox);
  bool value();

 public slots:
  void setValue(bool value);
  
 signals:
  void valueChanged(bool value);

 private:
  bool _value;

  bool lock;

  Q_OBJECT
};

#endif //QBOOLSYNC_H_
