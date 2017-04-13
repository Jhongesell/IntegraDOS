/* h4close.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifdef S4TEMP
   #include "t4test.h"
#endif

#include <time.h>

#ifndef S4UNIX
   #ifndef __TURBOC__
      #include <sys\locking.h>
      #define S4LOCKING
   #endif
   #ifdef __ZTC__
      extern volatile int  errno ;
   #endif
   #ifdef _MSC_VER
      #include <sys\types.h>
   #endif
   #ifdef __TURBOC__
      extern int cdecl errno ;
   #endif

   #include <sys\stat.h>
   #include <share.h>
#endif

#include <fcntl.h>
#include <errno.h>

#ifdef S4DO_ERRNO
   extern int errno ;
#endif

#ifdef S4WINDOWS
   #include <windows.h>
#endif


int S4FUNCTION h4close( H4FILE *h4 )
{
   int rc ;
   C4CODE *c4;


   c4 = h4->code_base;

   if ( h4->hand < 0 )
   {
      if ( c4->error_code < 0 )  return -1 ;
      return 0 ;
   }
   #ifdef S4WINDOWS
      rc = _lclose(h4->hand) ;
   #else
      rc =  close(h4->hand) ;
   #endif
   if ( rc < 0 )
   {
      if ( h4->name == 0 )
         e4error( c4, e4close, E4_CLOSE, (char *) 0 ) ;
      else                             
         e4error( c4, e4close, h4->name, (char *) 0 ) ;
   }

   if ( h4->is_temp )
      u4remove(h4->name) ;

   if ( h4->do_alloc_free )
      u4free( h4->name ) ;

   memset( (void *)h4, 0, sizeof(H4FILE) ) ;
   h4->hand =  -1 ;

   if ( c4->error_code < 0 )  return -1 ;

   return 0 ;
}

