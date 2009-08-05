#ifndef AADVANCEDEVENTSETTINGS_H
#define AADVANCEDEVENTSETTINGS_H

#include <QVBoxLayout>
#include <QDialog>

#include <aeventmanager/AModelFilter.h>

class AAdvancedEventSettings : public QDialog
{
    Q_OBJECT
public:
    AAdvancedEventSettings(QWidget* parent=0);
    virtual ~AAdvancedEventSettings();

    void setAvailableModels(QStringList jetTypes, QStringList misEtTypes,QStringList trackTypes);
    
public slots:
    void setModelFilter(AModelFilter *modelFilter);

    virtual void accept();

 private:
    AModelFilter *_modelFilter;
    

};

#endif // AADVANCEDEVENTSETTINGS_H
