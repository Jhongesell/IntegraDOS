/* b4block.c  (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
               
#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifdef S4MDX

B4BLOCK * S4FUNCTION b4alloc( T4TAG *t4, long fb )
{
   B4BLOCK *b4 ;

   if ( (b4 = (B4BLOCK *) y4alloc( t4->index->block_memory )) == 0 )
   {
      e4error( t4->code_base, e4memory, (char *) 0 ) ;
      return 0 ;
   }

   b4->tag =  t4 ;
   b4->file_block =  fb ;

   return b4 ;
}
    
void S4FUNCTION b4free( B4BLOCK *b4 )
{
   y4free( b4->tag->index->block_memory, b4 ) ;
}

int S4FUNCTION b4flush( B4BLOCK *b4 )
{
   int rc ;
   I4INDEX *i4 ;

   if ( b4->changed )
   {
      i4 =  b4->tag->index ;
      rc =  h4write( &i4->file, (long)b4->file_block*I4MULTIPLY, &b4->n_keys, i4->header.block_rw) ;
      if ( rc < 0 )  return rc ;
      b4->changed =  0 ;
   }
   return 0 ;
}

void S4FUNCTION b4go_eof( B4BLOCK *b4 )
{
   b4->key_on = b4->n_keys ;
}

void  S4FUNCTION b4insert( B4BLOCK *b4, void *k, long r )
{
   int left_len ;
   B4KEY_DATA *data_ptr, *next_ptr ;

   #ifdef S4DEBUG
      if ( k == 0 || r <= 0L )
    e4severe( e4parm, "b4insert()", (char *) 0 ) ;
   #endif

   data_ptr =  b4key( b4, b4->key_on ) ;
   next_ptr =  b4key( b4, b4->key_on+1 ) ;

   left_len =  b4->tag->index->header.block_rw -  (b4->key_on+1) * b4->tag->header.group_len -
         sizeof(short) - sizeof(char[6]) ;

   #ifdef S4DEBUG
      if ( b4->key_on < 0  ||  b4->key_on > b4->n_keys  ||  left_len < 0 )
    e4severe( e4info, "b4insert()", (char *) 0 ) ;
   #endif

   memmove( next_ptr, data_ptr, left_len ) ;
   b4->n_keys++ ;
   memcpy( data_ptr->value, k, b4->tag->header.value_len ) ;
   memcpy( (void *)&data_ptr->num, (void *)&r, sizeof(r) ) ;
   b4->changed =  1 ;
}

B4KEY_DATA *S4FUNCTION b4key( B4BLOCK *b4, int i_key )
{
   return  (B4KEY_DATA *) ((char *) &b4->info + b4->tag->header.group_len*i_key) ;
}

char *S4FUNCTION b4key_key( B4BLOCK *b4, int i_key )
{
   return  ((B4KEY_DATA *) (((char *) &b4->info) + b4->tag->header.group_len*i_key))->value ;
}

int S4FUNCTION b4lastpos( B4BLOCK *b4 )
{
   if ( b4leaf(b4) )
      return b4->n_keys - 1 ;
   else
      return b4->n_keys ;
}

int S4FUNCTION b4leaf( B4BLOCK *b4 )
{
   B4KEY_DATA *key_ptr =  b4key( b4, b4->n_keys ) ;
   return( key_ptr->num == 0L ) ;
}

long  S4FUNCTION b4recno( B4BLOCK *b4 )
{
   return b4key(b4,b4->key_on)->num ;
}

void  S4FUNCTION b4remove( B4BLOCK *b4 )
{
   B4KEY_DATA  *key_cur, *key_next ;
   int  left_len ;

   key_cur  =  b4key(b4,b4->key_on) ;
   key_next =  b4key(b4,b4->key_on+1) ;

   left_len =  b4->tag->index->header.block_rw - sizeof(b4->n_keys) - sizeof(b4->dummy)
           - (b4->key_on+1) * b4->tag->header.group_len ;

   #ifdef S4DEBUG
      if ( b4->key_on < 0  ||  b4->key_on > b4lastpos(b4) )
    e4severe( e4info, "b4remove", (char *) 0 ) ;
   #endif

   if ( left_len > 0 )
      memmove( key_cur, key_next, left_len ) ;

   b4->n_keys-- ;
   b4->changed =  1 ;
}

int  S4FUNCTION b4seek( B4BLOCK *b4, char *search_value, int len )
{
   int rc, key_lower, key_upper, save_rc, key_cur ;
   S4CMP_FUNCTION *cmp;

   /* key_cur must be between  key_lower and  key_upper */
   key_lower =  -1 ;
   key_upper =  b4->n_keys ;

   cmp =  b4->tag->cmp ;

   if ( key_upper == 0 )
   {
      b4->key_on = 0 ;
      return r4after ;
   }

   save_rc =  1 ;

   for(;;)  /* Repeat until the key is found */
   {
      key_cur  =  (key_upper + key_lower) / 2  ;
      rc =  (*cmp)( b4key_key(b4,key_cur), search_value, len ) ;

      if ( rc >= 0 )
      {
         key_upper =  key_cur ;
         save_rc =  rc ;
      }
      else
         key_lower =  key_cur ;

      if ( key_lower >= (key_upper-1) )  /* then there is no exact match */
      {
         b4->key_on =   key_upper ;
         if ( save_rc )  return r4after ;
         return 0 ;
      }
   }
}

int S4FUNCTION b4skip( B4BLOCK *b4, long n )
{
   int num_left ;

   if ( n > 0 )
   {
      num_left = b4->n_keys - b4->key_on ;
      if ( b4leaf(b4) )
         if ( num_left != 0 )
            num_left -- ;
   }
   else
      num_left =  -b4->key_on ;

   if ( ( n <= 0L ) ?  ((long) num_left <= n)  :  ((long) num_left >= n)  )
   {
      b4->key_on =  b4->key_on+ (int) n ;
      return (int) n ;
   }
   else
   {
      b4->key_on =  b4->key_on+num_left ;
      return num_left ;
   }
}
#endif

#ifdef S4FOX

/* calculates and assigns general leaf-block header values */
void S4FUNCTION b4leaf_init( B4BLOCK *b4 )
{
   T4TAG *t4 = b4->tag ;
   long r_len ;
   int c_len, t_len ;
   int   k_len = t4->header.key_len ;
   unsigned long ff = 0xFFFFFFFF ;

   for ( c_len = 0 ; k_len ; k_len>>=1, c_len++ ) ;
   k_len = t4->header.key_len ;  /* reset the key length */
   b4->node_hdr.trail_cnt_len = b4->node_hdr.dup_cnt_len = c_len ;

   b4->node_hdr.trail_byte_cnt = (unsigned char)0xFF >> (8 - c_len % 8) ;
   b4->node_hdr.dup_byte_cnt = b4->node_hdr.trail_byte_cnt ;

   r_len = d4reccount( b4->tag->index->data ) ;
   for ( c_len = 0 ; r_len ; r_len>>=1, c_len++ ) ;
   b4->node_hdr.rec_num_len = c_len + (( 8 - ( 2 * b4->node_hdr.trail_cnt_len % 8 )) % 8) ;
   if ( b4->node_hdr.rec_num_len < 12 ) b4->node_hdr.rec_num_len = 12 ;

   for( t_len = b4->node_hdr.rec_num_len + b4->node_hdr.trail_cnt_len + b4->node_hdr.dup_cnt_len ;
        (t_len / 8)*8 != t_len ; t_len++, b4->node_hdr.rec_num_len++ ) ;  /* make at an 8-bit offset */

   b4->node_hdr.rec_num_mask = ff >> sizeof(long)*8 - b4->node_hdr.rec_num_len ;
   b4->node_hdr.info_len = (b4->node_hdr.rec_num_len + b4->node_hdr.trail_cnt_len + b4->node_hdr.dup_cnt_len) / 8 ;
   b4->node_hdr.free_space = B4BLOCK_SIZE - sizeof( B4STD_HEADER ) - sizeof( B4NODE_HEADER ) ;
}

