/* i4check.c   (C)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

typedef struct
{
   F4FLAG    flag ;

   T4TAG    *tag ;
   char     *old_key ;
   long      old_rec ;
   long      num_recs ;
   int       do_compare ;  /* Do not compare the first time */
   C4CODE   *code_base ;
} C4CHECK ;

static int c4check_init( C4CHECK *check, C4CODE *cb, T4TAG *t4, long n_recs )
{
   memset( check, 0, sizeof(C4CHECK) ) ;

   if ( f4flag_init( &check->flag, cb, n_recs ) < 0 )
      return -1 ;

   check->code_base =  cb ;
   check->tag       =  t4 ;
   check->num_recs  =  n_recs ;

   #ifdef S4MDX
      check->old_key =  (char *) u4alloc( t4->header.value_len ) ;
   #else
      check->old_key =  (char *) u4alloc( t4->header.key_len ) ;
   #endif
   return 0 ;
}

static void c4check_free( C4CHECK *c4 )
{
   u4free( c4->flag.flags ) ;
   u4free( c4->old_key ) ;
}

static int  c4check_record( C4CHECK *check )
{
   B4KEY_DATA *key_data ;
   T4TAG *t4 ;
   char *new_ptr ;
   int len, rc ;

   t4 =  check->tag ;

   key_data =  t4key( check->tag ) ;
   if ( key_data == 0 )  
      return e4error( check->code_base, e4result, (char *) 0 ) ;

   if ( key_data->num < 1  ||  key_data->num > check->num_recs )
      return e4error( check->code_base, e4info, E4_INFO_INC, check->tag->alias, (char *) 0 ) ;

   if ( f4flag_is_set( &check->flag, key_data->num) )
      return e4error( check->code_base, e4info, E4_INFO_REP, check->tag->alias, (char *) 0 ) ;
   else
      f4flag_set( &check->flag, key_data->num ) ;

   if ( d4go( t4->index->data, key_data->num ) < 0)  return( -1) ;
   len =  e4key( t4->expr, &new_ptr ) ;

   #ifdef S4MDX
      if ( len != t4->header.value_len )
   #else
      if ( len != t4->header.key_len )
   #endif
         return e4error( check->code_base, e4result, E4_RESULT_UNE, t4->alias, (char *) 0 ) ;

   #ifdef S4MDX
      if ( memcmp( new_ptr, key_data->value, t4->header.value_len ) != 0 )
   #else
      if ( memcmp( new_ptr, key_data->value, t4->header.key_len ) != 0 )
   #endif
         return e4error( check->code_base, e4result, E4_RESULT_TAG, t4->alias, (char *) 0 ) ;

   if ( check->do_compare )
   {
      #ifdef S4FOX
         rc = memcmp( check->old_key, new_ptr, check->tag->header.key_len ) ;
      #else
         #ifdef N4OTHER
	    rc = (*t4->cmp)( check->old_key, new_ptr, check->tag->header.key_len ) ;
         #else
            rc = (*t4->cmp)( check->old_key, new_ptr, check->tag->header.value_len ) ;
         #endif
      #endif

      if ( rc > 0)
	 e4error( check->code_base, e4result, E4_RESULT_THE, t4->alias, (char *) 0 ) ;
      if ( rc == 0  &&  key_data->num <= check->old_rec )
	 e4error( check->code_base, e4result, E4_RESULT_REC, t4->alias, (char *) 0 ) ;

      #ifdef S4FOX
         if ( t4->header.type_code & 0x01 )
      #else
         if ( t4->header.unique )
      #endif
            if ( rc == 0 )
	       e4error( check->code_base, e4result, E4_RESULT_IDE, t4->alias, (char *) 0 ) ;
   }
   else
      check->do_compare =  1 ;

   #ifdef S4MDX
      memcpy( check->old_key, new_ptr, t4->header.value_len ) ;
   #else
      memcpy( check->old_key, new_ptr, t4->header.key_len ) ;
   #endif

   check->old_rec =  key_data->num ;

   if ( check->code_base->error_code < 0 ) return -1 ;
   return 0 ;
}

