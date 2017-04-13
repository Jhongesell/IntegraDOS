/* h4lock.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifdef S4TEMP
   #include "t4test.h"
#endif

#include <time.h>

#ifdef S4UNIX
   #include <sys/locking.h>
#else
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


int S4FUNCTION h4lock( H4FILE *h4, long o_set, long num_bytes )
{
   int rc ;

   if ( h4->code_base->error_code < 0 )  return -1 ;

   #ifdef S4LOCK_OFF
      return 0 ;
   #else
      #ifdef S4DEBUG
         if( o_set < 0 || num_bytes < 0 )
            e4severe( e4parm, E4_PARM_INV,(char *) 0 ) ;
      #endif

      errno =  0 ;
      #ifdef S4LOCKING
         #ifdef S4WINDOWS
            _llseek( h4->hand, o_set, 0 ) ;
         #else
            lseek( h4->hand, o_set, 0 ) ;
         #endif
         rc =  locking( h4->hand, LK_NBLCK, num_bytes ) ;
      #else
         rc =  lock( h4->hand, o_set, num_bytes ) ;
      #endif
      if (rc == 0 ||  errno == EINVAL)
      {
         #ifdef S4LOCK_CHECK
            l4lock_save( h4->hand, o_set, num_bytes ) ;
         #endif
         return( 0) ;  /* Single User or Success */
      }

      if (errno != EACCES)
         return e4error( h4->code_base, e4lock, h4->name, (char *) 0 ) ;

      if ( ! h4->code_base->wait )  return( r4locked ) ;

      for(;;)
      {
         time_t  old_time ;

         #ifdef S4TEMP
	    if ( d4display_quit( &display ) )
               e4severe( e4result, E4_RESULT_EXI, (char *) 0 ) ;
         #endif

         time( &old_time) ;   /* wait a second & try lock again */
         #ifdef S4WINDOWS

            while (  time( (time_t *) 0 ) <=  old_time)
            {
            /* Give some other application a chance to run. */
            }
         #else
            while (  time( (time_t *) 0 ) <=  old_time) ;
         #endif

         #ifdef S4LOCKING
            #ifdef S4WINDOWS
               _llseek( h4->hand, o_set, 0 ) ;
            #else
               lseek( h4->hand, o_set, 0 ) ;
            #endif
            rc =  locking( h4->hand, LK_NBLCK, num_bytes ) ;
         #else
            rc =  lock( h4->hand, o_set, num_bytes ) ;
         #endif
         if ( rc == 0 )
         {
            #ifdef S4LOCK_CHECK
               l4lock_save( h4->hand, o_set, num_bytes ) ;
	    #endif
            return( 0) ;
         }
      }
   #endif
}

int S4FUNCTION h4unlock( H4FILE *h4, long pos, long num_bytes )
{
   int     rc ;

   if ( h4->code_base->error_code < 0 )  return -1 ;

   #ifndef S4LOCK_OFF
      #ifdef S4LOCK_CHECK
      l4lock_remove( h4->hand, pos, num_bytes ) ;
      #endif

      errno =  0 ;
      #ifdef S4LOCKING
         #ifdef S4WINDOWS
            _llseek( h4->hand, pos, 0 ) ;
         #else
            lseek( h4->hand, pos, 0 ) ;
         #endif
         rc =  locking( h4->hand, LK_UNLCK, num_bytes ) ;
      #else
         rc =  unlock( h4->hand, pos, num_bytes ) ;
      #endif
      if (rc < 0  && errno != EINVAL )
	 return e4error( h4->code_base, e4unlock, h4->name, (char *) 0 ) ;
   #endif

   return 0 ;
}
