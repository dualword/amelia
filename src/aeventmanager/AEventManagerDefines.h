#ifndef AEVENTMANAGERDEFINES_H_
#define AEVENTMANAGERDEFINES_H_

// We are using the Visual Studio Compiler and building Shared libraries
#if defined (_WIN32)
  #if defined(AEventManager_EXPORTS)
    #define AEVENTMANAGER_EXPORT __declspec(dllexport)
  #else
    #define AEVENTMANAGER_EXPORT __declspec(dllimport)
  #endif
#else /* defined (_WIN32) */
 #define AEVENTMANAGER_EXPORT
#endif

#endif //AEVENTMANAGERDEFINES_H_
