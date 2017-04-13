/* h4create.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

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


int S4FUNCTION h4create( H4FILE *h4, C4CODE *c4, char *name, int do_alloc )
{
   int len ;
   #ifdef S4WINDOWS
      OFSTRUCT of_struct ;
      int exist_handle ;
   #else
      int  extra_flag ;
      int  oflag, pmode ;
   #endif
   #ifdef S4UNIX
      char lwr_name[68] ;
   #endif

   h4->code_base =  c4 ;
   h4->name =  0 ;
   h4->hand =  -1 ;
   h4->is_temp =  0 ;
   h4->do_alloc_free =  0 ;

   if ( c4->error_code < 0 )  return -1 ;
   c4->error_code =  0 ;

   #ifndef S4WINDOWS
      #ifdef S4UNIX
         oflag = (int) O_CREAT | O_TRUNC | O_RDWR ;
         pmode = 0666 ;  /* Allow full read/write permission  */
      #else
         oflag = (int) O_CREAT | O_TRUNC | O_BINARY | O_RDWR ;
         pmode    =  (int) (S_IREAD  | S_IWRITE) ;
      #endif
   #endif
   
   #ifdef S4UNIX
       /* Force to lower case for SCO Foxbase Compatibility */
      extra_flag  = 0 ;
      if ( c4->safety )  extra_flag =  O_EXCL ;
   
      strncpy( lwr_name, name, sizeof(lwr_name) ) ;
      lwr_name[sizeof(lwr_name)-1] = '\000' ;
      c4lower( lwr_name ) ;
   
      h4->hand =  open( lwr_name, extra_flag | oflag , pmode ) ;
   #else
      #ifdef S4WINDOWS
         exist_handle =  -1 ;
      
         if ( c4->safety )
          exist_handle =  OpenFile( name, &of_struct, OF_EXIST ) ;
      
         if ( exist_handle < 0 )  /* if file doesn't exist, create */
         h4->hand = OpenFile( name, &of_struct, OF_CREATE | OF_READWRITE | OF_SHARE_DENY_NONE ) ;
      #else
         extra_flag  = 0 ;
         if ( c4->safety )  extra_flag =  O_EXCL ;
         h4->hand =  sopen( name, extra_flag | oflag, SH_DENYNO, pmode ) ;
      #endif
   #endif

   if ( h4->hand < 0 )
   {
      if ( c4->create_error )
	 return e4error( h4->code_base, e4create, E4_CREATE_FIL, name, (char *) 0 ) ;
      c4->error_code =  r4no_create ;
      return r4no_create ;
   }

   if ( do_alloc )
   {
      h4->name =  (char *) u4alloc( (len = strlen(name)+ 1) ) ;
      if ( h4->name == 0 )
      {
	 h4close( h4 ) ;
	 return e4error( h4->code_base, e4memory, (char *) 0 ) ;
      }
      u4ncpy( h4->name, name, len ) ;
      h4->do_alloc_free =  1 ;
   }
   else
      h4->name =  name ;

   return 0 ;
}

