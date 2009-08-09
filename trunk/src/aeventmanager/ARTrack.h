#ifndef ARTRACK_H_
#define ARTRACK_H_

#include "ATrack.h"

//! Objects of this class represent reconstructedtracks in the event (Track)
class AEVENTMANAGER_EXPORT ARTrack : public ATrack
{
public:
  ARTrack();
  ~ARTrack();
  
  
  //basic parameters
  int barcode;
  float chi2;
  float d0;
  float z0;
  
  /*!
   * The reconstructed polyline 
   * \return The reconstructed polyline for this track
   */
  std::vector<irr::core::vector3df> getPolyline();

  /*!
   * \return The reconstruction type
   */
  QString RTrackType();

  /*!
   * \param cotTheta
   */
  void setCotTheta(float cotTheta);
  
 private:
  std::vector<irr::core::vector3df> polyLine;
  
  QString _RTrackType;

  // AEvent should have acess to all objects, because it is reposible for loading them..
  friend class AEvent;
  friend class AXmlEvent;
};
#endif //ARTRACK_H_
