#include "AMonitor.h"
#include <QDebug>

#include <QGraphicsScene>
#include <QWidget>
#include <QLayout>

AMonitor::AMonitor(QString _name,QString _description,QWidget *w,QGraphicsItem* parent)
  :name(_name),description(_description),QGraphicsProxyWidget(parent)
{ 
  setWidget(w);
  
  //Configure the description text...
  QFont font;
  font.setBold( true );
  font.setPointSize( 24 );
  font.setStyleHint( QFont::Times );
  font.setStyleStrategy( QFont::PreferAntialias );
  
  if(!description.isEmpty())
    {
      setAcceptHoverEvents(true);

      descItem.setBrush(QColor("white"));
      descItem.setFont(font);
      descItem.setText(description);
      scene()->addItem(&descItem);

      descItem.setPos(QPointF(-descItem.boundingRect().width(),0));

      descTimeline.setDuration(500);
      descTimeline.setCurveShape(QTimeLine::EaseInCurve);
      descAnimator.setTimeLine(&descTimeline);
      descAnimator.setItem(&descItem);
      descAnimator.setPosAt(0,QPointF(-descItem.boundingRect().width(),50));
      descAnimator.setPosAt(1,QPointF(400,50));
    }

  setWidgetEnabled(false);
}

void AMonitor::setWidget(QWidget *w)
{
  tmpwidget=w;

  QGraphicsProxyWidget::setWidget(w);
}

void AMonitor::storeWidget()
{
  geo=tmpwidget->geometry();
  setWidgetEnabled(true);

  descItem.setPos(QPointF(-descItem.boundingRect().width(),0));

  QGraphicsProxyWidget::setWidget(0);
}

void AMonitor::restoreWidget()
{
  setWidgetEnabled(false);
  QWidget* parent=tmpwidget->parentWidget();
  if(parent)
    {
      QLayout *layout=parent->layout();
      layout->removeWidget(tmpwidget);
    }
  

  tmpwidget->setParent(0);
  tmpwidget->setGeometry(geo);
  QGraphicsProxyWidget::setWidget(tmpwidget);
  tmpwidget->show();
  tmpwidget->setUpdatesEnabled(true);
}

void AMonitor::setWidgetEnabled(bool status)
{
  if(tmpwidget == 0) return;

  QList<QWidget*> children=tmpwidget->findChildren<QWidget*>();
  for(int i=0;i<children.size();i++)
    if(children[i]->parent()==tmpwidget)
      {
	bool oldstatus=children[i]->isEnabled();
	children[i]->setEnabled(widgetsEnabled[children[i]] && status);
	widgetsEnabled[children[i]]=oldstatus;
      }

  if(status)
    tmpwidget->grabKeyboard();
}

void AMonitor::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  descTimeline.setDirection(QTimeLine::Forward);
  if(descTimeline.state()!=QTimeLine::Running) descTimeline.start();
}

void AMonitor::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  descTimeline.setDirection(QTimeLine::Backward);
  if(descTimeline.state()!=QTimeLine::Running) descTimeline.start();
}

void AMonitor::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  emit clicked();
}
