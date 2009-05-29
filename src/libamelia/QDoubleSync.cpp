#include "QDoubleSync.h"
#include <QDebug>s

QDoubleSync::QDoubleSync()
  :_value(0.),lock(false)
{ }

QDoubleSync::QDoubleSync(double value)
  :_value(value),lock(false)
{ }

void QDoubleSync::syncSlider(QAbstractSlider *slider)
{
  connect(slider,SIGNAL(valueChanged(int)),
	  this,SLOT(setValue(int)));
  connect(this,SIGNAL(valueChanged(int)),
	  slider,SLOT(setValue(int)));
  slider->setValue((int)_value);
}

void QDoubleSync::syncSpinBox(QSpinBox *spinBox)
{
  connect(spinBox,SIGNAL(valueChanged(int)),
	  this,SLOT(setValue(int)));
  connect(this,SIGNAL(valueChanged(int)),
	  spinBox,SLOT(setValue(int)));
  spinBox->setValue((int)_value);
}

void QDoubleSync::syncSpinBox(QDoubleSpinBox *spinBox)
{
  connect(spinBox,SIGNAL(valueChanged(double)),
	  this,SLOT(setValue(double)));
  connect(this,SIGNAL(valueChanged(double)),
	  spinBox,SLOT(setValue(double)));
  spinBox->setValue(_value);
}

void QDoubleSync::setValue(int newValue)
{
  setValue((double)newValue);
}

void QDoubleSync::setValue(double newValue)
{
  if(_value==newValue || lock) return;

  lock=true;
  _value=newValue;
  emit valueChanged(_value);
  emit valueChanged((int)_value);
  lock=false;
}

double QDoubleSync::value()
{
  return _value;
}
