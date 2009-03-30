#ifndef ALABNOTEBOOKTABLEMODEL_H_
#define ALABNOTEBOOKTABLEMODEL_H_

#include <QAbstractListModel>

#include "ALabNoteBookData.h"

class ALabNoteBookTableModel : public QAbstractListModel
{
public:
  ALabNoteBookTableModel(QObject* parent=0);
  ~ALabNoteBookTableModel();
  
  int rowCount( const QModelIndex & parent = QModelIndex() ) const;
  
  QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
									      
public slots:
  void setLabNoteBookData(ALabNoteBookData *data);
  void setActiveEntry(ALabNoteBookEntry *entry);
  void refresh();

private:
  ALabNoteBookData *_data;
  ALabNoteBookEntry *_activeEntry;

  Q_OBJECT
};

#endif //ALABNOTEBOOKTABLEMODEL_H_
