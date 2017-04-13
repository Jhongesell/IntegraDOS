/* i4tag.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

/* 't4num ...' routine returns:
     -1      -  Data     <   Search
   r4success -  Data    ==   Search
   r4after   -  Data     >   Search
*/

#ifndef N4OTHER

#ifndef S4LANGUAGE
#ifndef u4memcmp
int S4CALL u4memcmp( S4CMP_PARM p1, S4CMP_PARM p2, size_t len )
{
   return memcmp( p1, p2, len ) ;
}
#endif
#endif

#ifdef S4FOX
void t4dbl_to_fox( char *result, double doub )
{
   char i ;
   if ( doub >= 0 )
   {
      for ( i = 0 ; i < 8 ; i++ )
    result[i] = *((char *)&doub + 7 - i ) ;
      result[0] += 0x80 ;
   }
   else /* negative */
      for ( i = 0 ; i < 8 ; i++ )
    result[i] = ~*((char *)&doub + 7 - i ) ;
}

void t4str_to_fox( char *result, char *input_ptr, int input_ptr_len )
{
   t4dbl_to_fox( result, c4atod( input_ptr, input_ptr_len ) ) ;
}

void t4dtstr_to_fox( char *result, char *input_ptr, int input_ptr_len )
{
   t4dbl_to_fox( result, (double) a4long( input_ptr ) ) ;
}

/*
static void t4fox_to_dbl( char *fox_result, char *input, int dummy )
{
   double result ;
   char i ;
                    
   if ( input[0] >> 7 )  
   {
      *((char *)&result + 7) = input[0] - 0x80 ;
      for ( i = 0 ; i < 7 ; i++ )
         *((char *)&result + i) = input[7-i] ;
   }
   else 
      for ( i = 0 ; i < 8 ; i++ )
         *((char *)&result + i) = ~input[7-i] ;
   memcpy( fox_result, &result, sizeof(double) ) ;
}
*/

int S4CALL t4desc_memcmp( S4CMP_PARM data_ptr, S4CMP_PARM search_ptr, size_t len )
{
   return -1 * memcmp( data_ptr, search_ptr, len ) ;
}

static void t4no_change_str( char *a, char *b, int l)
{
   memcpy(a,b,l) ;
}

int S4FUNCTION t4rl_bottom( T4TAG *t4 )
{
   int rc ;
   B4BLOCK *block_on ;

   rc = t4lock(t4) ;  if ( rc != 0 )  return rc ;  /* Error or lock */
   if ( t4up_to_root(t4) < 0 )  return -1 ;

   b4go( t4block(t4), t4block(t4)->header.n_keys - 1 ) ;
   do
   {
      if ( (rc = t4down(t4)) < 0 )  return -1 ;
      b4go( t4block(t4), t4block(t4)->header.n_keys - 1 ) ;
   } while ( rc == 0 ) ;

   block_on =  t4block(t4) ;
   if ( block_on->key_on > 0 )
   block_on->key_on =  block_on->header.n_keys-1 ;
   
   #ifdef S4DEBUG
      if ( block_on->key_on < 0 )  e4severe( e4info, "t4bottom()", (char *) 0 ) ;
   #endif

   return 0 ;
}
   
int S4FUNCTION t4bottom( T4TAG *t4 )
{
   if ( t4->header.descending )   /* if descending, go bottom means go top */
      return t4rl_top( t4 ) ;
   else
      return t4rl_bottom( t4 ) ;
}

#else       /*  if not S4FOX  */

int S4FUNCTION t4bottom( T4TAG *t4 )
{
   int rc ;
   B4BLOCK *block_on ;

   rc = t4lock(t4) ;  if ( rc != 0 )  return rc ;  /* Error or lock */
   if ( t4up_to_root(t4) < 0 )  return -1 ;

   b4go_eof( t4block(t4) ) ;

   do
   {
      if ( (rc = t4down(t4)) < 0 )  return -1 ;
      b4go_eof( t4block(t4) ) ;
   } while ( rc == 0 ) ;

   block_on =  t4block(t4) ;
   if ( block_on->key_on > 0 )
   {
      block_on->key_on =  block_on->n_keys-1 ;
   }
   #ifdef S4DEBUG
      if ( block_on->key_on < 0 )  e4severe( e4info, "t4bottom()", (char *) 0 ) ;
   #endif

   return 0 ;
}

#endif


B4BLOCK *S4FUNCTION t4block( T4TAG *t4 )
{
   #ifdef S4DEBUG
      if ( t4->blocks.last == 0 )  e4severe( e4info, "t4block()", (char *) 0 ) ;
   #endif
   return (B4BLOCK *) t4->blocks.last ;
}

/* Returns  1 - Cannot move down; 0 - Success; -1 Error */
int S4FUNCTION t4down( T4TAG *t4 )
{
   long block_down ;
   B4BLOCK *block_on, *pop_block, *new_block ;
   I4INDEX *i4 ;

   if ( t4->code_base->error_code < 0 )  return -1 ;
   i4 =  t4->index ;

   block_on =  (B4BLOCK *) t4->blocks.last ;

   if ( block_on == 0 )    /* Read the root block */
   {
      if ( t4->header.root <= 0L )
	 if ( h4read_all( &i4->file, t4->header_offset,
	       &t4->header.root,sizeof(t4->header.root)) < 0 ) return -1 ;
      block_down =  t4->header.root ;
   }
   else
   {
      if ( b4leaf(block_on) )  return 1 ;
      block_down =  b4key(block_on,block_on->key_on)->num ;
      #ifdef S4DEBUG
	 if ( block_down <= 0L )  e4severe( e4info, "t4down()", E4_INFO_ILF, (char *) 0 ) ;
      #endif
   }

   /* Get memory for the new block */
   pop_block =  (B4BLOCK *) l4pop( &t4->saved ) ;
   new_block =  pop_block ;
   if ( new_block == 0 )
      new_block =  b4alloc( t4, block_down) ;
   if ( new_block == 0 )    return -1 ;
   l4add( &t4->blocks, new_block ) ;

   if ( pop_block == 0  ||  new_block->file_block != block_down )
   {
      if ( b4flush(new_block) < 0 )  return -1 ;

      #ifdef S4FOX
         if ( h4read_all( &i4->file, I4MULTIPLY*block_down,
            &new_block->header, B4BLOCK_SIZE) < 0 ) return -1 ;
      #else
         if ( h4read_all( &i4->file, I4MULTIPLY*block_down,
            &new_block->n_keys, i4->header.block_rw) < 0 ) return -1 ;
      #endif
      new_block->file_block =  block_down ;
      #ifdef S4FOX
         b4top( new_block ) ;  /* position to top of the block */
      #endif

      while( block_on = (B4BLOCK *) l4pop(&t4->saved) )
      {
         if ( b4flush(block_on) < 0 )  return -1 ;
         b4free( block_on ) ;
      }
   }

   #ifdef S4FOX
      new_block->key_on = -1 ;  /* reset the position so a true top() will occur */
      b4top( new_block ) ;
   #else
      new_block->key_on =  0 ;
   #endif

   return 0 ;
}

int S4FUNCTION t4eof( T4TAG *t4 )
{
   B4BLOCK *b4 ;
   b4 =  t4block(t4) ;

   #ifdef S4FOX
      return( b4->key_on >= b4->header.n_keys ) ;
   #else
      return( b4->key_on >= b4->n_keys ) ;
   #endif
}

