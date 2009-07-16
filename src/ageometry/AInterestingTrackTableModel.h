#ifndef AINTERESTINGTRACKTABLEMODEL_H_
#define AINTERESTINGTRACKTABLEMODEL_H_

#include <QAbstractTableModel>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QList>

#include <aeventmanager/AEvent.h>
#include <aeventmanager/ATrackCombination.h>

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

  void setEvent(AEvent* event);
  void clear();

 signals:
  void entrySelected(unsigned int trackID,bool shift);
  void entryDeselected(unsigned int trackID);

private:
  QList<ATrack*> tracks;

  QItemSelectionModel *selection;
};

#endif
