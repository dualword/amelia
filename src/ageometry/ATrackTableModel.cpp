#include "ATrackTableModel.h"

#include <QApplication>
#include <QInputDialog>
#include <QDebug>

unsigned int ATrackTableModel::selectionID=0;

ATrackTableModel::ATrackTableModel(QWidget* parent):QAbstractTableModel(parent)
{
  analysisData=new AEventAnalysisData("AGeometry");

  //Use this for selecting tracks by clicking on the table
  selection=new QItemSelectionModel(this);//parent->selectionModel();
  connect(selection,SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
	  this,SLOT(handleSelectionChanged(const QItemSelection&,const QItemSelection&)));
}

ATrackTableModel::~ATrackTableModel()
{
  selection->deleteLater();
}

void ATrackTableModel::handleNewEventLoaded(AEvent *event)
{
  disconnect(analysisData,SIGNAL(updated()),
	     this,SLOT(refresh()));

  analysisData=event->getAnalysisData("AGeometry");

  connect(analysisData,SIGNAL(updated()),
	  this,SLOT(refresh()));

  refresh();
}

void ATrackTableModel::setTracks(QList<ATrack *> tracks)
{
  analysisData->setCollection("bookmarked_tracks",tracks);
}

QList<ATrack*> ATrackTableModel::tracks() const
{
  return analysisData->getCollection("bookmarked_tracks");
}

void ATrackTableModel::addTable(QAbstractItemView* table)
{
  table->setModel(this);
  table->setSelectionModel(selection);
  table->setSelectionBehavior(QAbstractItemView::SelectRows);
}

int ATrackTableModel::rowCount(const QModelIndex& root) const
{
  return tracks().size();
}

int ATrackTableModel::columnCount(const QModelIndex& root) const
{
    return 2;
}

QVariant ATrackTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();
  
  if (index.row()>=tracks().size())
    return QVariant();
  
  if (role == Qt::DisplayRole)
    {
      switch (index.column())
        {
        case 0:
	  return tracks().at(index.row())->name();
        case 1:
	  if (tracks().at(index.row())->type() == ATrack::eSTrack)
            {
	      ASTrack* STrack = static_cast<ASTrack*>(tracks().at(index.row()));
	      return QString::number(STrack->Pt());
            }
	  else if (tracks().at(index.row())->type() == ATrack::eJet)
            {
	      AJet* Jet = static_cast<AJet*>(tracks().at(index.row()));
	      return QString::number(Jet->et);
            }
	  else return QString("N/A");
        case 2:
	  if (tracks().at(index.row())->type() == ATrack::eSTrack)
            {
	      ASTrack* STrack = static_cast<ASTrack*>(tracks().at(index.row()));
	      return QString::number(STrack->Mlv);
            }
	  else return QString("N/A");
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
	  /*case 2:
            return "M(lv)";*/
	}
    }
  
  if (orientation == Qt::Vertical)
    {
      QList<ATrack *> tracks=this->tracks();
      return QString::number(tracks[section]->trackID());
      //TODO Renable selection ID
      //selectionID());
    }
  return QVariant();
}

void ATrackTableModel::sort(int column, Qt::SortOrder order)
{
  //Do a bubble sort... Switch to something faster later?
  QList<ATrack*> tracks=this->tracks();
  emit layoutAboutToBeChanged();
  for (int i=0;i<tracks.size()-1;i++)
    {
      for (int j=i;j<tracks.size()-1;j++)
        {
	  switch (column)
            {
            case 0:
	      if ( (tracks[j]->trackID()<tracks[j+1]->trackID() && order==Qt::AscendingOrder) ||
		   (tracks[j]->trackID()>tracks[j+1]->trackID() && order==Qt::DescendingOrder) )
                {
		  tracks.swap(i,j+1);
                }
	      break;
            case 1:
	      if ( (tracks[j]->name()<tracks[j+1]->name() && order==Qt::AscendingOrder) ||
		   (tracks[j]->name()>tracks[j+1]->name() && order==Qt::DescendingOrder) )
                {
		  tracks.swap(i,j+1);
                }
	      break;
            case 2:
	      if ( (tracks[j]->Pt()<tracks[j+1]->Pt() && order==Qt::AscendingOrder) ||
		   (tracks[j]->Pt()>tracks[j+1]->Pt() && order==Qt::DescendingOrder) )
                {
		  tracks.swap(i,j+1);
                }
	      break;
            default:
	      break;
            }
        }
    }
  setTracks(tracks);
  emit layoutChanged();
}

void ATrackTableModel::deleteSelectedTracks()
{
  //Get rows. Since all colums should be selected, might as well select column 0.
  QModelIndexList rows=selection->selectedRows(0);
  //Go through each row...
  QList<ATrack *> tracks=this->tracks();
  for (int i=0;i<rows.size();i++)
    {
      //We gonna remove a row... The row in our list is given by (selected row # - rows already removed).
      //This is because the selected row # does not change, but it's position in list does as we remove rows.
      tracks.removeAt(rows[i].row()-i);
    }
  setTracks(tracks);
}

void ATrackTableModel::combineSelectedTracks()
{
  ATrackCombination *combo=new ATrackCombination(); //hold the stuff
  bool ok;
  QString name=QInputDialog::getText((QWidget*)QObject::parent(),
				     tr("Track Combination Name"),
				     tr("Combination Name:"),
				     QLineEdit::Normal,
				     combo->name(),
				     &ok);

  if (ok) //Proceed only if OK clicked
    {
      combo->setName(name);
      //Get rows. Since all colums should be selected, might as well select column 0.
      QModelIndexList rows=selection->selectedRows(0);
      //Go through each row...
      for (int i=0;i<rows.size();i++)
	combo->addTrack(tracks()[rows[i].row()]);
      
      QList<ATrack *> tracks=analysisData->getCollection("combined_tracks");
      tracks.append(combo);
      analysisData->setCollection("combined_tracks",tracks);
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
	  int id=tracks()[idxs[i].row()]->trackID();
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
	  int id=tracks()[idxs[i].row()]->trackID();
	  emit entrySelected(id,multi);
        }
    }
}

void ATrackTableModel::refresh()
{
  emit layoutAboutToBeChanged();
  emit layoutChanged();
}

void ATrackTableModel::clear()
{
  analysisData=new AEventAnalysisData("AGeometry");
  refresh();
}
