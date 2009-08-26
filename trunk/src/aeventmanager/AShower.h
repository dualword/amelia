#ifndef ASHOWER_H_
#define ASHOWER_H_

#include "AEventObject.h"

#include <QString>

//! Calorimenter showers
class AEVENTMANAGER_EXPORT AShower : public AEventObject
{
public:
  //! Constructor
  AShower();
  //! Destructor
  ~AShower();

  /*!
   * \param eta
   */
  void setEta(float eta);

  /*!
   * \param phi
   */
  void setPhi(float phi);
  
  /*!
   * \param calorimeter Calorimeter that this showers blongs to
   */
  void setCalorimeter(QString calorimeter);

  /*!
   * \return Calorimeter that this shower belongs to
   */
  QString calorimeter();

  /*!
   * \param energy
   */
  void setEnergy(float energy);

  /*!
   * \return Energy in this shower
   */
  float energy();
  
  /*!
   * \param sub ?
   */
  void setSub(int);
  
  /*!
   * Subdetector?
   * \return sub 
   */
  int sub();

  /*!
   * \param layer ?
   */
  void setLayer(int layer);

  /*!
   * \return layer ?
   */
  int layer();
  
private:
  
  QString _calometer;

  float _energy;

  int _sub;
  int _layer;

  Q_OBJECT
};

#endif //ASHOWER_H_