int S4FUNCTION t4flush( T4TAG *t4 )
{
   B4BLOCK *block_on ;

   if ( t4->code_base->error_code < 0 )  return -1 ;

   for( block_on = 0; block_on = (B4BLOCK *) l4next(&t4->saved,block_on); )
      if ( b4flush(block_on) < 0 )  return -1 ;

   for( block_on = 0; block_on = (B4BLOCK *) l4next(&t4->blocks,block_on); )
      if ( b4flush(block_on) < 0 )  return -1 ;

   if ( t4->root_write )
   {
      if ( h4write( &t4->index->file, t4->header_offset, 
               &t4->header.root, sizeof(t4->header.root)) < 0 )  return -1 ;
      t4->root_write =  0 ;
   }

   return 0 ;
}

int S4FUNCTION t4free_all( T4TAG *t4 )
{
   while ( t4up(t4) == 0 ) ;
   return t4free_saved(t4) ;
}

int S4FUNCTION t4free_saved( T4TAG *t4 )
{
   B4BLOCK *block_on ;

   if ( t4flush(t4) < 0 )  return -1 ;
   while ( block_on = (B4BLOCK *) l4pop(&t4->saved) )
   {
      if ( b4flush( block_on) < 0 )  return -1 ;
      b4free( block_on ) ;
   }
   return 0 ;
}

int S4FUNCTION t4go( T4TAG *t4, char *ptr, long rec_num )
{
   #ifdef S4FOX
      B4BLOCK *block_on ;
      int rc, k_len = t4->header.key_len ;
      unsigned long rec = x4reverse( rec_num ) ;
   
      if ( t4->code_base->error_code < 0 )  return -1 ;
   
      if ( (rc = t4lock(t4)) != 0 )  return rc ;  /* Not locked or error */
   
      /* Do initial search, moving up only as far as necessary */
      if ( t4up_to_root(t4) < 0 )  return -1 ;
   
      for(;;) /* Repeat until found */
      {
         block_on =  (B4BLOCK *) t4->blocks.last ;
         #ifdef S4DEBUG
            if ( block_on == 0 )  e4severe( e4info, "t4seek()", (char *) 0 ) ;
         #endif
   
         if ( b4leaf(block_on) )
         {
	    rc = b4seek( block_on, (char *) ptr, k_len ) ;
	    break ;
         }
         else
	    rc = b4r_brseek( block_on, (char *) ptr, k_len, rec ) ;
    
         if ( t4down(t4) < 0 )  return -1 ;
      }
   
      if ( rc != 0 ) return rc ;  /* leaf seek did not end in perfect find */
   
      /* now do the seek for recno on the leaf block */
      block_on = (B4BLOCK *)t4->blocks.last ;
      for(;;)
      {
         if ( (rec = t4recno(t4)) == rec_num )  return 0 ;
         if ( rec > rec_num )  return r4found ;
   
         rc =  (int) t4skip(t4,1L) ;
         if ( rc == -1 )  return -1 ;
         if ( rc == 0 )
         {
	    b4go_eof( t4block(t4) ) ;
	    return r4found ;
         }
   
         if ( x4dup_cnt( block_on, block_on->key_on ) + x4trail_cnt( block_on, block_on->key_on )
              != t4->header.key_len )  /* case where key changed */
            return r4found ;
      }
   #endif

   #ifndef S4FOX
      int  rc ;
      long rec ;
   
      if ( t4->code_base->error_code < 0 )  return -1 ;
   
      rc = t4seek( t4, ptr, t4->header.value_len ) ;
      if ( rc != 0 )  return rc ;
   
      for(;;)
      {
         if ( (rec = t4recno(t4)) == rec_num )  return 0 ;
         if ( rec > rec_num )  return r4found ;
   
         rc =  (int) t4skip(t4,1L) ;
         if ( rc == -1 )  return -1 ;
         if ( rc == 0 )
         {
            b4go_eof( t4block(t4) ) ;
            return r4found ;
         }
   
         if ( (*t4->cmp)( t4key(t4)->value, ptr, t4->header.value_len ) != 0 )  return r4found ;
      }
   #endif
}

B4KEY_DATA *S4FUNCTION t4key( T4TAG *t4 )
{
   B4BLOCK *b4 ;
   b4 =  (B4BLOCK *) t4->blocks.last ;
   return  b4key( b4, b4->key_on ) ;
}

int S4FUNCTION t4lock( T4TAG *t4 )
{
   I4INDEX *i4 ;
   int rc ;

   if ( t4->code_base->error_code < 0 )  return -1 ;
   if ( (i4 = t4->index)->file_locked )  return 0 ;
   if ( i4->tag_locked == t4 )  return 0 ;

   if ( i4->tag_locked != 0 )
      if ( i4unlock(i4) < 0 )  return -1 ;

   rc =  h4lock( &i4->file, t4->lock_pos, 1L ) ;
   if ( rc != 0 )  return rc ; /* Error or locked */

   i4->tag_locked =  t4 ;
   if ( i4version_check(i4) < 0 )  return -1 ;
   return 0 ;
}

/* 'pos' is an percentage, positioning is approximate. */

int S4FUNCTION t4position2( T4TAG *t4, double *result )
{
   *result = t4position( t4 );
   return 0;
}

double S4FUNCTION t4position( T4TAG *t4 )
{
   double pos ;
   B4BLOCK *block_on ;
   int n ;

   pos = .5 ;
   for ( block_on =  (B4BLOCK *) t4->blocks.last; block_on != 0; )
   {
      #ifdef S4FOX
         n =  block_on->header.n_keys ;
      #else
         n =  block_on->n_keys+1 ;
         if ( b4leaf(block_on) )  n-- ;
      #endif

      pos =  (block_on->key_on+pos)/n ;

      block_on =  (B4BLOCK *) block_on->link.p ;
      if ( block_on == (B4BLOCK *) t4->blocks.last )  break ;
   }
   #ifdef S4FOX
      if ( t4->header.descending )   /* backwards in file... */
         return 1.0 - pos ;
      else
   #endif
   return pos ;
}

int S4FUNCTION t4position_set( T4TAG *t4, double pos )
{
   int rc, n, final_pos ;
   B4BLOCK *block_on ;

   #ifdef S4FOX
      if ( t4->header.descending )   /* backwards in file... */
         pos = 1.0 - pos ;
   #endif

   rc =  t4lock(t4) ;   if ( rc != 0 )  return rc ; /* Error or locked */

   if ( t4up_to_root(t4) < 0 )  return -1 ;

   for(;;)
   {
      #ifdef S4DEBUG
         if ( pos < 0.0 || pos > 1.0 )  /* Could be caused by rounding error ? */
	    e4severe( e4parm, "t4position()", E4_INFO_ILP, (char *) 0 ) ;
      #endif

      block_on =  t4block(t4) ;

      #ifdef S4FOX
         n =  block_on->header.n_keys ;
      #else
         n =  block_on->n_keys+1 ;
         if ( b4leaf(block_on) )  n-- ;
      #endif

      final_pos =  (int) (n * pos) ;
      if ( final_pos == n )  final_pos-- ;
   
      #ifdef S4FOX
         b4go( block_on, final_pos ) ;
      #else
         block_on->key_on =  final_pos ;
      #endif

      pos =  pos*n - final_pos ;

      if ( (rc = t4down(t4)) < 0 )  return -1 ;
      if ( rc == 1 )  return 0 ;
   }
}

long S4FUNCTION t4recno( T4TAG *t4 )
{
   B4BLOCK *block_on ;

   block_on =  (B4BLOCK *) t4->blocks.last ;
   if ( block_on == 0 )  return -2L ;

   #ifndef S4FOX
      if ( ! b4leaf(block_on))  return -2L ;
   #endif

   return b4recno( block_on ) ;
}

