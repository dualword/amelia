#ifndef AWEBDEFINES_H_
#define AWEBDEFINES_H_

// We are using the Visual Studio Compiler and building Shared libraries
#if defined (_WIN32)
  #if defined(AWeb_EXPORTS)
    #define AWEB_EXPORT __declspec(dllexport)
  #else
    #define AWEB_EXPORT __declspec(dllimport)
  #endif
#else /* defined (_WIN32) */
 #define AWEB_EXPORT
#endif

#endif //AWEBDEFINES_H_