/* returns #number of significant bits in the rec */
int S4FUNCTION b4rec_len( long rec )
{
   char a ;
   char b = sizeof(long)*8 ;
   long mask = 0x01L << (b-1) ;    /* set leftmost bit  */
   for( a=0 ; a<b ; a++ )
   {
      if ( rec & mask ) return (b-a) ;
      mask >>= 1 ;
   }
   return 0 ;
}

/* calculates the # of trailing blanks on the key */
int S4FUNCTION b4calc_blanks( char *key_val, int len, char p_char )
{
   int a ;
   for ( a = len ; a > 0; a-- )
      if ( key_val[a-1] != p_char ) return ( len - a ) ;
   return len ;  /* all blanks */
}

/* calculate the # of duplicate bytes starting from the start of the string */
int S4FUNCTION b4calc_dups( char *ptr1, char *ptr2, int len )
{
   int a ;
   for ( a = 0 ; a < len; a++ )
      if ( ptr1[a] != ptr2[a] ) return a ;
   return len ;  /* all duplicates */
}

/* calculates the # of bytes required to add the new key before the present position */
/* also fills in the is structure, which holds insertion information */
/* dup/trail 1 = difference between new key and old key */
/* dup/trail 2 = difference between old key and it's previous key */
/* type : 0 = add, 1 = remove  (i.e. where calling from) */
int S4FUNCTION b4bytes_reqd( B4BLOCK *b4, INSERT_STRUCT *is, char *key_val, int type )
{
   int k_len = b4->tag->header.key_len ;

   #ifdef S4DEBUG
      if ( b4->header.n_keys == 0 )
         e4severe( e4info, "b4bytes_reqd()", E4_INFO_CIB, (char *) 0 ) ;
   #endif

   is->trail2 = b4->cur_trail_cnt ;
   is->trail1 = b4calc_blanks( key_val, k_len, b4->tag->p_char ) ;

   if ( type )
   {
      is->dup2 = b4->cur_dup_cnt ;
      is->dup1 = b4calc_dups( key_val, b4key_key( b4, b4->key_on ), k_len ) ;
      if( is->dup1 > k_len - b4->cur_trail_cnt )   /* cannot have more duplicates than number of bytes */
         is->dup1 = k_len - b4->cur_trail_cnt ;
   }
   else
   {
      if ( b4->key_on != 0 )   /* not first entry, so calculate duplicates */
      {
         is->dup1 = b4calc_dups( key_val, b4key_key( b4, b4->key_on-1 ), k_len ) ;
         if( is->dup1 > k_len - x4trail_cnt( b4, b4->key_on-1 ) )   /* cannot have more duplicates than number of bytes */
            is->dup1 = k_len - x4trail_cnt( b4, b4->key_on-1 ) ;
      }
      else
         is->dup1 = 0 ;

      if ( b4->key_on >= b4->header.n_keys )   /* last pos, != 0 (since always must be 1 key if here) */
         is->dup2 = x4dup_cnt( b4, b4->key_on-1 ) ;
      else
      {
         is->dup2 = b4calc_dups( key_val, b4key_key( b4, b4->key_on ), k_len-is->trail2 ) ;
         if( is->dup2 > k_len - is->trail1 )   /* cannot have more duplicates than number of bytes */
	    is->dup2 = k_len - is->trail1 ;
      }
   }

   if ( is->dup1 == k_len )
      is->trail1 = 0 ;  /* all duplicates */

   if ( is->dup2 == k_len )
      is->trail2 = 0 ;  /* all duplicates */

   return (2*k_len - is->trail2 - is->trail1 - is->dup1 - is->dup2 ) ;
}

/* retrieves the given record # in long format. */
long S4FUNCTION x4recno( B4BLOCK *b4, int num_in_block )
{
   int pos ;
   unsigned long *l_ptr = (unsigned long *)( b4->data + num_in_block * b4->node_hdr.info_len ) ;

   return ( *l_ptr & b4->node_hdr.rec_num_mask ) ;
}

int S4FUNCTION x4dup_cnt( B4BLOCK *b4, int num_in_block )
{
   int pos ;
   unsigned long *l_ptr ;

   if ( b4->node_hdr.info_len > 4 )  /* > size of long, so must do careful shifting and copying */
   {
      #ifdef S4DEBUG
	 if ( b4->node_hdr.rec_num_len <= 16 )
	    e4severe( e4info, "x4put_info", E4_INFO_CIB, (char *) 0 ) ;
      #endif
      l_ptr = (unsigned long *)( b4->data + num_in_block * b4->node_hdr.info_len + 2 ) ;
      pos = b4->node_hdr.rec_num_len - 16 ;
   }
   else
   {
      l_ptr = (unsigned long *)( b4->data + num_in_block * b4->node_hdr.info_len ) ;
      pos = b4->node_hdr.rec_num_len ;
   }

   return (int)( ( *l_ptr >> pos ) & b4->node_hdr.dup_byte_cnt ) ;
}

int S4FUNCTION x4trail_cnt( B4BLOCK *b4, int num_in_block )
{
   int pos ;
   unsigned long * l_ptr ;

   if ( b4->node_hdr.info_len > 4 )  /* > size of long, so must do careful shifting and copying */
   {
      #ifdef S4DEBUG
         if ( b4->node_hdr.rec_num_len <= 16 )
            e4severe( e4info, "x4put_info", E4_INFO_CIB, (char *) 0 ) ;
      #endif
      l_ptr = (unsigned long *)( b4->data + num_in_block * b4->node_hdr.info_len + 2 ) ;
      pos = b4->node_hdr.rec_num_len - 16 + b4->node_hdr.dup_cnt_len ;
   }
   else
   {
      l_ptr = (unsigned long *)( b4->data + num_in_block * b4->node_hdr.info_len ) ;
      pos = b4->node_hdr.rec_num_len + b4->node_hdr.dup_cnt_len;
   }

   return (int)( ( *l_ptr >> pos ) & b4->node_hdr.trail_byte_cnt ) ;
}

void S4FUNCTION x4put_info( B4NODE_HEADER *b4node_hdr, void *buffer, long rec, int trail, int dup )
{
   int pos ;
   unsigned char *buf = (unsigned char *) buffer ;
   unsigned long *l_ptr = (unsigned long *)buf ;

   memset( buf, 0, 6 ) ;
   *l_ptr = rec & b4node_hdr->rec_num_mask ;

   if ( b4node_hdr->info_len > 4 )  /* > size of long, so must do careful shifting and copying */
   {
      #ifdef S4DEBUG
	 if ( b4node_hdr->rec_num_len <= 16 )
	    e4severe( e4info, "x4put_info", E4_INFO_CIB, (char *) 0 ) ;
      #endif
      l_ptr = (unsigned long *)( buf + 2 ) ;  /* start at new pos */
      pos = b4node_hdr->rec_num_len - 16 ;
   }
   else
      pos = b4node_hdr->rec_num_len ;

   *l_ptr |= ((long)dup) << pos ;
   pos += b4node_hdr->dup_cnt_len ;
   *l_ptr |= ((long)trail) << pos ;
}

short S4FUNCTION x4reverse_short( short val )
{
   unsigned char i, out[2], *at_val ;
   at_val = (unsigned char *)&val ;

   for( i = 0 ; i < 2 ; i++ )
      out[i] = at_val[1 - i] ;
   return *((long  *)out) ;
}

long S4FUNCTION x4reverse( long val )
{
   unsigned char i, out[4], *at_val ;
   at_val = (unsigned char *)&val ;

   for( i = 0 ; i < 4 ; i++ )
      out[i] = at_val[3-i] ;
   return *((long  *)out) ;
}

