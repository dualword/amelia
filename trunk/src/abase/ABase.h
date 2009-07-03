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

#include <QGraphicsClickableSimpleTextItem.h>
#include <QGraphicsItemLayout.h>
#include <QTimeLineProxy.h>
#include <QWaitForReady.h>
#include <APlugin.h>

#include "AMonitorGroup.h"
#include "AMonitor.h"

#include "ABaseDefines.h"

class ABASE_EXPORT ABase : public QMainWindow, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

public:
    ABase ( QWidget *parent=0 );
    ~ABase();

    void load();

    QWaitForReady* startConditions();
    void addStyleSheet(QString file);

    void     setFakeCentralWidget(QWidget *);
    QWidget* fakeCentralWidget();

    QString currentMonitor();
    QString currentGroup();

public slots:
    void addMonitor( QString level,QString group ,QWidget *widget,QString description="",Qt::Alignment align=Qt::AlignHCenter);
    void addGroup( QString groupName );
    void changeToMonitor(const QString& level);
    void changeToGroup(const QString& level);
    void changeToMenu();

    void showButtonGroup();
    void hideButtonGroup();

    void showEverything();
    void handleLoadingFinished();
    void animationFinished();

protected:
    void keyPressEvent(QKeyEvent *);
    bool eventFilter(QObject *obj, QEvent *event);
    void resizeEvent(QResizeEvent* event);

    QPointF calculateBackgroundPosition();
    void setupViewport();

private:
    //Center...
    QWidget center;
    QGridLayout layout;

    //The menu scene and widget
    QGraphicsView menuWidget;
    QGraphicsScene menu;
    QGraphicsPixmapItem *background;
    QMap<QString,AMonitorGroup*> monitorGroups;

    QGraphicsSimpleTextItem *loading;

    //Buttons
    QFont buttonFont;
    QSignalMapper buttonMapper;
    QGraphicsItemLayout buttonGroup;
    QGraphicsClickableSimpleTextItem quit;
    QGraphicsItemAnimation buttonAnimator;
    QTimeLine buttonTimeLine;

    QSignalMapper monitorMapper;

    //Timer used for animations
    QTimeLineProxy timer;
    QGraphicsItemAnimation animation;
    QTimeLine parallaxTimer,parallaxTimerBg;
    QGraphicsItemAnimation parallaxAnimation,parallaxAnimationBg;

    //Currently shown things
    QString _currentGroup;
    QString _currentMonitor;
    QString previousMonitor;

    QWidget *_fakeCentralWidget;
    QWaitForReady _startConditions;
};

#endif // ABASE_H

