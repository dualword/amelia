#ifndef ATRACK_H_
#define ATRACK_H_

#include "AEventObject.h"

//! Objects of this class represent particle tracks in the event
class AEVENTMANAGER_EXPORT ATrack : public AEventObject
{
 public:
  //! Destructor
  ~ATrack();
  
  /*!
   * \brief Sets the PDG code for a particle
   *
   * By giving this particle a code, one makes it automatically known.
   *
   * \param code The PDG code
   */
  void setCode(int code);
  
  /*!
   * \brief Returns the PDG code for the particle
   * The PDG code might not always be valid, so any checks should be masked
   * with isKnown(). However, an invalid PDG code will always be 0.
   *
   * \return PDG code
   */
  int code();
  
  /*!
   * \brief Returns The index in the PDG mapping for this particle.
   * Can be used by other plugins to implement their own properties, like color tables for display.
   *
   * Returns 0 if not found.
   *
   * \return Index of the PDG mapping
   */
  int index();
  
  /*!
   * \brief Returns whether the type of this paricle is known
   * Known information comes from either MC truth or trigger information.
   * If both are available, MC truth will superceed trigger.
   *
   * \return Is the type known..
   */
  bool isKnown();
  
  /*!
   * Note: Always returns false if isKnown() is false...
   * \return Is this particle an electron?
   */
  bool isElectron();
  
  /*! 
   * Note: Always returns false if isKnown() is false...
   * \return Is this particle a muon?
   */
  bool isMuon();
  
  /*!
   * Note: Always returns false if isKnown() is false...
   * \return Is this particle a photon?
   */
  bool isPhoton();
  
  /*!
   * Note: Always returns false if isKnown() is false...
   * \return Is this particle a neutrino?
   */
  bool isNeutrino();
  
  /*!
   * Note: Always returns false if isKnown() is false...
   * \return Is this particle a charged hadron?
   */
  bool isChargedHadron();
  
  /*!
   * Note: Always returns false if isKnown() is false...
   * \return Is this particle a neutral hadron?
   */
  bool isNeutralHadron();
  
  /*!
   * Note: Always returns false if isKnown() is false...
   * \return Is this particle an electron?
   */
  bool isHadron();

  /*!
   * \return The track type
   */
  QString trackType();

  /*!
   * \param eta
   */
  void setEta(float eta);

  /*!
   * \param phi
   */
  void setPhi(float phi);

  /*!
   * \param charge
   */
  void setCharge(int charge);

  /*!
   * \param pt New transverse momentum of particle
   */
  void setPt(float pt);

  /*!
   * \return Charge of the particle
   */
  int charge();

  /*!
   * Interesting particles include:
   * - Unknown particles
   * - Leptons
   * - Photons
   *
   * \return Is this particle interesting?
   */
  virtual bool isInteresting();

 protected:
  //! Constructor
  ATrack(QString name,QString type);

 private:
  
  /* The following is a map between the PDG code of a particle
   * and other information about it.
   * Note: The first entry in each matches to an invalid particle.
   */
  static const int codelist[]; //!< List of possible PDG codes
  static const int chargelist[]; //!< List of charges associated with the PDG codes
  static const QString namelist[]; //!< Names associated with the PDG code
  
  /* Data */
  int _code;
  bool _known;
  int _charge;
  QString _trackType;

  Q_OBJECT
  
};
#endif //ATRACK_H_
