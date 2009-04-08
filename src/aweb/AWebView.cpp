#include "AWebView.h"

#include "ui_browser.h"

#include <QDebug>

AWebView::AWebView(QWidget* parent)
  :QMainWindow(parent),MainWebView(0),menu(0)
{ 
  connect(&buttonMapper,SIGNAL(mapped(const QString&)),
	  this,SLOT(handleButtonPressed(const QString&)));

  Ui::BrowserWindow browserUI;
  browserUI.setupUi(this);
  MainWebView=findChild<QWebView*>("MainWebView");
  menu=new ADropDownMenu(this);
  menu->move(width()-151,20);
  menu->resize(151,121);
}

AWebView::~AWebView()
{ }

void AWebView::addButton(const QString& text,const QString& link)
{ 
  QAction *action=new QAction(text,menu);
  menu->addAction(action);
  
  buttonMapper.setMapping(action,link);
  connect(action,SIGNAL(triggered()),
	  &buttonMapper,SLOT(map()));
}

void AWebView::setURL(const QString& url)
{
  MainWebView->setUrl(url);
}

void AWebView::resizeEvent(QResizeEvent* event)
{
  if(menu!=0)
    menu->move(event->size().width()-151,20);
}

void AWebView::handleButtonPressed(const QString& link)
{
  emit linkTriggered(link);
}
