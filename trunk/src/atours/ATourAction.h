#ifndef ATOURACTION_H_
#define ATOURACTION_H_

#include <QObject>
#include <QDomElement>
#include <QMap>
#include <QPoint>

class ATourAction : public QObject
{
public:
  ATourAction();

  virtual QString type();

  void setDuration(int);
  int duration();
  void setTime(int);
  int time();

  void setWidgetOfInterest(QString);
  virtual QString widgetOfInterest();

  bool isStateless();
  void setStateless(bool);

  bool isCursorable();
  void setCursorable(bool);

  void setNextAction(ATourAction*);
  ATourAction* nextAction();
  void setPreviousAction(ATourAction*);
  ATourAction* previousAction();

  bool canPerform(int frame);

  virtual void act();
  virtual void undo();
  virtual void update(double done);
  
  virtual void prepare();
  virtual void cleanup();

  virtual void loadFromXML(QDomElement actionElement);

  virtual QPoint cursor();

  void insertAction(ATourAction *action);
  ATourAction* getActionFor(int frame);

  static void addActionType(QMetaObject);
  static ATourAction* newInstance(QString classname);

private:
  ATourAction *_previous;
  ATourAction *_next;

  bool _stateless;
  bool _cursorable;
  int _duration;
  int _time;

  QString _widgetOfInterest;

  static QMap<QString,QMetaObject> _listOfActionTypes;
  Q_OBJECT
};

#endif //ATOURACTION_H_
