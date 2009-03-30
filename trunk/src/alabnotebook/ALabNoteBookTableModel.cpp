#include "ALabNoteBookTableModel.h"
#include <QDebug>
#include <QFont>

ALabNoteBookTableModel::ALabNoteBookTableModel(QObject* parent):QAbstractListModel(parent)
{
  _data=new ALabNoteBookData("ALabNoteBook");
}

ALabNoteBookTableModel::~ALabNoteBookTableModel()
{ }
  
int ALabNoteBookTableModel::rowCount( const QModelIndex& parent ) const
{
  QList<ALabNoteBookEntry*> entries=_data->entries();
  return entries.size();
}

QVariant ALabNoteBookTableModel::data( const QModelIndex& index, int role ) const
{
  if(!index.isValid())
    return QVariant();

  QList<ALabNoteBookEntry*> entries=_data->entries();
  ALabNoteBookEntry* entry=entries[index.row()];

  QFont font;
  switch(role)
    {
    case Qt::FontRole:
      if (_activeEntry==entry)
	{
	  font.setItalic(true);
	}
      return font;
      
    case Qt::DisplayRole:
      return entry->time().toString("yyyy-MM-dd h:mm:ss ap");
      
    case Qt::UserRole:
      return QVariant::fromValue<QObject*>(entry);

    default:
      return QVariant();

      
    }
}
									      
void ALabNoteBookTableModel::setLabNoteBookData(ALabNoteBookData *data)
{ 
  disconnect(_data,SIGNAL(updated()),
	     this,SLOT(refresh()));
  _data=data;
  connect(_data,SIGNAL(updated()),
	  this,SLOT(refresh()));
  refresh();
}

void ALabNoteBookTableModel::setActiveEntry(ALabNoteBookEntry *entry)
{
  _activeEntry=entry;
  refresh();
}

void ALabNoteBookTableModel::refresh()
{
  emit layoutAboutToBeChanged();
  emit layoutChanged();
}
