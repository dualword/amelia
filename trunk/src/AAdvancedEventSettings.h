


#ifndef AADVANCEDEVENTSETTINGS_H
#define AADVANCEDEVENTSETTINGS_H

#include <QUiLoader>
#include <QVBoxLayout>
#include "ALayerGUI.h"

class AAdvancedEventSettings : public QDialog
{
    Q_OBJECT
public:
    AAdvancedEventSettings(QWidget* parent=0);
    virtual ~AAdvancedEventSettings();
    QString selectedJet;
    QString selectedTrack;
    QString selectedMET;

public slots:
    void defineTrackType(QString ttype);
    void defineJetType(QString jtype);
    void defineMisEtType(QString mtype);
    virtual void accept();

signals:
    void changedTrackType(QString);
    void changedJetType(QString);
    void changedMisEtType(QString);
    void redraw();


};



#endif // AADVANCEDEVENTSETTINGS_H
