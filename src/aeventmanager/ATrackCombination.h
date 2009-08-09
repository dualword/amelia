#ifndef ATRACKCOMBINATION_H_
#define ATRACKCOMBINATION_H_

#include <QList>
#include <QString>
#include <QObject>

#include "AEventObject.h"
#include "ASTrack.h"
#include "AJet.h"

//! Track combinations
class AEVENTMANAGER_EXPORT ATrackCombination : public AEventObject 
{
  
 public:
  //! Constructor
  ATrackCombination();
  //! Copy-Constructor
  ATrackCombination(const ATrackCombination&);

  //! Destructor
  ~ATrackCombination();
  
  /*!
   * \brief Add particle into combination
   * \param object Paritcle to add in
   */
  void addTrack(AEventObject *object);

  /*!
   * \brief Delete particle from combination
   * \param object Paritcle to delete
   */
  bool deleteTrack(AEventObject *object);

  /*!
   * \return ID as a string..
   */
  QString IDString();

  /*!
   * \brief Set the name of the combination
   * Combinations allow the user to set a name
   * \param name The new name
   */
  void setName(QString name);

  /*!
   * \return Name of the combination
   */
  QString name();

  /*!
   * Format of default name:
   * id1|id2
   *
   * \param generateDefault Generate a default name, if none is set.
   *
   * \return Name of the combination
   */
  QString name(bool generateDefault);
  
  /*!
   * \return Numbre of particles in the combination
   */
  int size();

  /*!
   * \return Invariant mass of the combination
   */
  float getInvariantMass();

  /*!
   * \return The charge of the combination
   */
  int charge();

  /*!
   * Get particle from combination
   * \param idx Index of particle
   */
  AEventObject* operator[](unsigned int idx);

  /*!
   * Get particle from combination
   * \param idx Index of particle
   */
  AEventObject* getTrack(unsigned int idx);

  /*!
   * \brief Compare against another combination
   * Two combinations are equal if they contain pointers to same tracks
   *
   * \param other The combination to compare against
   *
   * \return True if the combinatins are same
   */
  bool operator==(ATrackCombination& other);

 public slots:
  /*!
   * \brief Handles updates of constituents.
   * ei: Recalculates invariant mass
   */
  void handleUpdates();

 private:
  QList<AEventObject*> objects;

  /* 4 vector */
  double Px;
  double Py;
  double Pz;
  double E;
  
  /* Data */
  float InvMass;
  int _charge;

  /*!
   * Recalculate
   */
  void recalculate();

  /*!
   * Recursive recalculate
   */
  void recalculate(AEventObject *trk);

  Q_OBJECT;
};
#endif
