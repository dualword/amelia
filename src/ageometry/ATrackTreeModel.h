#ifndef ATRACKTABLEMODEL_H_
#define ATRACKTABLEMODEL_H_

#include <QAbstractItemView>
#include <QModelIndex>
#include <QList>

#include <aeventmanager/AEventObject.h>
#include <aeventmanager/ATrackCombination.h>
#include <aeventmanager/AEvent.h>
#include <aeventmanager/AEventObjectCollection.h>

#include "QAbstractItemModelWithContextMenu.h"
#include "QAbstractTreeItem.h"

class ATrackTreeModel : public QAbstractItemModelWithContextMenu
{
public:
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
  bool removeRows(int row, int count,const QModelIndex& parent=QModelIndex());

  Qt::DropActions supportedDropActions() const;
  bool dropMimeData(const QMimeData *data,Qt::DropAction action,int row,int column,const QModelIndex &parent);
  QStringList mimeTypes() const;
  QMimeData* mimeData(const QModelIndexList &indexes) const;

  void addView(QAbstractItemView *view);

public slots:
  void handleNewEventLoaded(AEvent*);

  void clear();
  void refresh();

private:
  AEventObjectCollection *analysisData;

  QAbstractTreeItem *topItem;

  void clearInternalTree();
  QAbstractTreeItem* createInternalTree();
  void createInternalTree(AEventObject *track,QAbstractTreeItem *parent);

  void synchronizeItems(QAbstractTreeItem *newItem,QAbstractTreeItem *oldItem,QModelIndex index);

  Q_OBJECT
};

#endif
