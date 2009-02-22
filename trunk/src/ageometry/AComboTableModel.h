#ifndef ACOMBOTABLEMODEL_H_
#define ACOMBOTABLEMODEL_H_

#include <QAbstractTableModel>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QList>

#include "ATrackCombination.h"
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
  void addCombination(ATrackCombination* combo);
  void deleteSelectedCombinations();
  void clear();

private:
  QList<ATrackCombination*> combs;

  QItemSelectionModel *selection;
};

#endif