/* inserts an entry into a leaf block (-1 means error, 1 means no room) */
int S4FUNCTION b4insert_leaf( B4BLOCK *b4, void *vkey_data, long rec )
{
   char *key_data = (char *)vkey_data ;
   unsigned char buffer[6] ;
   int reqd_len, len, len2, rc, old_key_on, kon = b4->key_on ;
   INSERT_STRUCT is ;
   long old_rec ;
   char *to_pos, *val, *save_pos, *old_key, *o_cur_pos ;
   int k_len = b4->tag->header.key_len ;
   unsigned char i_len = b4->node_hdr.info_len ;

   if ( b4->built_on != b4->key_on )
      b4go( b4, b4->key_on ) ;

   if ( b4->header.n_keys == 0 )   /* new block, easy insert */
   {
      if( b4->node_hdr.free_space == 0 )  /* block needs initialization */
      {
         b4leaf_init( b4 ) ;
         i_len = b4->node_hdr.info_len ;  /* reset the i_len */
      }
      is.trail2 = b4calc_blanks( key_data, k_len, b4->tag->p_char ) ;
      reqd_len = k_len - is.trail2 ;
      b4->cur_pos -= reqd_len ;
      o_cur_pos = b4->cur_pos ;
      memcpy( b4->cur_pos, key_data, reqd_len ) ;  /* key */
      old_rec = rec ;
      save_pos = b4->data ;
      is.dup2 = 0 ;
      b4->header.n_keys++ ;
   }
   else
   {
      /* if the record is > than the mask, must reset the block with new parameters: */
      if ( b4rec_len( rec ) > b4->node_hdr.rec_num_len )
      {
         old_rec = b4->key_on ;
         rc = b4reindex( b4 ) ;
         if (rc != 0 ) return rc ;
         i_len = b4->node_hdr.info_len ;  /* update the i_len */
         b4go( b4, old_rec ) ;  /* reposition to the correct spot */
      }
      len =  k_len - b4->cur_dup_cnt - b4->cur_trail_cnt ;
      reqd_len = b4bytes_reqd( b4, &is, key_data, 0 ) - len ;
      #ifdef S4DEBUG
	 if ( reqd_len < 0 )
	    e4severe( e4info, "Corrupt index blocks", (char *) 0 ) ;
      #endif
      if ( ( reqd_len + i_len ) > b4->node_hdr.free_space )  return 1 ;   /* not enough room */
      /* do the actual insertion */

      old_rec = b4->current->num ;
      save_pos = b4->data + b4->key_on * i_len ;

      /* first do the key part */
      old_key_on = b4->key_on ;
      len2 = k_len - is.dup1 - is.trail1 ;

      if ( b4->key_on >= b4->header.n_keys )   /* no movement (i.e. at end of block) */
      {
         to_pos = b4->cur_pos - reqd_len ;
         old_key = b4key_key( b4, old_key_on ) ;
         len = k_len - is.dup2 - is.trail2 ;
         memcpy( to_pos+len2, old_key + is.dup2, len ) ;
         memcpy( to_pos, key_data + is.dup1, len2 ) ;
         b4->cur_pos = to_pos ;
         o_cur_pos = b4->cur_pos ;
      }
      else
      {
         long mov_len ;
         char *end_pos = b4->cur_pos + len ;
         b4go( b4, b4lastpos( b4 ) ) ;
         o_cur_pos = b4->cur_pos - reqd_len ;
         b4go_eof( b4 ) ;
         old_key = b4key_key( b4, old_key_on ) ;
         to_pos = b4->cur_pos - reqd_len ;
         #ifdef S4DEBUG
	    if ( (end_pos - b4->cur_pos) < 0 )
               e4severe( e4info, "b4insert_leaf()", E4_INFO_CIB, (char *) 0 ) ;
         #endif
         /* make sure the amount moved doesn't go past end of right side */
         /* of block, in case a global compaction is occurring. */
         mov_len = end_pos - b4->cur_pos ;
         if ( to_pos + mov_len > end_pos )
            mov_len = end_pos - to_pos + 1 ;
         memmove( to_pos, b4->cur_pos, mov_len ) ;
         len = k_len - is.dup2 - is.trail2 ;
         end_pos -= (len + len2) ;
         memcpy( end_pos, old_key + is.dup2, len ) ;
         b4->cur_pos = end_pos + len ;
         memcpy( b4->cur_pos, key_data + is.dup1, len2 ) ;
      }

      b4->built_on++ ;
      b4->header.n_keys++ ;
      b4->key_on = old_key_on ;

      /* then do the header part */
      if ( b4->header.n_keys == b4->key_on )
         len = 0 ;
      else
         len = (b4->header.n_keys - b4->key_on - 1)*i_len ;
      #ifdef S4DEBUG
         if ( len < 0 )
            e4severe( e4info, "b4insert_leaf()", E4_INFO_CIB, (char *) 0 ) ;
      #endif
      if ( len )  /* regular case */
      {
         memmove( save_pos+i_len, save_pos, len ) ;
         memset( save_pos, 0, i_len*2 ) ;  /* clear the info */
      }
      else
	 memset( save_pos-i_len, 0, i_len*2 ) ;  /* clear the info */

      x4put_info( &b4->node_hdr, buffer, rec, is.trail1, is.dup1 ) ;
      memcpy( save_pos, buffer, i_len ) ;

      if ( !len )  /* next insertion should go back one spot */
	 save_pos -= i_len ;
      else
	 save_pos += i_len ;
   }

   x4put_info( &b4->node_hdr, buffer, old_rec, is.trail2, is.dup2 ) ;
   memcpy( save_pos, buffer, i_len ) ;

   b4->changed =  1 ;
   b4->node_hdr.free_space -= ( reqd_len + i_len ) ;
   b4->cur_pos = o_cur_pos ;
   b4->cur_dup_cnt = x4dup_cnt( b4, b4->key_on ) ;
   b4->cur_trail_cnt = x4trail_cnt( b4, b4->key_on ) ;
   b4go( b4, kon ) ;
   return 0 ;
}

/* the new_flag marks whether we are inserting as an initial insertion routine */
/* or whether we are inserting into an existing block (an issue when 1 key */
/* exists in the block). */
int S4FUNCTION b4insert_branch( B4BLOCK *b4, void *k, long r, long r2, char new_flag )
{
   int left_len, move_len ;
   int g_len = b4->tag->header.key_len + 2 * sizeof(long) ;
   char *data_ptr, *next_ptr ;

   #ifdef S4DEBUG
      if ( k == 0 || r <= 0L )
         e4severe( e4parm, "b4insert_branch()", (char *) 0 ) ;
   #endif

   data_ptr = ((char *)&b4->node_hdr) + b4->key_on * g_len ;
   next_ptr = data_ptr + g_len ;

   left_len =  B4BLOCK_SIZE - sizeof( b4->header ) - g_len * b4->header.n_keys ;
   if ( left_len < g_len )  return 1 ;  /* not enough room */

   #ifdef S4DEBUG
      if ( b4->key_on < 0  ||  b4->key_on > b4->header.n_keys )
	 e4severe( e4info, "b4insert_branch()", (char *) 0 ) ;
   #endif

   move_len = g_len * (b4->header.n_keys - b4->key_on) ;

   memmove( next_ptr, data_ptr, move_len ) ;
   b4->header.n_keys++ ;
   memcpy( data_ptr, k, b4->tag->header.key_len ) ;
   memset( data_ptr + g_len - 2*sizeof(long), 0, sizeof(long) ) ;

   r2 = x4reverse( r2 ) ;
   memcpy( data_ptr + g_len - 2*sizeof(long), &r2, sizeof(long) ) ;

   r = x4reverse( r ) ;

   if ( !new_flag &&  (b4->key_on + 1) != b4->header.n_keys )
      memcpy(  next_ptr + g_len - sizeof(long), &r, sizeof(long) ) ;
   else
      memcpy( data_ptr + g_len - sizeof(long), &r, sizeof(long) ) ;

   b4->changed =  1 ;

   return 0 ;
}

int S4FUNCTION b4insert( B4BLOCK *b4, void *key_data, long rec, long rec2, char new_flag )
{
   if( b4->header.node_attribute >= 2 ) /* leaf */
      return b4insert_leaf( b4, key_data, rec ) ;
   else
      return b4insert_branch( b4, key_data, rec, rec2, new_flag ) ;
}

