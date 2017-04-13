/* d4go.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
         
#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int  S4FUNCTION d4go( D4DATA *d4, long rec )
{
   int rc ;
   rc =  d4flush_record(d4) ;  if ( rc != 0 ) return rc ;
   if ( d4->code_base->read_lock )
   {
      rc =  d4lock(d4,rec) ;
      if ( rc != 0 )  return rc ;
   }

   rc =  d4go_data(d4,rec) ;       if ( rc != 0 )  return rc ;
   d4->bof_flag = d4->eof_flag = 0 ;
   if ( d4lock_test(d4,rec)  )
   {
      memcpy( d4->record_old, d4->record, d4->record_width ) ;
      d4->rec_num_old =  d4->rec_num ;
      d4->memo_validated =  1 ;
   }
   else
      d4->memo_validated =  0 ;
   return rc ;
}

int  S4FUNCTION d4go_eof( D4DATA *d4 )
{
   int rc ;
   long count ;

   if ( (rc = d4flush_record(d4)) != 0 )  return rc ;
   count =  d4reccount(d4) ;  if ( count < 0 )  return -1 ;
   d4->rec_num =  count+1L ;
   d4->eof_flag =  1 ;
   if ( d4->rec_num == 1 )  d4->bof_flag =  1 ;
   memset( d4->record, ' ', d4->record_width ) ;
   return r4eof ;
}

int S4FUNCTION d4go_data( D4DATA *d4, long rec )
{
   unsigned len ;

   #ifdef S4DEBUG
      if ( rec < 1L )  e4severe( e4info, "d4go_data()", (char *)  0) ;
   #endif

   len =  h4read( &d4->file, d4rec_pos(d4,rec), d4->record, d4->record_width ) ;

   if ( len != d4->record_width )
   {
      if ( d4->code_base->go_error && ! (d4->code_base->error_code < 0) )
         return  e4error( d4->code_base, e4read, d4->file.name, (char *) 0 ) ;

      return r4entry ;
   }
   d4->rec_num =  rec ;
   return 0 ;
}

