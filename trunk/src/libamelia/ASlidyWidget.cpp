#include "ASlidyWidget.h"

#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <QMoveEvent>

ASlidyWidget::ASlidyWidget(QWidget *parent)
  :QWidget(parent)
{ 
  setLayout(new QVBoxLayout());

  setStyleSheet("background: gray;");
}

void ASlidyWidget::addWidget(QWidget *wdg)
{
  resize(wdg->size());
  layout()->addWidget(wdg);
}

void ASlidyWidget::moveEvent(QMoveEvent *event)
{

}

void ASlidyWidget::paintEvent(QPaintEvent *event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
