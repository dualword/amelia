#include "AComboTableModel.h"

#include <QDebug>

AComboTableModel::AComboTableModel(QWidget* parent):QAbstractTableModelWithContextMenu(parent)
{
  analysisData=new ATrackCollection("AGeometry");

  //Use this for selecting tracks by clicking on the table
  selection=new QItemSelectionModel(this);//parent->selectionModel();
}

AComboTableModel::~AComboTableModel()
{
  selection->deleteLater();
}

void AComboTableModel::handleNewEventLoaded(AEvent *event)
{
  disconnect(analysisData,SIGNAL(updated()),
	     this,SLOT(refresh()));

  analysisData=event->getAnalysisData<ATrackCollection>("AGeometry");

  connect(analysisData,SIGNAL(updated()),
	  this,SLOT(refresh()));
  
  refresh();
}

void AComboTableModel::setCombinations(QList<ATrackCombination *> combos)
{
  //Element by element cast..
  QList<ATrack*> copy;
  for(int i=0;i<combos.size();i++)
    {
      copy[i]=combos[i];
    }

  analysisData->setCollection("combined_tracks",copy);
}

QList<ATrackCombination*> AComboTableModel::combinations() const
{
  //Element by element cast..
  QList<ATrack*> original=analysisData->getCollection("combined_tracks");
  QList<ATrackCombination*> combos;
  for(int i=0;i<original.size();i++)
    {
      combos.append((ATrackCombination*)original[i]);
    }

  return combos;
}


void AComboTableModel::addTable(QAbstractItemView* table)
{
  table->setModel(this);
  table->setSelectionModel(selection);
  table->setSelectionBehavior(QAbstractItemView::SelectRows);
}

int AComboTableModel::rowCount(const QModelIndex& root) const
{
  return combinations().size();
}

int AComboTableModel::columnCount(const QModelIndex& root) const
{
  return 3;
}

QVariant AComboTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if(index.row()>=combinations().size())
        return QVariant();

  if (role == Qt::DisplayRole) {
    switch(index.column()) {
    case 0:
      return combinations()[index.row()]->name();
    case 1:
      return QString::number(combinations()[index.row()]->size());
    case 2:
      return QString::number(combinations()[index.row()]->getInvariantMass());
    }
  }
  else if(role == QAbstractTableModelWithContextMenu::MenuDataRole)
    {
      return QVariant::fromValue<QObject *>(combinations()[index.row()]);
    }
 
  return QAbstractTableModelWithContextMenu::data(index,role);
}

QVariant AComboTableModel::headerData (int section, Qt::Orientation orientation, int role) const
{
  if(role!=Qt::DisplayRole)
    return QVariant();

  if(orientation == Qt::Horizontal) {
    switch(section) {
    case 0:
      return "Name";
    case 1:
      return "# Tr";
    case 2:
      return "Inv.Mass";
    }
  }

  if(orientation == Qt::Vertical)
    return (section+1);
  return QVariant();
}

void AComboTableModel::sort(int column, Qt::SortOrder order) {
  //Do a bubble sort... Switch to something faster later?

  emit layoutAboutToBeChanged();
  QList<ATrackCombination*> combinations=this->combinations();
  for(int i=0;i<combinations.size()-1;i++) {
    for(int j=i;j<combinations.size()-1;j++) {
      switch(column) {
      case 0:
	if( (combinations[j]->name()<combinations[j+1]->name() && order==Qt::AscendingOrder) ||
	    (combinations[j]->name()>combinations[j+1]->name() && order==Qt::DescendingOrder) ) {
	  combinations.swap(i,j+1);
	}
	break;
      case 1:
	if( (combinations[j]->size()<combinations[j+1]->size() && order==Qt::AscendingOrder) ||
	    (combinations[j]->size()>combinations[j+1]->size() && order==Qt::DescendingOrder) ) {
	  combinations.swap(i,j+1);
	}
	break;
      default:
	break;
      }
    }
  }
  setCombinations(combinations);
  emit layoutChanged();
}

void AComboTableModel::addCombination(ATrackCombination *comb)
{
  //Make sure every track has a selection ID..
  QList<ATrackCombination*> combinations=this->combinations();
  combinations.append(comb);
  setCombinations(combinations);
}

void AComboTableModel::deleteSelectedCombinations()
{
  //Get rows. Since all colums should be selected, might as well select column 0.
  QModelIndexList rows=selection->selectedRows(0);
  //Go through each row...
  QList<ATrackCombination*> combinations=this->combinations();
  for(int i=0;i<rows.size();i++)
    {
      //We gonna remove a row... The row in our list is given by (selected row # - rows already removed).
      //This is because the selected row # does not change, but it's position in list does as we remove rows.
      emit beginRemoveRows(QModelIndex(),rows[i].row()-i,rows[i].row()-i);
      delete combinations.takeAt(rows[i].row()-i);
      emit endRemoveRows();
    }
  setCombinations(combinations);
}

void AComboTableModel::refresh()
{
  emit layoutAboutToBeChanged();
  emit layoutChanged();
}

void AComboTableModel::clear()
{
  analysisData=new ATrackCollection("AGeometry");
  refresh();
}
