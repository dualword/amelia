#ifndef AJET_H_
#define AJET_H_

#include "AEventObject.h"

//! Objects of this class represent jet in the event
class AEVENTMANAGER_EXPORT AJet : public AEventObject
{
public:
  //! Constructor
  AJet();
  //! Destructor
  ~AJet();

  //! Number of cells associated with this jet
  int numCells;

  /*!
   * \param jetType Reconstruction model used for jet
   */
  void setJetType(QString jetType);
  
  /*!
   * \return The type of the jet.
   */
  QString jetType();

  /*!
   * \param eta
   */
  void setEta(float eta);

  /*!
   * \param phi
   */
  void setPhi(float phi);

  /*!
   * \param et New transverse energy of particle
   */
  void setEt(float et);

  /*!
   * \param emFraction
   */
  void setEMFraction(float emFraction);

  /*!
   * \return emFraction
   */
  float emFraction();

 private:
  QString _jetType;

  float _emFraction;

  Q_OBJECT
};

#endif //AJET_H_
