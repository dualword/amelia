#ifndef AINTERESTINGTRACKTABLEMODEL_H_
#define AINTERESTINGTRACKTABLEMODEL_H_

#include <QAbstractTableModel>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QList>

#include "ATrackCombination.h"
#include "AGeometry.h"

class AInterestingTrackTableModel : public QAbstractTableModel {
  Q_OBJECT
public:
  static unsigned int selectionID; //Used for assigning reference ID's to tracks

  AInterestingTrackTableModel(QWidget* parent=0);
  ~AInterestingTrackTableModel();

  int rowCount(const QModelIndex& root) const;
  int columnCount(const QModelIndex& root) const;
  QVariant data(const QModelIndex &index, int role=Qt::DisplayRole ) const;
  QVariant headerData (int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
  void sort(int column, Qt::SortOrder order);

  void addTable(QAbstractItemView* table);

public slots:
  void addTrack(ATrack* strack);
  void handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
  void addToSelectedTracks();

  void setEvent(AEvent* event);
  void clear();

 signals:
  void entrySelected(int trackID,bool shift);
  void entryDeselected(int trackID);

  void tracksAdded(ATrack* track);

private:
  QList<ATrack*> tracks;

  QItemSelectionModel *selection;
};

#endif
