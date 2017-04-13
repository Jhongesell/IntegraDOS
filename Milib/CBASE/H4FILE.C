/* h4file.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

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


#ifdef S4NO_FILELENGTH
#include  <sys/types.h>
#include  <sys/stat.h>

long  filelength(hand )
int hand  ;
{
   struct stat   str_stat ;

   if (fstat( hand, &str_stat ) )         
      e4severe( e4result, "filelength()", (char *) 0 ) ;

   return( (long) str_stat.st_size ) ;
}
#endif

long S4FUNCTION h4length( H4FILE *h4 )
{
   long lrc ;

   if ( h4->code_base->error_code < 0 )  return -1 ;
   lrc =  filelength( h4->hand ) ;
   if ( lrc < 0L )  e4error( h4->code_base, e4len, h4->name, (char *) 0 ) ;
   return  lrc ;
}

int S4FUNCTION h4length_set( H4FILE *h4, long new_size )
{
   int rc ;
   if ( h4->code_base->error_code < 0 )  return -1 ;

   rc =  chsize( h4->hand, new_size ) ;
   if ( rc < 0 )
      return e4error( h4->code_base, e4len_set, E4_CREATE_FIL, h4->name, (char *) 0 ) ;
   return 0 ;
}

int  S4FUNCTION h4read_all( H4FILE *h4, long pos, void *ptr, unsigned ptr_len )
{
   long rc ;
   unsigned urc ;

   if ( h4->code_base->error_code < 0 )  return -1 ;

   #ifdef S4DEBUG
      if( pos < 0 )
         e4severe( e4parm, E4_PARM_H4R, (char *) 0 ) ;
   #endif

   #ifdef S4WINDOWS
      rc = _llseek( h4->hand, pos, 0 ) ;
   #else
      rc =  lseek( h4->hand, pos, 0 ) ;
   #endif
   if ( rc != pos )
      return h4read_error(h4) ;

   #ifdef S4WINDOWS
      urc =  (unsigned) _lread( h4->hand, (char *) ptr, ptr_len ) ;
   #else
      urc =  (unsigned) read( h4->hand, ptr, ptr_len ) ;
   #endif
   if ( urc != ptr_len )
      return h4read_error(h4) ;

   return 0 ;
}

unsigned  S4FUNCTION h4read( H4FILE *h4, long pos, void *ptr, unsigned ptr_len )
{
   long rc ;
   unsigned urc ;

   if ( h4->code_base->error_code < 0 )  return -1 ;

   #ifdef S4DEBUG
      if( pos < 0 )
         e4severe( e4parm, E4_PARM_H4R, (char *) 0 ) ;
   #endif

   #ifdef S4WINDOWS
      rc = _llseek( h4->hand, pos, 0 ) ;
   #else
      rc =  lseek( h4->hand, pos, 0 ) ;
   #endif
   if ( rc != pos )
   {
      h4read_error(h4) ;
      return 0 ;
   }

   #ifdef S4WINDOWS
      urc =  (unsigned) _lread( h4->hand, (char *) ptr, ptr_len ) ;
   #else
      urc =  (unsigned) read( h4->hand, ptr, ptr_len ) ;
   #endif
   if ( urc != ptr_len )
      if ( urc > ptr_len)
      {
         h4read_error(h4) ;
         return 0 ;
      }
   return urc ;
}

int S4FUNCTION h4read_error( H4FILE *h4 )
{
   return e4error( h4->code_base, e4read, E4_CREATE_FIL, h4->name, (char *) 0 ) ;
}

int S4FUNCTION h4replace( H4FILE *keep, H4FILE *old )
{
   int rc ;
   rc =  h4close( keep ) ;
   memcpy( (void *)keep, (void *)old, sizeof(H4FILE) ) ;
   memset( (void *)old, 0, sizeof(H4FILE) ) ;
   return rc ;
}

int S4FUNCTION h4temp( H4FILE *h4, C4CODE *c4, char *name_buf, int auto_remove )
{
   int     i, save_flag, rc ;
   time_t  t ;
   char    name[20], *name_ptr ;

   name_ptr =  name_buf ;
   if ( name_ptr == 0 )
      name_ptr =  name ;

   for ( i=0; i < 100; i++ )
   {
      time( &t ) ;
      t %= 10000L ;

      strcpy( name_ptr, "TEMP" ) ;
      c4ltoa45( t, name_ptr+4, -4 ) ;
      strcpy( name_ptr+8, ".TMP" ) ;

      save_flag =  c4->create_error ;
      c4->create_error =  0 ;
      rc =  h4create( h4, c4, name_ptr, name_buf == 0 ) ;
      c4->create_error =  save_flag ;
      if ( rc < 0 )  return -1 ;
      if ( rc == 0 )
      {
	 if ( auto_remove )  h4->is_temp =  1 ;
         return  0 ;
      }
   }

   return e4error( c4, e4create, E4_CREATE_TEM, (char *) 0 ) ;
}

int S4FUNCTION h4write( H4FILE *h4, long pos, void *ptr, unsigned ptr_len )
{
   unsigned urc ;

   if ( h4->code_base->error_code < 0 )  return -1 ;

   #ifdef S4DEBUG
      if( pos < 0 )
         e4severe( e4parm, "h4write()", (char *) 0 ) ;
   #endif

   #ifdef S4WINDOWS
      if ( _llseek( h4->hand, pos, 0 ) != pos )
   #else
      if ( lseek( h4->hand, pos, 0 )  != pos )
   #endif
	 return e4error( h4->code_base, e4write, h4->name, (char *) 0 ) ;

   #ifdef S4WINDOWS
      urc =  (unsigned) _lwrite( h4->hand, (char *) ptr, ptr_len ) ;
   #else
      urc =  (unsigned) write( h4->hand, ptr, ptr_len ) ;
   #endif
   if ( urc != ptr_len ) return e4error( h4->code_base, e4write, h4->name, (char *) 0 ) ;
   return 0 ;
}