/* tries to change the block so that a new record length will work */
/* a return of 1 means the block is too small to make the required change */
int S4FUNCTION b4reindex( B4BLOCK *b4 )
{
   int t_cnt, d_cnt, r_len, i, dup, trail ;
   int ni_len = b4->node_hdr.info_len + 1 ;
   unsigned char buffer[6] ;
   long rec ;
   int space_reqd = b4->header.n_keys ;   /* 1 byte xtra for each record */
   if ( space_reqd > b4->node_hdr.free_space ) return 1 ;  /* not enough room */

   for ( i = b4->header.n_keys-1 ; i >= 0 ; i-- )
   {
      dup = x4dup_cnt( b4, i ) ;
      trail = x4trail_cnt( b4, i ) ;
      rec = x4recno( b4, i ) ;
      memset( b4->data + i * ni_len, 0, ni_len ) ;

      b4->node_hdr.rec_num_len += 8 ;  /* for the new info */
      b4->node_hdr.info_len++ ;
      x4put_info( &b4->node_hdr, buffer, rec, trail, dup ) ;
      b4->node_hdr.rec_num_len -= 8 ;  /* to get the old info */ 
      b4->node_hdr.info_len-- ;
      memcpy( b4->data + i * ni_len, buffer, ni_len ) ;
   }
   b4->node_hdr.rec_num_mask |= (unsigned long)(0x000000FFL << b4->node_hdr.rec_num_len );
   b4->node_hdr.info_len++ ;
   b4->node_hdr.rec_num_len += 8 ;
   b4->node_hdr.free_space -= b4->header.n_keys ;
   return 0 ;
}

B4BLOCK *S4FUNCTION b4alloc( T4TAG *t4, long fb )
{
   B4BLOCK *b4 ;
   int len ;

   if ( (b4 = (B4BLOCK *) y4alloc( t4->index->block_memory )) == 0 )
   {
      e4error( t4->code_base, e4memory, (char *) 0 ) ;
      return 0 ;
   }
   memset( b4, 0, B4BLOCK_SIZE + sizeof(B4BLOCK) - sizeof(b4->header) - sizeof(b4->node_hdr) ) ;

   b4->tag =  t4 ;
   len = sizeof(long) + t4->header.key_len + 1;
   b4->current = (B4KEY_DATA *) u4alloc( len ) ;
   b4->saved = (B4KEY_DATA *) u4alloc( len ) ;

   b4->file_block =  fb ;
   b4->key_on = b4->built_on = -1 ; /* a 'top' function is required! */

   return b4 ;
}

void S4FUNCTION b4free( B4BLOCK *b4 )
{
   #ifdef S4DEBUG
      if ( b4->changed )  e4severe( e4info, "b4free()", (char *) 0 ) ;
   #endif
   u4free( b4->current ) ;
   u4free( b4->saved ) ;
   y4free( b4->tag->index->block_memory, b4 ) ;
}

int S4FUNCTION b4flush( B4BLOCK *b4 )
{
   int rc ;
   I4INDEX *i4 ;

   if ( b4->changed )
   {
      i4 =  b4->tag->index ;
      rc =  h4write( &i4->file, (long)b4->file_block*I4MULTIPLY, &b4->header.node_attribute, B4BLOCK_SIZE ) ;
      if ( rc < 0 )  return rc ;
      b4->changed =  0 ;
   }
   return 0 ;
}

int S4FUNCTION b4go( B4BLOCK *b4, int i )
{
   int rc = 0; 

   if ( i == b4->built_on )
   {
      b4->key_on = i ;
      /* update the current recno, and done, since key already built */
      if ( b4->header.node_attribute >= 2 ) /* leaf */
	 b4->current->num = x4recno( b4, i ) ;
      else
         b4->current->num = x4reverse( *(long *)(((unsigned char *)&b4->node_hdr)
			    + (i+1)*(2*sizeof(long) + b4->tag->header.key_len)
			    - sizeof(long) ) ) ;
      return 0 ;
   }

   if ( i >= b4->header.n_keys )
      rc = r4eof ;

   b4->key_on = b4->built_on ;
   if ( b4->header.node_attribute >= 2 ) /* leaf */
      b4skip( b4, i - b4->key_on, 1 ) ;
   else
   {
      b4->key_on = i ;
      b4->built_on = i ;
      if ( !rc ) /* if not eof, set values */
      {
	 memcpy( &b4->current->value,
		 (((char *)&b4->node_hdr) + i*(2*sizeof(long) + b4->tag->header.key_len ) ),
		  b4->tag->header.key_len ) ;
	 b4->current->num = x4reverse( *(long *)(((unsigned char *)&b4->node_hdr)
                            + (i+1)*(2*sizeof(long) + b4->tag->header.key_len)
                            - sizeof(long)) ) ;
      }
   }
   return rc ;
}

/* goes to one past the end of the block */
void S4FUNCTION b4go_eof( B4BLOCK *b4 )
{
   b4go( b4, b4->header.n_keys ) ;
}

B4KEY_DATA *S4FUNCTION b4key( B4BLOCK *b4, int i_key )
{
   if ( i_key == b4->built_on ) return b4->current ; /* already there! */
   if ( b4->header.node_attribute >= 2 ) /* leaf */
   {
      int save_kon = b4->key_on ;
      int save_on = b4->key_on = b4->built_on ;
      int o_trail = b4->cur_trail_cnt ;
      int o_dup = b4->cur_dup_cnt ;
      char *old_pos = b4->cur_pos ;
      B4KEY_DATA *temp = b4->current ;
      memcpy( &b4->saved->num, &b4->current->num, b4->tag->header.key_len + sizeof(long) ) ;
      b4go( b4, i_key ) ;
      b4->current = b4->saved ;
      b4->saved = temp ;
      b4->key_on = save_kon ;
      b4->built_on = save_on ;
      b4->cur_pos = old_pos ;
      b4->cur_trail_cnt = o_trail ;
      b4->cur_dup_cnt = o_dup ;
   }
   else /* branch */
   {
      memcpy( &b4->saved->value,
	      (((char *)&b4->node_hdr) + i_key*(2*sizeof( long ) + b4->tag->header.key_len ) ),
	       b4->tag->header.key_len ) ;
       b4->current->num = x4reverse( *(long *)( ((unsigned char *)&b4->node_hdr)
			  + (i_key+1)*(2*sizeof(long) + b4->tag->header.key_len)
                          - sizeof(long) ) ) ;
   }
   return b4->saved ;
}

char *S4FUNCTION b4key_key( B4BLOCK *b4, int i_key )
{
   return  b4key( b4, i_key )->value ;
}

int S4FUNCTION b4lastpos( B4BLOCK *b4 )
{
   return b4->header.n_keys-1 ;
}

int S4FUNCTION b4leaf( B4BLOCK *b4 )
{
   return( b4->header.node_attribute >= 2 ) ;
}

long S4FUNCTION b4recno( B4BLOCK *b4 )
{
   if ( b4->header.node_attribute >= 2 ) /* leaf */
      return b4->current->num ;
   else /* branch */
      return x4reverse( *(long *)(((unsigned char *)&b4->node_hdr)
			  + b4->key_on * (2*sizeof(long) + b4->tag->header.key_len)
			   + b4->tag->header.key_len ) ) ;
}

/* next function replaces the current entry with given str and rec#. */
void S4FUNCTION b4br_replace( B4BLOCK *b4, char *str, long rec )
{
   int k_len = b4->tag->header.key_len; 
   char *put_pl = ((char *)&b4->node_hdr) + b4->key_on * (2*sizeof(long) + k_len) ;
   memcpy( put_pl, str, k_len ) ;
   memcpy( b4->current->value, str, k_len ) ;
   memset( put_pl + k_len, 0, sizeof(long) ) ;
   rec = x4reverse( rec ) ;
   memcpy( put_pl + k_len, &rec, sizeof(long) ) ;

   b4->changed = 1 ;
}

