/* d4append.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int S4FUNCTION d4append( D4DATA *d4 )
{
   int rc, rc2, save_error, i ;
   T4TAG *tag_on ;
   F4MEMO *mfield ;
   long    new_id ;

   #ifdef S4DEBUG
      if ( d4->code_base->error_code < 0 )  return -1 ;
      if ( d4->rec_num != 0 )
         e4severe( e4result, "d4append()", E4_RESULT_D4A, (char *) 0 ) ;
   #endif

   /* 0. Lock record count bytes
      1. Update Memo File
      2. Update index and data file */

   rc =  d4lock_append( d4 ) ;  if ( rc != 0 )  return rc ;

   for ( i = 0; i < d4->n_fields_memo; i++ )
   {
      mfield =  d4->fields_memo+i ;
      mfield->is_changed =  0 ;
      if ( mfield->len > 0 )
      {
         new_id =  0L ;
         rc =  m4file_write( &d4->memo_file, &new_id, mfield->contents, mfield->len ) ;
         if ( rc != 0 )  return rc ;
         f4assign_long( mfield->field, new_id ) ;
      }
   }

   /* Now update the index file */
   d4->bof_flag =  d4->eof_flag = 0 ;
   d4->record_changed = 0 ;

   rc =  d4lock_index( d4 ) ;  if ( rc != 0 )  return rc ;

   d4->rec_num =  d4reccount(d4) + 1 ;

   for( tag_on = 0;;)
   {
      tag_on =  d4tag_next( d4, tag_on ) ;
      if ( tag_on == 0 )   break ;

      rc = t4add_calc( tag_on, d4->rec_num) ;
      if ( rc < 0 || rc ==  r4locked || rc == r4unique )
      {
         save_error =  e4error_set( d4->code_base, 0 ) ;

         /* Remove the keys which were just added */

	 while ( tag_on = d4tag_prev(d4,tag_on) )
            if ( (rc2 = t4remove_calc( tag_on, d4->rec_num)) < 0 )  return rc2 ;

         e4error_set( d4->code_base, save_error ) ;
         d4->rec_num =  0 ;
         return rc ;
      }
   }

   return d4append_data( d4 ) ;
}

int  S4FUNCTION d4append_start( D4DATA *d4, int use_memo_entries )
{
   int   rc, i ;
   char *save_ptr ;

   rc = d4flush_record(d4) ;   if ( rc != 0 )  return rc ;
   if ( d4->rec_num <= 0 )  use_memo_entries =  0 ;

   if ( use_memo_entries && d4->n_fields_memo > 0 )
   {
      #ifdef S4DEBUG
         if ( ! h4open_test( &d4->memo_file.file ) )
         e4severe( e4data, E4_DATA_MEM, (char *) 0 ) ;
      #endif

      /* Read in the current memo entries of the current record */
      if ( (rc = d4lock(d4, d4->rec_num)) != 0 )  return rc ;

      save_ptr   =  d4->record ;
      d4->record =  d4->record_old ;

      d4go_data( d4, d4->rec_num ) ;

      for ( i = 0; i < d4->n_fields_memo; i++ )
         m4read_low( d4->fields_memo[i].field ) ;

      d4->record =  save_ptr ;

      if ( d4->code_base->error_code < 0 )  return -1 ;
   }

   for ( i = 0; i < d4->n_fields_memo; i++ )
      f4assign_long( d4->fields_memo[i].field, 0 ) ;

   d4->rec_num =  0 ;
   return 0 ;
}

