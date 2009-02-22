#ifndef ADEFINES_H_
#define ADEFINES_H_

// We are using the Visual Studio Compiler and building Shared libraries
#if defined (_WIN32)
  #if defined(amelia_EXPORTS)
    #define LIBAMELIA_EXPORT __declspec(dllexport)
  #else
    #define LIBAMELIA_EXPORT __declspec(dllimport)
  #endif
#else /* defined (_WIN32) */
 #define LIBAMELIA_EXPORT
#endif


#endif //ADEFINES_H_