/* seek on the branch block, with the given record # as additional input */
/* recno should be in */
int S4FUNCTION b4r_brseek( B4BLOCK *b4, char *search_value, int len, long recno )
{
   int rc = b4seek( b4, search_value, len ) ;
   int k_len = b4->tag->header.key_len ;

   if ( rc == 0 )   /* string matches, so search on recno */
   {
      for(;;)
      {
	 if ( memcmp( &recno, ((char *)&b4->node_hdr) + b4->key_on*( 2*sizeof(long)+ k_len) + k_len, sizeof(long) ) <= 0
              || b4->key_on >= ( b4->header.n_keys - 1 ) )  /* best match, so done */
           break ;
         else if( b4->key_on < b4->header.n_keys )
            b4skip( b4, 1L, 1 ) ;

         if ( memcmp( b4key_key(b4,b4->key_on), search_value, len ) )  /* the key has changed, so stop here */
            break ;
      }
   }
   return rc ;
}

int S4FUNCTION b4seek( B4BLOCK *b4, char *search_value, int len )
{
   int rc, key_lower, key_upper, save_rc, key_cur ;
   if ( b4->header.n_keys == 0 )
   {
      b4top( b4 ) ;
      return r4after ;
   }

   if ( b4leaf(b4) ) return b4leaf_seek( b4, search_value, len ) ;
   /* otherwise do a standard binary seek! */

   /* key_cur must be between  key_lower and  key_upper */
   key_lower =  -1 ;
   key_upper =  b4->header.n_keys - 1 ;

   save_rc =  1 ;

   for(;;)  /* Repeat until the key is found */
   {
      key_cur = (key_upper + key_lower) / 2 ;
      /* only memcmp for branch blocks */
      rc = memcmp( b4key_key(b4,key_cur), search_value, len ) ;

      if ( rc >= 0 )
      {
         key_upper =  key_cur ;
         save_rc =  rc ;
      }
      else
         key_lower =  key_cur ;

      if ( key_lower >= (key_upper-1) )  /* then there is no exact match */
      {
         b4go( b4, key_upper ) ;
         if ( save_rc )  return r4after ;
         return 0 ;
      }
   }
}

int S4FUNCTION b4leaf_seek( B4BLOCK *b4, char *search_value, int len )
{
   S4CMP_FUNCTION *cmp = b4->tag->cmp ;

   /* special seek, since must build keys, start at the first entry, and */
   /* work from there, expanding and comparing as required! */

   /* first get a reference to compare with (i.e. the first key) */
   int add_pos, on_pos ;
   b4top( b4 ) ;
   on_pos =  (*cmp)( b4key_key(b4,b4->key_on), search_value, len ) ;
   if ( on_pos == 0 ) return 0 ;
   if ( on_pos == -1 ) return r4after ;
   for(;;)  /* while not found! */
   {
      if ( b4skip( b4, 1L, 1 ) != 1L ) return r4after ;  /* seek failed, at last position in block */
      if ( on_pos > b4->cur_dup_cnt + 1 )  /* gone past, so return r4after */
          return r4after ;
      if ( on_pos == b4->cur_dup_cnt + 1 )  /* re-check, since a vital position has changed */
      {
         if ( (add_pos = (*cmp)( b4key_key(b4,b4->key_on) + b4->cur_dup_cnt,
                          search_value + b4->cur_dup_cnt, len - b4->cur_dup_cnt ) ) == 0 )
            return 0 ;
         if ( add_pos == -1 ) return r4after ;  /* gone one past */
         on_pos += add_pos - 1 ;
      }
   }
}

void S4FUNCTION  b4remove_branch( B4BLOCK *b4 )
{
   char *key_cur, *key_next ;
   int  len, i_len = b4->tag->header.key_len + 2*sizeof(long) ;

   key_cur = ((char *)&b4->node_hdr) + i_len * b4->key_on ;

   len = (b4->header.n_keys - b4->key_on - 1) * i_len ;

   #ifdef S4DEBUG
      if ( b4->key_on < 0  ||  b4->key_on > b4lastpos(b4) ||
	   len < 0 || len > (B4BLOCK_SIZE - sizeof(B4STD_HEADER) - i_len ))
         e4severe( e4info, "b4remove_branch()", (char *) 0 ) ;
   #endif

   if ( len > 0 )
      memmove( key_cur, key_cur + i_len, len ) ;


   b4->header.n_keys-- ;
                                       /* reset the contents to 0 */
   memset( ((char *)&b4->node_hdr) + b4->header.n_keys * i_len, 0, i_len ) ;

   b4->changed =  1 ;
}

void S4FUNCTION b4remove_leaf( B4BLOCK *b4 )
{
   int len, len2, i_len = b4->node_hdr.info_len ;
   long rec, second_rec ;
   char *key ;
   INSERT_STRUCT is ;
   unsigned char buffer[6] ;
   int k_len = b4->tag->header.key_len ;
   int key_on = b4->key_on ;
   int type = 0 ;
   int remove_len = 0 ;
   char *i_pos, *left_pos, *start_pos ;
   char *temp_key = (char *)u4alloc( k_len ) ;

   if ( b4->key_on == 0 ) type = 1 ;   /* at front  or 1 key */
   else if (b4->key_on == (b4->header.n_keys - 1) ) type = 2 ;  /* at end > 1 key */

   start_pos = b4->cur_pos ;
   b4go_eof( b4 ) ;
   left_pos = b4->cur_pos ;
   b4go( b4, key_on ) ;

   if ( type != 2 )  /* if at end_pos, do special */
   {
      int num_move = b4->header.n_keys - b4->key_on - 1 ;
      /* amt room = current len - reqd_len of prev and next key */
      int old_len = k_len - b4->cur_dup_cnt - b4->cur_trail_cnt ;
      if ( !type )
      {
         b4skip( b4, -1L, 1 ) ;
         old_len += (k_len - b4->cur_dup_cnt - b4->cur_trail_cnt ) ;
         b4skip( b4, 1L, 1 ) ;
      }

      b4skip( b4, 1L, 1 ) ;
      second_rec = b4key( b4, b4->key_on )->num ;
      if ( type )
      {
         is.trail1 = b4calc_blanks( b4key_key( b4, b4->key_on ), k_len, b4->tag->p_char ) ;
         is.dup1 = 0 ;
         remove_len = old_len - b4->cur_trail_cnt - b4->cur_dup_cnt + is.trail1  ;
      }
      else
      {
         memcpy( temp_key, b4key_key( b4, b4->key_on ), k_len ) ;
         old_len += (k_len - b4->cur_dup_cnt - b4->cur_trail_cnt ) ;
         b4skip( b4, -2L, 1 ) ;
         remove_len =  old_len - b4bytes_reqd( b4, &is, temp_key, 1 ) ;
         #ifdef S4DEBUG
            if ( remove_len < 0 )
               e4severe( e4info, "Corrupt index blocks", (char *) 0 ) ;
         #endif
         memcpy( temp_key, b4key_key( b4, b4->key_on ), k_len ) ; /* for later use */
	 rec = b4key( b4, b4->key_on )->num ;
         b4skip( b4, 2L, 1 ) ;
      }

      key = b4key_key( b4, b4->key_on ) ;
      /* remove key part */
      len2 = k_len - is.dup1 - is.trail1 ;
      if ( !type )  /* put the new key that is right of the removed one */
      {
         len = k_len - is.dup2 - is.trail2 ;
         b4->cur_pos += (old_len - len) ;
         memcpy( b4->cur_pos, temp_key + is.dup2, len ) ;
         b4->cur_pos -= len2 ;
      }
      else
         b4->cur_pos = start_pos + old_len - len2 ;
      /* now put the new key that is left of the removed one */
      memcpy( b4->cur_pos, key + is.dup1, len2 ) ;
      if ( remove_len )
      {
         memmove( left_pos + remove_len, left_pos, b4->cur_pos - left_pos - remove_len ) ;
         /* and delete the data at the left */
         memset( left_pos, 0, remove_len ) ;
      }

      /* remove the info part */
      memmove( b4->data + i_len * key_on, b4->data + i_len*(key_on+1), num_move*i_len ) ;
      i_pos = b4->data + i_len * key_on ;
      if ( type )
      {
         len = i_len ;
         memset( i_pos, '\0', len ) ;
      }
      else  /*  */
      {
	 len = 2*i_len ;
	 memset( i_pos-i_len, '\0', len ) ;
      }
      x4put_info( &b4->node_hdr, buffer, second_rec, is.trail1, is.dup1 ) ;
      memcpy( i_pos, buffer, i_len ) ;

      if ( !type )
      {
	 i_pos -= i_len ;
	 x4put_info( &b4->node_hdr, buffer, rec, is.trail2, is.dup2 ) ;
         memcpy( i_pos, buffer, i_len ) ;
      }
   }
   else  /* on last key, so move to previous one */
   {
      int len ;
      char *n_end_pos, *end_pos = b4->cur_pos ;
      b4go( b4, b4->key_on-1 ) ;
      /* clear the key data: */
      n_end_pos = b4->cur_pos ;
      len = n_end_pos - end_pos ;
      #ifdef S4DEBUG
         if ( len < 0 )
            e4severe( e4info, "b4remove_leaf()", E4_INFO_CIB, (char *) 0 ) ;
      #endif
      if ( len > 0 )
         memset( end_pos, 0, len ) ;
   }

   /* now blank out the info */
   u4free( temp_key ) ;
   b4->header.n_keys-- ;
   memset( b4->data + i_len * b4->header.n_keys, 0, i_len ) ;
   b4->changed =  1 ;
   b4->node_hdr.free_space += ( remove_len + b4->node_hdr.info_len ) ;
}

