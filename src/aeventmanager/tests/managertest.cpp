#include <aeventmanager/AXmlEvent.h>
#include <aeventmanager/AEventPackage.h>
#include <aeventmanager/AEventManager.h>
#include <aeventmanager/AEventManagerScene.h>
#include <aeventmanager/AEventManagerTreeView.h>

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QTreeView>
#include <QHBoxLayout>

#include "AShittyHistogram.h"
#include "ManagerTestHelper.h"


int main(int argc,char* argv[])
{
  QApplication app(argc,argv);
  
  QWidget win;
  win.resize(500,300);
  
  QHBoxLayout layout(&win);

  AEventManager manager;
  manager.load();
  AEventManagerScene model(&manager);

  AEventManagerTreeView tree(&win);
  tree.setModel(&model);
  layout.addWidget(&tree);

  QGraphicsScene scene(&win);
  QGraphicsView view(&scene,&win);
  layout.addWidget(&view);
  AShittyHistogram hist(1);
  scene.addItem(&hist);

  ManagerTestHelper help(&hist,&model);
 
  help.connect(&tree,SIGNAL(eventClicked(AEvent*)),
	       &help,SLOT(handleNewEvent(AEvent*)));

  win.show();

  return app.exec();
}
