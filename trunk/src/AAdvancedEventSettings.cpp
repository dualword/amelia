#include "AAdvancedEventSettings.h"


AAdvancedEventSettings::AAdvancedEventSettings(QWidget* parent)
        : QDialog(0)
{
    selectedJet = QString("");
    selectedTrack = QString("");
    selectedMET = QString("");
}

AAdvancedEventSettings::~AAdvancedEventSettings()
{}

void AAdvancedEventSettings::defineTrackType(QString ttype)
{
    selectedTrack = ttype;
}

void AAdvancedEventSettings::defineJetType(QString jtype)
{
    selectedJet = jtype;
}

void AAdvancedEventSettings::defineMisEtType(QString mtype)
{
    selectedMET = mtype;
}

void AAdvancedEventSettings::accept()
{
    bool selectionChanged = false;
    if (selectedJet != QString(""))
    {
        emit changedJetType(selectedJet);
        selectionChanged = true;
    }

    if (selectedTrack != QString(""))
    {
        emit changedTrackType(selectedTrack);
        selectionChanged = true;
    }

    if (selectedMET != QString(""))
    {
        changedMisEtType(selectedMET);
        selectionChanged = true;
    }

    if (selectionChanged)
        emit redraw();
}


