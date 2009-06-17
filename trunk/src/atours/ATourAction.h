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

  void setDuration(int);
  int duration();
  void setTime(int);
  int time();

  bool isStateless();
  void setStateless(bool);

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

  virtual QString widgetOfInterest();
  virtual QPoint cursor();

  void insertAction(ATourAction *action);
  ATourAction* getActionFor(int frame);

  static void addActionType(QMetaObject);
  static ATourAction* newInstance(QString classname);

private:
  ATourAction *_previous;
  ATourAction *_next;

  bool _stateless;
  int _duration;
  int _time;


  static QMap<QString,QMetaObject> _listOfActionTypes;
  Q_OBJECT
};

#endif //ATOURACTION_H_
