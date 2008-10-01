#include "AComboTableModel.h"

AComboTableModel::AComboTableModel(QWidget* parent):QAbstractTableModel(parent)
{
	//Use this for selecting tracks by clicking on the table
	selection=new QItemSelectionModel(this);//parent->selectionModel();
}

AComboTableModel::~AComboTableModel()
{
  for(int i=0;i<combs.size()-1;i++)
      delete combs.takeAt(i);

  selection->deleteLater();
}

void AComboTableModel::addTable(QAbstractItemView* table)
{
	table->setModel(this);
	table->setSelectionModel(selection);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
}

int AComboTableModel::rowCount(const QModelIndex& root) const
{
  return combs.size();
}

int AComboTableModel::columnCount(const QModelIndex& root) const
{
  return 3;
}

QVariant AComboTableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if(index.row()>=combs.size())
        return QVariant();

  if (role == Qt::DisplayRole) {
    switch(index.column()) {
    case 0:
      return combs[index.row()]->getName();
    case 1:
      return QString::number(combs[index.row()]->size());
    case 2:
      return QString::number(combs[index.row()]->getInvariantMass());
    }
  }
  return QVariant();
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
  for(int i=0;i<combs.size()-1;i++) {
    for(int j=i;j<combs.size()-1;j++) {
      switch(column) {
      case 0:
	if( (combs[j]->getName()<combs[j+1]->getName() && order==Qt::AscendingOrder) ||
	    (combs[j]->getName()>combs[j+1]->getName() && order==Qt::DescendingOrder) ) {
	  combs.swap(i,j+1);
	}
	break;
      case 1:
	if( (combs[j]->size()<combs[j+1]->size() && order==Qt::AscendingOrder) ||
	    (combs[j]->size()>combs[j+1]->size() && order==Qt::DescendingOrder) ) {
	  combs.swap(i,j+1);
	}
	break;
      default:
	break;
      }
    }
  }
  emit layoutChanged();
}

void AComboTableModel::addCombination(ATrackCombination *comb)
{
  //Make sure every track has a selection ID..
  comb->fixIDs();

  beginInsertRows(QModelIndex(),0,0);
  combs.append(comb);
  endInsertRows();
}

void AComboTableModel::deleteSelectedCombinations()
{
  //Get rows. Since all colums should be selected, might as well select column 0.
  QModelIndexList rows=selection->selectedRows(0);
  //Go through each row...
  for(int i=0;i<rows.size();i++)
    {
      //We gonna remove a row... The row in our list is given by (selected row # - rows already removed).
      //This is because the selected row # does not change, but it's position in list does as we remove rows.
      emit beginRemoveRows(QModelIndex(),rows[i].row()-i,rows[i].row()-i);
      delete combs.takeAt(rows[i].row()-i);
      emit endRemoveRows();
    }
}

void AComboTableModel::clear()
{
  if(combs.size()==0) return;
  emit beginRemoveRows(QModelIndex(),0,combs.size()-1);
  combs.clear();
  emit endRemoveRows();
}
