#ifndef QDOUBLESYNC_H_
#define QDOUBLESYNC_H_

#include <QAbstractSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>

class QDoubleSync : public QObject
{
 public:
  QDoubleSync();
  QDoubleSync(double value);

  void syncSlider(QAbstractSlider *slider);  
  void syncSpinBox(QSpinBox *spinBox);
  void syncSpinBox(QDoubleSpinBox *spinBox);

  double value();

 public slots:
  void setValue(double value);
  void setValue(int value);
  
 signals:
  void valueChanged(double value);
  void valueChanged(int value);

 private:
  double _value;

  double lock;

  Q_OBJECT
};

#endif //QDOUBLESYNC_H_
