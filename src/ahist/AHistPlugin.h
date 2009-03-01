#ifndef AHISTPLUGIN_H
#define AHISTPLUGIN_H

#include <APlugin.h>

#include <QTableView>

#include "AllHistograms.h"

class AHistPlugin : public QObject,public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

 public:
  AHistPlugin(QObject* parent=0);
  ~AHistPlugin();
  
  void load();

 private:
  QTableView *combinedTracksTable;
  AllHistograms *hists;
};

#endif // AHISTPLUGIN_H