void S4FUNCTION  b4remove( B4BLOCK *b4 )
{
   if( b4->header.node_attribute >= 2 ) /* leaf */
      b4remove_leaf( b4 ) ;
   else
      b4remove_branch( b4 ) ;
}

/* returns the # skipped! */
/* update flag marks whether or not the current value should be updated */
/* on the skip.  (i.e. used for b4go() and b4seek() ) ; */
int S4FUNCTION b4skip( B4BLOCK *b4, long n, int update )
{
   long num_left = n ;
   if ( b4->header.node_attribute < 2 ) /* branch */
      num_left -= b4branch_skip( b4, n ) ;
   else
   {
      if ( num_left < 0 )
      {
         int kon = b4->key_on ;
         if ( num_left + b4->key_on < 0 ) return -(b4->key_on) ;
         if ( -num_left < B4DO_BACK_LEVEL || !update )
            for( ; num_left ; num_left++ )
	       if ( b4back_skip( b4, update ) < 0 )
                  break ;
         if ( num_left != 0 )  /* go from top */
         {
            num_left = n + kon ;
            b4->key_on = -1 ;
            b4top( b4 ) ;
         }
      }

      for( ; num_left ; num_left-- )
         if ( b4do_skip( b4, update ) != 0 ) break ;
   }
   return ( n - num_left ) ;
}

/* this function skips right one key in a compact leaf block */
/* returns r4eof if skipped past last key */
int S4FUNCTION b4do_skip( B4BLOCK *b4, int update )
{
   char len ;

   #ifdef S4DEBUG
      if ( update )
         if ( b4->built_on != b4->key_on )
            e4severe( e4info, "b4do_skip", E4_INFO_EPV, (char *) 0 ) ;
   #endif

   b4->key_on++ ;

   if ( b4->key_on >= b4->header.n_keys )
      return r4eof ;

   if ( update )
   {
      b4->built_on++ ;
      b4->cur_dup_cnt = x4dup_cnt( b4, b4->key_on ) ;
      b4->cur_trail_cnt = x4trail_cnt( b4, b4->key_on ) ;
      len = b4->tag->header.key_len - b4->cur_dup_cnt - b4->cur_trail_cnt ;
      #ifdef S4DEBUG
         if (len < 0 || len > b4->tag->header.key_len || ( b4->cur_pos - len ) < b4->data )
            e4severe( e4info, "b4do_skip", E4_INFO_CIF, (char *) 0 ) ;
      #endif
      b4->cur_pos -= len ;
      memcpy( b4->current->value + b4->cur_dup_cnt, b4->cur_pos, len ) ;
      memset( b4->current->value + b4->tag->header.key_len - b4->cur_trail_cnt, b4->tag->p_char, b4->cur_trail_cnt ) ;
   }
   b4->current->num = x4recno( b4, b4->key_on ) ;

   return 0 ;
}

/* this function tries to skip left one key in a compact leaf block */
int S4FUNCTION b4back_skip( B4BLOCK *b4, int update )
{
   int len, trail, dup  ;

   #ifdef S4DEBUG
      if ( update )
         if ( b4->built_on != b4->key_on )
            e4severe( e4info, "b4do_skip", E4_INFO_EPV, (char *) 0 ) ;
      if ( b4->key_on <= 0 )
         e4severe( e4info, "b4back_skip()", E4_INFO_COD, (char *) 0 ) ;
   #endif

   b4->key_on -- ;
   if ( update )   /* don't mark if r4eof */
      b4->built_on-- ;

   if ( b4->key_on >= b4->header.n_keys )   /* at one past eob */
      return 0 ;

   if ( update )   /* don't mark if r4eof */
   {
      trail = x4trail_cnt( b4, b4->key_on ) ;
      dup = x4dup_cnt( b4, b4->key_on ) ;
      if ( b4->cur_trail_cnt > trail || b4->cur_dup_cnt < dup )  /* can't go back */
	 return -1 ;
      b4->cur_pos += b4->tag->header.key_len - b4->cur_dup_cnt - b4->cur_trail_cnt ;
      b4->cur_dup_cnt = dup ;
      b4->cur_trail_cnt = trail ;
      len = b4->tag->header.key_len - dup - trail ;
      #ifdef S4DEBUG
         if (len < 0 || len > b4->tag->header.key_len )
            e4severe( e4info, "b4back_skip", E4_INFO_CIF, (char *) 0 ) ;
      #endif

      memcpy( b4->current->value + dup, b4->cur_pos, len ) ;
      /* can maybe optomize next line by setting only the bytes needed to be reset (but does calculation override the benefits?) */
      memset( b4->current->value + b4->tag->header.key_len - trail, b4->tag->p_char, trail ) ;
   }
   b4->current->num = x4recno( b4, b4->key_on ) ;

   return 0 ;
}

int S4FUNCTION b4branch_skip( B4BLOCK *b4, long n )
{
   int num_left ;

   if ( n > 0 )
      num_left = b4->header.n_keys - b4->key_on ;
   else
      num_left =  -b4->key_on ;

   if ( ( n <= 0 ) ?  (num_left <= n)  :  (num_left >= n)  )
   {
      b4go( b4, b4->key_on + (int) n ) ;
      return (int) n ;
   }
   else
   {
      b4go( b4, b4->key_on + num_left ) ;
      return num_left ;
   }
}

int S4FUNCTION b4top( B4BLOCK *b4 )
{
   if ( b4->built_on == 0 && b4->key_on != -1 )   /* if key_on == -1, do top, otherwise dependent on built on setting */
   {
      b4->key_on = 0 ;
      return 0 ;  /* already at top! */
   }
   b4->key_on = b4->built_on = 0 ;
   if ( b4leaf( b4 ) )
   {
      b4->dup_pos = (b4->node_hdr.info_len - 1) * 8 - 8*(b4->node_hdr.trail_cnt_len / 8) + (b4->node_hdr.trail_cnt_len % 8) ;
      b4->trail_pos = (b4->node_hdr.info_len - 1) * 8 ;
      b4->rec_pos = (b4->node_hdr.info_len - 1) * 8 - 8*((b4->node_hdr.trail_cnt_len + b4->node_hdr.dup_cnt_len) / 8) +
		    ((b4->node_hdr.trail_cnt_len + b4->node_hdr.dup_cnt_len) % 8) ;
      b4->cur_dup_cnt = x4dup_cnt( b4, 0 ) ;
      b4->cur_trail_cnt = x4trail_cnt( b4, 0 ) ;

      memset( b4->current->value, b4->tag->p_char, b4->tag->header.key_len ) ;
      b4->cur_pos = b4->data + 488 ;
      if ( b4->header.n_keys > 0 )   /* non-empty block */
      {
	 int len = b4->tag->header.key_len - b4->cur_dup_cnt - b4->cur_trail_cnt ;
	 #ifdef S4DEBUG
	    if (len < 0 || len > b4->tag->header.key_len )
	       e4severe( e4info, "b4top()", E4_INFO_CIF, (char *) 0 ) ;
	 #endif
	 b4->cur_pos -= len ;
	 memcpy( b4->current->value + b4->cur_dup_cnt, b4->cur_pos, len ) ;
	 b4->current->num = x4recno( b4, 0 ) ;
      }
      else
	 b4->current->num = 0 ;
   }
   else /* branch */ 
   {
      memcpy( &b4->current->value,(char *)&b4->node_hdr, b4->tag->header.key_len ) ;
      b4->current->num = x4reverse( *(long *)( ((unsigned char *)&b4->node_hdr)
			  + sizeof(long) + b4->tag->header.key_len) ) ;
   }
   return 0 ;
}
#endif