int S4FUNCTION t4seek( T4TAG *t4, void *ptr, int len_ptr )
{
   int rc ;
   B4BLOCK *block_on ;
   #ifdef S4FOX
      int inc_pos, d_set=0 ;
      unsigned char *c_ptr = (unsigned char *) ptr ;
   #endif

   if ( (rc = t4lock(t4)) != 0 )  return rc ;  /* Not locked or error */

   #ifdef S4DEBUG
      #ifdef S4FOX
         if ( len_ptr != t4->header.key_len && t4type(t4) != t4str )
            e4severe( e4parm, "t4seek()", (char *) 0 ) ;
      #else
         if ( len_ptr != t4->header.value_len && t4type(t4) != t4str )
	    e4severe( e4parm, "t4seek()", (char *) 0 ) ;
      #endif
   #endif

   #ifdef S4FOX
      if ( len_ptr > t4->header.key_len )
         len_ptr =  t4->header.key_len ;
      if ( t4->header.descending )   /* look for current item less one: */
      {
	 for( inc_pos = len_ptr-1 ; d_set == 0 && inc_pos >=0 ; inc_pos-- )
	    if ( c_ptr[inc_pos] != 0xFF )
            {
               c_ptr[inc_pos]++ ;
               d_set = 1 ;
            }
      }
   #else
      if ( len_ptr > t4->header.value_len )
	 len_ptr =  t4->header.value_len ;
   #endif

   /* Do initial search, moving up only as far as necessary */
   if ( t4up_to_root(t4) < 0 )  return -1 ;

   for(;;) /* Repeat until found */
   {
      block_on =  (B4BLOCK *) t4->blocks.last ;
      #ifdef S4DEBUG
         if ( block_on == 0 )  e4severe( e4info, "t4seek()", (char *) 0 ) ;
      #endif

      rc = b4seek( block_on, (char *) ptr, len_ptr ) ;
      if ( b4leaf(block_on) )  break ;

      if ( t4down(t4) < 0 )  return -1 ;
   }
   #ifdef S4FOX
      if ( t4->header.descending )   /* must go back one! */
      {
	 c_ptr[inc_pos+1]-- ; /* reset the search_ptr ; */
         if ( d_set )
         {
            rc = t4skip( t4, -1L ) ;
            if ( rc == 0L )  /* bof = eof condition */
               rc = r4eof ;
            else
            {
               b4go( t4block(t4), t4block(t4)->key_on ) ;
	       if ( (*t4->cmp)( b4key_key(t4block(t4),t4block(t4)->key_on), ptr, len_ptr ))
                  rc = r4after ;
               else
                  rc = 0 ;  /* successful find */
            }
         }
         else
         {
            if ( rc == 0 )  /* the item was found, so go top, */
               t4top( t4 ) ;
            else  /* otherwise want an eof type condition */
            {
               b4go_eof( block_on ) ;
               rc = r4eof ;
            }
         }
      }
   #endif
   return rc ;
}

long S4FUNCTION t4skip( T4TAG *t4, long num_skip )
{
   int rc, sign ;
   long num_left ;
   B4BLOCK *block_on ;

   rc =  t4lock(t4) ;
   if ( rc != 0 )  return -num_skip ; /* Error or locked */

   num_left =  num_skip ;

   rc = 0 ;

   block_on =  (B4BLOCK *) t4->blocks.last ;
   if ( block_on == 0 )
   {
      if ( (rc =  t4top(t4)) < 0 )  return -num_skip ;
      block_on =  (B4BLOCK *) t4->blocks.last ;
   }

   #ifdef S4DEBUG
      if ( ! b4leaf(block_on) )  e4severe( e4info, "t4skip()", (char *) 0 ) ;
   #endif

   #ifdef S4FOX 
      for(;;)
      {
         long go_to ;
         num_left -= b4skip( block_on, num_left, 0 ) ;
         if ( num_left == 0)  return( num_skip ) ;  /* Success */
   
         if ( num_left > 0 )
          go_to = block_on->header.right_node ;
         else
          go_to = block_on->header.left_node ;
   
         if ( go_to == -1 )
          break ;
   
         if ( b4flush(block_on) < 0 )  return -num_skip ;
         if ( h4read_all( &t4->index->file, I4MULTIPLY*go_to,
            &block_on->header, B4BLOCK_SIZE) < 0 ) return -num_skip ;
   
         block_on->file_block = go_to ;
         block_on->key_on = -1 ; /* force a 'top' */
         b4top( block_on ) ;  /* position to top of the block */
   
         if ( num_left < 0 )
          num_left += block_on->header.n_keys ;
         else
          num_left -= 1 ;  /* moved to the next entry */
      }
      return (num_skip - num_left) ;
   #endif
   
   #ifndef S4FOX
      if ( num_skip < 0)
         sign = -1 ;
      else
         sign = 1 ;
   
      for(;;)
      {
         while ( (rc = t4down(t4)) == 0 )
            if ( sign < 0 )
            {
               block_on =  t4block(t4) ;
               b4go_eof( block_on ) ;
               if ( b4leaf(block_on) )
               {
                  block_on->key_on-- ;
                  #ifdef S4DEBUG
   	          if ( block_on->key_on < 0 )
   	             e4severe( e4info, "t4skip()", (char *) 0 ) ;
                  #endif
               }
            }
   
         block_on =  t4block(t4) ;

         if (rc < 0 )  return( -num_skip ) ;  /* Error */
   
         num_left -=  b4skip( block_on, num_left ) ;
	 if ( num_left == 0)  return( num_skip ) ;  /* Success */

	 do  /* Skip 1 to the next leaf block  */
	 {
	    if ( (B4BLOCK *) block_on->link.p == block_on )
	    {
	       if ( num_skip > 0 )
	       {
	          if ( t4bottom(t4) < 0 )  return -num_skip ;
	       }
               else
                  if ( t4top(t4) < 0 ) return -num_skip ;
      
               return( num_skip - num_left ) ;
            }
            else
               t4up(t4) ;
   
            block_on =  (B4BLOCK *) t4->blocks.last ;
         }  while ( b4skip( block_on, (long) sign) != sign) ;
   
         num_left -= sign ;
      }
   #endif
}

B4BLOCK *S4FUNCTION t4split( T4TAG *t4, B4BLOCK *old_block )
{
   long  new_file_block ;
   B4BLOCK *new_block ;
   int tot_len, new_len ;

   if ( t4->code_base->error_code < 0 )  return 0 ;
   new_file_block =  i4extend( t4->index ) ;

   new_block =  b4alloc( t4, new_file_block ) ;
   if ( new_block == 0 )  return 0 ;

   new_block->changed =  1 ;
   old_block->changed =  1 ;

   #ifdef S4FOX
      if ( b4leaf( old_block ) )
         t4leaf_split( t4, old_block, new_block ) ;
      else
         t4branch_split( t4, old_block, new_block ) ;
   
      new_block->header.right_node = old_block->header.right_node ;
      new_block->header.left_node = old_block->file_block ;
      old_block->header.right_node = new_block->file_block ;
   
      if ( new_block->header.right_node != -1 )   /* must change left ptr for next block over */
         h4write( &t4->index->file, new_block->header.right_node + 2*sizeof(short),
                  &new_block->file_block, sizeof( new_block->header.left_node ) ) ;
   #else
      /* NNNNOOOO  N - New, O - Old */
      new_block->n_keys  =  (old_block->n_keys+1)/2 ;
      old_block->n_keys -=  new_block->n_keys ;
      new_block->key_on  =  old_block->key_on ;
   
      tot_len =  t4->index->header.block_rw - sizeof(old_block->n_keys) - sizeof(old_block->dummy) ;
      new_len =  new_block->n_keys * t4->header.group_len ;
   
      memcpy( b4key(new_block,0), b4key(old_block,0), new_len ) ;
      memmove( b4key(old_block,0), b4key(old_block,new_block->n_keys), tot_len - new_len ) ;
      old_block->key_on =  old_block->key_on - new_block->n_keys ;
   #endif

   return new_block ;
}

