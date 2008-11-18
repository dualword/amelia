#include "AEventManager.h"

#include <QDir>
#include <QDebug>
#include <QFont>

#include <iostream>
using namespace std;

AEventManager::AEventManager(QString loc)
{
    QDir dir(loc);
    dir.setFilter(QDir::Dirs);
    QStringList entries=dir.entryList();
    for (int i=0;i<entries.size();i++)
    {
        QDir event(loc+"/"+entries[i]);
        if (event.exists(".metainfo"))
        {
            AEventPackage pkg;
            pkg.load(loc+"/"+entries[i]);
            packages.append(pkg);
        }
    }

}

QModelIndex AEventManager::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    int id;
    if (!parent.isValid())
    {
        id=-1;
    }
    else
    {
        id=parent.row();
    }
    return createIndex(row,column,id);
}

QModelIndex AEventManager::parent(const QModelIndex& index) const
{
    if (!index.isValid()) return QModelIndex();

    int id=index.internalId();
    if (id==-1)
    {
        return QModelIndex();
    }
    else
    {
        return createIndex(id,0,-1);
    }
}

int AEventManager::rowCount(const QModelIndex &index) const
{
    if (!index.isValid()) return packages.size();

    int id=index.internalId();
    if (id==-1)
    {
        AEventPackage pkg=packages[index.row()];
        return pkg.eventCount();
    }

    return 0;
}

int AEventManager::columnCount(const QModelIndex &index) const
{
    return 2;
}

QVariant AEventManager::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int id=index.internalId();

    QFont font;

    switch (role)
    {
    case Qt::FontRole:
        if (id!=-1)
        { // Event Name
            AEventPackage pkg=packages[index.internalId()];
            bool read=pkg.event(index.row())->read;
            if (!read)
                font.setBold(true);

            if (activeEvent==index)
            {
                font.setItalic(true);
            }
        }
        else
        { // Package Name
            font.setBold(true);

            if (index==activePackage)
                font.setItalic(true);

            font.setPointSize(14);
        }

        return font;

    case Qt::DisplayRole:
        if (id==-1)
        {
            if (index.column()==0)
            {
                AEventPackage pkg=packages[index.row()];
                return pkg.name();
            }
        }
        else
        {
            AEventPackage pkg=packages[index.internalId()];
            AEvent *e=pkg.event(index.row());
            if (index.column()==0)
            {
                return e->filename;
            }
            else
            {
                QString tagString="";
                for (QSetIterator<QString> iter(e->tags);iter.hasNext();)
                    tagString+=iter.next()+" ";
                return tagString;
            }
        }

    case Qt::ForegroundRole:
        if (id!=-1)
        {
            if (index==activeEvent)
                return Qt::red;
        }
    /*case Qt::ToolTipRole:
        if (id!=-1 && index.column()==0)
        {
            AEventPackage pkg=packages[index.internalId()];
            AEvent *e=pkg.event(index.row());
            AXmlEvent *event=AXmlEvent::CachedEvent(e->location);

            QString toolTip=
                "<html><b>Tracks:</b> \t\t"+QString::number(event->EventComplete.numTracks)+"<br/>"
                "<b>Neutral Hadrons:</b> \t"+QString::number(event->EventComplete.numNeutralHadrons)+"<br/>"
                "<b>Charged Hadrons:</b> \t"+QString::number(event->EventComplete.numChargedHadrons)+"<br/>"
                "<b>Photons:</b> \t\t"+QString::number(event->EventComplete.numPhotons)+"<br/>"
                "<b>Muons:</b> \t\t"+QString::number(event->EventComplete.numMuons)+"<br/>"
                "<b>Electrons:</b> \t\t"+QString::number(event->EventComplete.numElectrons)+
                "</html>";

            return toolTip;
        }*/
    }
    /*else if(role==Qt::DecorationRole)
      {
        if(id!=-1)
      {
        if(index.column()==1)
          {
            QList<QVariant> iconList;
            iconList.append(QIcon(":/media/higgs.png"));
            iconList.append(QIcon(":/media/blackhole.png"));
            return iconList;
          }
      }
      }*/
    return QVariant();
}

AEvent* AEventManager::getEvent(const QModelIndex& index)
{
    int id=index.internalId();
    if (id!=-1 && index.isValid())
    {
        AEventPackage pkg=packages[index.internalId()];
        AEvent *e=pkg.event(index.row());
        return e;
    }
    return 0;
}

AEventPackage AEventManager::getPackage(const QModelIndex& index)
{
    int id=index.internalId();
    if (id==-1)
    {
        AEventPackage pkg=packages[index.row()];
        return pkg;
    }
    AEventPackage e;
    return e;
}

void AEventManager::setActivePackage(const QModelIndex& index)
{
    activePackage=index;
}

QModelIndex AEventManager::nextEvent()
{
    if (!activePackage.isValid())
        return QModelIndex();

    AEventPackage pkg=getPackage(activePackage);
    if (pkg.eventCount()>1)
    {
        int id=activeEvent.row();
        id++;
        if (id==pkg.eventCount())
            id=0;

        return index(id,0,activePackage);
    }

    return QModelIndex();

}

void AEventManager::eventLoaded(QString loc)
{
    for (int i=0;i<packages.size();i++)
    {
        for (int j=0;j<packages[i].eventCount();j++)
        {
            AEvent *e=packages[i].event(j);
            if (e->location==loc)
            {
                e->read=true;
                packages[i].save();
                QModelIndex pkg=index(i,0,QModelIndex());
                QModelIndex eve=index(j,0,pkg);
                activePackage=pkg;
                activeEvent=eve;
                return;
            }
        }
    }
}

void AEventManager::eventUnloaded()
{
    activeEvent=QModelIndex();
}

void AEventManager::tagActiveEvent(const QString& tag)
{
    tagEvent(activeEvent,tag);
}

void AEventManager::tagEvent(QModelIndex index,const QString& tag)
{
    AEvent *e=getEvent(index);
    if (e)
    {
        e->tags.insert(tag);

        packages[index.internalId()].save();
    }
}

void AEventManager::detagActiveEvent(const QString& tag)
{
    detagEvent(activeEvent,tag);
}

void AEventManager::detagEvent(QModelIndex index,const QString& tag)
{
    AEvent *e=getEvent(index);
    if (e)
    {
        e->tags.remove(tag);

        packages[index.internalId()].save();
    }
}
