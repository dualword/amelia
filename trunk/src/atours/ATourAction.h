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

  virtual QString widgetOfInterest();

  virtual void doAction();
  virtual void updateAction(double done);
  virtual void endAction();
  virtual void undoAction();
  virtual void cleanupAction();

  virtual void loadFromXML(QDomElement actionElement);

  virtual QPoint cursor();

  ATourAction* previousAction();
  void setPreviousAction(ATourAction *action);
  ATourAction* nextAction();
  void setNextAction(ATourAction *action);

  static void addActionType(QMetaObject);
  static ATourAction* newInstance(QString classname);

protected:
  int frameFromTime(double time);

  void addSubaction(ATourAction* subaction);

  template <class T>
  T previousAction()
  {
    ATourAction *action=previousAction();
    while(action)
      {
	T castAction=qobject_cast<T>(action);
	if(castAction) return castAction;
	action=action->previousAction();
      }
    return 0;
  }

  template <class T>
  T nextAction()
  {
    ATourAction *action=nextAction();
    while(action)
      {
	T castAction=qobject_cast<T>(action);
	if(castAction) return castAction;
	action=action->nextAction();
      }
    return 0;
  }

private:
  int _duration;

  ATourAction *_previousAction;
  ATourAction *_nextAction;

  QList<ATourAction*> subactions;
  QList<bool> subactionsPerformed;

  static QMap<QString,QMetaObject> _listOfActionTypes;

  QString _widgetOfInterest;

  Q_OBJECT
};

#endif //ATOURACTION_H_
