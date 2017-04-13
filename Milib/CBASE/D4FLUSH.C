/* d4flush.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int S4FUNCTION d4flush_all( D4DATA *d4 )
{
   int rc ; 
   #ifdef S4FOX
      I4INDEX *index_on, *index_start ;
   #else
      T4TAG *tag_on ;
   #endif

   rc =  d4flush_record(d4) ;  if ( rc != 0 )  return rc ;

   #ifdef S4FOX
      index_on = index_start = (I4INDEX *) l4first( &d4->indexes ) ;
      if ( index_on != 0 )
      do
      {
	 rc = i4flush( index_on ) ;
	 if ( rc != 0 )  return rc ;
	 l4next( &d4->indexes, index_on ) ;
      } while ( index_on != index_start ) ;
   #else
      for( tag_on = 0;;)
      {
         tag_on =  d4tag_next( d4, tag_on ) ;
         if ( tag_on == 0 )   break ;

         rc = t4flush(tag_on) ;
         if ( rc != 0 )  return rc ;
      }
   #endif
      
   return 0 ;
}

int S4FUNCTION d4flush_record( D4DATA *d4)
{
   int i, rc ;

   if ( d4 == 0 )  return -1 ;
   if ( d4->code_base->error_code < 0 )  return -1 ;

   if ( d4->rec_num <= 0 || d4eof(d4) )
   {
      for ( i = 0; i < d4->n_fields_memo; i++ )
         m4reset( d4->fields_memo[i].field ) ;
      d4->record_changed = 0 ;
      return 0 ;
   }

   if ( d4->record_changed )
   {
      rc =  d4write( d4, d4->rec_num ) ;
      if ( rc != 0 )  return rc ;
   }

   for ( i = 0; i < d4->n_fields_memo; i++ )
      m4reset( d4->fields_memo[i].field ) ;

   return 0 ;
}