int S4FUNCTION t4check( T4TAG *t4 )
{
   C4CHECK  check ;
   int      rc, is_record, keys_skip ;
   I4INDEX *i4 ;
   D4DATA  *d4 ;
   C4CODE  *c4 ;
   T4TAG   *old_selected_tag ;
   B4BLOCK *block_on ;
   B4KEY_DATA *key_branch, *key_leaf ;
   long     base_size, on_rec ;
   char    *ptr ;
   #ifdef S4FOX
      char *temp_val ;
      long temp_lng ;
   #endif

   i4 =  t4->index ;
   d4 =  i4->data ;
   c4 =  t4->code_base ;

   rc =  d4lock_file( d4 ) ;   if ( rc != 0 )  return rc ;
   if ( (rc = t4lock(t4)) != 0 )  return rc ;

   if ( (rc = d4flush_record(d4)) < 0 )  return -1 ;
   if ( rc != 0 )
      return e4error( c4, e4result, "t4check()", E4_RESULT_D4F, (char *) 0 ) ;

   old_selected_tag =  d4tag_selected( d4 ) ;
   d4tag_select( d4, t4 ) ;

   base_size =  d4reccount( d4 ) ;
   if ( base_size < 0L )  return -1 ;

   rc = d4top(d4) ;
   if (rc < 0 ) return -1 ;
   if (rc == 0) rc = 1 ;

   if ( base_size == 0L )
   {
      if ( t4skip( t4, 1L ) == 0 )
      {
         d4tag_select( d4, old_selected_tag ) ;
	 return( 0 ) ;
      }
      else
	 return e4error( c4, e4info, "t4check()", E4_INFO_DAT, (char *) 0 ) ;
   }

   if ( c4check_init( &check, c4, t4, base_size ) < 0 )
      return -1 ;

   while ( rc == 1 )
   {
      if ( (rc = c4check_record(&check)) != 0 )  break ;
      rc =  (int) t4skip(t4, 1L) ;
      if ( rc < 0 )  break ;
   }

   if ( rc < 0 )
   {
      c4check_free( &check ) ;
      return -1 ;
   }

   is_record =  1 ;

   /* Now Test for Duplication */
   for ( on_rec = 1;  on_rec <= base_size; on_rec++)
   {
      #ifndef N4OTHER
         if ( t4->filter != 0 )
         {
            if ( d4go(d4,on_rec) < 0 )  break ;
	    is_record =  e4true( t4->filter ) ;
         }
      #endif

      if ( f4flag_is_set(&check.flag,on_rec) )
      {
	 if ( ! is_record )
         {
            e4error( c4, e4info,
		  "t4check()", "Key should have been filtered:", t4->alias, (char *) 0 ) ;
            break ;
         }
      }
      else
      {
	 if ( ! is_record )  continue ;

         #ifdef S4FOX
            if ( t4->header.type_code & 0x01 )
         #else
            if ( t4->header.unique )
         #endif
            {
	       if ( d4go(d4,on_rec) < 0 )  break ;
	       if ( e4key( t4->expr, &ptr) < 0 )  break ;
	       if ( d4seek( d4, ptr ) == 0 )  continue ;
            }

         e4error( c4, e4info, "t4check()", E4_INFO_REC, "T4TAG:", t4->alias, (char *) 0 ) ;
         break ;
      }
   }

   c4check_free( &check ) ;
   if ( t4->code_base->error_code < 0 )
      return -1 ;

   /* Now make sure the block key pointers match the blocks they point to. */
   /* This needs to be true for d4seek to function perfectly. */

   if ( (rc = d4bottom(d4)) < 0 )  return -1 ;

   if ( rc == 3 )
   {
      d4tag_select( d4, old_selected_tag ) ;
      return 0 ;
   }

   for(;;)
   {
      #ifdef S4FOX
         keys_skip = -t4block(t4)->header.n_keys ;
      #else
         keys_skip = -t4block(t4)->n_keys ;
      #endif

      rc =  (int) t4skip( t4, (long) keys_skip ) ;
      if ( c4->error_code < 0 )  return -1 ;
      if ( rc !=  keys_skip )
      {
         d4tag_select( d4, old_selected_tag ) ;
         return 0 ;
      }

      block_on =  (B4BLOCK *) t4->blocks.last ;
      if ( block_on == 0 )  return e4error( c4, e4info, E4_INFO_TAG, t4->alias, (char *) 0 ) ;

      #ifdef S4FOX
         temp_val = (char *)u4alloc( t4->header.key_len ) ;
         memcpy( temp_val, b4key_key( block_on, block_on->key_on ), t4->header.key_len ) ;
         temp_lng = b4recno( block_on ) ;

         if ( t4go( t4, temp_val, temp_lng ) != 0 )
         {
            u4free( temp_val ) ;
            return e4error( c4, e4info, E4_INFO_TAG, t4->alias, (char *) 0 ) ;
         }
         u4free( temp_val ) ;
      #endif

      #ifndef S4CLIPPER
         while( block_on = (B4BLOCK *) block_on->link.p )
         {
            if ( block_on == (B4BLOCK *) t4->blocks.last )  break ;

            #ifdef S4FOX
	       if ( block_on->key_on < block_on->header.n_keys )
            #else
               if ( block_on->key_on < block_on->n_keys )
            #endif
            {
               key_branch =  b4key( block_on, block_on->key_on ) ;
               key_leaf   =  b4key( t4block(t4), t4block(t4)->key_on ) ;
   
	       #ifdef S4MDX
		  if ( memcmp( key_branch->value, key_leaf->value, t4->header.value_len) != 0)
	       #else
		  if ( memcmp( key_branch->value, key_leaf->value, t4->header.key_len) != 0)
	       #endif
                     return e4error( c4, e4info, E4_INFO_TAG, t4->alias, (char *) 0 ) ;

               break ;
            }
         }
         if ( block_on == 0 )  return e4error( c4, e4info, E4_INFO_TAG, t4->alias, (char *) 0 ) ;
      #endif
   }
}

