#ifndef ASTRACK_H_
#define ASTRACK_H_

#include "ATrack.h"

//Objects of this class represent simulated tracks in the event (STr)
class AEVENTMANAGER_EXPORT ASTrack : public ATrack
{
public:
  //! Constructor
  ASTrack();
  //! Destructor
  ~ASTrack();

  float phiVertex;
  float rhoVertex;
  float zVertex;

  float maxAngle;

 private:
};
#endif //ASTRACK_H_
