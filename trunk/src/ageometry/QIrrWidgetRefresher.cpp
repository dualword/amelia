#include "QIrrWidgetRefresher.h"

#include <QApplication>

QIrrWidgetRefresher::QIrrWidgetRefresher(QIrrWidget *p)
  :p(p)
{
  time.start();
}
  
io::IAttributes* QIrrWidgetRefresher::createUserData(ISceneNode *forSceneNode)
{ }

void QIrrWidgetRefresher::OnCreateNode(ISceneNode *node)
{
  if( time.elapsed()>=40)
    {
      QApplication::processEvents();
      p->forceUpdate();
      time.restart();
    }
}

void QIrrWidgetRefresher::OnReadUserData(ISceneNode *forSceneNode, io::IAttributes *userData)
{ }