#ifdef S4FOX
void S4FUNCTION t4branch_split( T4TAG *t4, B4BLOCK *old_block, B4BLOCK *new_block )
{
   int new_len ;
   int g_len = t4->header.key_len + 2*sizeof( long ) ;
   char *o_pos ;

   /* NNNNOOOO  N - New, O - Old */
   new_block->header.n_keys  =  (old_block->header.n_keys+1)/2 ;
   old_block->header.n_keys -=  new_block->header.n_keys ;

   new_len =  new_block->header.n_keys * g_len ;

   o_pos = ((char *)&old_block->node_hdr) + g_len * old_block->header.n_keys ;
   memcpy( &new_block->node_hdr, o_pos, new_len ) ;
   new_block->header.node_attribute = 0 ;
   old_block->header.node_attribute = 0 ;
   new_block->key_on =  old_block->key_on - old_block->header.n_keys ;

   /* clear the old data */
   memset( o_pos, 0, new_len ) ;
}

void S4FUNCTION t4leaf_split( T4TAG *t4, B4BLOCK *old_block, B4BLOCK *new_block )
{
   char *end_pos, *obd_pos, *obi_pos ;
   unsigned char buffer[6] ;
   int len, n_keys ;
   long put_data ;
   int k_len = t4->header.key_len ;
   int i_len = old_block->node_hdr.info_len ;
   int b_len = B4BLOCK_SIZE - sizeof( old_block->header ) - sizeof( old_block->node_hdr )
               - old_block->header.n_keys * i_len
               - old_block->node_hdr.free_space ;

   b4top( old_block ) ;
   end_pos = old_block->cur_pos - b_len/2 ;
   if ( end_pos = old_block->cur_pos )   /* end and start the same, 1 byte */
   {
      n_keys = old_block->header.n_keys / 2 ;
      for ( len = 0 ; len < old_block->header.n_keys - n_keys ; len++ )
         b4skip( old_block, 1L, 1 ) ;
   }
   else
   {
      while ( old_block->cur_pos > end_pos )
         b4skip( old_block, 1L, 1 ) ;
      n_keys = old_block->header.n_keys - old_block->key_on ;  /* new # keys for new_block */
   }

   /* copy the general information */
   memcpy( &new_block->header, &old_block->header,
           sizeof( old_block->header ) + sizeof( old_block->node_hdr ) ) ;

   /* put 1st key of new block */
   new_block->cur_trail_cnt = b4calc_blanks( old_block->current->value, k_len, t4->p_char ) ;
   len = k_len - new_block->cur_trail_cnt ;
   new_block->cur_pos = ((char *)&new_block->header) + B4BLOCK_SIZE - len ;
   memcpy( new_block->cur_pos, old_block->current->value, len ) ;

   /* copy remaining key data */
   obd_pos = ((char *)&old_block->header) + B4BLOCK_SIZE - b_len ;
   len = old_block->cur_pos - obd_pos ;
   new_block->cur_pos -= len ;
   memcpy( new_block->cur_pos, obd_pos, len ) ;

   /* copy the info data */
   obi_pos =  old_block->data + old_block->key_on * i_len ;
   memcpy( new_block->data, obi_pos, n_keys * i_len ) ;

   /* clear the old data */
   obd_pos += len ;
   #ifdef S4DEBUG
      if ( obd_pos < obi_pos )
         e4severe( e4info, "t4leaf_split in i4tag.c", E4_INFO_BMC, (char *) 0 ) ;
   #endif
   memset( obi_pos, 0, obd_pos - obi_pos ) ;

   /* now reset the place new info data for the first key */
   memset( new_block->data, 0, i_len ) ;
   x4put_info( &new_block->node_hdr, buffer, old_block->current->num, new_block->cur_trail_cnt, 0 ) ;
   memcpy( new_block->data, buffer, i_len ) ;

   new_block->header.n_keys = n_keys ;
   old_block->header.n_keys -= n_keys ;
   new_block->header.node_attribute = 2 ;
   old_block->header.node_attribute = 2 ;
   b4top( old_block ) ;
   b4go_eof( old_block ) ;
   b4top( new_block ) ;
   b4go_eof( new_block ) ;
   new_block->node_hdr.free_space = new_block->cur_pos - new_block->data
                                    - new_block->header.n_keys * i_len ;
   old_block->node_hdr.free_space = old_block->cur_pos - old_block->data
                                    - old_block->header.n_keys * i_len ;
}
#endif

#ifdef S4FOX
   int S4FUNCTION t4rl_top( T4TAG *t4 )
   {
      int  rc ;
   
      rc =  t4lock(t4) ;   if ( rc != 0 )  return rc ;  /* Error or locked */
      if ( t4up_to_root(t4) < 0 )  return -1 ;
   
      ((B4BLOCK *)t4->blocks.last)->key_on =  0 ;
      do
      {
         if ( (rc = t4down(t4)) < 0 )  return -1 ;
         b4top((B4BLOCK *)t4->blocks.last) ;
      } while ( rc == 0 ) ;
   
      return 0 ;
   }
   
   int S4FUNCTION t4top( T4TAG *t4 )
   {
      if ( t4->header.descending )   /* if descending, go top means go bottom */
         return t4rl_bottom( t4 ) ;
      else
         return t4rl_top( t4 ) ;
   }
#else
   int S4FUNCTION t4top( T4TAG *t4 )
   {
      int  rc ;
   
      rc =  t4lock(t4) ;   if ( rc != 0 )  return rc ;  /* Error or locked */
      if ( t4up_to_root(t4) < 0 )  return -1 ;
   
      ((B4BLOCK *)t4->blocks.last)->key_on =  0 ;
      do
      {
         if ( (rc = t4down(t4)) < 0 )  return -1 ;
   
         ((B4BLOCK *)t4->blocks.last)->key_on =  0 ;
   
      } while ( rc == 0 ) ;
   
      return 0 ;
   }
#endif

int S4FUNCTION t4type( T4TAG *t4 )
{
 #ifdef S4FOX
   return t4->expr->type ;
 #else
   return t4->header.type ;
 #endif
}

int  S4FUNCTION t4up( T4TAG *t4 )
{
   if ( t4->blocks.last == 0 )  return 1 ;
   l4add( &t4->saved, l4pop(&t4->blocks) ) ;
   return 0 ;
}

int  S4FUNCTION t4up_to_root( T4TAG *t4 )
{
   L4LINK *link_on ;

   while( link_on = (L4LINK *) l4pop(&t4->blocks) )
      l4add( &t4->saved, link_on ) ;

   return t4down(t4) ;
}
#endif   /*  ifndef N4OTHER  */


#ifdef N4OTHER

#ifndef u4memcmp
int S4CALL u4memcmp( S4CMP_PARM p1, S4CMP_PARM p2, size_t len )
{
   return memcmp( p1, p2, len ) ;
}
#endif

