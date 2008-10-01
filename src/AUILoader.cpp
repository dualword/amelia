#include "AUILoader.h"

AUILoader::AUILoader(QObject* parent):QUiLoader(parent) {}

//This functions needs to be overridden and creation of custom placeholders needs to be handled
QWidget* AUILoader::createWidget(const QString &className, QWidget* parent, const QString &name)
{
  QWidget *obj;
    if (className == "AGeometry")
    {
        obj= new AGeometry(parent);
    }
    else if (className == "AViewport")
    {
      obj = new AViewport(parent);

        if (name=="LeftViewport")
        {
	  ((AViewport*)obj)->setViewport(AGeometry::Orthogonal);
        }
        else if (name=="RightViewport")
        {
	  ((AViewport*)obj)->setViewport(AGeometry::Projective);
        }
    }
    else if (className == "ALayerGUI")
    {
      obj = new ALayerGUI(parent);
    } 
	else if(className == "AMainView")
	{
		obj = new AMainView(parent);
	}
    else
      {
	obj=QUiLoader::createWidget(className, parent, name);
      }

    obj->setObjectName(name);

    return obj;
}
