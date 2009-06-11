FIND_PATH(Irrlicht_INCLUDE_DIR irrlicht.h /usr/include/irrlicht /usr/local/include/irrlicht /usr/local/include ) 

FIND_LIBRARY(Irrlicht_LIBRARY NAMES irrlicht libIrrlicht.a PATH /usr/lib /usr/local/lib ) 

IF (Irrlicht_INCLUDE_DIR AND Irrlicht_LIBRARY)
  SET(Irrlicht_FOUND TRUE)
  MESSAGE(STATUS "Found Dir")
  LINK_DIRECTORIES(${Irrlicht_LIBRARY})
ENDIF (Irrlicht_INCLUDE_DIR AND Irrlicht_LIBRARY)

IF (Irrlicht_FOUND)
  IF (NOT Irrlicht_FIND_QUIETLY)
    MESSAGE(STATUS "Found Irrlicht: ${Irrlicht_LIBRARY}")
  ENDIF (NOT Irrlicht_FIND_QUIETLY)
ELSE (Irrlicht_FOUND)
  IF (Irrlicht_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find Irrlicht")
  ENDIF (Irrlicht_FIND_REQUIRED)
ENDIF (Irrlicht_FOUND)
