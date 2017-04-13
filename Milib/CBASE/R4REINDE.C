/* r4reindex.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#include "r4reinde.h"

#ifndef N4OTHER

int S4FUNCTION i4reindex( I4INDEX *i4 )
{
   R4REINDEX reindex ;
   int rc ;
   #ifdef S4FOX
      T4TAG *tag_on ;
      B4BLOCK *block ;
      long r_node, go_to ;
      char tag_name[11] ;
      int i ;
   #endif

   if ( ! i4->file_locked )
   {
      if ( i4unlock(i4) < 0 )  return -1 ;
      rc =  h4lock( &i4->file, L4LOCK_POS, L4LOCK_POS ) ;
      if ( rc != 0 )  return rc ;
      i4->file_locked =  1 ;
   }

   r4reindex_init( &reindex, i4 ) ;
   r4reindex_tag_headers_calc( &reindex ) ;
   r4reindex_blocks_alloc(&reindex) ;

   #ifdef S4FOX
      tag_name[10] = '\0' ;

      if ( i4->tag_index->header.type_code >= 64 )  /* if .cdx */
      {
         reindex.tag = i4->tag_index ;
         if ( s4init( &reindex.sort, i4->code_base, i4->tag_index->header.key_len, 0 ) < 0 )  return -1 ;
         reindex.sort.cmp = u4memcmp ;
   
         for( tag_on = 0, i=1; tag_on =  (T4TAG *) l4next(&i4->tags, tag_on); i++ )
         {
            int len = strlen( tag_on->alias ) ;
            memset( tag_name, ' ', 10 ) ;
            memcpy( tag_name, tag_on->alias, len ) ; 
            if ( s4put( &reindex.sort, 2*B4BLOCK_SIZE*i, tag_name, "" ) < 0)
               return -1 ;
            #ifdef S4DEBUG
               reindex.key_count++ ;
            #endif
         }
   
         if ( (rc = r4reindex_write_keys(&reindex)) != 0 )
         {
            r4reindex_free( &reindex ) ;
            return rc ;
         }
      }
      else
         if ( reindex.n_tags > 1 )   /* should only be 1 tag in an .idx */
            return e4error( i4->code_base, e4index, "Corrupt .idx index file", (char *) 0 ) ;
   #endif

   for( reindex.tag = 0; reindex.tag =  (T4TAG *) l4next(&i4->tags, reindex.tag); )
   {
      r4reindex_supply_keys( &reindex ) ;
      if ( (rc = r4reindex_write_keys(&reindex)) != 0 )
      {
         r4reindex_free( &reindex ) ;
         return rc ;
      }
   }

   rc =  r4reindex_tag_headers_write( &reindex ) ;

   #ifdef S4FOX
      /* now must fix the right node branches for all blocks by moving leftwards */
      for( tag_on = 0; tag_on =  (T4TAG *) l4next(&i4->tags, tag_on); )
      {
         for( t4rl_bottom( tag_on ) ; tag_on->blocks.last ; t4up( tag_on ) )
         {
            block  = t4block( tag_on ) ;
            r_node = block->header.left_node ;
            go_to  = block->header.left_node ;
   
            while ( go_to != -1 )
            {
               r_node = block->file_block ;
               if ( block->changed )
                  if ( b4flush( block ) < 0 )  return -1 ;
               if ( h4read_all( &tag_on->index->file, I4MULTIPLY*go_to,
                  &block->header, B4BLOCK_SIZE) < 0 ) return -1 ;
               block->file_block = go_to ;
               if ( block->header.right_node != r_node )  /* if a bad value */
               {
	          block->header.right_node = r_node ;
                  block->changed = 1 ;
               }
               go_to = block->header.left_node ;
            }
         }
      }
   #endif

   r4reindex_free( &reindex ) ;
   return rc ;
}

int r4reindex_init( R4REINDEX *r4, I4INDEX *i4 )
{
   memset( (void *)r4, 0, sizeof(R4REINDEX) ) ;

   r4->index =  i4 ;
   r4->data  =  i4->data ;
   r4->code_base =  i4->code_base ;

   r4->min_keysmax = INT_MAX ;
   r4->start_block =  0 ;

   #ifndef S4FOX
      r4->blocklen =  i4->header.block_rw ;
   #endif

   r4->buffer_len =  i4->code_base->mem_size_sort_buffer ;
   if ( r4->buffer_len < 1024 )  r4->buffer_len =  1024 ;

   r4->buffer =  (char *) u4alloc( r4->buffer_len ) ;
   if ( r4->buffer == 0 )
      return e4error( i4->code_base, e4memory, (char *) 0 ) ;

   #ifdef S4FOX
      r4->lastblock = 1024 ;  /* leave space for the index header block  */
   #endif

   return 0 ;
}

void r4reindex_free( R4REINDEX *r4 )
{
   u4free( r4->buffer ) ;
   u4free( r4->start_block ) ;
   s4free( &r4->sort ) ;
}

int r4reindex_blocks_alloc( R4REINDEX *r4 )
{
   long on_count ;

   if ( r4->code_base->error_code < 0 )  return -1 ;

   #ifdef S4DEBUG
      if ( (unsigned) r4->min_keysmax > INT_MAX )
         e4severe( e4info, "r4reindex_blocks_alloc()", (char *) 0 ) ;
   #endif

   /* Calculate the block stack height */
   on_count =  d4reccount( r4->data ) ;
   for ( r4->n_blocks = 2; on_count != 0L; r4->n_blocks++ )
      on_count /=  r4->min_keysmax ;

   #ifdef S4FOX
      r4->start_block =  (R4BLOCK_DATA *) u4alloc( (long) B4BLOCK_SIZE * r4->n_blocks ) ;
   #else
      r4->start_block =  (R4BLOCK_DATA *) u4alloc( (long) r4->blocklen * r4->n_blocks ) ;
   #endif

   if ( r4->start_block == 0 )
      return e4error( r4->code_base, e4memory, E4_MEMORY_B, (char *) 0 ) ;

   #ifdef S4FOX
      r4->n_blocks_used = 0 ;
   #endif

   return 0 ;
}

int r4reindex_supply_keys( R4REINDEX *r4 )
{
   H4SEQ_READ seq_read ;
   E4EXPR *expr, *filter ;
   char   *key_result ;
   int     i, *filter_result ;
   long    count, i_rec ;
   D4DATA *d4 ;
   T4TAG  *t4 ;

   d4 =  r4->data ;
   t4 =  r4->tag ;
   #ifdef S4DEBUG
      r4->key_count =  0L ;
   #endif

   h4seq_read_init( &seq_read, &d4->file, d4rec_pos(d4,1L), r4->buffer,
          r4->buffer_len) ;

   #ifdef S4FOX
      if ( s4init( &r4->sort, r4->code_base, t4->header.key_len, 0 ) < 0 )  return -1 ;

      r4->sort.cmp = u4memcmp ;
   #else
      if ( s4init( &r4->sort, r4->code_base, t4->header.value_len, 0 ) < 0 )  return -1 ;

      r4->sort.cmp =  t4->cmp ;
   #endif

   expr  =  t4->expr ;
   filter=  t4->filter ;
   count =  d4reccount( d4 ) ;

   for ( i_rec = 1L; i_rec <= count; i_rec++ )
   {
      if ( h4seq_read_all( &seq_read, d4->record, d4->record_width ) < 0 )
         return -1 ;
      d4->rec_num =  i_rec ;

      for ( i = 0; i < d4->n_fields_memo; i++ )
         m4reset( d4->fields_memo[i].field ) ;

      if ( filter )
      {
         e4vary( filter, (char **) &filter_result ) ;
         #ifdef S4DEBUG
            if ( e4type( filter ) != t4log )
               e4severe( e4result, "r4reindex_supply_keys()", (char *) 0 ) ;
         #endif
         if ( ! *filter_result )  continue ;
         t4->has_keys = 1 ;
      }

      e4key( expr, &key_result ) ;

      if ( s4put( &r4->sort, i_rec, key_result, "" ) < 0)
         return -1 ;
      #ifdef S4DEBUG
         r4->key_count++ ;
      #endif
   }

   return 0 ;
}

