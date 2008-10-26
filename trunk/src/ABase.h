#ifndef ABASE_H
#define ABASE_H

#include <QString>
#include <QMainWindow>
#include <QGridLayout>
#include <QFile>
#include <QMenuBar>
#include <QToolBar>
#include <QDebug>
#include <QMessageBox>
#include <QTimerEvent>
#include <QWidget>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QTime>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QSignalMapper>

#include "QGraphicsClickablePixmapItem.h"

class ABase : public QMainWindow
{
  Q_OBJECT
  
  public:
  ABase ( QWidget *parent=0 );
  ~ABase();
	  
public slots:
  void on_GeoButton_clicked();
  void on_MenuButton_clicked();
  void on_BackButton_clicked();
  void on_CompizButton_clicked();
  void on_QuitButton_clicked();
  void on_MenuButton_activated();
  void on_BackButton_activated();
  void on_QuitButton_activated();
  
  void addLevel( QString level );
  void changeToLevel(const QString& level);

  void changeToMenu();

  void animationFinished();
protected:
  void keyPressEvent(QKeyEvent *);

  QPointF calculateScaledWidgetGroupPosition();
  void updatePixmaps();
  void setupViewport();

private:
  //Holds a list of the virual screens loaded
  QMap<QString, QWidget*> widgets;
  QMap<QString, QGraphicsClickablePixmapItem*> items;

  //Center...
  QWidget center;
  QGridLayout layout;

  //The menu scene and widget
  QGraphicsView menuWidget;
  QGraphicsScene menu;
  QGraphicsItemGroup widgetGroup;

  QSignalMapper mapper;

  //Timer used for animations
  QTimeLine timer;
  QGraphicsItemAnimation animation;

  //Current layout
  QString current;
  QString previous;

  
};

#endif // ABASE_H
