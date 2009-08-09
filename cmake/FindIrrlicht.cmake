FIND_PATH(Irrlicht_INCLUDE_DIR irrlicht.h /usr/include/irrlicht /usr/local/include/irrlicht /usr/local/include ) 

FIND_PATH(Irrlicht_LIBRARY_DIR irrlicht libIrrlicht.a PATH /usr/lib /usr/local/lib ) 

IF (Irrlicht_INCLUDE_DIR AND Irrlicht_LIBRARY_DIR)
  SET(Irrlicht_FOUND TRUE)
  MESSAGE(STATUS "Found Dir")
  
  SET(SYSTEMSPECIFIC_LIBRARIES)
  IF(UNIX)
    MESSAGE(STATUS "System UNIX")
    FIND_PACKAGE(OpenGL REQUIRED)
    FIND_PACKAGE(X11)
    FIND_LIBRARY(XXF86VM_LIBRARY Xxf86vm)
    SET(SYSTEMSPECIFIC_LIBRARIES
      ${X11_LIBRARIES}
      ${XXF86VM_LIBRARY}
      ${OPENGL_LIBRARIES}
      jpeg
      png
      )
  ENDIF(UNIX)

  IF(APPLE)
    FIND_LIBRARY(IOKIT_LIBRARY IOKit REQUIRED)
    SET(SYSTEMSPECIFIC_LIBRARIES
      ${SYSTEMSPECIFIC_LIBRARIES}
      ${IOKIT_LIBRARY})
  ENDIF(APPLE)

  SET(Irrlicht_LIBRARY Irrlicht ${SYSTEMSPECIFIC_LIBRARIES})

ENDIF (Irrlicht_INCLUDE_DIR AND Irrlicht_LIBRARY_DIR)

IF (Irrlicht_FOUND)
  IF (NOT Irrlicht_FIND_QUIETLY)
    MESSAGE(STATUS "Found Irrlicht: ${Irrlicht_LIBRARY_DIR}")
  ENDIF (NOT Irrlicht_FIND_QUIETLY)
ELSE (Irrlicht_FOUND)
  IF (Irrlicht_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find Irrlicht")
  ENDIF (Irrlicht_FIND_REQUIRED)
ENDIF (Irrlicht_FOUND)
