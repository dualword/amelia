#ifndef AEVENTOBJECT_H_
#define AEVENTOBJECT_H_

#include <QString>
#include <QObject>
#include <irrlicht.h>
#include <vector>


#include "AEventManagerDefines.h"

///! A generic "object" of the event.
class AEVENTMANAGER_EXPORT AEventObject : public QObject
{
 public:
  enum eType //element type
  {
    eUndefined,
    eTrack,
    eJet,
    eShower,
    eFCALShower,
    eMissingEt,
    eCombination
  };

  //! Constructor
  AEventObject(QString name,AEventObject::eType);

  //! Copy-Constructor
  AEventObject(const AEventObject&);

  /*!
   * \return Type of the event object
   */
  eType type();

  /*!
   * This is usually the ID assigned by Athena
   * \return ID of the track
   */
  unsigned int ID();
  
  /*!
   * \return The human-readable name of the object
   */
  virtual QString name();

  /*!
   * \return The pseudo-rapidity of the particle
   */
  float eta();
  
  /*!
   * \return The phi angle of the particle
   */
  float phi();
  
  /*!
   * \return The theta angle of the particle, as calculated from the psudo-rapidity.
   */
  float theta();
  
  /*!
   * \return What the heck is this? Some hyperbolic angle?
   */
  float getTl();

  /*!
   * \return Transverse momentum of the particle
   */
  float pt();

  /*!
   * \return Transverse energy of the particle
   */
  float et();
  
  /*!
   * \return is track interesting
   */
  virtual bool isInteresting();

 signals:
  void updated();

 protected:
  /*!
   * \param name The human-readable name of the object.
   */
  void setName(QString name);

  /* Kinematic Variables */
  float _eta;
  float _phi;
  float _theta;
  float _pt;
  float _et;

 private:
  /* Data */
  eType _type;

  QString _name;
  unsigned int _ID;

  bool _isInteresting;

 // AEvent should have acess to all objects, because it is reposible for loading them..
  friend class AEvent;
  friend class AXmlEvent;

  Q_OBJECT
};

#endif // AEVENTOBJECT_H_
