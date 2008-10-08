#include "ATrackTableModel.h"

#include <QApplication>
#include <QInputDialog>

unsigned int ATrackTableModel::selectionID=0;

ATrackTableModel::ATrackTableModel(QWidget* parent):QAbstractTableModel(parent)
{
    tracks=QList<ATrack*>();

    //Use this for selecting tracks by clicking on the table
	selection=new QItemSelectionModel(this);//parent->selectionModel();
	connect(selection,SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
			this,SLOT(handleSelectionChanged(const QItemSelection&,const QItemSelection&)));
}

ATrackTableModel::~ATrackTableModel()
{
	selection->deleteLater();
}

void ATrackTableModel::addTable(QAbstractItemView* table)
{
	table->setModel(this);
	table->setSelectionModel(selection);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
}

int ATrackTableModel::rowCount(const QModelIndex& root) const
{
    return tracks.size();
}

int ATrackTableModel::columnCount(const QModelIndex& root) const
{
    return 3;
}

QVariant ATrackTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row()>=tracks.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return tracks.at(index.row())->name.c_str();
        case 1:
            return QString::number(tracks.at(index.row())->pt);
        case 2:
            return QString::number(tracks.at(index.row())->Mlv);
        }
    }
    return QVariant();
}

QVariant ATrackTableModel::headerData (int section, Qt::Orientation orientation, int role) const
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
            return "M(lv)";
        }
    }

    if (orientation == Qt::Vertical)
    {
        return QString::number(tracks[section]->selectionID);
    }
    return QVariant();
}

void ATrackTableModel::sort(int column, Qt::SortOrder order)
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

void ATrackTableModel::addTrack(ATrack* strack)
{
    if (strack->selectionID==0)
        strack->selectionID=(++ATrackTableModel::selectionID);

    if (tracks.indexOf(strack)<0) //Dupe Check
    {
        qDebug() << "Adding track for " << strack->name.c_str();
        beginInsertRows(QModelIndex(),0,0);
        tracks.append(strack);
        strack->isInList=true;
        endInsertRows();
    }
}

void ATrackTableModel::deleteSelectedTracks()
{
    //Get rows. Since all colums should be selected, might as well select column 0.
    QModelIndexList rows=selection->selectedRows(0);
    //Go through each row...
    for (int i=0;i<rows.size();i++)
    {
        //We gonna remove a row... The row in our list is given by (selected row # - rows already removed).
        //This is because the selected row # does not change, but it's position in list does as we remove rows.
        emit beginRemoveRows(QModelIndex(),rows[i].row()-i,rows[i].row()-i);
        //tracks[rows[i].row-i]->isInList=false;
        tracks.removeAt(rows[i].row()-i);
        emit endRemoveRows();
    }
}

void ATrackTableModel::combineSelectedTracks()
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

        emit tracksCombined(combo);
    }
    else
    {
        delete combo;//We no need it...
    }
}

void ATrackTableModel::handleSelectionChanged(const QItemSelection& selected,const QItemSelection& deselected)
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

void ATrackTableModel::clear()
{
  if(tracks.size()==0) return;
  emit beginRemoveRows(QModelIndex(),0,tracks.size()-1);
  tracks.clear();
  emit endRemoveRows();
}