#ifndef S4NDX
static int  flag_blocks( T4TAG *t4, F4FLAG *f4 )
{
   int i, rc ;
   B4BLOCK *block_on ;
   long flag_no ;

   if ( (rc = t4down(t4)) < 0 )  return -1 ;
   if ( rc == 1 )
      e4severe( e4result, (char *) 0 ) ;

   block_on =  t4block(t4) ;

   #ifdef S4FOX
      flag_no = block_on->file_block / B4BLOCK_SIZE ;
   #else
      #ifdef S4CLIPPER
         flag_no = (block_on->file_block) * I4MULTIPLY / B4BLOCK_SIZE ;
      #else
         #ifdef S4NDX
            flag_no = (block_on->file_block-4) * I4MULTIPLY / B4BLOCK_SIZE ;
         #else
            flag_no = (block_on->file_block-4) * I4MULTIPLY / t4->index->header.block_rw ;
         #endif
      #endif
   #endif

   if ( f4flag_is_set( f4, flag_no ) )
      e4severe( e4result, E4_RESULT_COR, (char *) 0 ) ;

   if ( f4flag_set( f4, flag_no ) < 0 )  return -1 ;
   if ( ! b4leaf(block_on) )

      #ifdef S4MDX
         for( i = 0; i <= block_on->n_keys; i++ )
         {
            block_on->key_on =  i ;
            if ( flag_blocks( t4, f4 ) < 0 )  return -1 ;
         }
      #else
         #ifdef S4CLIPPER
	    for( i = 0; i <= block_on->n_keys; i++ )
	    {
	       block_on->key_on =  i ;
	       if ( flag_blocks( t4, f4 ) < 0 )  return -1 ;
	    }
         #else
	    for( i = 0; i < block_on->header.n_keys; i++ )
	    {
	       b4go( block_on, i ) ;
	       if ( flag_blocks( t4, f4 ) < 0 )  return -1 ;
	    }
         #endif
      #endif

   t4up(t4) ;
   return 0 ;
}
#endif   /*  ifndef S4NDX  */

