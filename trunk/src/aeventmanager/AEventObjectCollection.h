#ifndef AEVENTOBJECTCOLLECTION_H_
#define AEVENTOBJECTCOLLECTION_H_

#include <QList>
#include <QMap>
#include <QString>
#include <QObject>
#include <QDomElement>

#include "AEventObject.h"
#include "AEventAnalysisData.h"

//! Container to store collections of event objects
class AEVENTMANAGER_EXPORT AEventObjectCollection : public AEventAnalysisData
{
 public:
  /*!
   * \brief Constructor
   * \param modulename The name of the collection
  */
  Q_INVOKABLE AEventObjectCollection(QString modulename);
  //! Destructor
  ~AEventObjectCollection();

  /*!
   * \return Number of objects in this collection
   */
  int size();

  /*!
   * \brief Add object into colection.
   * Note: Does no add an object if it already exists in the collection.
   * \param object The event object to add
   */
  void add(AEventObject*);

  /*!
   * \return Object corresponding to index idx in this collection
   */
  AEventObject* get(int idx);

  /*!
   * \brief Removes an object from this collection
   *
   * \param idx Index of object to remove
   */
  void remove(int idx);

  /*!
   * \param object Object to check for
   * \return True if this collection contains the event object object, otherwise returns false.
   */
  bool contains(AEventObject *object);

  /*!
   * Writes the data to a file using XML
   * 
   * \param in The stream to write into
   */
  void writeToFile(QTextStream& in);

  /*!
   * Loads the objects from an xml file
   * \param ele The XML element corresponding to this collection
   * \param e The event that this collection belongs to
   */
  void loadFromXML(QDomElement ele, class AEvent* e);

 public slots:
  /*!
   * Resave on updates
   */
  void handleUpdates();

 private:
  //! List of objects in this collection
  QList<AEventObject*> _objects;

  /*!
   * \brief A recusrive function that reads objects from XML. 
   * Used to handle combinations..
   *
   * \param event The event that this collection belongs into
   * \param ele The XML element to read from
   *
   * \return list of objects found in this element
   */
  QList<AEventObject*> readObjectsFromXmlElement(AEvent *event, const QDomElement& ele);

  /*!
   * \brief A recusrive function that to write objects into XML. 
   * Used to handle combinations..
   *
   * \param in The text stream to write into
   * \param object The object
   *
   * \return list of objects found in this element
   */
  void writeObjectToXmlFile(QTextStream& in,AEventObject* object);


  Q_OBJECT
};

#endif //AEVENTOBJECTCOLLECTION_H_