B4BLOCK *S4FUNCTION t4block( T4TAG *t4 )
{
   #ifdef S4DEBUG
      if ( t4->blocks.last == 0 )  e4severe( e4info, "t4block()", (char *) 0 ) ;
   #endif
   return (B4BLOCK *) t4->blocks.last ;
}

int S4FUNCTION t4bottom( T4TAG *t4 )
{
   int rc ;
   B4BLOCK *block_on ;

   rc = t4lock(t4) ;  if ( rc != 0 )  return rc ;  /* Error or lock */
   if ( t4up_to_root(t4) < 0 )  return -1 ;

   b4go_eof( t4block(t4) ) ;

   do
   {
      if ( (rc = t4down(t4)) < 0 )  return -1 ;
      b4go_eof( t4block(t4) ) ;
   } while ( rc == 0 ) ;

   block_on =  t4block(t4) ;
   if ( block_on->key_on > 0 )
      block_on->key_on =  block_on->n_keys-1 ;

   #ifdef S4DEBUG
      if ( block_on->key_on < 0 )  e4severe( e4info, "t4bottom()", (char *) 0 ) ;
   #endif

   return 0 ;
}

/* Returns  1 - Cannot move down; 0 - Success; -1 Error */
int S4FUNCTION t4down( T4TAG *t4 )
{
   long block_down ;
   B4BLOCK *block_on, *pop_block, *new_block ;

   if ( t4->code_base->error_code < 0 )  return -1 ;

   block_on =  (B4BLOCK *) t4->blocks.last ;

   if ( block_on == 0 )    /* Read the root block */
   {
      if ( t4->header.root <= 0L )
      #ifdef S4NDX
	 if ( h4read_all( &t4->file, t4->header.header_offset,
	       &t4->header.root, sizeof(t4->header.root)) < 0 ) return -1 ;
      #else
         #ifdef S4CLIPPER
	    if ( h4read_all( &t4->file, t4->header.header_offset + 2*sizeof(short),
                  &t4->header.root, sizeof(t4->header.root)) < 0 ) return -1 ;
         #endif
      #endif
      block_down =  t4->header.root ;
   }
   else
   {
      if ( b4leaf(block_on) )  return 1 ;
      block_down =  b4key( block_on, block_on->key_on )->pointer ;
      #ifdef S4DEBUG
	 if ( block_down <= 0L )  e4severe( e4info, "t4down()", E4_INFO_ILF, (char *) 0 ) ;
      #endif
   }

   /* Get memory for the new block */
   pop_block =  (B4BLOCK *) l4pop( &t4->saved ) ;
   new_block =  pop_block ;
   if ( new_block == 0 )
      new_block =  b4alloc( t4, block_down) ;
   if ( new_block == 0 )    return -1 ;
   l4add( &t4->blocks, new_block ) ;

   #ifdef S4NDX
      if ( pop_block == 0  ||  new_block->file_block != block_down )
   #else
      if ( pop_block == 0  ||  new_block->file_block*I4MULTIPLY != block_down )
   #endif
   {
      if ( b4flush(new_block) < 0 )  return -1 ;

      #ifdef S4NDX
	 if ( h4read_all( &t4->file, I4MULTIPLY * block_down,
	       &new_block->n_keys, B4BLOCK_SIZE ) < 0 ) return -1 ;
	 new_block->file_block =  block_down ;
      #else
         #ifdef S4CLIPPER
	    if ( h4read_all( &t4->file, block_down,
	          &new_block->n_keys, B4BLOCK_SIZE ) < 0 ) return -1 ;
	    new_block->file_block =  block_down/512 ;
         #endif
      #endif

      while( block_on = (B4BLOCK *) l4pop(&t4->saved) )
      {
         if ( b4flush(block_on) < 0 )  return -1 ;
         b4free( block_on ) ;
      }
   }

   new_block->key_on =  0 ;
   return 0 ;
}

int S4FUNCTION t4eof( T4TAG *t4 )
{
   B4BLOCK *b4 ;
   b4 =  t4block(t4) ;
   return( b4->key_on >= b4->n_keys ) ;
}

int S4FUNCTION t4flush( T4TAG *t4 )
{
   B4BLOCK *block_on ;

   if ( t4->code_base->error_code < 0 )  return -1 ;

   for( block_on = 0; block_on = (B4BLOCK *) l4next(&t4->saved,block_on); )
      if ( b4flush(block_on) < 0 )  return -1 ;

   for( block_on = 0; block_on = (B4BLOCK *) l4next(&t4->blocks,block_on); )
      if ( b4flush(block_on) < 0 )  return -1 ;

   if ( t4->root_write )
   {
      #ifdef S4NDX
         if ( h4write( &t4->file, t4->header_offset, 
              &t4->header.root, sizeof(t4->header.root)) < 0 )  return -1 ;
      #else
         #ifdef S4CLIPPER
            if ( h4write( &t4->file, t4->header_offset + 2*sizeof( short ),
                 &t4->header.root, sizeof(t4->header.root)) < 0 )  return -1 ;
         #endif
      #endif
      t4->root_write =  0 ;
   }

   return 0 ;
}

int S4FUNCTION t4free_all( T4TAG *t4 )
{
   while ( t4up(t4) == 0 ) ;
   return t4free_saved(t4) ;
}

int S4FUNCTION t4free_saved( T4TAG *t4 )
{
   B4BLOCK *block_on ;

   if ( t4flush(t4) < 0 )  return -1 ;
   while ( block_on = (B4BLOCK *) l4pop(&t4->saved) )
   {
      if ( b4flush( block_on) < 0 )  return -1 ;
      b4free( block_on ) ;
   }
   return 0 ;
}


B4KEY_DATA *S4FUNCTION t4key( T4TAG *t4 )
{
   B4BLOCK *b4 ;
   b4 =  (B4BLOCK *) t4->blocks.last ;
   return  b4key( b4, b4->key_on ) ;
}

int S4FUNCTION t4lock( T4TAG *t4 )
{
   I4INDEX *i4 = t4->index ;
   int rc ;

   if ( t4->code_base->error_code < 0 )  return -1 ;
   if ( t4->index->file_locked )  return 0 ;
   if ( i4->tag_locked == t4 )  return 0 ;

   if ( i4->tag_locked != 0 )
      if ( i4unlock( i4 ) < 0 )  return -1 ;

   rc = h4lock( &t4->file, L4LOCK_POS, L4LOCK_POS ) ;
   if ( rc != 0 )  return rc ; /* Error or locked */
   i4->tag_locked = t4 ;
   if ( t4version_check( t4 ) < 0 )  return -1 ;
   t4->file_locked = 1 ;
   return 0 ;
}

int S4FUNCTION t4position2( T4TAG *t4, double *result )
{
   *result = t4position( t4 );
   return 0;
}

int S4FUNCTION t4position_set( T4TAG *t4, double pos )
{
   int rc, n, final_pos ;
   B4BLOCK *block_on ;

   rc =  t4lock(t4) ;   if ( rc != 0 )  return rc ; /* Error or locked */

   if ( t4up_to_root(t4) < 0 )  return -1 ;

   for(;;)
   {
      #ifdef S4DEBUG
	 if ( pos < 0.0 || pos > 1.0 )  /* Could be caused by rounding error ? */
	    e4severe( e4parm, "t4position()", E4_INFO_ILP, (char *) 0 ) ;
      #endif

      block_on =  t4block(t4) ;

      n =  block_on->n_keys + 1 ;
      if ( b4leaf(block_on) )  n-- ;

      final_pos =  n * pos ;
      if ( final_pos == n )  final_pos-- ;
   
      block_on->key_on = final_pos ;
      pos =  pos*n - final_pos ;

      if ( (rc = t4down(t4)) < 0 )  return -1 ;
      if ( rc == 1 )  return 0 ;
   }
}

