#include "AInterestingTrackTableModel.h"

#include <QApplication>
#include <QInputDialog>

unsigned int AInterestingTrackTableModel::selectionID=0;

AInterestingTrackTableModel::AInterestingTrackTableModel(QWidget* parent):QAbstractTableModel(parent)
{
    tracks=QList<ATrack*>();

    //Use this for selecting tracks by clicking on the table
    selection=new QItemSelectionModel(this);//parent->selectionModel();
    connect(selection,SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
            this,SLOT(handleSelectionChanged(const QItemSelection&,const QItemSelection&)));
}

AInterestingTrackTableModel::~AInterestingTrackTableModel()
{
    selection->deleteLater();
}

void AInterestingTrackTableModel::addTable(QAbstractItemView* table)
{
    table->setModel(this);
    table->setSelectionModel(selection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
}

int AInterestingTrackTableModel::rowCount(const QModelIndex& root) const
{
    return tracks.size();
}

int AInterestingTrackTableModel::columnCount(const QModelIndex& root) const
{
    return 5;
}

QVariant AInterestingTrackTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row()>=tracks.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        ATrack* track = tracks.at(index.row());
        switch (index.column())
        {
        case 0:
            return track->name.c_str();
        case 1:
            if (track->Type == ATrack::eJet || track->Type == ATrack::eSTrack)
            {
                return QString::number(track->pt);
            }
            else return QString("N/A");
        case 2:
            if (track->Type == ATrack::eJet)
            {
                AJet* jet = track->getThisJet();
                return QString::number(jet->eta);
            }
            else if (track->Type == ATrack::eSTrack)
            {
                ASTrack* strack = track->getThisSTrack();
                return QString::number(strack->eta);
            }
            else
            {
                return "N/A";
            }

        case 3:
           if (track->Type == ATrack::eJet)
            {
                AJet* jet = track->getThisJet();
                return QString::number(jet->phi);
            }
            else if (track->Type == ATrack::eSTrack)
            {
                ASTrack* strack = track->getThisSTrack();
                return QString::number(strack->phi);
            }
            else
            {
                return "N/A";
            }
        case 4:
            if (track->Type == ATrack::eSTrack)
            {
                return QString::number(tracks.at(index.row())->trackID);
            }
            else
            {
                return "N/A";
            }


        }
    }
    return QVariant();
}

QVariant AInterestingTrackTableModel::headerData (int section, Qt::Orientation orientation, int role) const
{
    if (role!=Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return "Name";
        case 1:
            return "pT";
        case 2:
            return QChar (0x3B7); //eta
        case 3:
            return QChar (0x3C6); //phi
        case 4:
            return "ID";
        }
    }

    if (orientation == Qt::Vertical)
    {
        return QString::number(tracks[section]->selectionID);
    }
    return QVariant();
}

void AInterestingTrackTableModel::sort(int column, Qt::SortOrder order)
{
    //Do a bubble sort... Switch to something faster later?

    emit layoutAboutToBeChanged();
    for (int i=0;i<tracks.size()-1;i++)
    {
        for (int j=i;j<tracks.size()-1;j++)
        {
            switch (column)
            {
            case 0:
                if ( (tracks[j]->trackID<tracks[j+1]->trackID && order==Qt::AscendingOrder) ||
                        (tracks[j]->trackID>tracks[j+1]->trackID && order==Qt::DescendingOrder) )
                {
                    tracks.swap(i,j+1);
                }
                break;
            case 1:
                if ( (tracks[j]->name<tracks[j+1]->name && order==Qt::AscendingOrder) ||
                        (tracks[j]->name>tracks[j+1]->name && order==Qt::DescendingOrder) )
                {
                    tracks.swap(i,j+1);
                }
                break;
            case 2:
                if ( (tracks[j]->pt<tracks[j+1]->pt && order==Qt::AscendingOrder) ||
                        (tracks[j]->pt>tracks[j+1]->pt && order==Qt::DescendingOrder) )
                {
                    tracks.swap(i,j+1);
                }
                break;
            default:
                break;
            }
        }
    }
    emit layoutChanged();
}

void AInterestingTrackTableModel::addTrack(ATrack* strack)
{
    if (tracks.indexOf(strack)<0) //Dupe Check
    {
        beginInsertRows(QModelIndex(),0,0);
        tracks.append(strack);
        endInsertRows();
    }
}


void AInterestingTrackTableModel::addToSelectedTracks()
{

    ATrackCombination *combo=new ATrackCombination(); //hold the stuff

    bool ok;
    QString name=QInputDialog::getText((QWidget*)QObject::parent(),tr("Track Combination Name"),tr("Combination Name:"),QLineEdit::Normal,combo->getName(),&ok);

    if (ok) //Proceed only if OK clicked
    {
        combo->setName(name);
        //Get rows. Since all colums should be selected, might as well select column 0.
        QModelIndexList rows=selection->selectedRows(0);
        //Go through each row...
        for (int i=0;i<rows.size();i++)
            combo->addTrack(tracks[rows[i].row()]);

        //emit tracksAdded(combo);
    }
    else
    {
        delete combo;//We no need it...
    }
}

void AInterestingTrackTableModel::handleSelectionChanged(const QItemSelection& selected,const QItemSelection& deselected)
{
    // PERFORM DESELECTION
    //Deselection should be performed first, because in the case of singletrack selection, everything will be deselected anyways. Even though in theory
    //there shouldn't be any problems, might as well do this to keep safe.
    QModelIndexList idxs=deselected.indexes();
    for (int i=0;i<idxs.size();i++)
    {
        if (idxs[i].column()==0) //We are expecting entire row to be selected, so to avoid duplicate indexes we just check the one belonging to the first column
        {
            int id=tracks[idxs[i].row()]->trackID;
            emit entryDeselected(id);
        }
    }

    // PERFORM SELECTION
    idxs=selected.indexes();

    bool multi = ((QApplication::keyboardModifiers() & (Qt::ShiftModifier | Qt::ControlModifier)) > 0);


    for (int i=0;i<idxs.size();i++)
    {
        if (idxs[i].column()==0) //We are expecting entire row to be selected, so to avoid duplicate indexes we just check the one belonging to the first column
        {
            int id=tracks[idxs[i].row()]->trackID;
            emit entrySelected(id,multi);
        }
    }
}

void AInterestingTrackTableModel::clear()
{
    if (tracks.size()==0) return;
    emit beginRemoveRows(QModelIndex(),0,tracks.size()-1);
    tracks.clear();
    emit endRemoveRows();
}

void AInterestingTrackTableModel::getInterestingTracks()
{
    clear();
    Aevent ievent = geo->XmlEvt->EventComplete;
    int ptMinimum = 2;
    for ( vector<ATrack>::iterator go = ievent.Tracks.begin(); go!=ievent.Tracks.end(); go++ )
    {
        if ( go->Type == go->eJet )
            addTrack(go->node->trackPointer);

        if ( go->Type == go->eMissingEt )
            addTrack(go->node->trackPointer);

        if ( go->pt >= ptMinimum )
        {
            if ( go->Type == go->eSTrack )
            {
                ASTrack* goo = go->getThisSTrack();
                if ( goo->code == 11 || goo->code == -11 ) //electrons
                {
                    addTrack(goo->node->trackPointer);
                }

                if ( abs ( goo->code ) == 13 ) //muons
                {
                    addTrack(go->node->trackPointer);
                }

                if ( goo->code == 22 ) //photons
                {
                    addTrack(goo->node->trackPointer);
                }
            }
        }
    }
}