/* checks that all blocks in the file are on free list or are being used */
static int  i4check_blocks( I4INDEX *i4 )
{
   T4TAG  *tag_on ;
   F4FLAG  flags ;
   long    tot_blocks, free_block, eof_block_no, len ;
   C4CODE *c4 ;
   int     i, flag_no ;
   #ifndef S4FOX
      T4DESC  desc[48] ;
   #endif

   #ifndef N4OTHER
      c4 =  i4->code_base ;
   
      if ( i4lock(i4) < 0 )  return -1 ;
   
      len =  h4length(&i4->file) ;

      #ifdef S4MDX
         tot_blocks =  (len-2048) / i4->header.block_rw ;
      #else
         tot_blocks =  len / B4BLOCK_SIZE ;
      #endif
   
      /* First Flag for the Free Chain */
      f4flag_init( &flags, i4->code_base, tot_blocks ) ;

      eof_block_no =  len/I4MULTIPLY ;
      #ifdef S4FOX
	 for ( free_block = i4->tag_index->header.free_list ; free_block != 0L ; )
      #else
	 for ( free_block = i4->header.free_list ; free_block != 0L ; )
      #endif
         {
	    if ( free_block == eof_block_no  ||  c4->error_code < 0 )  break ;
   
	    #ifdef S4MDX
	       flag_no =  (free_block-4)*I4MULTIPLY/i4->header.block_rw ;
	    #else
	       flag_no =  free_block / B4BLOCK_SIZE ;
            #endif
   
	    if ( free_block >= eof_block_no  || f4flag_is_set(&flags, flag_no) )
	    {
	       e4error( c4, e4index, E4_INDEX_COR, (char *) 0 ) ;
	       break ;
	    }
	    f4flag_set( &flags, flag_no ) ;
   
	    #ifdef S4MDX
	       h4read_all( &i4->file, free_block * I4MULTIPLY + sizeof(long),
			         &free_block, sizeof(free_block)) ;
	    #else
	       h4read_all( &i4->file, free_block * I4MULTIPLY,
			            &free_block, sizeof(free_block)) ;
            #endif
         }
   
      #ifdef S4FOX
         /* do the header tag */
         tag_on = i4->tag_index ;
         flag_no = (int) ((tag_on->header_offset) / (long) B4BLOCK_SIZE) ;
         if ( f4flag_is_set( &flags, flag_no ) )
	    e4severe( e4result, E4_RESULT_COR, (char *) 0 ) ;
         f4flag_set( &flags, flag_no ) ;
         f4flag_set( &flags, flag_no+1 ) ;  /* tag header is 2 blocks long */
      
         if ( t4free_all(tag_on) >= 0 )
         {
            flag_blocks( tag_on, &flags ) ;
      
            /* Now Flag for each block in each tag */
            i = 1 ;
            for ( tag_on =  0; tag_on = (T4TAG *) l4next(&i4->tags,tag_on); i++ )
            {
               flag_no = (int) ((tag_on->header_offset) / (long) B4BLOCK_SIZE) ;
               if ( f4flag_is_set( &flags, flag_no ) )
                  e4severe( e4result, E4_RESULT_COR, (char *) 0 ) ;
               f4flag_set( &flags, flag_no ) ;
               f4flag_set( &flags, flag_no+1 ) ;  /* tag header is 2 blocks long */
      
	       if ( t4free_all(tag_on) < 0 ) break ;
	       flag_blocks( tag_on, &flags ) ;
	    }
         }
      #else
         /* Read header information to flag the tag header blocks */
         h4read_all( &i4->file, 512, desc, sizeof(desc) ) ;
   
         /* Now Flag for each block in each tag */
         i = 1 ;
         for ( tag_on =  0; tag_on = (T4TAG *) l4next(&i4->tags,tag_on); i++ )
         {
	    flag_no = (int) ((desc[i].header_pos * I4MULTIPLY - 2048) / (long) i4->header.block_rw) ;
	    if ( f4flag_is_set( &flags, flag_no ) )
	       e4severe( e4result, E4_RESULT_COR, (char *) 0 ) ;
	    f4flag_set( &flags, flag_no ) ;
   
	    if ( t4free_all(tag_on) < 0 ) break ;
	    flag_blocks( tag_on, &flags ) ;
         }
      #endif
   
      if ( f4flag_is_all_set( &flags, 0, tot_blocks - 1 ) == 0 )
         e4error( i4->code_base, e4result, E4_RESULT_LOS, (char *) 0 ) ;
   
      u4free( flags.flags ) ;
      if ( i4->code_base->error_code < 0 )  return -1 ;
   
   #endif
   return 0 ;
}

int S4FUNCTION i4check( I4INDEX *i4 )
{
   T4TAG *tag_on ;
   #ifdef S4FOX
      int old_desc, rc ;
   #endif

   if ( i4flush(i4) < 0 )  return -1 ;
   if ( i4check_blocks(i4) < 0 )
      return -1 ;

   for( tag_on = 0; tag_on = (T4TAG *) l4next(&i4->tags, tag_on); )
   #ifdef S4FOX
   {
      old_desc = tag_on->header.descending ;
      tag_on->header.descending = 0 ;   /* force ascending */
      rc = t4check( tag_on ) ;
      tag_on->header.descending = old_desc ;   /* return to previous */
      if ( rc < 0 ) return rc ;
   }
   #else
      if ( t4check(tag_on) < 0 )
         return -1 ;
   #endif

   return 0 ;
}

int S4FUNCTION d4check( D4DATA *d4 )
{
   I4INDEX *index_on ;
   int rc ;

   if ( (rc = d4lock_file(d4)) != 0 )  return rc ;

   for( index_on = 0; index_on =  (I4INDEX *) l4next(&d4->indexes,index_on); )
      if ( i4check(index_on) < 0 )
         return -1 ;

   return 0 ;
}