double S4FUNCTION t4position( T4TAG *t4 )
{
   double pos ;
   B4BLOCK *block_on ;
   int n ;
   #ifdef S4CLIPPER
      int first = 1 ;
   #endif

   pos = .5 ;

   for ( block_on =  (B4BLOCK *) t4->blocks.last; block_on != 0; )
   {
      int n =  block_on->n_keys + 1 ;
      #ifdef S4CLIPPER
	 if ( first )
	  {
	     n-- ;
	     first = 0 ;
	  }
      #else
         if ( b4leaf(block_on) )  n-- ;
      #endif

      pos =  (block_on->key_on+pos)/n ;
      block_on =  (B4BLOCK *) block_on->link.p ;
      if ( block_on == (B4BLOCK *) t4->blocks.last )  break ;
   }
   return pos ;
}

long S4FUNCTION t4recno( T4TAG *t4 )
{
   B4BLOCK *block_on ;

   block_on =  (B4BLOCK *) t4->blocks.last ;
   if ( block_on == 0 )  return -2L ;
   #ifdef S4NDX
      if ( ! b4leaf(block_on))  return -2L ;
   #endif

   return b4recno( block_on ) ;
}

int S4FUNCTION t4seek( T4TAG *t4, void *ptr, int len_ptr )
{
   int rc ;
   B4BLOCK *block_on ;

   #ifdef S4CLIPPER
      int upper_fnd = 0 ;
      int upper_aft = 0 ;
   #endif

   if ( (rc = t4lock(t4)) != 0 )  return rc ;  /* Not locked or error */

   #ifdef S4DEBUG
	 if ( len_ptr != t4->header.key_len && t4type(t4) != t4str )
	    e4severe( e4parm, "t4seek()", (char *) 0 ) ;
   #endif

   /* Do initial search, moving up only as far as necessary */
   if ( t4up_to_root(t4) < 0 )  return -1 ;


   for(;;) /* Repeat until found */
   {
      block_on =  (B4BLOCK *) t4->blocks.last ;
      #ifdef S4DEBUG
         if ( block_on == 0 )  e4severe( e4info, "t4seek()", (char *) 0 ) ;
      #endif

      rc = b4seek( block_on, (char *) ptr, len_ptr ) ;
      #ifdef S4NDX
         if ( b4leaf( block_on ) )  return rc ;
      #else
         #ifdef S4CLIPPER
            if ( b4leaf( block_on ) )
	    {
	       if ( rc == r4after && upper_aft && block_on->key_on >= block_on->n_keys )
	           while( upper_aft-- > 1 )
		      t4up( t4 ) ;
	       if ( rc == 0 || !upper_fnd ) return rc ;
   
	       while( upper_fnd-- > 1 )
	           t4up( t4 ) ;
	       return 0 ;
	    }
            if ( rc == 0 )
	     {
	        upper_fnd = 1 ;
	        upper_aft = 0 ;
	     }
	     else if ( rc == r4after && !upper_fnd && !( block_on->key_on >= block_on->n_keys ) )
	        upper_aft = 1 ;
         #endif
      #endif

      if ( t4down( t4 ) < 0 )
         return -1 ;

      #ifdef S4CLIPPER
         if ( upper_fnd ) upper_fnd++ ;
         if ( upper_aft ) upper_aft++ ;
      #endif
   }
}

long S4FUNCTION t4skip( T4TAG *t4, long num_skip )
{
   int rc, sign ;
   B4BLOCK *block_on ;

   #ifdef S4NDX
      long num_left = num_skip ;
   #endif
   #ifdef S4CLIPPER
      long j ;
   #endif

   rc =  t4lock(t4) ;  if ( rc != 0 )  return rc ; /* Error or locked */

   if ( num_skip < 0)
      sign = -1 ;
   else
      sign = 1 ;

   block_on = t4block( t4 ) ;
   if ( block_on == 0 )
   {
      if ( (rc =  t4top(t4)) < 0 )  return -num_skip ;
      block_on =  (B4BLOCK *) t4->blocks.last ;
   }

   #ifdef S4NDX
      #ifdef S4DEBUG
	 if ( ! b4leaf(block_on) )  e4severe( e4info, "t4skip()", (char *) 0 ) ;
      #endif

      for(;;)
      {
	 while ( (rc = t4down(t4)) == 0 )
            if ( sign < 0 )
            {
               block_on =  t4block(t4) ;
               b4go_eof( block_on ) ;
               if ( b4leaf(block_on) )
	       {
                  block_on->key_on-- ;
                  #ifdef S4DEBUG
		     if ( block_on->key_on < 0 )
   	                e4severe( e4info, "t4skip()", (char *) 0 ) ;
		  #endif
	       }
            }
         block_on =  t4block(t4) ;

	 if (rc < 0 )  return( -num_skip ) ;  /* Error */

         num_left -=  b4skip( block_on, num_left ) ;
         if ( num_left == 0)  return( num_skip ) ;  /* Success */

         do  /* Skip 1 to the next leaf block  */
         {
            #ifdef S4DEBUG
               if ( t4->blocks.last == 0 )
                  e4severe( e4result, "t4skip()", (char *) 0 ) ;
            #endif
            if ( l4prev( &t4->blocks, t4->blocks.last ) == 0 )  /* root block */
            {
               if ( num_skip > 0 )
               {
		  if ( t4bottom( t4 ) < 0 )  return -num_skip ;
               }
               else
		  if ( t4top( t4 ) < 0 ) return -num_skip ;

               return( num_skip - num_left ) ;
            }
	    t4up( t4 ) ;
	    block_on =  (B4BLOCK *) t4->blocks.last ;
	 }  while ( b4skip( block_on, (long) sign) != sign) ;
	 num_left -= sign ;
      }
   #else
      #ifdef S4CLIPPER
         for( j = num_skip; j != 0; j -= sign )  /* skip 1 * num_skip */
         {
             if ( b4leaf(block_on) )
	     {
	        if ( b4skip( block_on, (long) sign) != sign )  /* go up */
	        {
		   int go_on = 1 ;
	           while ( go_on )
		   {
		      if ( l4prev( &t4->blocks, t4->blocks.last ) == 0 )  /* root block */
                      {
                         if ( num_skip > 0 )
                         {
                            if ( t4bottom( t4 ) < 0 )  return -num_skip ;
                         }
                         else
                            if ( t4top( t4 ) < 0 ) return -num_skip ;
   
                         return ( num_skip - j ) ;
                      }
   
		      rc = t4up( t4 ) ;
                      block_on = t4block( t4 ) ;
		      if ( rc != 0 ) return -1 ;
   
		      if ( sign > 0 )  /* forward skipping */
		      {
		         if ( !( block_on->key_on >= block_on->n_keys ) )
		            go_on = 0 ;
		      }
		      else   /* backward skipping */
		      {
		         if ( ! (block_on->key_on == 0) )
		         {
		            b4skip( block_on, -1L ) ;
		            go_on = 0 ;
		         }
		      }
		   }
	        }
	     }
	     else
	     {
               if ( sign > 0 )
		   b4skip( block_on, 1L ) ;
	        while ( (rc = t4down( t4 ) ) == 0 )
	        {
                   if ( sign < 0 )
	           {
		      block_on =  t4block( t4 ) ;
		      b4go_eof( block_on ) ;
		      if ( b4leaf( block_on ) )
		         block_on->key_on-- ;
	           }
	        }
	        block_on =  t4block( t4 ) ;
	     }
         }
         return num_skip ;
      #endif
   #endif
}