int  r4reindex_tag_headers_calc( R4REINDEX *r4 )
{
   char *result ;
   T4TAG *tag ;
   int keysmax ;

   r4->n_tags =  0 ;
   for( tag =  0; tag =  (T4TAG *) l4next( &r4->index->tags, tag); )
   {
      if ( t4free_all(tag) < 0 )  return -1 ;

      #ifdef S4FOX
	 tag->header.key_len =  e4key( tag->expr, &result ) ;
	 t4init_seek_conv( tag, tag->expr->type ) ;
	 if ( tag->header.key_len < 0 )  return -1 ;

	 keysmax = ( B4BLOCK_SIZE - sizeof(B4STD_HEADER) ) /
				( tag->header.key_len + 2*sizeof(long) ) ;

	 if ( keysmax < r4->min_keysmax )
	    r4->min_keysmax =  keysmax ;
      #else
	 tag->header.value_len =  e4key( tag->expr, &result ) ;
	 if ( tag->header.value_len < 0 )  return -1 ;
   
         tag->header.type = (char) e4type( tag->expr ) ;
         t4init_seek_conv( tag, tag->header.type ) ;
         tag->header.group_len =  tag->header.value_len+ 2*sizeof(long)-1 ;
         tag->header.group_len-=  tag->header.group_len % sizeof(long) ;
   
	 tag->header.keys_max =
                 (r4->index->header.block_rw - sizeof(short)-6-sizeof(long)) / 
                      tag->header.group_len;
         if ( tag->header.keys_max < r4->min_keysmax )
            r4->min_keysmax =  tag->header.keys_max ;
      #endif

      r4->n_tags++ ;
   }

   #ifdef S4FOX
      if ( r4->index->tag_index->header.type_code >= 64 )
      {
         r4->lastblock += ((long)r4->n_tags-1)*2*B4BLOCK_SIZE + B4BLOCK_SIZE ;
         t4init_seek_conv( r4->index->tag_index, t4str ) ;
      }
      else
         r4->lastblock -= B4BLOCK_SIZE ;
   #else
      r4->lastblock_inc =  r4->index->header.block_rw/ 512 ;
      r4->lastblock =  4 + (r4->n_tags-1)*r4->lastblock_inc ;
   #endif

   return 0 ;
}

T4TAG *r4reindex_find_i_tag( R4REINDEX *r4, int i_tag )
{
   /* First 'i_tag' starts at '1' for this specific routine */
   T4TAG *tag_on ;
   tag_on =  (T4TAG *) l4first( &r4->index->tags ) ;

   while ( --i_tag >= 1 )
   {
      tag_on =  (T4TAG *) l4next( &r4->index->tags, tag_on ) ;
      if ( tag_on == 0 )  return 0 ;
   }
   return tag_on ;
}

#ifndef S4FOX

#define GARBAGE_LEN 518

int r4reindex_tag_headers_write( R4REINDEX *r4 )
{
   /* First, calculate the T4DESC.left_chld, T4DESC.right_chld values, T4DESC.parent values */
   int  higher[49], lower[49], parent[49] ;
   T4TAG *tag_on, *tag_ptr ;
   I4INDEX *i4 ;
   D4DATA  *d4 ;
   int n_tag, i_tag, j_field, len, save_code ;
   T4DESC tag_info ;
   char *ptr ;

   memset( (void *)higher, 0, sizeof(higher) ) ;
   memset( (void *)lower,  0, sizeof(lower) ) ;
   memset( (void *)parent, 0, sizeof(parent) ) ;

   i4 =  r4->index ;
   d4 =  r4->data ;

   tag_on =  (T4TAG *) l4first( &i4->tags ) ;
   if ( tag_on != 0 )
   {
      n_tag = 1 ;

      for (; tag_on = (T4TAG *) l4next( &i4->tags, tag_on); )
      {
	 n_tag++ ;
	 i_tag = 1 ;
	 for (;;)
	 {
	    tag_ptr =  r4reindex_find_i_tag( r4, i_tag ) ;
	    #ifdef S4DEBUG
	       if ( tag_ptr == 0 || i_tag < 0 || i_tag >= 48 || n_tag > 48 )
		  e4severe( e4result, "r4reindex_tag_headers_write()", (char *) 0 );
	    #endif
	    if ( memcmp( tag_on->alias, tag_ptr->alias, sizeof(tag_on->alias)) < 0)
	    {
	       if ( lower[i_tag] == 0 )
	       {
		  lower[i_tag] =  n_tag ;
		  parent[n_tag] = i_tag ;
		  break ;
	       }
	       else
		  i_tag =  lower[i_tag] ;
	    }
	    else
	    {
	       if ( higher[i_tag] == 0 )
	       {
		  higher[i_tag] =  n_tag ;
		  parent[n_tag] =  i_tag ;
		  break ;
	       }
	       else
		  i_tag =  higher[i_tag] ;
	    }
	 }
      }
   }

   /* Now write the headers */
   h4seq_write_init( &r4->seqwrite, &i4->file, 0L, r4->buffer, r4->buffer_len ) ;

   i4->header.eof =  r4->lastblock + r4->lastblock_inc ;
   i4->header.free_list = 0L ;
   u4yymmdd( i4->header.yymmdd ) ;
   h4seq_write( &r4->seqwrite, &i4->header, sizeof(I4HEADER) ) ;

   h4seq_write_repeat( &r4->seqwrite, 512-sizeof(I4HEADER)+17, 0 ) ;
   /* There is a 0x01 on byte 17 of the first 32 bytes. */
   h4seq_write( &r4->seqwrite, "\001", 1 ) ;  
   h4seq_write_repeat( &r4->seqwrite, 14, 0 ) ;

   tag_on =  (T4TAG *) l4first( &i4->tags ) ;

   for ( i_tag = 0; i_tag < 47; i_tag++ )
   {
      memset( (void *)&tag_info, 0, sizeof(tag_info) ) ;

      if ( i_tag < r4->n_tags )
      {
         tag_info.header_pos =  4 + (long) i_tag * r4->lastblock_inc ;
         tag_on->header_offset =  tag_info.header_pos * 512 ;

         memcpy( (void *)tag_info.tag, tag_on->alias, sizeof(tag_info.tag) ) ;

         tag_info.index_type =  tag_on->header.type ;
         tag_info.x1000  =  0x1000 ;
         tag_info.x2     =  2 ;
         tag_info.left_chld =  (char) lower[i_tag+1] ;
	 tag_info.right_chld  =  (char) higher[i_tag+1] ;
         tag_info.parent = (char) parent[i_tag+1] ;

         if ( i4->header.is_production )
         {
            save_code =  i4->code_base->field_name_error ;
            i4->code_base->field_name_error =  0 ;
	    j_field =  d4field_number( d4, tag_on->expr->source ) ;
            i4->code_base->field_name_error =  save_code ;
            if ( j_field > 0 )
               h4write( &d4->file, (j_field+1)*sizeof(F4FIELD_IMAGE)-1, "\001", 1 ) ;
         }
         tag_on =  (T4TAG *) l4next( &i4->tags, tag_on ) ;
      }

      if ( h4seq_write( &r4->seqwrite, &tag_info, sizeof(tag_info)) < 0 )  return -1 ;
   }

   for (tag_on = 0; tag_on = (T4TAG *) l4next(&i4->tags,tag_on); )
   {
      if ( h4seq_write( &r4->seqwrite, &tag_on->header,sizeof(tag_on->header)) < 0 )
         return -1 ;

      ptr =  tag_on->expr->source ;
      len =  strlen(ptr) ;
      h4seq_write( &r4->seqwrite, ptr, len) ;
      h4seq_write_repeat( &r4->seqwrite, 221-len, 0 ) ;

      if( tag_on->filter != 0 )
      {
         h4seq_write_repeat( &r4->seqwrite, 1, 1 ) ;
         if (tag_on->has_keys)
            h4seq_write_repeat( &r4->seqwrite, 1, 1 ) ;
         else
            h4seq_write_repeat( &r4->seqwrite, 1, 0 ) ;
      }
      else
         h4seq_write_repeat( &r4->seqwrite, 2, 0 ) ;

      /* write extra space up to filter write point */
      h4seq_write_repeat( &r4->seqwrite, GARBAGE_LEN-3 , 0 ) ;

      if ( tag_on->filter == 0 )
         len =  0 ;
      else
      {
         ptr =  tag_on->filter->source ;
         len =  strlen(ptr) ;
         h4seq_write( &r4->seqwrite, ptr, len ) ;
      }
      h4seq_write_repeat( &r4->seqwrite, 
         r4->blocklen - GARBAGE_LEN - len - 220 - sizeof(tag_on->header), 0 );
   }
   h4length_set( &i4->file, i4->header.eof * 512) ;

   if ( h4seq_write_flush(&r4->seqwrite) < 0 ) return -1 ;
   return 0 ;
}

