#ifndef AEVENTPACKAGE_H
#define AEVENTPACKAGE_H

#include <QList>
#include <QString>

#include "AEvent.h"

class AEventPackage : public QObject
{
  Q_OBJECT
public:
    AEventPackage(QObject *parent=0);
    ~AEventPackage();

    void load(const QString&);

    void setName(QString);
    QString name();

    int eventCount();
    int indexOf(AEvent*);
    AEvent* event(int);

public slots:
    void save();


private:
    QList<AEvent *> events;

    QString _name;
    QString location;
};

#endif
