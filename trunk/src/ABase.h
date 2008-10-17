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

struct VirtualScreen {
  QWidget* widget;

  QMenuBar* menubar;
  QList<QToolBar *> toolbars;
};

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
  void on_QuitButton_clicked();
  void on_MenuButton_activated();
  void on_BackButton_activated();
  void on_QuitButton_activated();
  
  void addLevel( QString level );
  void changeToLevel(QString level);

protected slots:
  void timerEvent(QTimerEvent *);
  void keyPressEvent(QKeyEvent *);
  bool eventFilter(QObject *obj, QEvent *event);

private:
  //Holds a list of the virual screens loaded
  QMap<QString, VirtualScreen*> screens;

  //Base widgets that control stuff
  QWidget *center;
  QWidget *area;
  QGridLayout *layout;

  //Needed amount to move to show the new layout
  QPoint delta;

  //Current layout
  QString current;

  bool geoNotLoaded;
};

#endif // ABASE_H