int r4reindex_write_keys( R4REINDEX *r4 )
{
   T4TAG *t4 ;
   char  last_key[I4MAX_KEY_SIZE], *key_data ;
   int   is_unique, rc ;
   void *dummy_ptr ;
   long  key_rec ;

   t4 =  r4->tag ;

   r4->grouplen =  t4->header.group_len ;
   r4->valuelen =  t4->header.value_len ;
   r4->keysmax  =  t4->header.keys_max ;
   memset( (void *)r4->start_block, 0, r4->n_blocks*r4->blocklen ) ;

   if ( s4get_init(&r4->sort) < 0 )  return -1 ;
   h4seq_write_init( &r4->seqwrite, &r4->index->file, (r4->lastblock+r4->lastblock_inc)*512, r4->buffer,r4->buffer_len) ;

   #ifdef S4DEBUG
      if ( I4MAX_KEY_SIZE < r4->sort.sort_len )  
         e4severe( e4info, "r4reindex_write_keys()", E4_INFO_EXK, (char *) 0 ) ;
   #endif

   memset( (void *)last_key, 0, sizeof(last_key) ) ;
   is_unique =  t4->header.unique ;

   for(;;)  /* For each key to write */
   {
      if ( (rc = s4get( &r4->sort, &key_rec, (void **) &key_data, &dummy_ptr)) < 0)  return -1 ;

      #ifdef S4DEBUG
         if ( r4->key_count < 0L  ||  r4->key_count == 0L && rc != 1
              ||  r4->key_count > 0L && rc == 1 )
            e4severe( e4info, "r4reindex_write_keys()", (char *) 0 ) ;
         r4->key_count-- ;
      #endif

      if ( rc == 1 )  /* No more keys */
      {
         if ( r4reindex_finish(r4) < 0) return -1 ;
         if ( h4seq_write_flush(&r4->seqwrite) < 0 )  return -1 ;
         break ;
      }

      if ( is_unique )
      {
         if ( (*t4->cmp)( key_data, last_key, r4->sort.sort_len) == 0 )
         {
            switch( t4->unique_error )
            {
               case e4unique:
                  return e4error( r4->code_base, e4unique, E4_UNIQUE, t4->alias, (char *) 0 ) ;

               case r4unique:
                  return r4unique ;

               default:
                  continue ;
            }
         }
         memcpy( last_key, key_data, r4->sort.sort_len ) ;
      }

      /* Add the key */
      if ( r4reindex_add( r4, key_rec, key_data) < 0 )  return -1 ;
   }

   /* Now complete the tag header info. */
   t4->header.root =  r4->lastblock ;
   return 0 ;
}

int r4reindex_add( R4REINDEX *r4, long rec, char *key_value )
{
   B4KEY_DATA *key_to ;
   R4BLOCK_DATA *start_block ;
   #ifdef S4DEBUG
      long  dif ;
   #endif

   start_block =  r4->start_block ;
   if ( start_block->n_keys >= r4->keysmax )
   {
      if ( r4reindex_todisk(r4) < 0 )  return -1 ;
      memset( (void *)start_block, 0, r4->blocklen ) ;
   }

   key_to =  (B4KEY_DATA *)
        (start_block->info + (start_block->n_keys++) * r4->grouplen) ;

   #ifdef S4DEBUG
      dif =  (char *) key_to -  (char *) start_block ;
      if ( dif+ r4->grouplen > r4->blocklen || dif < 0 )
         e4severe( e4result, "r4reindex_add()", (char *) 0 ) ;
   #endif
   key_to->num =  rec ;
   memcpy( (void *)key_to->value, key_value, r4->valuelen ) ;

   return 0 ;
}

int r4reindex_finish( R4REINDEX *r4 )
{
   R4BLOCK_DATA *block ;
   int i_block ;
   B4KEY_DATA *key_to ;
   #ifdef S4DEBUG
      long dif ;
   #endif

   if ( h4seq_write(&r4->seqwrite, r4->start_block,r4->blocklen) < 0 ) return -1 ;
   r4->lastblock +=  r4->lastblock_inc ;
   block =  r4->start_block ;

   for( i_block=1; i_block < r4->n_blocks; i_block++ )
   {
      block =  (R4BLOCK_DATA *) ((char *)block + r4->blocklen) ;
      if ( block->n_keys >= 1 )
      {
         key_to =  (B4KEY_DATA *) (block->info + block->n_keys*r4->grouplen) ;
         #ifdef S4DEBUG
            dif =  (char *) key_to  -  (char *) block ;
            if ( dif+sizeof(long) > r4->blocklen  ||  dif<0 )
               e4severe( e4result, "r4reindex_finish()", (char *) 0 ) ;
         #endif
         key_to->num =  r4->lastblock ;
                        
         if ( h4seq_write( &r4->seqwrite, block,r4->blocklen) < 0) return -1;
         r4->lastblock +=  r4->lastblock_inc ;
      }
   }
   return 0 ;
}

int r4reindex_todisk( R4REINDEX *r4 )
{
   R4BLOCK_DATA *block ;
   int i_block ;
   B4KEY_DATA *key_on, *keyto ;
   #ifdef S4DEBUG
      long dif ;
   #endif

   /* Writes out the current block and adds references to higher blocks */
   block  =  r4->start_block ;
   i_block= 0 ;

   key_on = (B4KEY_DATA *) (block->info + (block->n_keys-1) * r4->grouplen) ;

   #ifdef S4DEBUG
      dif =  (char *) key_on -  (char *) block ;
      if ( dif+ r4->grouplen > r4->blocklen || dif < 0 )
         e4severe( e4result, "r4reindex_add()", (char *) 0 ) ;
   #endif

   for(;;)
   {
      if ( h4seq_write( &r4->seqwrite, block, r4->blocklen) < 0 ) return -1 ;
      if ( i_block != 0 )  memset( (void *)block, 0, r4->blocklen ) ;
      r4->lastblock +=  r4->lastblock_inc ;
   
      block =  (R4BLOCK_DATA *) ((char *)block + r4->blocklen) ;
      i_block++ ;
      #ifdef S4DEBUG
         if ( i_block >= r4->n_blocks )  e4severe( e4info, "r4reindex_todisk()", (char *) 0 ) ;
      #endif

      keyto =  (B4KEY_DATA *) (block->info +  block->n_keys * r4->grouplen) ;
      #ifdef S4DEBUG
         dif =  (char *) keyto -  (char *) block  ;
         if ( dif+sizeof(long) > r4->blocklen || dif < 0 )
            e4severe( e4result, "r4reindex_todisk()", (char *) 0 ) ;
      #endif
      keyto->num =  r4->lastblock ;

      if ( block->n_keys < r4->keysmax )
      {
         block->n_keys++ ;
         #ifdef S4DEBUG
            if ( dif+r4->grouplen > r4->blocklen )
               e4severe( e4result, "r4reindex_todisk()", (char *) 0 ) ;
         #endif
         memcpy( keyto->value, key_on->value, r4->valuelen ) ;
         return 0 ;
      }
   }
}
#endif   /* ifndef S4FOX */

