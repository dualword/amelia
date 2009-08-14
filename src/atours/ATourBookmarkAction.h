#ifndef ATOURBOOKMARKOBJECTACTION_H_
#define ATOURBOOKMARKOBJECTACTION_H_

#include "ATourAction.h"

#include <ageometry/ATrackTreeModel.h>
#include <ageometry/ALayerGUI.h>

#include <aeventmanager/AEventObjectCollection.h>

class ATourBookmarkAction : public ATourAction
{
public:
  Q_INVOKABLE ATourBookmarkAction();
  Q_INVOKABLE ATourBookmarkAction(QString msg);

  QString type();

  QPoint cursor();

  void loadFromXML(QDomElement actionElement);
  
  void act();
  void undo();

  void prepare();
  void cleanup();

private:
  int trackid;
  bool state;

  ALayerGUI *layerGUI;

  QSet<AEvent*> modifiedEvents; // Set of events that had their bookmark list modified. We need to loop over them during cleanup and clear them.

  AEventObject* objectOfInterest();
  AEventObjectCollection* bookmarkCollection();
  
  void removeFromBookmarks();
  void addToBookmarks();

  Q_OBJECT
};

#endif //ATOURBOOKMARKOBJECTACTION_H_
