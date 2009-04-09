#ifndef AGEOMETRYDEFINES_H_
#define AGEOMETRYDEFINES_H_

// We are using the Visual Studio Compiler and building Shared libraries
#if defined (_WIN32)
  #if defined(AGeometry_EXPORTS)
    #define AGEOMETRY_EXPORT __declspec(dllexport)
  #else
    #define AGEOMETRY_EXPORT __declspec(dllimport)
  #endif
#else /* defined (_WIN32) */
 #define AGEOMETRY_EXPORT
#endif

#endif //AGEOMETRYDEFINES_H_
