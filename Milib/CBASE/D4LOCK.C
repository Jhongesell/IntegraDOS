/* d4data.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#define L4LOCK_POS  1000000000L

int  S4FUNCTION d4lock( D4DATA *d4, long rec )
{
   int rc ;

   #ifdef S4DEBUG
      if ( rec <= 0 )
         e4severe( e4parm, "d4lock()", (char *) 0 ) ;
   #endif

   if ( d4lock_test(d4,rec) > 0 )  return 0 ;
   d4unlock_append(d4) ;
   d4unlock_index(d4) ;
   d4unlock_records(d4) ;
   if ( (rc = h4lock( &d4->file, L4LOCK_POS+rec, 1L) ) != 0 )  return rc ;

   d4->num_locked =  1 ;
   *d4->locks =  rec ;
   return 0 ;
}

int  S4FUNCTION d4lock_append( D4DATA *d4 )
{
   int rc ;

   if ( d4 == 0 )  return -1 ;

   if ( d4lock_test_append(d4) > 0 )  return 0 ;
   d4unlock_index(d4) ;

   rc =  h4lock( &d4->file, L4LOCK_POS, 1L ) ;
   if ( rc != 0 )  return rc ;
   d4->append_lock =  1 ;
   return 0 ;
}

int  S4FUNCTION d4lock_file( D4DATA *d4 )
{
   int rc ;

   if ( d4lock_test_file(d4) > 0 )  return 0 ;
   d4unlock_all(d4) ;
   if ( (rc = h4lock(&d4->file, L4LOCK_POS,L4LOCK_POS)) != 0 )  return rc ;
   d4->file_lock =  1 ;
   return 0 ;
}

int S4FUNCTION d4lock_index( D4DATA *d4)
{
   int rc, are_locked ;
   I4INDEX *index_on ;

   if ( d4 == 0 )  return -1 ;

   /* First do any necessary unlocking. */
   are_locked =  1 ;
   for ( index_on = 0; index_on = (I4INDEX *) l4next(&d4->indexes,index_on);)
   {
      if ( index_on->file_locked && are_locked )  continue ;

      are_locked =  0 ;
      if ( i4unlock( index_on ) != 0 )
         return -1 ;
   }

   for ( index_on = 0; index_on = (I4INDEX *) l4next(&d4->indexes,index_on);)
      if ( (rc =  i4lock(index_on)) != 0 )  return rc ;
   if ( d4->code_base->error_code < 0 )  return -1 ;

   return  0 ;
}

int  S4FUNCTION d4lock_group( D4DATA *d4, long *recs, int n_recs )
{
   int i, rc ;

   if ( d4lock_test_file(d4) > 0 )  return 0 ;

   d4unlock_append(d4) ;
   d4unlock_index(d4) ;
   if ( d4unlock_records(d4) < 0 )  return -1 ;

   if ( d4->n_locks < n_recs )
   {
      if ( d4->n_locks > 1)  u4free(d4->locks) ;
      d4->locks =  (long *) u4alloc( sizeof(long) * n_recs ) ;
      if ( d4->locks == 0 )
      {
         d4->n_locks =  1 ;
         d4->locks =  &d4->locked_record ;
         return  e4error( d4->code_base, e4memory, (char *) 0 ) ;
      }
      d4->n_locks =  n_recs ;
   }

   d4->num_locked =  n_recs ;
   for ( i = 0; i < n_recs; i++ )
   {
      #ifdef S4DEBUG
         if ( i > 0 )
            if ( recs[i-1] >= recs[i] )
               e4severe( e4parm, E4_PARM_REC, (char *) 0 ) ;
      #endif

      if ( (rc = h4lock( &d4->file, L4LOCK_POS+recs[i], 1L )) != 0 )
      {
         d4->num_locked =  i ;
         return rc ;
      }
      d4->locks[i] =  recs[i] ;
   }

   return 0 ;
}

int S4FUNCTION d4lock_test_file( D4DATA *d4 )
{
   if ( d4 == 0 )  return -1 ;
   if ( d4->code_base->error_code < 0 ) return -1 ;
   return  d4->file_lock ? 1 : 0 ;
}

int S4FUNCTION d4lock_test_append( D4DATA *d4 )
{
   if ( d4 == 0 )  return -1 ;
   if ( d4->code_base->error_code < 0 ) return -1 ;
   return  (d4->file_lock || d4->append_lock)  ?  1 : 0 ;
}

int S4FUNCTION d4lock_test( D4DATA *d4, long rec )
{
   int i ;

   if ( d4 == 0 )  return -1 ;
   if ( d4->code_base->error_code < 0 ) return -1 ;
   if ( d4->file_lock )  return 1 ;
   for ( i = 0; i < d4->num_locked; i++ )
      if ( d4->locks[i] == rec )  return 1 ;
   return 0 ;
}