#ifdef S4NDX
B4BLOCK *S4FUNCTION t4split( T4TAG *t4, B4BLOCK *old_block )
{
   long  new_file_block ;
   B4BLOCK *new_block ;
   int tot_len, new_len ;

   if ( t4->code_base->error_code < 0 )  return 0 ;
   new_file_block = t4extend( t4 ) ;

   new_block =  b4alloc( t4, new_file_block ) ;
   if ( new_block == 0 )  return 0 ;

   new_block->changed =  1 ;
   old_block->changed =  1 ;

   /* NNNNOOOO  N - New, O - Old */
   new_block->n_keys = ( old_block->n_keys )/2 ;
   old_block->n_keys -=  new_block->n_keys ;

   new_len =  new_block->n_keys * t4->header.group_len ;

   memmove(  b4key(new_block,0),  b4key(old_block, old_block->n_keys ),
      new_len+ (sizeof(long)*(!(b4leaf( old_block )) ) ) ) ;
   new_block->key_on = old_block->key_on - old_block->n_keys ;

   old_block->n_keys -= !( b4leaf( old_block ) ) ;

   return new_block ;
} 
#else
#ifdef S4CLIPPER
/* NTX only needs to do a copy and adjust the index pointers */
B4BLOCK *S4FUNCTION t4split( T4TAG *t4, B4BLOCK *old_block )
{
   long  new_file_block ;
   B4BLOCK *new_block ;
   int tot_len, new_len, is_branch ;

   if ( t4->code_base->error_code < 0 )  return 0 ;
   new_file_block = t4extend( t4 ) ;

   new_block =  b4alloc( t4, new_file_block ) ;
   if ( new_block == 0 )  return 0 ;

   new_block->changed =  1 ;
   old_block->changed =  1 ;

   memcpy( new_block->data, old_block->data, B4BLOCK_SIZE - ( t4->header.keys_max + 2 ) * sizeof(short) ) ;

   new_block->n_keys = ( old_block->n_keys )/2 ;
   old_block->n_keys -=  new_block->n_keys ;

   is_branch = !b4leaf( old_block ) ;

   memcpy( new_block->pointers, &old_block->pointers[old_block->n_keys + is_branch],
           new_block->n_keys * sizeof(short) ) ;
   memcpy( &new_block->pointers[new_block->n_keys], old_block->pointers, 
	   (old_block->n_keys + is_branch) * sizeof(short) ) ;
   new_block->key_on = old_block->key_on - old_block->n_keys - is_branch ;

   new_block->n_keys -= is_branch ;

   return new_block ;
}
#endif
#endif

int S4FUNCTION t4top( T4TAG *t4 )
{
   int  rc ;

   rc =  t4lock(t4) ;   if ( rc != 0 )  return rc ;  /* Error or locked */
   if ( t4up_to_root(t4) < 0 )  return -1 ;

   ((B4BLOCK *)t4->blocks.last)->key_on =  0 ;
   do
   {
      if ( (rc = t4down(t4)) < 0 )  return -1 ;
      ((B4BLOCK *)t4->blocks.last)->key_on =  0 ;
   } while ( rc == 0 ) ;

   return 0 ;
}

int S4FUNCTION t4type( T4TAG *t4 )
{
   return t4->header.type ;
}

int  S4FUNCTION t4up( T4TAG *t4 )
{
   if ( t4->blocks.last == 0 )  return 1 ;
   l4add( &t4->saved, l4pop(&t4->blocks) ) ;
   return 0 ;
}

int  S4FUNCTION t4up_to_root( T4TAG *t4 )
{
   L4LINK *link_on ;

   while( link_on = (L4LINK *) l4pop(&t4->blocks) )
      l4add( &t4->saved, link_on ) ;

   return t4down(t4) ;
}

int S4FUNCTION t4close( T4TAG *t4 )
{
   int final_rc = e4error_set( t4->code_base, 0 ) ;
   int save_wait =  t4->code_base->wait, to_remove ;
   D4DATA *d4 = t4->index->data ;

   t4->code_base->wait =  1 ;

   if ( d4 != 0 )
      if ( d4flush_all( d4 ) < 0 )
	 final_rc = e4error_set( t4->code_base, 0 ) ;

   if ( i4unlock( t4->index ) < 0 )
      final_rc = e4error_set( t4->code_base, 0 ) ;

   t4free_all( t4 ) ;

   if ( h4open_test( &t4->file ) )
      if ( h4close( &t4->file ) < 0 )
	 final_rc = e4error_set( t4->code_base, 0 ) ;

   t4->code_base->wait =  save_wait ;
   e4error_set( t4->code_base, final_rc ) ;

   return  final_rc ;
}

long S4FUNCTION t4extend( T4TAG *t4 )
{
   long old_eof ;
   unsigned len ;
   C4CODE *c4 = t4->index->code_base ;

   if ( c4->error_code < 0 )  return -1 ;

   #ifdef S4DEBUG
      if ( t4->header.version == t4->header.old_version )
	 e4severe( e4info, "t4extend()", (char *) 0 ) ;
   #endif

   old_eof = t4->header.eof ;

   #ifdef S4NDX
      #ifdef S4DEBUG
        if (old_eof <= 0L )  e4severe( e4info, "t4extend()", (char *) 0 ) ;
      #endif

      t4->header.eof += B4BLOCK_SIZE / I4MULTIPLY ;
      return old_eof ;
   #else
      #ifdef S4CLIPPER
         if ( old_eof != 0 )   /* case where free-list exists */
            t4->header.eof = 0L ;
         else
         {
	    old_eof = h4length( &t4->file ) ;
	    h4length_set( &t4->file, h4length( &t4->file ) + 1024 ) ; /* and extend the file */
         }
         return old_eof/512 ;
      #endif
   #endif
}

int S4FUNCTION t4go( T4TAG *t4, char *ptr, long rec_num )
{
   int  rc ;
   long rec ;
   
   if ( t4->code_base->error_code < 0 )  return -1 ;

   rc = t4seek( t4, ptr, t4->header.key_len ) ;
   if ( rc != 0 )  return rc ;

   for(;;)
   {
      if ( (rec = t4recno(t4)) == rec_num )  return 0 ;
      if ( rec > rec_num )  return r4found ;

      rc =  (int) t4skip(t4,1L) ;
      if ( rc == -1 )  return -1 ;
      if ( rc == 0 )
      {
         b4go_eof( t4block(t4) ) ;
         return r4found ;
      }

	 if ( (*t4->cmp)( t4key(t4)->value, ptr, t4->header.key_len ) != 0 )  return r4found ;
   }
}

