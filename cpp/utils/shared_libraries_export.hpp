#ifndef _SHARED_LIBRARIES_EXPORT_HPP_1CEFD9D159A24319896C053D79054709_
#define _SHARED_LIBRARIES_EXPORT_HPP_1CEFD9D159A24319896C053D79054709_

/*************************************/
/*    shared_libraries_export.hpp    */
/*             Version 2.0           */
/*             2021/06/12            */
/*************************************/

// clang-format off
#if defined SHARED_LIBS_LINK
    #if defined _WIN32 || defined __CYGWIN__
      #ifdef BUILDING_SHARED_LIBS
        #ifdef __GNUC__
          #define DLL_PUBLIC __attribute__ ((dllexport))
        #else
          #define DLL_PUBLIC __declspec(dllexport)
        #endif
      #else
        #ifdef __GNUC__
          #define DLL_PUBLIC __attribute__ ((dllimport))
        #else
          #define DLL_PUBLIC __declspec(dllimport)
        #endif
      #endif
      #define DLL_LOCAL
    #else
      #if __GNUC__ >= 4
        #define DLL_PUBLIC __attribute__ ((visibility ("default")))
        #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
      #else
        #define DLL_PUBLIC
        #define DLL_LOCAL
      #endif
    #endif
#else
    #define DLL_PUBLIC
    #define DLL_LOCAL
#endif

#endif
