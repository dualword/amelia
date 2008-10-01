#ifndef ATRACKTABLEMODEL_H_
#define ATRACKTABLEMODEL_H_

#include <QAbstractTableModel>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QList>

#include "AXmlEvent.h"
#include "ATrackCombination.h"

class ATrackTableModel : public QAbstractTableModel {
  Q_OBJECT
public:
  static unsigned int selectionID; //Used for assigning reference ID's to tracks

  ATrackTableModel(QWidget* parent=0);
  ~ATrackTableModel();

  int rowCount(const QModelIndex& root) const;
  int columnCount(const QModelIndex& root) const;
  QVariant data(const QModelIndex &index, int role=Qt::DisplayRole ) const;
  QVariant headerData (int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
  void sort(int column, Qt::SortOrder order);

  void addTable(QAbstractItemView* table);

public slots:
  void addTrack(track* strack);
  void deleteSelectedTracks();
  void handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
  void combineSelectedTracks();
  
  void clear();

 signals:
  void entrySelected(int trackID,bool shift);
  void entryDeselected(int trackID);

  void tracksCombined(ATrackCombination* combo);

private:
  QList<track*> tracks;

  QItemSelectionModel *selection;
};

#endif
