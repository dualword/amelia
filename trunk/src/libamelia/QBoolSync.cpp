#include "QBoolSync.h"

QBoolSync::QBoolSync()
  :_value(false),lock(false)
{ }

QBoolSync::QBoolSync(bool value)
  :_value(value),lock(false)
{ }

void QBoolSync::syncButton(QAbstractButton *button)
{
  connect(button,SIGNAL(toggled(bool)),
	  this,SLOT(setValue(bool)));
  connect(this,SIGNAL(valueChanged(bool)),
	  button,SLOT(setChecked(bool)));
  button->setChecked(_value);
}

void QBoolSync::syncGroupBox(QGroupBox *groupBox)
{
  connect(groupBox,SIGNAL(toggled(bool)),
	  this,SLOT(setValue(bool)));
  connect(this,SIGNAL(valueChanged(bool)),
	  groupBox,SLOT(setChecked(bool)));
  groupBox->setChecked(_value);
}


void QBoolSync::setValue(bool newValue)
{
  if(_value==newValue || lock) return;

  lock=true;
  _value=newValue;
  emit valueChanged(_value);
  lock=false;
}

bool QBoolSync::value()
{
  return _value;
}