#ifdef S4FOX
int r4reindex_tag_headers_write( R4REINDEX *r4 )
{
   T4TAG *tag_on, *tag_ptr ;
   I4INDEX *i4 ;
   int n_tag, j_field, save_code, tot_len, expr_hdr_len ;
   int i_tag = 2 ;
   char *ptr ;

   i4 =  r4->index ;

   /* Now write the headers  */
   h4seq_write_init( &r4->seqwrite, &i4->file, 0L, r4->buffer, r4->buffer_len ) ;

   i4->tag_index->header.free_list = 0L ;
   expr_hdr_len = 3*sizeof(short) + 4*sizeof(char) ;
   i4->eof =  r4->lastblock + B4BLOCK_SIZE ;

   if ( i4->tag_index->header.type_code >= 64 )
   {
      h4seq_write( &r4->seqwrite, &i4->tag_index->header, T4HEADER_WR_LEN ) ;  /* write first header part */
      h4seq_write_repeat( &r4->seqwrite, 486, 0 ) ;
      h4seq_write( &r4->seqwrite, &i4->tag_index->header.descending, expr_hdr_len ) ;
      h4seq_write_repeat( &r4->seqwrite, 512, 0 ) ;  /* no expression */
   }

   for (tag_on = 0; tag_on = (T4TAG *) l4next(&i4->tags,tag_on); )
   {
      if ( i4->tag_index->header.type_code >= 64 )
         tag_on->header_offset = ((long)i_tag) * B4BLOCK_SIZE ;
      else
         tag_on->header_offset = 0L ;

      if ( h4seq_write( &r4->seqwrite, &tag_on->header, T4HEADER_WR_LEN) < 0 )
         return -1 ;
      h4seq_write_repeat( &r4->seqwrite, 486, 0 ) ;
      h4seq_write( &r4->seqwrite, &tag_on->header.descending, expr_hdr_len ) ;

      ptr =  tag_on->expr->source ;
      tot_len = tag_on->header.expr_len ;
      h4seq_write( &r4->seqwrite, ptr, tag_on->header.expr_len ) ;

      if ( tag_on->filter != 0 )
      {
         ptr =  tag_on->filter->source ;
         h4seq_write( &r4->seqwrite, ptr, tag_on->header.filter_len ) ;
         tot_len += tag_on->header.filter_len ;
      }
      h4seq_write_repeat( &r4->seqwrite, B4BLOCK_SIZE - tot_len, 0 );
      i_tag += 2 ;
   }
   h4length_set( &i4->file, i4->eof ) ;

   if ( h4seq_write_flush(&r4->seqwrite) < 0 ) return -1 ;
   return 0 ;

}

int r4reindex_write_keys( R4REINDEX *r4 )
{
   char  last_key[I4MAX_KEY_SIZE], *key_data ;
   int   is_unique, rc, c_len, t_len, last_trail ;
   void *dummy_ptr ;
   long  key_rec, r_len ;
   T4TAG *t4 = r4->tag ;
   int   k_len = t4->header.key_len ;
   unsigned long ff = 0xFFFFFFFF ;
   R4BLOCK_DATA *r4block ;
   int on_count ;

   last_key[I4MAX_KEY_SIZE-1] = '\0' ;

   memset( last_key, r4->tag->p_char, k_len ) ;

   for ( c_len = 0 ; k_len ; k_len>>=1, c_len++ ) ;
   k_len = t4->header.key_len ;  /* reset the key length */
   r4->node_hdr.trail_cnt_len = r4->node_hdr.dup_cnt_len = c_len ;

   r4->node_hdr.trail_byte_cnt = (unsigned char)0xFF >> ( 8 - ((c_len / 8) * 8 + c_len % 8)) ;
   r4->node_hdr.dup_byte_cnt = r4->node_hdr.trail_byte_cnt ;

   r_len = d4reccount( r4->data ) ;
   for ( c_len = 0 ; r_len ; r_len>>=1, c_len++ ) ;
   r4->node_hdr.rec_num_len = c_len ;
   if ( r4->node_hdr.rec_num_len < 12 ) r4->node_hdr.rec_num_len = 12 ;

   for( t_len = r4->node_hdr.rec_num_len + r4->node_hdr.trail_cnt_len + r4->node_hdr.dup_cnt_len ;
        (t_len / 8)*8 != t_len ; t_len++, r4->node_hdr.rec_num_len++ ) ;  /* make at an 8-bit offset */

   r4->node_hdr.rec_num_mask = ff >> sizeof(long)*8 - r4->node_hdr.rec_num_len ;

   r4->node_hdr.info_len = (r4->node_hdr.rec_num_len + r4->node_hdr.trail_cnt_len + r4->node_hdr.dup_cnt_len) / 8 ;
   r4->valuelen =  t4->header.key_len ;
   r4->grouplen =  t4->header.key_len + 2*sizeof(long) ;

   memset( r4->start_block, 0, r4->n_blocks*B4BLOCK_SIZE ) ;

   #ifdef S4FOX
      for ( r4block = r4->start_block, on_count = 0 ; on_count < r4->n_blocks;
	    r4block = (R4BLOCK_DATA *) ( (char *)r4block + B4BLOCK_SIZE), on_count++ )
      {
         memset( r4block, 0, B4BLOCK_SIZE ) ;
         r4block->header.left_node = -1 ;
         r4block->header.right_node = -1 ;
      }
   #endif

   r4->node_hdr.free_space = B4BLOCK_SIZE - sizeof( B4STD_HEADER ) - sizeof( B4NODE_HEADER ) ;
   r4->keysmax  =  (B4BLOCK_SIZE - sizeof( B4STD_HEADER ) ) / r4->grouplen ;

   if ( s4get_init(&r4->sort) < 0 )  return -1 ;
   h4seq_write_init( &r4->seqwrite, &r4->index->file, r4->lastblock+B4BLOCK_SIZE, r4->buffer,r4->buffer_len) ;

   #ifdef S4DEBUG
      if ( I4MAX_KEY_SIZE < r4->sort.sort_len )  e4severe( e4info, "r4reindex_write_keys()", (char *) 0 ) ;
   #endif
   last_trail = k_len ;   /* default is no available duplicates */
   is_unique =  t4->header.type_code & 0x01 ;

   for(;;)  /* For each key to write */
   {
      if ( (rc = s4get( &r4->sort, &key_rec, (void **) &key_data, &dummy_ptr)) < 0)  return -1 ;

      #ifdef S4DEBUG
         if ( r4->key_count < 0L  ||  r4->key_count == 0L && rc != 1
              ||  r4->key_count > 0L && rc == 1 )
            e4severe( e4info, "r4reindex_write_keys()", (char *) 0 ) ;
         r4->key_count-- ;
      #endif

      if ( rc == 1 )  /* No more keys */
      {
         if ( r4reindex_finish(r4, last_key ) < 0) return -1 ;
         if ( h4seq_write_flush(&r4->seqwrite) < 0 )  return -1 ;
         break ;
      }

      if ( is_unique )
      {
         if ( (*t4->cmp)( key_data, last_key, r4->sort.sort_len) == 0 )
         {
            switch( t4->unique_error )
            {
               case e4unique:
                  return e4error( r4->code_base, e4unique, E4_UNIQUE, t4->alias, (char *) 0 ) ;

               case r4unique:
                  return r4unique ;

               default:
                  continue ;
            }
         }
      }

      /* Add the key */
      if ( r4reindex_add( r4, key_rec, key_data, last_key, &last_trail ) < 0 )  return -1 ;
      memcpy( last_key, key_data, r4->sort.sort_len ) ;
   }

   /* Now complete the tag header info. */
   t4->header.root =  r4->lastblock ;
   return 0 ;
}

/* for compact leaf nodes only */
int r4reindex_add( R4REINDEX *r4, long rec, char *key_value, char *last_key, int *last_trail )
{
   R4BLOCK_DATA *start_block = r4->start_block ;
   int dup, trail ;
   int k_len = r4->valuelen ;
   int i_len = r4->node_hdr.info_len ;
   int len ;
   unsigned char buffer[6] ;
   char *info_pos ;

   if ( start_block->header.n_keys == 0 )   /* reset */
   {
      dup = 0 ;
      r4->cur_pos = ((char *)start_block) + B4BLOCK_SIZE ;
      memcpy( ((char *)start_block) + sizeof( B4STD_HEADER ), &r4->node_hdr, sizeof( B4NODE_HEADER ) ) ;
      start_block->header.node_attribute |= 2 ;   /* leaf block */
      *last_trail = k_len ;
   }
   else
      dup = b4calc_dups( key_value, last_key, k_len ) ;

   if ( dup > k_len - *last_trail )  /* don't allow duplicating trail bytes */
      dup = k_len - *last_trail ;

   if ( dup == k_len ) /* duplicate key */
      trail = 0 ;
   else
      trail = b4calc_blanks( key_value, k_len, r4->tag->p_char ) ;

   *last_trail = trail ;

   len = k_len - dup - trail ;
   if ( r4->node_hdr.free_space < i_len + len )
   {
      if ( r4reindex_todisk(r4, last_key) < 0 )  return -1 ;
      r4->node_hdr.free_space = B4BLOCK_SIZE - sizeof( B4STD_HEADER ) - sizeof( B4NODE_HEADER ) ;
      dup = 0 ;
      r4->cur_pos = ((char *)start_block) + B4BLOCK_SIZE ;
      memcpy( ((char *)&start_block->header) + sizeof( B4STD_HEADER ), &r4->node_hdr, sizeof( B4NODE_HEADER ) ) ;
      start_block->header.node_attribute |= 2 ;   /* leaf block */
      trail = b4calc_blanks( key_value, k_len, r4->tag->p_char ) ;
      len = k_len - trail ;
   }

   r4->cur_pos -= len ;
   memcpy( r4->cur_pos, key_value + dup, len ) ;

   info_pos = ((char *)&start_block->header) + sizeof(B4STD_HEADER) +
               sizeof(B4NODE_HEADER) + start_block->header.n_keys*i_len ;
   x4put_info( &r4->node_hdr, buffer, rec, trail, dup ) ;
   memcpy( info_pos, buffer, i_len ) ;

   r4->node_hdr.free_space -= ( len + i_len ) ;
   start_block->header.n_keys++ ;
   return 0 ;
}