T4TAG *S4FUNCTION t4open( D4DATA *d4, I4INDEX *i4ndx, char *file_name )
{
   C4CODE  *c4 ;
   I4INDEX *i4 ;
   char   buf[258], buffer[1024], expr_buf[221], *ptr ;
   T4TAG *t4 ;
   int rc, len ;
   H4SEQ_READ seq_read ;

   #ifdef S4DEBUG
      if ( d4 == 0 || file_name == 0)   e4severe( e4parm, "i4open()", (char *) 0 ) ;
   #endif

   c4  =  d4->code_base ;

   if ( i4ndx == 0 )   /* must create an index for the tag */
   {
      if ( c4->index_memory == 0 )
	 c4->index_memory =  y4memory_type( c4->mem_start_index, sizeof(I4INDEX),
					    c4->mem_expand_index, 0 ) ;
      i4 =  (I4INDEX *) y4alloc( c4->index_memory ) ;
      if ( i4 == 0 )
      {
         e4error( c4, e4memory, (char *) 0 ) ;
         return 0 ;
      }
      i4->data =  d4 ;
      i4->code_base =  c4 =  d4->code_base ;
      u4name_piece( i4->alias, sizeof(i4->alias), file_name, 0, 0 ) ;
      l4add( &d4->indexes, i4 ) ;
   }
   else
      i4 = i4ndx ;

   if ( i4->block_memory == 0 )
      i4->block_memory = y4memory_type( c4->mem_start_block,
	      sizeof(B4BLOCK) + B4BLOCK_SIZE -
	      sizeof(B4KEY_DATA) - sizeof(short) - sizeof(char[2]),
	      c4->mem_expand_block, 0 ) ;

   if ( i4->block_memory == 0 )
   {
      e4error( c4, e4memory, (char *) 0 ) ;
      i4close(i4) ;
      return 0 ;
   }

   if ( c4->tag_memory == 0 )
   {
      c4->tag_memory =  y4memory_type( c4->mem_start_tag, sizeof(T4TAG),
				    c4->mem_expand_tag, 0 ) ;
      if ( c4->tag_memory == 0 )
      {
	    e4error( c4, e4memory, (char *) 0 ) ;
	    return 0 ;
      }
   }

   t4 =  (T4TAG *) y4alloc( c4->tag_memory ) ;
   if ( t4 == 0 )
   {
      e4error( c4, e4memory, (char *) 0 ) ;
      return 0 ;
   }

   u4ncpy( buf, file_name, sizeof(buf) ) ;
   c4upper(buf) ;

   #ifdef S4NDX
      u4name_ext( buf, sizeof(buf), "NDX", 0 ) ;
   #else
      #ifdef S4CLIPPER
	 u4name_ext( buf, sizeof(buf), "NTX", 0 ) ;
      #endif
   #endif

   rc =  h4open( &t4->file, c4, buf, 1 ) ;  if ( rc != 0 )  return 0 ;
   l4add( &i4->tags, t4 ) ;   /* add the tag to its index list */

   h4seq_read_init( &seq_read, &t4->file, 0, buffer, 1024 ) ;
   #ifdef S4NDX
      if ( h4seq_read_all( &seq_read, &t4->header.root, sizeof(I4IND_HEAD_WRITE) ) < 0 ) return 0 ;
   #else
      #ifdef S4CLIPPER
	 if ( h4seq_read_all( &seq_read, &t4->header.sign, sizeof(I4IND_HEAD_WRITE) ) < 0 ) return 0 ;
      #endif
   #endif
   t4->header.header_offset = 0 ;
   t4->header.type = 0 ;

   /* Perform some checks */
   if ( t4->header.key_len   > I4MAX_KEY_SIZE    ||
	t4->header.key_len  <= 0  ||
      #ifdef S4CLIPPER
	 t4->header.keys_max  != 2* t4->header.keys_half  ||
	 t4->header.keys_half <= 0               ||
	 t4->header.group_len != t4->header.key_len+ 8  ||
	(t4->header.sign != 0x6        &&  t4->header.sign != 0x106) )
      #else
	t4->header.key_len+8 > t4->header.group_len  ||
	t4->header.keys_max  < 4  ||
	t4->header.keys_max  > 50  ||
	t4->header.eof <= 0L )
      #endif
      {
	 e4error( c4, e4index, buf, (char *) 0 ) ;
      return 0 ;
   }

   t4->lock_pos = L4LOCK_POS ;
   t4->unique_error =  c4->default_unique_error ;
   t4->cmp =  u4memcmp ;
   t4->header.root =  -1 ;

   u4name_piece( t4->alias, sizeof(t4->alias), file_name, 0, 0 ) ;

   h4seq_read_all( &seq_read, expr_buf, sizeof(expr_buf)-1 ) ;
   c4trim_n( expr_buf, sizeof(expr_buf) ) ;
   t4->expr =  e4parse( i4->data, expr_buf ) ;

   len =  e4key( t4->expr, &ptr ) ;
   if ( len < 0 )
   {
      e4error( c4, e4info, "t4open()", E4_INFO_KEY, (char *) 0 ) ;
      return 0 ;
   }

   t4->header.type = (char) e4type(t4->expr) ;

   if ( t4->header.key_len !=  (short) len )
   {
      e4error( c4, e4index, i4->file.name, (char *) 0 ) ;
      return 0 ;
   }

   t4init_seek_conv(t4,t4->header.type) ;

   if ( i4->block_memory == 0 )
      i4->block_memory = y4memory_type( c4->mem_start_block,
	 sizeof(B4BLOCK) + B4BLOCK_SIZE -
	 sizeof(B4KEY_DATA) - sizeof(short) - sizeof(char[2]),
         c4->mem_expand_block, 0 ) ;

   if ( i4->block_memory == 0 )
   {
      t4close(t4) ;
      return 0 ;
   }
   t4->index = i4 ;
   t4->code_base = c4 ;
   return t4 ;
}

#ifdef S4CLIPPER
int S4FUNCTION t4shrink( T4TAG *t4, long block_no )
{
   t4->header.eof =  block_no*512 ;
   return 0 ;
}
#endif

int S4FUNCTION t4update_header( T4TAG *t4 )
{
   if ( t4->index->code_base->error_code < 0 )  return -1 ;

   if ( t4->header.old_version != t4->header.version )
   {
	 if (h4write( &t4->file,0L, (char *)&t4->header, sizeof(t4->header)) < 0) return -1;
      #ifdef S4NDX
	 if ( h4write(&t4->file, 0L, &t4->header.root, sizeof(I4IND_HEAD_WRITE)) < 0) return -1 ;
	 if ( h4write(&t4->file, sizeof(I4IND_HEAD_WRITE) + I4MAX_EXPR_SIZE,
	      &t4->header.version, sizeof(t4->header.version)) < 0) return -1 ;
      #else
         #ifdef S4CLIPPER
	    if ( h4write(&t4->file, 0L, &t4->header.sign, sizeof(I4IND_HEAD_WRITE)) < 0) return -1 ;
	    if ( h4write(&t4->file, sizeof(I4IND_HEAD_WRITE) + I4MAX_EXPR_SIZE,
		 &t4->header.unique, sizeof(t4->header.unique)) < 0) return -1 ;
	 #endif
      #endif
      t4->header.old_version = t4->header.version ;
   }
   return 0;
}

int S4FUNCTION t4version_check( T4TAG *t4 )
{
   #ifdef S4NDX
      if ( h4read_all( &t4->file,  sizeof(I4IND_HEAD_WRITE) + I4MAX_EXPR_SIZE,
	   &t4->header.version, sizeof(t4->header.version )) < 0 ) return -1 ;
   #else
      if ( h4read_all( &t4->file, 0L, &t4->header.sign , sizeof(I4IND_HEAD_WRITE)) < 0 ) return -1 ;

   #endif

   if ( t4->header.version == t4->header.old_version )
      return 0 ;

   t4->header.old_version = t4->header.version ;

   if ( t4free_all( t4 ) < 0 )  /* Should be a memory operation only */
      e4severe( e4result, "t4version_check()", (char *) 0 ) ;

  return 0;
}

#endif   /*  ifndef N4OTHER  */
