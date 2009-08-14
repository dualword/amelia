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

//! Model that shows a collection of event objects
class ATrackTreeModel : public QAbstractItemModelWithContextMenu
{
public:
  //! Constructor
  ATrackTreeModel(QWidget* parent=0);
  //! Destructor
  ~ATrackTreeModel();

  /*!
   * \return Current collection name
   */
  QString collectionName();

  /*!
   * Helper function that adds this model to a view, along with a sort filter view
   * \param view The view to add to
  */
  void addView(QAbstractItemView *view);
  
  /* Standard Qt model/view methods */
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

public slots:
  /*!
   * Set new event and change the data displayed
   * \param event The event from which to get the collection
   */
  void setEvent(AEvent *event);
  
  /*!
   * Use a different collection form the current event
   * \param name
   */
  void setCollectionName(QString name);
  
  /*!
   * Clear everything
   */
  void clear();

  /*!
   * Refresh collection between the currently
   * displayed data and the data stored in the
   * collection
   */
  void refresh();

private:
  //! The name of the collection stored in the event
  QString _collectionName;
  
  //! The collection
  AEventObjectCollection *analysisData;
  //! The event
  AEvent *_event;
  
  /*!
   * Updates the analysis data object from the current collection and current event.
   */
  void updateAnalysisData();

  /* Internal representation of the tree functions */
  //! The top item
  QAbstractTreeItem *topItem;

  //! Clear the tree
  void clearInternalTree();
  //! Create the tree from the analysis item
  QAbstractTreeItem* createInternalTree();
  //! Recusrive function that loops over the children of track, and creates a substree, if this is a combination
  void createInternalTree(AEventObject *track,QAbstractTreeItem *parent);
  //! Synchronize the items between the two trees, an emit the correct model/view signals
  void synchronizeItems(QAbstractTreeItem *newItem,QAbstractTreeItem *oldItem,QModelIndex index);

  Q_OBJECT
};

#endif