int r4reindex_finish( R4REINDEX *r4, char *key_value )
{
   R4BLOCK_DATA *block = r4->start_block ;
   int i_block ;

   if ( r4->n_blocks_used <= 1 ) /* just output first block */
   {
      memcpy( ((char *)block) + sizeof( B4STD_HEADER ),
	      &r4->node_hdr, sizeof( B4NODE_HEADER ) ) ;
      block->header.node_attribute |= (short)3 ;   /* leaf and root block */
      if ( h4seq_write( &r4->seqwrite, block,B4BLOCK_SIZE) < 0) return -1;
      r4->lastblock +=  B4BLOCK_SIZE ;
   }
   else
   {
      long l_recno ;

      memcpy( &l_recno, ((char *) (&block->header)) + sizeof(B4STD_HEADER)
	      + sizeof(B4NODE_HEADER) + (block->header.n_keys - 1) * r4->node_hdr.info_len, sizeof( long ) ) ;
      l_recno &= r4->node_hdr.rec_num_mask ;

      if ( block->header.node_attribute >= 2 )  /* if leaf, record free_space */
      memcpy( ((char *)block) + sizeof( B4STD_HEADER ),
	      &r4->node_hdr, sizeof( B4NODE_HEADER ) ) ;

      if ( h4seq_write(&r4->seqwrite, r4->start_block,B4BLOCK_SIZE) < 0 )
         return -1 ;
      r4->lastblock +=  B4BLOCK_SIZE ;

      l_recno = x4reverse( l_recno ) ;

      for( i_block=1; i_block < r4->n_blocks_used ; i_block++ )
      {
         block =  (R4BLOCK_DATA *) ((char *)block + B4BLOCK_SIZE) ;
         if ( block->header.n_keys >= 1 )
         {
            long rev_lb ;
            char *keyto = ((char *) (&block->header)) + sizeof(B4STD_HEADER) +
                          block->header.n_keys * r4->grouplen ;
            block->header.n_keys++ ;
            #ifdef S4DEBUG
               if ( (char *) keyto -  (char *) block + r4->grouplen > B4BLOCK_SIZE ||
                    (char *) keyto -  (char *) block < 0 )
                  e4severe( e4result, "r4reindex_finish()", (char *) 0 ) ;
            #endif
            memcpy( keyto, key_value, r4->valuelen ) ;
	    keyto += r4->valuelen ;
	    memcpy( keyto, &l_recno, sizeof( long ) ) ;
            rev_lb = x4reverse( r4->lastblock ) ;
	    memcpy( keyto + sizeof( long ), &rev_lb, sizeof( long ) ) ;
                        
            if ( i_block == r4->n_blocks_used - 1 )
               block->header.node_attribute = 1 ;  /* root block */
            if ( h4seq_write( &r4->seqwrite, block,B4BLOCK_SIZE) < 0)
               return -1;
            r4->lastblock +=  B4BLOCK_SIZE ;
         }
      }
   }

   return 0 ;
}

/* Writes out the current block and adds references to higher blocks */
int r4reindex_todisk( R4REINDEX *r4, char* key_value )
{
   R4BLOCK_DATA *block  =  r4->start_block ;
   long l_recno ;
   int tn_used = 1, i_block = 0 ;

   memcpy( &l_recno, ((unsigned char *) (&block->header)) + sizeof(B4STD_HEADER)
	   + sizeof(B4NODE_HEADER) + (block->header.n_keys - 1) * r4->node_hdr.info_len, sizeof( long ) ) ;
   l_recno &= r4->node_hdr.rec_num_mask ;
   l_recno = x4reverse( l_recno ) ;

   for(;;)
   {
      tn_used++ ;
      r4->lastblock +=  B4BLOCK_SIZE ;
      /* next line only works when on leaf branches... */
      block->header.right_node = r4->lastblock + B4BLOCK_SIZE ;
      if ( block->header.node_attribute >= 2 )  /* if leaf, record free_space */
         memcpy( ((char *)block) + sizeof( B4STD_HEADER ),
   	         &r4->node_hdr.free_space, sizeof( r4->node_hdr.free_space ) ) ;
      if ( h4seq_write( &r4->seqwrite, block, B4BLOCK_SIZE) < 0 ) return -1 ;
      memset( block, 0, B4BLOCK_SIZE ) ;
      block->header.left_node = r4->lastblock ;
      block->header.right_node = -1 ;
   
      block =  (R4BLOCK_DATA *) ((char *)block + B4BLOCK_SIZE) ;
      i_block++ ;
      #ifdef S4DEBUG
         if ( i_block >= r4->n_blocks )
            e4severe( e4info, "r4reindex_todisk()", (char *) 0 ) ;
      #endif

      if ( block->header.n_keys < r4->keysmax )
      {
	 long rev_lb ;
	 char *keyto = ((char *) (&block->header)) + sizeof(B4STD_HEADER) +
		       block->header.n_keys * r4->grouplen ;
	 block->header.n_keys++ ;
	 #ifdef S4DEBUG
	    if ( (char *) keyto -  (char *) block + r4->grouplen > B4BLOCK_SIZE || (char *) keyto -  (char *) block < 0 )
	       e4severe( e4result, "r4reindex_todisk()", (char *) 0 ) ;
	 #endif
	 memcpy( keyto, key_value, r4->valuelen ) ;
	 keyto += r4->valuelen ;
	 memcpy( keyto, &l_recno, sizeof( long ) ) ;
	 rev_lb = x4reverse( r4->lastblock ) ;
	 memcpy( keyto + sizeof( long ), &rev_lb, sizeof( long ) ) ;

         if ( block->header.n_keys < r4->keysmax )  /* then done, else do next one up */
         {
            if ( tn_used > r4->n_blocks_used )
               r4->n_blocks_used = tn_used ;
            return 0 ;
         }
      }
      #ifdef S4DEBUG
         else  /* should never occur */
            e4severe( e4result, "r4reindex_todisk()", (char *) 0 ) ;
      #endif
   }
}
#endif   /* ifdef S4FOX  */
#endif   /* ifndef N4OTHER  */


#ifdef N4OTHER

#ifdef S4CLIPPER
   int  v4clipper_len = 17 ;
   int  v4clipper_dec =  2 ;
#endif

#ifdef S4NDX
   B4KEY_DATA   *r4key( R4BLOCK_DATA *r4, int i, int keylen)
   {
      return (B4KEY_DATA *) (&r4->info+i*keylen) ;
   }
#else
   #ifdef S4CLIPPER
      B4KEY_DATA   *r4key( R4BLOCK_DATA *r4, int i, int keylen)
      {
         return (B4KEY_DATA *) ((char *)&r4->n_keys + r4->block_index[i]) ;
      }
   #endif
#endif

static int i4do_lock ( I4INDEX *i4 )
{
   T4TAG *tag_on ;
   int rc ;

   if ( !i4->file_locked )
   {
      if ( i4unlock(i4) < 0 )  return -1 ;
      for( tag_on = 0; tag_on = (T4TAG *)l4next( &i4->tags, tag_on ); )
      {
         rc =  h4lock( &tag_on->file, L4LOCK_POS, L4LOCK_POS ) ;
         if ( rc != 0 )
         {
	    for( tag_on = 0; tag_on = (T4TAG *)l4next( &i4->tags, tag_on ); )
               if ( tag_on->file_locked )
                  h4unlock( &tag_on->file, L4LOCK_POS, L4LOCK_POS ) ;
            return rc ;
         }
         tag_on->file_locked = 1 ;
      }
      i4->file_locked =  1 ;
   }
   return 0 ;
}

