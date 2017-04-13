/* d4write.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int S4FUNCTION d4write( D4DATA *d4, long rec )
{
   /* 0. Validate memo id's */
   /* 1. Update Keys */
   /* 2. Update Memo Information */
   /* 3. Update Data H4FILE */

   int rc, final_rc, i ; 

   if ( d4->n_fields_memo > 0 )
      if ( (rc =  d4validate_memo_ids(d4)) != 0 )  return rc ;
   if ( (rc =  d4write_keys(d4,rec)) != 0 )  return rc ;

   final_rc =  0 ;

   /* First cycle through the fields to be flushed */
   for ( i = 0; i < d4->n_fields_memo; i++ )
   {
      if ( (rc = m4flush( d4->fields_memo[i].field)) < 0 )  return -1 ;
      if ( rc > 0 )  final_rc =  rc ;
   }

   if ( d4write_data( d4, rec ) < 0 )  return -1 ;
   return final_rc ;
}

int S4FUNCTION d4write_data( D4DATA *d4, long rec )
{
   int rc ;

   #ifdef S4DEBUG
      if ( d4->code_base->error_code < 0 )  return -1 ;
      if ( rec <= 0L )  e4severe( e4parm, "d4write()", (char *) 0 ) ;
   #endif
   rc =  d4lock( d4, rec ) ;  if ( rc != 0 )  return rc ;
   d4->record_changed =  0 ;
   d4->file_changed =  1 ;
   return h4write( &d4->file, d4rec_pos(d4, rec), d4->record, d4->record_width ) ;
}

int S4FUNCTION d4write_keys( D4DATA *d4, long rec )
{
   char  new_key_buf[I4MAX_KEY_SIZE] ;
   char *old_key, *temp_ptr, *old_buffer ;
   int rc, rc2, save_error, key_len, old_key_added, add_new_key ;
   T4TAG *tag_on ;

   if ( d4 == 0 )  return -1 ;

   if ( d4->code_base->error_code < 0 )   return -1 ;
   d4->bof_flag =  d4->eof_flag = 0 ;
   d4->record_changed = 0 ;

   rc =  d4lock(d4, rec) ;  if ( rc != 0 )  return rc ;

   if ( d4->indexes.n_link > 0 )
   {
      if ( d4read_old( d4, d4->rec_num ) < 0 )  return -1 ;
      if ( memcmp( d4->record_old, d4->record, d4->record_width) == 0 )
         return 0 ;
   }

   rc =  d4lock_index(d4) ;  if ( rc != 0 )  return rc ;

   for( tag_on = 0;;)
   {
      tag_on =  d4tag_next( d4, tag_on ) ;
      if ( tag_on == 0 )   break ;

      old_key_added =  add_new_key =  1 ;

      key_len =  e4key( tag_on->expr, &temp_ptr ) ;
      if ( key_len < 0 )  return -1 ;
      #ifdef S4DEBUG
	 #ifdef S4MDX
            if ( key_len != tag_on->header.value_len || key_len > I4MAX_KEY_SIZE)
	 #else
            if ( key_len != tag_on->header.key_len || key_len > I4MAX_KEY_SIZE)
         #endif
   	       e4severe( e4result, (char *) 0 ) ;
      #endif

      memcpy( new_key_buf, temp_ptr, key_len ) ;

      if ( tag_on->filter )
         add_new_key =  e4true( tag_on->filter ) ;

      old_buffer =  d4->record ;
      d4->record =  d4->record_old ;

      if ( tag_on->filter )
         old_key_added =  e4true( tag_on->filter ) ;
      key_len = e4key( tag_on->expr, &old_key ) ;

      d4->record =  old_buffer ;

      if ( key_len < 0 )  return key_len ;
      if ( old_key_added == add_new_key )   
	 if ( memcmp( new_key_buf, old_key, key_len) == 0 )
	    continue ;

      if ( old_key_added )
         rc =  t4remove( tag_on, old_key, rec ) ;

      if ( rc == 0 )
      {
         if ( add_new_key )
            rc =  t4add( tag_on, new_key_buf, rec ) ;
         if ( rc < 0 || rc == r4unique )
         {
            save_error =  e4error_set(d4->code_base,0) ;
            if ( old_key_added )
               if ( t4add(tag_on,old_key,rec) < 0 )  return -1 ;
            e4error_set(d4->code_base,save_error) ;
         }
      }

      #ifdef S4DEBUG
         if ( rc == r4locked )  e4severe( e4result, "d4write()", (char *) 0 ) ;
      #endif

      if ( rc < 0  ||  rc == r4unique )
      {
         save_error =  e4error_set( d4->code_base, 0 ) ;

         /* Remove the keys which were just added */
         for( tag_on = 0;;)
         {
            tag_on =  d4tag_next( d4, tag_on ) ;
            if ( tag_on == 0 )   break ;

            old_buffer =  d4->record ;  
            d4->record =  d4->record_old ;

            rc2 = t4remove_calc(tag_on, rec) ;

            d4->record =  old_buffer ;
            if ( rc2 < 0 )  return -1 ;

            if ( t4add_calc(tag_on, rec) < 0 )  return -1 ;
         }
         e4error_set( d4->code_base, save_error) ;
         return rc ;
      }
   }
   return 0 ;
}


