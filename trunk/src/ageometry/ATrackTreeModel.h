#ifndef ATRACKTABLEMODEL_H_
#define ATRACKTABLEMODEL_H_

#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QList>

#include <aeventmanager/ATrack.h>
#include <aeventmanager/ATrackCombination.h>
#include <aeventmanager/AEvent.h>
#include <aeventmanager/ATrackCollection.h>

#include "QAbstractTreeItem.h"

class ATrackTreeModel : public QAbstractItemModel 
{
public:
  static unsigned int selectionID; //Used for assigning reference ID's to tracks

  ATrackTreeModel(QWidget* parent=0);
  ~ATrackTreeModel();
  
  QModelIndex index(int row, int column, const QModelIndex& parent=QModelIndex()) const;
  QModelIndex parent(const QModelIndex &index) const;
  int rowCount(const QModelIndex& root) const;
  int columnCount(const QModelIndex& root) const;
  bool setData(const QModelIndex&, const QVariant&, int);
  QVariant data(const QModelIndex &index, int role=Qt::DisplayRole ) const;
  QVariant headerData (int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
  Qt::ItemFlags flags(const QModelIndex&) const;
  void sort(int column, Qt::SortOrder order);

  Qt::DropActions supportedDropActions() const;
  bool dropMimeData(const QMimeData *data,Qt::DropAction action,int row,int column,const QModelIndex &parent);
  QStringList mimeTypes() const;
  QMimeData* mimeData(const QModelIndexList &indexes) const;

  void addView(QAbstractItemView* view);

public slots:
  void handleNewEventLoaded(AEvent*);

  void deleteSelectedTracks();
  void handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
  void combineSelectedTracks();

  void clear();
  void refresh();

signals:
  void entrySelected(int trackID,bool shift);
  void entryDeselected(int trackID);

  void combineButtonEnabled(bool);

private:
  ATrackCollection *analysisData;

  QItemSelectionModel *selection;

  QList<QAbstractTreeItem*> treeItems;

  QList<ATrack*> tracks() const;
  void setTracks(QList<ATrack*>);

  void performSelection(ATrack *track,bool multi);
  void performDeselection(ATrack *track);

  void clearInternalTree();
  void clearInternalTree(QAbstractTreeItem *parent);
  void createInternalTree();
  void createInternalTree(ATrack *track,QAbstractTreeItem *parent,int row);

  QAbstractTreeItem* findTreeItem(QObject *data,QAbstractTreeItem* parentItem,int parentRow) const;

  Q_OBJECT
};

#endif
