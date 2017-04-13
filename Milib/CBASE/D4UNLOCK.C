/* d4unlock.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int S4FUNCTION d4unlock_all( D4DATA *d4 )
{
   int rc ;
   rc = d4flush_record(d4) ;  if ( rc != 0 )  return rc ;
   d4unlock_all_data( d4 ) ;
   return  d4unlock_index( d4 ) ;
}

int S4FUNCTION d4unlock_all_data( D4DATA *d4 )
{
   d4unlock_file(d4) ;
   d4unlock_append(d4) ;
   return  d4unlock_records(d4) ;
}

int S4FUNCTION d4unlock_append( D4DATA *d4 )
{
   if ( d4 == 0 )  return -1 ;

   if ( d4->append_lock )
   {
      if ( h4unlock( &d4->file, L4LOCK_POS, 1L ) < 0 )  return -1 ;
      d4->append_lock =  0 ;
      d4->header.num_recs =  -1 ;
   }
   if ( d4->code_base->error_code < 0 )  return -1 ;
   return 0 ;
}

int S4FUNCTION d4unlock_file( D4DATA *d4 )
{
   if ( d4 == 0 )  return -1 ;

   if ( d4->file_lock )
   {
      if ( h4unlock( &d4->file, L4LOCK_POS, L4LOCK_POS ) < 0 )  return -1 ;
      d4->rec_num_old =  -1 ;
      d4->memo_validated =  0 ;
      d4->file_lock =  0 ;
      d4->header.num_recs =  -1 ;
   }
   if ( d4->code_base->error_code < 0 )  return -1 ;
   return 0 ;
}

int S4FUNCTION d4unlock_index( D4DATA *d4 )
{
   I4INDEX *index_on ;

   if ( d4 == 0 )  return -1 ;
   if ( d4->code_base->error_code < 0 )  return -1 ;

   for ( index_on = 0; index_on = (I4INDEX *) l4next(&d4->indexes,index_on); )
      if ( i4unlock(index_on) < 0 )  return -1 ;

   return 0 ;
}

int S4FUNCTION d4unlock_records( D4DATA *d4 )
{
   if ( d4 == 0 )  return -1 ;

   if ( d4->code_base->error_code < 0 )  return -1 ;
   d4->rec_num_old =  -1 ;
   d4->memo_validated =  0 ;

   while ( --d4->num_locked >= 0 )
      if ( h4unlock( &d4->file, L4LOCK_POS+ d4->locks[d4->num_locked], 1L ) < 0 )  return -1 ;
   return 0 ;
}
               

