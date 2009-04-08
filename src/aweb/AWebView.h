#ifndef AWEBVIEW_H_
#define AWEBVIEW_H_

#include <QMainWindow>
#include <QSignalMapper>
#include <QWebView>
#include <QResizeEvent>

#include "ADropDownMenu.h"

class AWebView : public QMainWindow
{
 public:
  AWebView(QWidget* parent=0);
  ~AWebView();

  void addButton(const QString& text,const QString& link);
  void setURL(const QString& url);

 public slots:
  void handleButtonPressed(const QString& link);

 signals:
  void linkTriggered(QString link);

 protected:
  void resizeEvent(QResizeEvent* event);

 private:
  QSignalMapper buttonMapper;
  ADropDownMenu* menu;
  QWebView* MainWebView;

  Q_OBJECT
};

#endif //AWEBVIEW_H_
