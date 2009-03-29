#ifndef ACOMBOTABLEMODEL_H_
#define ACOMBOTABLEMODEL_H_

#include <QAbstractTableModel>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QList>

#include <aeventmanager/AEvent.h>
#include <aeventmanager/ATrackCombination.h>
#include <aeventmanager/ATrackCollection.h>

#include "QAbstractTableModelWithContextMenu.h"

class AComboTableModel : public QAbstractTableModelWithContextMenu {
  Q_OBJECT
public:
  AComboTableModel(QWidget* parent=0);
  ~AComboTableModel();
  
  void addTable(QAbstractItemView*);
  
  int rowCount(const QModelIndex& root) const;
  int columnCount(const QModelIndex& root) const;
  QVariant data(const QModelIndex &index, int role=Qt::DisplayRole ) const;
  QVariant headerData (int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
  void sort(int column, Qt::SortOrder order);

public slots:
  void handleNewEventLoaded(AEvent*);

  void addCombination(ATrackCombination* combo);
  void deleteSelectedCombinations();
  
  void clear();
  void refresh();

private:
  ATrackCollection *analysisData;

  QItemSelectionModel *selection;

  QList<ATrackCombination*> combinations() const;
  void setCombinations(QList<ATrackCombination*>);
};

#endif
