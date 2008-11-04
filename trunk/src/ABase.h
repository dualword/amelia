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

struct ALayout
{
  QString uicfile;
  QString description;
  QWidget *widget;

  QGraphicsClickablePixmapItem *item;
  QGraphicsTextItem *descItem;

  QGraphicsItemAnimation *rotateAnimation;
  QTimeLine *descTimer;
  QGraphicsItemAnimation *descAnimation;
};

class ABase : public QMainWindow
{
  Q_OBJECT
  
  public:
  ABase ( QWidget *parent=0 );
  ~ABase();

  void addPixmapUpdateReason(QString,const char *);
	  
public slots:
  void on_GeoButton_clicked();
  void on_MenuButton_clicked();
  void on_BackButton_clicked();
  void on_CompizButton_clicked();
  void on_QuitButton_clicked();
  void on_MenuButton_activated();
  void on_BackButton_activated();
  void on_QuitButton_activated();
  
  void addLevel( QString level ,QString description="");
  void changeToLevel(const QString& level);

  void changeToMenu();

  void updatePixmap(const QString uifile);

  void animationFinished();

protected:
  void keyPressEvent(QKeyEvent *);
  bool eventFilter(QObject *obj, QEvent *event);

  QPointF calculateScaledWidgetGroupPosition();
  QPointF calculateBackgroundPosition();
  void setupViewport();

private:
  //Holds a list of the virual screens loaded
  QMap<QString, ALayout> widgets;

  //Center...
  QWidget center;
  QGridLayout layout;

  //The menu scene and widget
  QGraphicsView menuWidget;
  QGraphicsScene menu;
  QGraphicsItemGroup widgetGroup;
  QGraphicsPixmapItem *background;

  QSignalMapper mapper;
  QSignalMapper updateMapper;
  QMap<QString,QList<const char *> > reasonsToUpdate;

  //Timer used for animations
  QTimeLine timer;
  QGraphicsItemAnimation animation;
  QTimeLine parallaxTimer,parallaxTimerBg;
  QGraphicsItemAnimation parallaxAnimation,parallaxAnimationBg;

  //Current layout
  QString current;
  QString previous;

  
};

#endif // ABASE_H