int S4FUNCTION i4reindex( I4INDEX *i4 )
{
   int rc ;
   T4TAG *tag_on ;

   rc = i4do_lock( i4 ) ;
   if ( rc != 0 ) return rc  ;

   for( tag_on = 0; tag_on = (T4TAG *)l4next( &i4->tags, tag_on ); )
   {
      rc = t4reindex( tag_on ) ;
      if ( rc != 0 ) return rc ;
   }
   return 0 ;
}

int S4FUNCTION t4reindex( T4TAG *t4 )
{
   R4REINDEX reindex ;
   I4INDEX *i4 ;
   int rc ;

   i4 = t4->index ;

   rc = i4do_lock( i4 ) ;
   if ( rc != 0 ) return rc  ;

   r4reindex_init( &reindex, t4 ) ;
   r4reindex_tag_headers_calc( &reindex, t4 ) ;
   r4reindex_blocks_alloc(&reindex) ;

   r4reindex_supply_keys( &reindex, t4 ) ;
   if ( (rc = r4reindex_write_keys( &reindex, t4 )) != 0 )
   {
      r4reindex_free( &reindex ) ;
      return rc ;
   }

   rc =  r4reindex_tag_headers_write( &reindex, t4 ) ;

   #ifdef S4CLIPPER
      if( rc != 0 ) return rc ;
      if ( reindex.stranded )   /* add stranded entry */
      {
	 t4add( t4, reindex.stranded->value, reindex.stranded->num ) ;
         t4flush( t4 ) ;
      }
   #endif

   r4reindex_free( &reindex ) ;
   return rc ;
}

int r4reindex_init( R4REINDEX *r4, T4TAG *t4 )
{
   I4INDEX *i4 ;

   i4 = t4->index ;

   memset( r4, 0, sizeof(R4REINDEX) ) ;

   r4->data  =  t4->index->data ;
   r4->code_base =  t4->code_base ;

   r4->min_keysmax = INT_MAX ;
   r4->start_block =  0 ;

   r4->buffer_len =  i4->code_base->mem_size_sort_buffer ;
   if ( r4->buffer_len < 1024 )  r4->buffer_len =  1024 ;

   r4->buffer =  (char *) u4alloc( r4->buffer_len ) ;
   if ( r4->buffer == 0 )
      return e4error( i4->code_base, e4memory, (char *) 0 ) ;

   return 0 ;
}

void r4reindex_free( R4REINDEX *r4 )
{
   u4free( r4->buffer ) ;
   u4free( r4->start_block ) ;
   s4free( &r4->sort ) ;
}

int r4reindex_blocks_alloc( R4REINDEX *r4 )
{
   long on_count, num_sub ;

   if ( r4->code_base->error_code < 0 )  return -1 ;

   #ifdef S4DEBUG
      if ( (unsigned) r4->min_keysmax > INT_MAX )
         e4severe( e4info, "r4reindex_blocks_alloc()", (char *) 0 ) ;
   #endif

   /* Calculate the block stack height */
   on_count =  d4reccount( r4->data ) ;

   #ifdef S4DEBUG
      if ( on_count == - 1 )
         return e4error( r4->code_base, e4info, E4_INFO_CAL, (char *) 0 ) ;
   #endif

   #ifdef S4NDX
      for ( r4->n_blocks = 2; on_count != 0L; r4->n_blocks++ )
         on_count /=  r4->min_keysmax ;
      r4->start_block =  (R4BLOCK_DATA *) u4alloc( (long) B4BLOCK_SIZE * r4->n_blocks ) ;
   #else
      #ifdef S4CLIPPER
         num_sub = r4->min_keysmax ;
         for ( r4->n_blocks = 0; on_count > 0L; r4->n_blocks++ )
         {
            on_count -=  num_sub ;
	    num_sub *= r4->min_keysmax ;
         }
         r4->n_blocks ++ ;
         if( r4->n_blocks < 2 ) r4->n_blocks = 2 ;
         r4->start_block =  (R4BLOCK_DATA *) u4alloc( (long) ( B4BLOCK_SIZE + 2 * sizeof( void *) ) * r4->n_blocks ) ;
      #endif
   #endif

   if ( r4->start_block == 0 )
      return e4error( r4->code_base, e4memory, E4_MEMORY_B, (char *) 0 ) ;

   return 0 ;
}

int r4reindex_supply_keys( R4REINDEX *r4, T4TAG *t4 )
{
   H4SEQ_READ seq_read ;
   E4EXPR *expr;
   char   *key_result ;
   int     i ;
   long    count, i_rec ;
   D4DATA *d4 ;

   d4 =  r4->data ;
   #ifdef S4DEBUG
      r4->key_count =  0L ;
   #endif

   h4seq_read_init( &seq_read, &d4->file, d4rec_pos(d4,1L), r4->buffer,
	  r4->buffer_len) ;

   if ( s4init( &r4->sort, r4->code_base, t4->header.key_len, 0 ) < 0 )  return -1 ;
   r4->sort.cmp =  t4->cmp ;

   expr  =  t4->expr ;
   count =  d4reccount( d4 ) ;

   for ( i_rec = 1L; i_rec <= count; i_rec++ )
   {
      if ( h4seq_read_all( &seq_read, d4->record, d4->record_width ) < 0 )
         return -1 ;
      d4->rec_num =  i_rec ;

      for ( i = 0; i < d4->n_fields_memo; i++ )
         m4reset( d4->fields_memo[i].field ) ;

      e4key( expr, &key_result ) ;

      if ( s4put( &r4->sort, i_rec, key_result, "" ) < 0)
         return -1 ;
      #ifdef S4DEBUG
         r4->key_count++ ;
      #endif
   }

   return 0 ;
}

int  r4reindex_tag_headers_calc( R4REINDEX *r4, T4TAG *t4 )
{
   char *result ;
   int keysmax ;

   if ( t4free_all( t4 ) < 0 )  return -1 ;

   t4->header.key_len =  e4key( t4->expr, &result ) ;
   if ( t4->header.key_len < 0 )  return -1 ;

   t4->header.type = (char) e4type( t4->expr ) ;

   #ifdef S4CLIPPER
      #ifdef S4DEBUG
         if ( t4->header.type == t4num_doub || t4->header.type == t4num_str )
            e4severe( e4info, "r4reindex_headers_clac()", E4_INFO_IVT, (char *) 0 ) ;
      #endif
      if ( t4->header.type == t4num_clip )
	  t4->header.key_dec = t4->expr->num_decimals ;
      else
          t4->header.key_dec = 0 ;
   #endif

   if ( t4->header.type < 0 )  return -1 ;

   t4init_seek_conv( t4, t4->header.type ) ;

   #ifdef S4NDX
      t4->header.group_len = t4->header.key_len + 3*sizeof(long)
          - t4->header.key_len % sizeof(long) ;
      if ( t4->header.key_len%sizeof(long) == 0 )
         t4->header.group_len -= sizeof(long) ;
      t4->header.int_or_date = ( t4->header.type == t4num_doub ||
				     t4->header.type == t4date_doub );
      t4->header.db_type = t4->header.type ;

      t4->header.keys_max =
	 ( B4BLOCK_SIZE - 2*sizeof(long)) / t4->header.group_len ;
   #else
      #ifdef S4CLIPPER
         t4->header.group_len =  t4->header.key_len+8 ;
         t4->header.keys_half =  (1020/ (t4->header.group_len+2) - 1)/ 2;
         t4->header.sign   =  6 ;
         t4->header.keys_max  =  t4->header.keys_half *2 ;
         if ( t4->header.keys_max < 2 )
         {
	     e4severe( e4info, "Reindex:index_header_calc", E4_INFO_BAD, (char *) 0 ) ;
	     return -1 ;
         }
      #endif
   #endif

   if ( t4->header.keys_max < r4->min_keysmax )
      r4->min_keysmax =  t4->header.keys_max ;

   r4->lastblock_inc = B4BLOCK_SIZE / 512 ;
   r4->lastblock = 0 ;

   return 0 ;
}

