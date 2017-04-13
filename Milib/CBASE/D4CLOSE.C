/* d4close.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

extern char m4null_char ;

int S4FUNCTION d4close( D4DATA *d4 )
{
   int final_rc, save_wait, i ;
   I4INDEX *index_on, *index_next ;

   if ( d4 == 0 )  return -1 ;
   final_rc =  e4error_set( d4->code_base, 0) ;
   save_wait =  d4->code_base->wait ;
   d4->code_base->wait =  1 ;

   if ( h4open_test(&d4->file) )
   {
      if ( d4flush_all(d4) < 0 )  final_rc =  e4error_set(d4->code_base, 0) ;

      if ( d4->file_changed )
      {
         d4lock_append(d4) ;
         if ( d4update_header(d4,1,1) < 0 )  final_rc =  e4error_set(d4->code_base, 0) ;
      }
      if ( d4unlock_all(d4) < 0 )  final_rc =  e4error_set(d4->code_base,0) ;
   }

   for( index_next = (I4INDEX *) l4first(&d4->indexes);; )
   {
      index_on =  index_next ;
      index_next =  (I4INDEX *) l4next( &d4->indexes, index_on ) ;
      if ( index_on == 0 )  break ;

      if ( i4close(index_on) < 0 )
         final_rc =  e4error_set(d4->code_base,0) ;
   }

   if ( h4open_test(&d4->file) )
      l4remove( &d4->code_base->data_list, d4 ) ;

   if ( h4close(&d4->file) < 0 )  final_rc =  e4error_set(d4->code_base,0) ;

   u4free( d4->record ) ;
   u4free( d4->record_old ) ;
   u4free( d4->fields )  ;

   if ( d4->fields_memo != 0 )
   {
      for ( i = 0; i < d4->n_fields_memo; i++ )
         if ( d4->fields_memo[i].contents != &m4null_char )
            u4free( d4->fields_memo[i].contents ) ;
      u4free( d4->fields_memo ) ;
   }

   if ( d4->header.version & 0x80 )
      if ( h4close(&d4->memo_file.file) < 0 )
         final_rc =  e4error_set(d4->code_base,0) ;

   if ( d4->n_locks > 1 )
      u4free( d4->locks ) ;

   d4->code_base->wait =  save_wait ;
   y4free( d4->code_base->data_memory, d4 ) ;

   e4error_set( d4->code_base, final_rc ) ;
   return  final_rc ;
}

