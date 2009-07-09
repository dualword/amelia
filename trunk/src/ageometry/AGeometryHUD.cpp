#include "AGeometryHUD.h"

AGeometryHUD::AGeometryHUD(AGeometry *geo)
  :geo(geo),textNode(0)
{ 
  connect(geo,SIGNAL(finishedLoading()),
	  this,SLOT(setupElements()));
  connect(geo,SIGNAL(cameraSwitched(int)),
	  this,SLOT(handleNewCamera(int)));
  connect(geo,SIGNAL(cameraControlSwitched(bool)),
	  this,SLOT(handleCameraControl(bool)));

  geo->installEventFilter(this);
}

void AGeometryHUD::setupElements()
{
  gui=geo->getGUIEnvironment();
  
  core::dimension2d<u32> size=geo->getVideoDriver()->getScreenSize();

  core::rect<s32> textRect(100,size.Height-50,size.Width-100,size.Height);
  
  textNode=gui->addStaticText(L"",
			      textRect);
  textNode->setOverrideColor(SColor(255,255,255,255));
  textNode->setTextAlignment(EGUIA_CENTER,EGUIA_SCALE);
  textNode->setEnabled(false);
}

void AGeometryHUD::setText(QString text)
{ 
  textNode->setVisible(true);
  textNode->setText(text.toStdWString().c_str());
  geo->makeDirty();
}

void AGeometryHUD::clearText()
{ 
  if(textNode)
    {
      textNode->setVisible(false);
      textNode->setText(L"");
      geo->makeDirty();
    }
}

void AGeometryHUD::handleNewCamera(int camid)
{
  if(camid==AGeometry::FPS)
    setText("Press SPACE or double click to engage camera");
  else
    clearText();
}

void AGeometryHUD::handleCameraControl(bool grabbed)
{
  if(grabbed)
    setText("Press SPACE or double click to release camera");
  else
    setText("Press SPACE or double click to engage camera");
}

 bool AGeometryHUD::eventFilter(QObject *obj,QEvent *event)
 { 	 
   if(obj!=geo) return false; 	 
   
   if(event->type()==QEvent::Resize) 	 
     { 	 
       QResizeEvent *resizeEvent=(QResizeEvent*)(event); 	 
       QSize size=resizeEvent->size(); 	 
       core::rect<s32> textRect(100,size.height()-50,size.width()-100,size.height()); 	 
       if(textNode) 	 
	 { 	 
	   textNode->setRelativePosition(textRect); 	 
	   geo->makeDirty();
	 } 	 
     } 	 
   
   return false; 	 
 }