#ifdef N4OTHER

B4BLOCK * S4FUNCTION b4alloc( T4TAG *t4, long fb )
{
   B4BLOCK *b4 ;
   #ifdef S4CLIPPER
      short offset ;
      int i ;
   #endif

   if ( (b4 = (B4BLOCK *) y4alloc( t4->index->block_memory )) == 0 )
   {
      e4error( t4->code_base, e4memory, (char *) 0 ) ;
      return 0 ;
   }

   b4->tag =  t4 ;
   b4->file_block =  fb ;
   #ifdef S4CLIPPER
      offset = ( b4->tag->header.keys_max + 2 +
                 ( (b4->tag->header.keys_max/2)*2 != b4->tag->header.keys_max )
               ) * sizeof(short) ;
      for ( i = 0 ; i <= b4->tag->header.keys_max ; i++ )
	 b4->pointers[i] = (short)(( b4->tag->header.group_len * i )) + offset ;
      b4->data = (B4KEY_DATA *) ((char *)&b4->n_keys + b4->pointers[0]) ;  /* first entry */
   #endif

   return b4 ;
}

void S4FUNCTION b4free( B4BLOCK *b4 )
{
   y4free( b4->tag->index->block_memory, b4 ) ;
}

int S4FUNCTION b4flush( B4BLOCK *b4 )
{
   int rc ;

   if ( b4->changed )
   {
      #ifdef S4NDX
	 rc = h4write( &b4->tag->file, (long)b4->file_block*I4MULTIPLY, &b4->n_keys, B4BLOCK_SIZE ) ;
      #else
         #ifdef S4CLIPPER
	    rc = h4write( &b4->tag->file, (long)b4->file_block*I4MULTIPLY, &b4->n_keys, B4BLOCK_SIZE ) ;
         #endif
      #endif
      if ( rc < 0 )  return rc ;
      b4->changed =  0 ;
   }
   return 0 ;
}

void S4FUNCTION b4append( B4BLOCK *b4, long pointer )
{
   #ifdef S4CLIPPER
      long adj_pointer = pointer * 512 ;
   #endif
   B4KEY_DATA *data_ptr ;
   #ifdef S4DEBUG
      int left_len ;
      if ( b4leaf( b4 ) || pointer <1L )
	 e4severe( e4parm, "b4append()", (char *) 0 ) ;
   #endif

   b4go_eof( b4 ) ;
   data_ptr = b4key( b4, b4->key_on ) ;

   #ifdef S4DEBUG
      left_len = B4BLOCK_SIZE - b4->key_on * b4->tag->header.group_len -
	 sizeof(short) - sizeof(char[2]) - sizeof(pointer) ;
      if ( b4->key_on < 0  ||  b4->key_on != b4->n_keys ||  left_len < 0 )
	  e4severe( e4info, "b4append()", (char *) 0 ) ;
   #endif

   #ifdef S4NDX
      memcpy( &data_ptr->pointer, &pointer, sizeof(pointer) ) ;
   #else
      memcpy( &data_ptr->pointer, &adj_pointer, sizeof(pointer) ) ;
   #endif
   b4->changed =  1 ;
}

#ifdef S4CLIPPER
void S4FUNCTION b4append2( B4BLOCK *b4, void *k, long r, long pointer )
{
   B4KEY_DATA *data_ptr ;
   long adj_pointer ;

   b4go_eof( b4 ) ;

   if( ( b4->n_keys > 0) && !b4leaf( b4 ) )  b4->key_on++ ;   /* insert after branch */

   data_ptr  =  b4key( b4, b4->key_on ) ;
   adj_pointer = pointer * 512 ;
   b4->n_keys++ ;

   memcpy( data_ptr->value, k, b4->tag->header.group_len-2*sizeof(long) ) ;
   memcpy( &data_ptr->num, &r, sizeof(r) ) ;
   memcpy( &data_ptr->pointer, &adj_pointer, sizeof(pointer) ) ;
   b4->changed =  1 ;
}
#endif

#ifdef S4NDX
int S4FUNCTION b4find( B4BLOCK *b4, long f_block, B4BLOCK *out_block )
{
   B4BLOCK *cur_block ;

   cur_block = (B4BLOCK *)l4first( &b4->tag->blocks ) ;
   if ( cur_block != 0 )
   {
      do
      {
	 if ( cur_block->file_block == f_block )
	 {
	    memcpy( &(out_block->changed), &(cur_block->changed),
	       (B4BLOCK_SIZE + 2*sizeof(int) + sizeof(long) + sizeof(void *)) ) ;
	    out_block->changed = 0 ;   /* temp block, so can delete if changed */
	    return( 0 ) ;
	 }
	 cur_block = (B4BLOCK *)l4next( &b4->tag->blocks, cur_block ) ;
      } while ( cur_block != 0 ) ;
   }

   cur_block = (B4BLOCK *)l4first( &b4->tag->saved ) ;
   if ( cur_block != 0 )
   {
      do
      {
	 if ( cur_block->file_block == f_block )
         {
	    memcpy( &(out_block->changed), &(cur_block->changed),
	       (B4BLOCK_SIZE + 2*sizeof(int) + sizeof(long) + sizeof(void *)) ) ;
            out_block->changed = 0 ;
            return ( 0 ) ;
         }
	 cur_block = (B4BLOCK *)l4next( &b4->tag->saved, cur_block ) ;
      } while ( cur_block != 0 ) ;
   }

   return ( -1 ) ;
}

int S4FUNCTION b4get_last_key( B4BLOCK *b4, char *key_info )
{
   int rc = 0 ;
   long block_down ;
   B4BLOCK *temp_block ;
   char *key_data ;

   #ifdef S4DEBUG
      if ( b4leaf( b4 ) )
      e4severe( e4info, "b4get_last_key()", (char *) 0 ) ;
   #endif

   block_down =  b4key(b4, b4->key_on)->pointer ;

   temp_block = b4alloc( b4->tag, block_down) ;
   if ( temp_block == 0 )    return -1 ;

   do
   {
      #ifdef S4DEBUG
         if ( block_down <= 0L )
            e4severe( e4info, "b4get_last_key()", (char *)0 ) ;
      #endif
      if ( b4find( b4, block_down, temp_block ) < 0 )
      {
	 if ( h4length( &b4->tag->file ) <= I4MULTIPLY*block_down)
         {
            b4free(temp_block) ;
            return -1 ;   /* error */
         }
         else
         {
	    if ( h4read_all( &b4->tag->file, I4MULTIPLY * block_down,
		 &temp_block->n_keys, B4BLOCK_SIZE ) < 0 ) return -1 ;
	       temp_block->file_block =  block_down ;
         }
      }

      temp_block->key_on = b4lastpos( temp_block ) ;
      block_down =  b4key(temp_block, temp_block->key_on)->pointer ;
   } while( !( b4leaf( temp_block ) ) ) ;

   key_data = b4key_key( temp_block, temp_block->key_on ) ;
   
   if( strcmp( key_info, key_data ) != 0 )
   {
      strcpy( key_info, key_data) ;
      rc++ ;   /* rc = 1 */
   }

   b4free( temp_block ) ;

   return rc ;
}

