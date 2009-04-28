#ifndef QIRRWIDGETREFRESHER_H_
#define QIRRWIDGETREFRESHER_H_

#include "QIrrWidget.h"

class QIrrWidgetRefresher : public irr::scene::ISceneUserDataSerializer
{
 public:
  vector<scene::ISceneNode*> *allModules;

  QIrrWidgetRefresher(QIrrWidget *parent);
  
  io::IAttributes *createUserData(ISceneNode *forSceneNode);
  void OnCreateNode(ISceneNode *node);
  void OnReadUserData(ISceneNode *forSceneNode, io::IAttributes *userData);
  
 private:
  QIrrWidget *p;
  QTime time;
};

#endif // QIRRWIDGETREFRESHER_H_
