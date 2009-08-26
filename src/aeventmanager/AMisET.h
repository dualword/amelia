#ifndef AMISET_H_
#define AMISET_H_

#include "AEventObject.h"
#include "AEventManagerDefines.h"

//! Objects of this class represent Missing Energy
class AEVENTMANAGER_EXPORT AMisET : public AEventObject
{
public:
  //! Constructor
  AMisET();
  //! Destructor
  ~AMisET();
  
  /*!
   * \param et
   */
  void setEt(float et);

  /*!
   * \param etx
   */
  void setEtX(float etx);
  
  /*!
   * \return etx
   */
  float etx();

  /*!
   * \param ety
   */
  void setEtY(float ety);
  
  /*!
   * \return ety
   */
  float ety();

  /*!
   * \param type Type of missing et reconstruction
   */
  void setMisETType(QString type);

  /*!
   * \return Type of missing et reconstruction
   */
  QString misETType();

private:
  QString _misETType;

  //! X component of et
  float _etx;
  //! Y component of et
  float _ety;
};
#endif //AMISET_H_
