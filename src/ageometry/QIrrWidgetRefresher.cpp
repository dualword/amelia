#include "QIrrWidgetRefresher.h"

#include <QApplication>

QIrrWidgetRefresher::QIrrWidgetRefresher(QIrrWidget *p)
  :p(p),allModules(0)
{
  time.start();
}
  
io::IAttributes* QIrrWidgetRefresher::createUserData(ISceneNode *forSceneNode)
{ return 0; }

void QIrrWidgetRefresher::OnCreateNode(ISceneNode *node)
{
  if(allModules!=0)
  {
	if ( ( node->getType() != ESNT_EMPTY ) && ( node->getType() != ESNT_LIGHT ) )
		if ( ( node->getID() != 4 ) && ( node->getID() !=7 ) )
		{
			node->setID ( 0x0008 );
			allModules->push_back ( node );
		}
  }
  if( time.elapsed()>=40)
    {
      QApplication::processEvents();
      p->forceUpdate();
      time.restart();
    }
}

void QIrrWidgetRefresher::OnReadUserData(ISceneNode *forSceneNode, io::IAttributes *userData)
{ }