int r4reindex_tag_headers_write( R4REINDEX *r4, T4TAG *t4 )
{
   D4DATA  *d4 ;
   int n_tag, i_tag, j_field, len, save_code ;
   char *ptr ;
   I4IND_HEAD_WRITE ihwrite ;

   /* Now write the headers */
   h4seq_write_init( &r4->seqwrite, &t4->file, 0L, r4->buffer, r4->buffer_len ) ;

   #ifdef S4NDX
      t4->header.eof = r4->lastblock + r4->lastblock_inc ;
   #else
      t4->header.eof = 0 ;
   #endif


   #ifdef S4NDX
      h4seq_write( &r4->seqwrite, &t4->header.root, sizeof(I4IND_HEAD_WRITE) ) ;
   #else
      #ifdef S4CLIPPER
         h4seq_write( &r4->seqwrite, &t4->header.sign, sizeof(I4IND_HEAD_WRITE) ) ;
      #endif
   #endif

   ptr =  t4->expr->source ;
   len =  strlen(ptr) ;
   h4seq_write( &r4->seqwrite, ptr, len) ;
   h4seq_write_repeat( &r4->seqwrite, 1, ' ' ) ;
   h4seq_write_repeat( &r4->seqwrite, I4MAX_EXPR_SIZE - len, 0 ) ;

   h4seq_write( &r4->seqwrite, &t4->header.version, sizeof( t4->header.version ) ) ;

   #ifdef S4NDX
      h4length_set( &t4->file, t4->header.eof * 512) ;
   #else
      #ifdef S4CLIPPER
         h4length_set( &t4->file, (r4->lastblock + r4->lastblock_inc) * 512) ;
      #endif
   #endif

   if ( h4seq_write_flush(&r4->seqwrite) < 0 ) return -1 ;
   return 0 ;
}

int r4reindex_write_keys( R4REINDEX *r4, T4TAG *t4 )
{
   char  last_key[I4MAX_KEY_SIZE], *key_data ;
   int   is_unique, rc ;
   void *dummy_ptr ;
   long  key_rec ;

   r4->grouplen =  t4->header.group_len ;
   r4->valuelen =  t4->header.key_len ;
   r4->keysmax  =  t4->header.keys_max ;
   #ifdef S4CLIPPER
      memset( r4->start_block, 0, ( (long)B4BLOCK_SIZE + 2 * sizeof( void *) ) * r4->n_blocks ) ;
   #else
      memset( r4->start_block, 0, r4->n_blocks*B4BLOCK_SIZE ) ;
   #endif

   if ( s4get_init(&r4->sort) < 0 )  return -1 ;
   h4seq_write_init( &r4->seqwrite, &t4->file, (r4->lastblock+r4->lastblock_inc)*512, r4->buffer,r4->buffer_len) ;

   #ifdef S4DEBUG
      if ( I4MAX_KEY_SIZE < r4->sort.sort_len )  
         e4severe( e4info, "r4reindex_write_keys()", E4_INFO_EXK, (char *) 0 ) ;
   #endif

   memset( last_key, 0, sizeof(last_key) ) ;
   is_unique =  t4->header.unique ;

   for(;;)  /* For each key to write */
   {
      if ( (rc = s4get( &r4->sort, &key_rec, (void **) &key_data, &dummy_ptr)) < 0)  return -1 ;

      #ifdef S4DEBUG
         if ( r4->key_count < 0L  ||  r4->key_count == 0L && rc != 1
              ||  r4->key_count > 0L && rc == 1 )
            e4severe( e4info, "r4reindex_write_keys()", (char *) 0 ) ;
         r4->key_count-- ;
      #endif

      if ( rc == 1 )  /* No more keys */
      {
         if ( r4reindex_finish( r4 ) < 0 ) return -1 ;
         if ( h4seq_write_flush( &r4->seqwrite ) < 0 )  return -1 ;
         break ;
      }

      if ( is_unique )
      {
	 if ( (*t4->cmp)( key_data, last_key, r4->sort.sort_len) == 0 )
         {
            switch( t4->unique_error )
            {
               case e4unique:
                  return e4error( r4->code_base, e4unique, E4_UNIQUE, t4->alias, (char *) 0 ) ;

               case r4unique:
                  return r4unique ;

               default:
                  continue ;
            }
         }
         memcpy( last_key, key_data, r4->sort.sort_len ) ;
      }

      /* Add the key */
      if ( r4reindex_add( r4, key_rec, key_data) < 0 )  return -1 ;
   }

   /* Now complete the tag header info. */
   #ifdef S4NDX
      t4->header.root =  r4->lastblock ;
   #else
      #ifdef S4CLIPPER
         t4->header.root =  r4->lastblock * 512 ;
      #endif
   #endif

   return 0 ;
}

int r4reindex_add( R4REINDEX *r4, long rec, char *key_value )
{
   B4KEY_DATA *key_to ;
   R4BLOCK_DATA *start_block ;
   int i ;
   #ifdef S4DEBUG
      long  dif ;
   #endif
   #ifdef S4CLIPPER
      short offset ;
   #endif

   start_block =  r4->start_block ;

   /* for NTX, if keysmax, then todisk() with the latest value... */

   #ifdef S4NDX
      if ( start_block->n_keys >= r4->keysmax )
      {
         if ( r4reindex_todisk(r4) < 0 )  return -1 ;
         memset( start_block, 0, B4BLOCK_SIZE ) ;
      }
   #else
      #ifdef S4CLIPPER
         if ( start_block->n_keys == 0 )   /* first, so add references */
         {
            offset = ( r4->keysmax + 2 + ( ( r4->keysmax / 2 ) * 2 != r4->keysmax ) ) * sizeof(short) ;
            start_block->block_index = &start_block->n_keys + 1 ;  /* 1 short off of n_keys */
	    for ( i = 0 ; i <= r4->keysmax ; i++ )
	        start_block->block_index[i] = r4->grouplen * i + offset ;
	     start_block->data = (char *) &start_block->n_keys + start_block->block_index[0] ;  /* first entry */
         }
         if ( start_block->n_keys >= r4->keysmax )
         {
	    if ( r4reindex_todisk( r4, rec, key_value ) < 0 )  return -1 ;
	    memset( start_block, 0, B4BLOCK_SIZE + 2 * sizeof( void *) ) ;
            return 0 ;
         }
      #endif
   #endif

   key_to =  r4key( start_block, start_block->n_keys++, r4->grouplen ) ;

   #ifdef S4DEBUG
      dif =  (char *) key_to -  (char *) start_block ;
      if ( dif+ r4->grouplen > B4BLOCK_SIZE || dif < 0 )
	 e4severe( e4result, "r4reindex_add()", (char *) 0 ) ;
   #endif
   key_to->num =  rec ;
   memcpy( key_to->value, key_value, r4->valuelen ) ;

   return 0 ;
}

