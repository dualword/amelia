#include "AAdvancedEventSettings.h"
#include <QDebug>
#include <QComboBox>

AAdvancedEventSettings::AAdvancedEventSettings(QWidget* parent)
  : QDialog(parent)
{}

AAdvancedEventSettings::~AAdvancedEventSettings()
{}

void AAdvancedEventSettings::setAvailableModels(QStringList jetTypes, QStringList misEtTypes,QStringList trackTypes)
{
  QComboBox *jetBox=findChild<QComboBox *>("JetComboBox");
  jetBox->clear();
  jetBox->addItems(jetTypes);
  jetBox->setEnabled(!jetTypes.empty());

  QComboBox *misEtBox=findChild<QComboBox *>("MisEtComboBox");
  misEtBox->clear();
  misEtBox->addItems(misEtTypes);
  misEtBox->setEnabled(!misEtTypes.empty());

  QComboBox *trackBox=findChild<QComboBox *>("TrackComboBox");
  trackBox->clear();
  trackBox->addItems(trackTypes);
  trackBox->setEnabled(!trackTypes.empty());
}

void AAdvancedEventSettings::setModelFilter(AModelFilter *modelFilter)
{
  _modelFilter=modelFilter;

  QComboBox *jetBox=findChild<QComboBox *>("JetComboBox");
  QString jetModel=modelFilter->jetType();
  int index=jetBox->findText(jetModel);
  jetBox->setCurrentIndex(index);

  QComboBox *misEtBox=findChild<QComboBox *>("MisEtComboBox");
  QString misEtModel=modelFilter->misEtType();
  index=misEtBox->findText(misEtModel);
  misEtBox->setCurrentIndex(index);

  QComboBox *trackBox=findChild<QComboBox *>("TrackComboBox");
  QString trackModel=modelFilter->trackType();
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