void  S4FUNCTION b4insert( B4BLOCK *b4, void *k, long r, long pointer )
{
   B4KEY_DATA *data_ptr, *next_ptr ;
   int left_len ;
   #ifdef S4DEBUG
      if ( k == 0 || r < 0L || pointer <0L )  
         e4severe( e4parm, "b4insert()", (char *) 0 ) ;
   #endif

   data_ptr = b4key( b4, b4->key_on ) ;
   next_ptr=  b4key( b4, b4->key_on + 1 ) ;

   left_len =  B4BLOCK_SIZE -  (b4->key_on + 1) * b4->tag->header.group_len -
		   sizeof(short) - sizeof(char[2]) ;   /* 2 for S4NDX version, not 6 */

   #ifdef S4DEBUG
      if ( b4->key_on < 0  ||  ( b4->key_on >  b4->n_keys && b4leaf( b4 ) )  ||
         ( b4->key_on > (b4->n_keys+1)  &&  !b4leaf( b4 ) )  || left_len < 0 )
	 e4severe( e4info, "b4insert()", (char *) 0 ) ;
   #endif

   memmove( next_ptr, data_ptr, left_len ) ;
   b4->n_keys++ ;
   memcpy( data_ptr->value, k, b4->tag->header.key_len ) ;
   memcpy( &data_ptr->num, &r, sizeof(r) ) ;
   memcpy( &data_ptr->pointer, &pointer, sizeof(pointer) ) ;
   b4->changed =  1 ;
}
#else
#ifdef S4CLIPPER
/* clipper just puts at end and inserts into the index part */
void  S4FUNCTION b4insert( B4BLOCK *b4, void *k, long r, long pointer )
{
   short temp, insert_pos ;
   #ifdef S4DEBUG
      if ( k == 0 || r < 0L || pointer <0L )
	 e4severe( e4parm, "b4insert()", (char *) 0 ) ;
   #endif

   insert_pos = b4->key_on ;

   /* put at block end: */
   b4append2( b4, k, r, pointer ) ;

   temp = b4->pointers[b4->key_on] ;   /* save the placed position */
   memmove( &b4->pointers[insert_pos+1], &b4->pointers[insert_pos], sizeof(short) * ( b4lastpos( b4 ) - insert_pos - (!b4leaf( b4 ) && b4->n_keys < 2) ) ) ;
   b4->pointers[insert_pos] = temp ;
}
#endif   /*  ifdef S4CLIPPER  */
#endif   /*  ifdef S4NDX  */

/* goes to one past the end of the block */
void S4FUNCTION b4go_eof( B4BLOCK *b4 )
{
   b4->key_on = b4->n_keys ;
}

B4KEY_DATA *S4FUNCTION b4key( B4BLOCK *b4, int i_key )
{
   #ifdef S4NDX
      return  (B4KEY_DATA *) ((char *) &b4->data + b4->tag->header.group_len * i_key) ;
   #else
      #ifdef S4CLIPPER
         #ifdef S4DEBUG
	    if ( i_key > 2 + b4->tag->header.keys_max )
	       e4severe( e4parm, "b4key()", (char *) 0 ) ;
         #endif
         return  (B4KEY_DATA *) ((char *) &b4->n_keys + b4->pointers[i_key] ) ;
      #endif
   #endif
}

char *S4FUNCTION b4key_key( B4BLOCK *b4, int i_key )
{
   return  b4key( b4, i_key )->value ;
}

int S4FUNCTION b4lastpos( B4BLOCK *b4 )
{
   if ( !b4leaf( b4 ) )
      return b4->n_keys ;
   else
      return b4->n_keys - 1 ;
}

int S4FUNCTION b4leaf( B4BLOCK *b4 )
{
   B4KEY_DATA *key_ptr =  b4key( b4, 0 ) ;    /* if 1st is not ptr, whole block not */
   return( key_ptr->pointer == 0L ) ;  /* pointer=0 if key (leaf), else not */
}

long  S4FUNCTION b4recno( B4BLOCK *b4 )
{
   return b4key( b4, b4->key_on )->num ;
}

#ifdef S4NDX
void  S4FUNCTION b4remove( B4BLOCK *b4 )
{
   B4KEY_DATA *key_on ;
   int left_len = -1 ;

   key_on = b4key( b4, b4->key_on ) ;

   if( !b4leaf( b4 ) )
   {
     if ( b4->key_on >= b4->n_keys )
	left_len = 0 ;
     else   /* on last pos, so copy only the rec_num */
        if ( b4lastpos( b4 ) + 1 == b4->n_keys ) 
           left_len = sizeof( long ) ;
   }

   if ( left_len == -1 ) /* if not yet initialized */
      left_len = B4BLOCK_SIZE - sizeof(b4->n_keys) - sizeof(b4->dummy) - (b4->key_on+1) * b4->tag->header.group_len ;

   #ifdef S4DEBUG
      if ( b4->key_on < 0  ||  b4->key_on > b4lastpos( b4 )  ||  left_len < 0 )
	 e4severe( e4info, "b4remove()", (char *) 0 ) ;
   #endif

   if ( left_len > 0 )
   {
      B4KEY_DATA *key_next=  b4key( b4, b4->key_on+1 ) ;
      memmove( key_on, key_next, left_len ) ;
   }
   b4->n_keys-- ;
   b4->changed =  1 ;
}
#else
#ifdef S4CLIPPER
void  S4FUNCTION b4remove( B4BLOCK *b4 )
{
   short temp ;

   /* just delete this entry */
   temp = b4->pointers[b4->key_on] ;
   memmove( &b4->pointers[b4->key_on], &b4->pointers[b4->key_on+1],
            sizeof(short) * (b4lastpos( b4 ) - b4->key_on) ) ;
   b4->pointers[b4lastpos( b4 )] = temp ;
   b4->n_keys-- ;
   b4->changed =  1 ;
}
#endif  /*  ifdef S4CLIPPER  */
#endif  /*  ifdef S4NDX  */

int S4FUNCTION b4room( B4BLOCK *b4 )
{
   if ( b4leaf( b4 ) ) return ( b4->n_keys < b4->tag->header.keys_max ) ;

   return( ( b4->n_keys < b4->tag->header.keys_max ) && ( ( B4BLOCK_SIZE -
      b4->n_keys * b4->tag->header.group_len - sizeof(short) - 2*sizeof(char)) >= sizeof(long) ) ) ;
}

int  S4FUNCTION b4seek( B4BLOCK *b4, char *search_value, int len )
{
   int rc, save_rc, key_cur ;
   S4CMP_FUNCTION *cmp = b4->tag->cmp ;

   /* key_on must be between  key_lower and  key_upper */
   int key_lower = -1 ;
   int key_upper = b4->n_keys ;

   if ( key_upper == 0 )
   {
      b4->key_on = 0 ;
      return r4after ;
   }

   save_rc =  1 ;

   for(;;)  /* Repeat until the key is found */
   {
      key_cur  =  (key_upper + key_lower) / 2  ;
      rc =  (*cmp)( b4key_key(b4,key_cur), search_value, len ) ;

      if ( rc >= 0 )
      {
	 key_upper = key_cur ;
	 save_rc = rc ;
      }
      else
	 key_lower = key_cur ;

      if ( key_lower >= (key_upper-1) )  /* then there is no exact match */
      {
	 b4->key_on =   key_upper ;
         if ( save_rc )  return r4after ;
         return 0 ;
      }
   }
}

int S4FUNCTION b4skip( B4BLOCK *b4, long n )
{
   int num_left ;

   if ( n > 0 )
   {
      num_left = b4->n_keys - b4->key_on ;
      if ( b4leaf( b4 ) )
      if ( num_left != 0 )
         num_left -- ;
   }
   else
      num_left =  -b4->key_on ;

   if ( ( n <= 0L ) ?  ((long) num_left <= n)  :  ((long) num_left >= n)  )
   {
      b4->key_on =  b4->key_on+ (int) n ;
      return (int) n ;
   }
   else
   {
      b4->key_on =  b4->key_on+num_left ;
      return num_left ;
   }
}

#endif  /*  ifdef S4MDX  */
