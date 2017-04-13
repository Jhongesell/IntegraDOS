/* p4port.h (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved.

   Declarations for non-standard C runtime library functions.
*/

#ifdef S4UNIX
   #define S4LOCKING
   #define S4VARARGS
   #define S4PORTABLE
   #define S4DO_ERRNO
   #define S4NO_FILELENGTH
   #define S4NO_KBHIT
   #define S4NO_MEMMOVE   
   #define S4NO_POW
   #define S4NO_REMOVE
   #define S4NO_RENAME
   #define S4NO_SIZE_T
   #define S4NO_SPAWNL
   #define S4NO_STRLWR
   #define S4NO_STRNICMP    
   #define S4NO_STRUPR
#endif

#ifdef S4UNIX
   #include <tinfo.h>
   typedef  void  S4PTR * S4CMP_PARM ;
   extern  long  filelength() ;
   extern  char *fcvt() ;
   extern  long  lseek() ;
   #define USHRT_MAX      0xFFFFU
   #define INT_MAX        0x7FFF
   #define UINT_MAX       0xFFFFU
#endif

#ifdef S4VARARGS
   #include <varargs.h>
#endif

#ifdef S4NO_SIZE_T
   typedef unsigned size_t ;
#endif

#ifdef S4NO_STRNICMP
   int strnicmp() ;
#endif

#ifdef S4NO_MEMMOVE
   void *memmove() ;
#endif
