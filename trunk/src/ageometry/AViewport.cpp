#include "AViewport.h"

#include "AGeometry.h"

AViewport::AViewport(QWidget* parent):QPushButton(parent)
{

  AGeometry* geo=parent->findChild<AGeometry*>("Geometry");
  if (geo)
    {
//        cout << "Connect one and all!" << endl;
        connect(this,SIGNAL(requestViewportSwitch(int)),geo,SLOT(setViewport(int)));
        connect(this,SIGNAL(requestUpdate(int)),geo,SLOT(renderViewport(int)));
        connect(geo,SIGNAL(viewportSwitched(int,int)),this,SLOT(handleViewportSwitch(int,int)));
        connect(geo,SIGNAL(viewportUpdated(int,QImage)),this,SLOT(handleViewportUpdate(int,QImage)));
        //connect(geo->XmlEvt,SIGNAL(eventChanged()),this,SLOT(update()));
    }

    highlightSkin=QImage(":/media/viewporthover_skin.png");
    viewportSkin=QImage(":/media/viewport_skin.png");

    handleViewportUpdate(_viewport,QImage());
}

AViewport::~AViewport() {}

void AViewport::setViewport(int cam)
{
    _viewport=cam;
}

int AViewport::viewport() const
{
    return _viewport;
}

void AViewport::handleViewportSwitch(int from,int to)
{
  if (_viewport==to)
    {
      _viewport=from;
    }
}

void AViewport::handleViewportUpdate(int cam,QImage newshot)
{
  if (cam==_viewport)
    {
      QImage normal(QSize(160,111),QImage::Format_ARGB32);
      QPainter painterN(&normal);
      painterN.fillRect(QRect(0,0,160,111),Qt::black);
      painterN.drawImage(QRect(4,4,152,103), newshot);
      painterN.setCompositionMode(QPainter::CompositionMode_SourceOver);
      painterN.drawImage(viewportSkin.rect(), viewportSkin);
      painterN.end();
      cachedNormal=QPixmap::fromImage(normal);

      QRect highlightRect=highlightSkin.rect();
      highlightRect.moveTo(4,4);
      QImage highlight(QSize(160,111),QImage::Format_ARGB32);
      QPainter painterH(&highlight);
      painterH.fillRect(QRect(0,0,160,111),Qt::black);
      painterH.drawImage(QRect(4,4,152,103), newshot);
      painterH.setCompositionMode(QPainter::CompositionMode_SourceOver);
      painterH.drawImage(highlightRect, highlightSkin);
      painterH.drawImage(viewportSkin.rect(), viewportSkin);
      painterH.end();
      cachedHighlighted=QPixmap::fromImage(highlight);

      repaint();
    }
}

void AViewport::paintEvent(QPaintEvent* event)
{
  //TODO: Figure out how to enable stylesheets
  QPainter painter(this);

  if (underMouse())
    painter.drawPixmap(cachedHighlighted.rect(),cachedHighlighted);
  else
    painter.drawPixmap(cachedNormal.rect(),cachedNormal);
  
  painter.end();
}

void AViewport::mousePressEvent(QMouseEvent* event)
{
    emit requestViewportSwitch(_viewport);
    QPushButton::mousePressEvent (event);
}

void AViewport::update()
{
    emit requestUpdate(_viewport);
}
