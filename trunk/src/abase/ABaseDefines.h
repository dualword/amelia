#ifndef ABASEDEFINES_H_
#define ABASEDEFINES_H_

// We are using the Visual Studio Compiler and building Shared libraries
#if defined (_WIN32)
  #if defined(ABase_EXPORTS)
    #define ABASE_EXPORT __declspec(dllexport)
  #else
    #define ABASE_EXPORT __declspec(dllimport)
  #endif
#else /* defined (_WIN32) */
 #define ABASE_EXPORT
#endif

#endif //ABASEDEFINES_H_