int r4reindex_finish( R4REINDEX *r4 )
{
   int i_block = 1, i ;
   B4KEY_DATA *key_to ;
   #ifdef S4DEBUG
      long dif ;
   #endif

   R4BLOCK_DATA *block = r4->start_block ;

   #ifdef S4CLIPPER
      short offset ;
      long pointer ;

      if ( r4->n_blocks <= 2 )  /* empty database if n_keys = 0 */
      {
         if ( r4->start_block->n_keys == 0 )   /* first, so add references */
         {
            offset = ( r4->keysmax + 2 + ( (r4->keysmax/2)*2 != r4->keysmax ) ) * sizeof(short) ;
            r4->start_block->block_index = &r4->start_block->n_keys + 1 ;  /* 1 short off of n_keys */
	    for ( i = 0 ; i <= r4->keysmax ; i++ )
	        r4->start_block->block_index[i] = r4->grouplen * i + offset ;
	    r4->start_block->data = (char *) &r4->start_block->n_keys + r4->start_block->block_index[0] ;
         }
         i_block ++ ;
	 r4->stranded = 0 ;
	 pointer = 0 ;
         if ( h4seq_write( &r4->seqwrite, &r4->start_block->n_keys, B4BLOCK_SIZE) < 0 ) return -1 ;
         r4->lastblock += r4->lastblock_inc ;
      }
      else if ( r4->start_block->n_keys != 0 )
      {
	 /* just grab the pointer for upward placement where belongs */
         r4->stranded = 0 ;
         if ( h4seq_write( &r4->seqwrite, &r4->start_block->n_keys, B4BLOCK_SIZE) < 0 ) return -1 ;
         r4->lastblock += r4->lastblock_inc ;
         pointer = r4->lastblock*512 ;
         block =  (R4BLOCK_DATA *) ((char *)block + B4BLOCK_SIZE + 2*sizeof(void *) ) ;
         i_block++ ;
      }
      else       /* stranded entry, so add after */
      {
	 while (block->n_keys == 0 && i_block < r4->n_blocks )
	 {
	    block =  (R4BLOCK_DATA *) ((char *)block + B4BLOCK_SIZE + 2*sizeof(void *) ) ;
	    i_block++ ;
	 }

	 r4->stranded = r4key( block, block->n_keys - 1, 0 ) ;
	 block->n_keys -- ;
	 if( block->n_keys > 0 )
	 {
            if ( h4seq_write( &r4->seqwrite, &block->n_keys, B4BLOCK_SIZE) < 0 ) return -1 ;
            r4->lastblock += r4->lastblock_inc ;
	    pointer = 0 ;
	 }
	 else
	    pointer = r4key( block, block->n_keys, 0 )->pointer ;
         block =  (R4BLOCK_DATA *) ((char *)block + B4BLOCK_SIZE + 2*sizeof(void *) ) ;
	 i_block++ ;
      }

      /* now position to the last spot, and place the branch */
      if( i_block < r4->n_blocks )
      {
	 while (block->n_keys == 0 && i_block < r4->n_blocks )
         {
	    block =  (R4BLOCK_DATA *) ((char *)block + B4BLOCK_SIZE + 2*sizeof(void *) ) ;
	    i_block++ ;
         }

         /* now place the pointer for data that goes rightward */
         if( block->n_keys <= r4->keysmax && pointer != 0)
         {
	    key_to = r4key( block, block->n_keys, 0 ) ;
	    key_to->pointer = pointer ;
            pointer = 0 ;
            if ( h4seq_write( &r4->seqwrite, &block->n_keys, B4BLOCK_SIZE) < 0 ) return -1 ;
            r4->lastblock += r4->lastblock_inc ;
            block =  (R4BLOCK_DATA *) ((char *)block + B4BLOCK_SIZE + 2*sizeof(void *) ) ;
            i_block++ ;
         }
      }

   #else
      #ifdef S4NDX
         if ( h4seq_write(&r4->seqwrite, &r4->start_block->n_keys, B4BLOCK_SIZE) < 0 ) return -1 ;
         r4->lastblock +=  r4->lastblock_inc ;
      #endif
   #endif

   for(; i_block < r4->n_blocks; i_block++ )
   {
      if ( block->n_keys >= 1 )
      {
	 key_to =  r4key( block, block->n_keys, r4->grouplen ) ;
         #ifdef S4DEBUG
	    dif =  (char *) key_to  -  (char *) block ;
            if ( dif+sizeof(long) > B4BLOCK_SIZE  ||  dif<0 )
               e4severe( e4result, "r4reindex_finish()", (char *) 0 ) ;
         #endif
         #ifdef S4NDX
	    key_to->pointer =  r4->lastblock ;
         #else
            #ifdef S4CLIPPER
               key_to->pointer =  r4->lastblock * 512 ;
            #endif
         #endif
                        
         if ( h4seq_write( &r4->seqwrite, &block->n_keys, B4BLOCK_SIZE) < 0) return -1;
         r4->lastblock +=  r4->lastblock_inc ;
      }
      #ifdef S4NDX
         block = (R4BLOCK_DATA *) ((char *)block + B4BLOCK_SIZE) ;
      #else
         #ifdef S4CLIPPER
            block = (R4BLOCK_DATA *) ((char *)block + B4BLOCK_SIZE + 2*sizeof(void *) ) ;
         #endif
      #endif
   }

   return 0 ;
}

#ifdef S4NDX
int r4reindex_todisk( R4REINDEX *r4 )
{
   R4BLOCK_DATA *block ;
   int i_block ;
   B4KEY_DATA *key_on, *keyto ;
   #ifdef S4DEBUG
      long dif ;
   #endif

   /* Writes out the current block and adds references to higher blocks */
   block  =  r4->start_block ;
   i_block= 0 ;

   key_on =  r4key( block, block->n_keys-1, r4->grouplen ) ;

   #ifdef S4DEBUG
      dif =  (char *) key_on -  (char *) block ;
      if ( dif+ r4->grouplen > B4BLOCK_SIZE || dif < 0 )
	 e4severe( e4result, "r4reindex_todisk()", (char *) 0 ) ;
   #endif

   for(;;)
   {
      if ( h4seq_write( &r4->seqwrite, &block->n_keys, B4BLOCK_SIZE) < 0 ) return -1 ;
      if ( i_block != 0 )  memset( block, 0, B4BLOCK_SIZE ) ;
      r4->lastblock +=  r4->lastblock_inc ;

      block =  (R4BLOCK_DATA *) ((char *)block + B4BLOCK_SIZE) ;
      i_block++ ;
      #ifdef S4DEBUG
         if ( i_block >= r4->n_blocks )  e4severe( e4info, "r4reindex_todisk()", (char *) 0 ) ;
      #endif

      keyto =  r4key( block, block->n_keys, r4->grouplen) ;
      #ifdef S4DEBUG
         dif =  (char *) keyto -  (char *) block  ;
         if ( dif+sizeof(long) > B4BLOCK_SIZE || dif < 0 )
            e4severe( e4result, "r4reindex_todisk()", (char *) 0 ) ;
      #endif
      keyto->pointer = r4->lastblock ;

      if ( block->n_keys < r4->keysmax )
      {
         block->n_keys++ ;
         #ifdef S4DEBUG
            if ( dif+r4->grouplen > B4BLOCK_SIZE )
               e4severe( e4result, "r4reindex_todisk()", (char *) 0 ) ;
         #endif
         memcpy( keyto->value, key_on->value, r4->valuelen ) ;
         return 0 ;
      }
   }
}
#else
#ifdef S4CLIPPER
int r4reindex_todisk( R4REINDEX *r4, long rec, char *key_value )
{
   R4BLOCK_DATA *block ;
   int i_block, i ;
   B4KEY_DATA *key_on, *key_to ;
   short offset ;
   #ifdef S4DEBUG
      long dif ;
   #endif

   /* Writes out the current block and adds references to higher blocks */
   block  =  r4->start_block ;
   i_block= 0 ;

   #ifdef S4DEBUG
      key_on =  r4key( block, block->n_keys, r4->grouplen ) ;
      dif =  (char *) key_on -  (char *) &block->n_keys ;
      if ( dif+ r4->grouplen > B4BLOCK_SIZE || dif < 0 )
         e4severe( e4result, "r4reindex_add()", (char *) 0 ) ;
   #endif

   for(;;)
   {
      if ( h4seq_write( &r4->seqwrite, &block->n_keys, B4BLOCK_SIZE) < 0 ) return -1 ;
      if ( i_block != 0 )  memset( block, 0, B4BLOCK_SIZE ) ;
      r4->lastblock +=  r4->lastblock_inc ;

      block =  (R4BLOCK_DATA *) ((char *)block + B4BLOCK_SIZE + 2*sizeof(void *) ) ;
      i_block++ ;
      #ifdef S4DEBUG
         if ( i_block >= r4->n_blocks )  e4severe( e4info, "r4reindex_todisk()", (char *) 0 ) ;
      #endif

      if ( block->n_keys == 0 )   /* set up the branch block... */
      {
         offset = ( r4->keysmax + 2 + ( ( r4->keysmax / 2 ) * 2 != r4->keysmax ) ) * sizeof(short) ;
         block->block_index = &block->n_keys + 1 ;
         for ( i = 0 ; i <= r4->keysmax ; i++ )
            block->block_index[i] = r4->grouplen * i + offset ;
         block->data = (char *) &block->n_keys + block->block_index[ 0 ] ;
      }

      key_to =  r4key( block, block->n_keys, r4->grouplen ) ;
      #ifdef S4DEBUG
	 dif =  (char *) key_to -  (char *) block  ;
         if ( dif+sizeof(long) > B4BLOCK_SIZE || dif < 0 )
            e4severe( e4result, "r4reindex_todisk()", (char *) 0 ) ;
      #endif
      key_to->pointer = r4->lastblock * 512 ;

      if ( block->n_keys < r4->keysmax )
      {
         #ifdef S4DEBUG
            if ( dif+r4->grouplen > B4BLOCK_SIZE )
               e4severe( e4result, "r4reindex_todisk()", (char *) 0 ) ;
         #endif
         key_to->num = rec ;
         memcpy( key_to->value, key_value, r4->valuelen ) ;
         block->n_keys++ ;
         return 0 ;
      }
      #ifdef S4DEBUG
         if ( block->n_keys > r4->keysmax )
             e4severe( e4info, "r4reindex_todisk()", E4_INFO_NKE, (char *) 0 ) ;
      #endif
   }
}
#endif   /* ifdef S4CLIPPER   */
#endif   /* ifdef S4NDX       */
#endif   /* ifdef N4OTHER     */

