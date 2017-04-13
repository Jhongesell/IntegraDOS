/* h4open.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

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


int S4FUNCTION h4open( H4FILE *h4, C4CODE *c4, char *name, int alloc_copy )
{
   int len ;
   int oflag, pmode ;
   #ifdef S4WINDOWS
      OFSTRUCT of_struct ;
   #endif
   #ifdef S4UNIX
      char lwr_name[68] ;  /* Force to lower case for SCO Foxbase Compatibility */
   #endif

   memset( (void *)h4, 0, sizeof(H4FILE) ) ;
   h4->code_base =  c4 ;
   h4->hand =  -1 ;
   h4->do_alloc_free =  0 ;

   if ( c4->error_code < 0 )  return -1 ;
   c4->error_code =  0 ;

   h4->is_temp =  0 ;

   #ifdef S4UNIX
      oflag    =  (int)  O_RDWR ;
      pmode    =   0666 ;  /* Allow complete read and write permissions */
   #else
      oflag    =  (int) (O_BINARY | O_RDWR) ;
      pmode    =  (int) (S_IREAD  | S_IWRITE) ;
   #endif

   #ifdef S4UNIX
      strncpy( lwr_name, name, sizeof(lwr_name) ) ;
      lwr_name[sizeof(lwr_name)-1] = '\000' ;
      c4lower( lwr_name ) ;
   
      h4->hand =  open( lwr_name, oflag , pmode ) ;
   #else
      #ifdef S4WINDOWS
         h4->hand = OpenFile( name, &of_struct, OF_READWRITE | OF_SHARE_DENY_NONE ) ;
      #else
         h4->hand =  sopen( name, oflag , SH_DENYNO, pmode ) ;
      #endif
   #endif

   if ( h4->hand < 0 )
   {
      if ( h4->code_base->open_error )
	 return  e4error( h4->code_base, e4open, E4_CREATE_FIL, name, (char *) 0 ) ;
      else
      {
	 c4->error_code =  r4no_open ;
         return  r4no_open ;
      }
   }

   if ( alloc_copy )
   {
      h4->name =  (char *) u4alloc( (long) (len = strlen(name)+ 1) ) ;
      if ( h4->name == 0 )
      {
	 h4close( h4 ) ;
	 return e4error( h4->code_base, e4memory, (char *) 0 ) ;
      }
      h4->do_alloc_free =  1 ;
      u4ncpy( h4->name, name, (unsigned) len ) ;
   }
   else
      h4->name =  name ;

   return 0 ;
}

int S4FUNCTION h4open_test( H4FILE *h4 )
{
   return  h4->hand >= 0 ;
}


