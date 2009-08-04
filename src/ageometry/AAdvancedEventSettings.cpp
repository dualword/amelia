#include "AAdvancedEventSettings.h"
#include <QDebug>
#include <QComboBox>

AAdvancedEventSettings::AAdvancedEventSettings(QWidget* parent)
  : QDialog(parent)
{}

AAdvancedEventSettings::~AAdvancedEventSettings()
{}

void AAdvancedEventSettings::setModelFilter(AModelFilter *modelFilter)
{
  _modelFilter=modelFilter;

  QComboBox *jetBox=findChild<QComboBox *>("JetComboBox");
  QString jetModel=modelFilter->jetTypeString();
  int index=jetBox->findText(jetModel);
  jetBox->setCurrentIndex(index);

  QComboBox *misEtBox=findChild<QComboBox *>("MisEtComboBox");
  QString misEtModel=modelFilter->misEtTypeString();
  index=misEtBox->findText(misEtModel);
  misEtBox->setCurrentIndex(index);

  QComboBox *trackBox=findChild<QComboBox *>("TrackComboBox");
  QString trackModel=modelFilter->trackTypeString();
  index=trackBox->findText(trackModel);
  trackBox->setCurrentIndex(index);
}

void AAdvancedEventSettings::accept()
{
  QComboBox *jetBox=findChild<QComboBox *>("JetComboBox");
  QString jetModel=jetBox->currentText();
  _modelFilter->setJetType(jetModel);

  QComboBox *misEtBox=findChild<QComboBox *>("MisEtComboBox");
  QString misEtModel=misEtBox->currentText();
  _modelFilter->setMisEtType(misEtModel);

  QComboBox *trackBox=findChild<QComboBox *>("TrackComboBox");
  QString trackModel=trackBox->currentText();
  _modelFilter->setTrackType(trackModel);
}


