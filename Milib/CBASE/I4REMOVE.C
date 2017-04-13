/* i4remove.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifndef N4OTHER
                       /* Remove the current key */
int S4FUNCTION t4remove_current( T4TAG *t4 ) 
{
   void *new_key_info ;
   B4BLOCK *block_on, *block_iterate ;
   I4INDEX *i4 ;
   int  less_than_last ;
   #ifdef S4FOX
      long rec ;
      int z_len ;
   #else
      long shrink_br = 0 ;
      B4BLOCK *shrink_block = 0 ;
   #endif

   if ( t4->code_base->error_code < 0 )  return -1 ;
   i4 =  t4->index ;

   new_key_info =  0 ;

   #ifdef S4FOX
      i4->tag_index->header.version =  i4->version_old+1 ;

      for ( block_on = (B4BLOCK *) t4->blocks.last; block_on != 0; )
      {
         int bl_removed = 0 ;
         if ( new_key_info == 0 )  /* then delete entry */
         {
            if ( b4lastpos( block_on ) == 0 )
            {
               if ( block_on == (B4BLOCK *) l4first( &t4->blocks ) )
               {
                  /* Root Block, do not delete */
                  z_len = block_on->tag->header.key_len + sizeof(long) ;
                  #ifdef S4DEBUG
                     if ( block_on->header.left_node != -1 ||
                          block_on->header.right_node != -1 ||
                          block_on->header.n_keys != 1 )
                        e4severe( e4info, "t4remove_current()", E4_INFO_CIB, (char *) 0 ) ;
                  #endif
	          memset( &block_on->data, 0, B4BLOCK_SIZE - sizeof(B4STD_HEADER) - sizeof(B4NODE_HEADER) ) ;
                  if ( !b4leaf( block_on ) )   /* if not a leaf, then reset node_hdr too */
                  {
                     memset( &block_on->node_hdr, 0, sizeof(B4NODE_HEADER) ) ;
                     b4leaf_init( block_on ) ;  
                  }
	          memset( block_on->current, 0, z_len ) ;
	          memset( block_on->saved, 0, z_len ) ;
                  block_on->header.n_keys = 0 ;
                  block_on->header.left_node = -1 ;
                  block_on->header.right_node = -1 ;
	          block_on->key_on =  -1 ;
	          block_on->built_on =  -1 ;
                  block_on->header.node_attribute = 3 ;  /* root and leaf */ 
                  block_on->changed = 1 ;
	       }
	       else /* This block is to be deleted */
	       {
                  long l_node = block_on->header.left_node ;
                  long r_node = block_on->header.right_node ;
	          l4remove( &t4->blocks, block_on ) ;
	          if ( i4shrink( i4, block_on->file_block) < 0 )  return -1 ;
                  block_on->changed = 0 ;

                  if ( l_node != -1L )
                  {
                     if ( h4read_all( &i4->file, I4MULTIPLY*l_node,
                        &block_on->header, B4BLOCK_SIZE) < 0 ) return -1 ;
		     block_on->file_block =  l_node ;
                     block_on->header.right_node = r_node ;
                     block_on->changed = 1 ;
                     b4flush( block_on ) ;
                  }

                  if ( r_node != -1L )
                  {
                     if ( h4read_all( &i4->file, I4MULTIPLY*r_node,
                        &block_on->header, B4BLOCK_SIZE) < 0 ) return -1 ;
		     block_on->file_block =  r_node ;
                     block_on->header.left_node = l_node ;
                     block_on->changed = 1 ;
                     b4flush( block_on ) ;
                  }

	          b4free( block_on ) ;
	          block_on =  (B4BLOCK *) t4->blocks.last ;
                  bl_removed = 1 ;
	       }
	    }
	    else
	    {
	       less_than_last =  0 ;
	       if ( block_on->key_on < b4lastpos(block_on) )
	          less_than_last =  1 ;
	       b4remove( block_on ) ;
	       if ( less_than_last )  return 0 ;

	       /* On last entry */
               b4go( block_on, b4lastpos(block_on) ) ;
	       new_key_info =  b4key_key( block_on, block_on->key_on ) ;
	       rec = b4recno( block_on ) ;
	    }
         }
         else  /* Adjust entry */
         {
	    b4br_replace( block_on, (char *)new_key_info, rec ) ;
	    if ( block_on->key_on != b4lastpos( block_on ) )  /* not on end key, so exit, else continue */
               return 0 ;
         }

         if ( !bl_removed )
         {
            block_on =  (B4BLOCK *) block_on->link.p ;
            if ( block_on ==  (B4BLOCK *) t4->blocks.last )  break ;
         }
      }
   #endif

   #ifndef S4FOX
      i4->header.version =  i4->version_old+1 ;

      for ( block_iterate =  (B4BLOCK *) l4last( &t4->blocks );; )
      {
         block_on =  block_iterate ;
         /* Calculate the previous block while the current block exists. */
         block_iterate =  (B4BLOCK *) l4prev( &t4->blocks, block_iterate ) ;
         if ( block_on == 0 )  break ;

         if ( new_key_info == 0 )  /* then delete entry */
         {
            if ( b4lastpos( block_on ) == 0 )
            {
	       if ( block_on == (B4BLOCK *) l4first( &t4->blocks ) )
	       {
	          /* Root B4BLOCK, do not delete */
                  block_on->changed =  1 ;
	          block_on->key_on =  0 ;
	          memset( &block_on->n_keys, 0, i4->header.block_rw ) ;

                  /* if a filter exists, must modify the setting (set to 0 ) */
                  if (t4->filter )
                  {
                     h4write(&t4->index->file, t4->header_offset+sizeof(t4->header)+222,
                          (char *) "\0", (int) 1 ) ;
                     t4->has_keys = 0 ;
                  }
               }
               else
	       {
                  /* This block is to be deleted */
	          l4remove( &t4->blocks, block_on ) ;
	          if ( i4shrink( i4, block_on->file_block) < 0 )  return -1 ;
	          b4free( block_on ) ;
               }
            }
            else
            {
               if ( shrink_br )
               {
                  block_on->changed = 1 ;
                  b4key( block_on, block_on->key_on )->num = shrink_br ;
                  shrink_block->changed = 0 ;
	          l4remove( &t4->blocks, shrink_block ) ;
	          if ( i4shrink( i4, shrink_block->file_block) < 0 )  return -1 ;
	          b4free( shrink_block ) ;
                  return 0 ;
               }

               less_than_last =  0 ;

	       if ( block_on->key_on < b4lastpos(block_on) )
                  less_than_last =  1 ;

               b4remove( block_on ) ;

               if (!b4leaf(block_on) && b4lastpos(block_on) == 0 )
               {
                  if (block_on == (B4BLOCK *) l4first( &t4->blocks ) )
                  {
                                   /* first update the tags root */
                     t4->header.root = b4key( block_on, (int) 0 )->num ;
                     h4write( &t4->index->file, t4->header_offset,
                         (void S4PTR *) t4->header.root, sizeof(t4->header.root) ) ;

                                  /* then delete the existing root */
                     b4remove( block_on ) ;
	             if ( i4shrink( t4->index, block_on->file_block) < 0 )
                         return -1 ;
                     block_on->changed = 0 ;   /* make sure changed flag is reset */
	             b4free( block_on ) ;      /* since block is now empty  */
                     return 0 ;
                  }

                  shrink_br = b4key( block_on, (int) 0 )->num ;
                  shrink_block = block_on ;
               }
               else
               {
	          if ( less_than_last )  return 0 ;

                  /* On last entry */
                  block_on->key_on =  b4lastpos(block_on) ;
                  new_key_info =  b4key_key( block_on, block_on->key_on ) ;
               }
            }
         }
         else  /* Adjust entry */
         {
	    if ( block_on->key_on < b4lastpos(block_on) )
	    {
               /* Just change the entry */
	       block_on->changed = 1 ;
	       memcpy( b4key_key( block_on, block_on->key_on), new_key_info,
		       t4->header.value_len ) ;
	       return 0 ;
            }
         }
      }
   #endif

   return 0 ;
}

int S4FUNCTION t4remove( T4TAG *t4, char *ptr, long rec )
{
   int rc ; 

   rc =  t4go( t4, ptr, rec ) ;  if ( rc < 0 )  return rc ;
   if ( rc != 0 )  return r4entry ;

   return t4remove_current( t4 ) ;
}

int S4FUNCTION t4remove_calc( T4TAG *t4, long rec )
{
   char *ptr ;
   e4key( t4->expr, &ptr ) ;
   return t4remove( t4, ptr, rec ) ;
}

#endif   /* ifndef N4OTHER  */


#ifdef N4OTHER

#ifdef S4NDX
                                /* Remove the current key */
int S4FUNCTION t4remove_current( T4TAG *t4 ) 
{
   void *new_key_info ;
   B4BLOCK *block_on, *block_iterate ;
   int  less_than_last ;

   if ( t4->code_base->error_code < 0 )  return -1 ;

   new_key_info =  0 ;

   t4->header.version =  t4->header.old_version+1 ;

   for ( block_iterate =  (B4BLOCK *) l4last( &t4->blocks );; )
   {
      block_on =  block_iterate ;
      /* Calculate the previous block while the current block exists. */
      block_iterate =  (B4BLOCK *) l4prev( &t4->blocks, block_iterate ) ;
      if ( block_on == 0 )  break ;

      if ( new_key_info == 0 )  /* then delete entry */
      {
         if ( b4lastpos( block_on ) == 0 )
         {
            block_on->key_on =  0 ;
            memset( &block_on->n_keys, 0, B4BLOCK_SIZE ) ;
            block_on->changed = 1 ;
	    if ( block_on == (B4BLOCK *) l4first( &t4->blocks ) )
	    {
	       t4->header.root = 1L ;
	       t4->header.eof = 2L ;
            }
            else
	    {
               l4remove( &t4->blocks, block_on ) ;
               if ( b4flush( block_on ) < 0 )  return -1 ;
               b4free( block_on ) ;
            }
         }
         else
         {
            int  less_than_last =  0 ;
            if ( block_on->key_on < b4lastpos( block_on ) )  
               less_than_last =  1 ;
            b4remove( block_on ) ;
	    if ( less_than_last )  return 0 ;

            block_on->key_on = b4lastpos( block_on ) ;
            new_key_info =  b4key_key( block_on, block_on->key_on ) ;
         }
      }
      else
      {
         if ( block_on->key_on < b4lastpos( block_on ) )
         {
            block_on->changed = 1 ;
	    memcpy( b4key_key( block_on, block_on->key_on ), new_key_info, t4->header.key_len ) ;
            return 0 ;
         }
      }
   }
   return 0 ;
}
#else
#ifdef S4CLIPPER
int S4FUNCTION t4get_replace_entry( T4TAG *t4, B4KEY_DATA *insert_spot, B4BLOCK *block_on )
{
   int num_down = 0 ;
   if ( b4leaf( block_on ) ) return 0 ;

   block_on->key_on = block_on->key_on + 1 ;
   while ( !b4leaf( block_on ) )
   {
      if ( t4down( t4 ) != 0 ) return -1 ;
      block_on = (B4BLOCK *) t4->blocks.last ;
      num_down++ ;
   }
   memcpy( &insert_spot->num, &(b4key( block_on, block_on->key_on )->num), sizeof(long) + t4->header.key_len ) ;
   b4remove( block_on ) ;
   return 1 ;
}

void S4FUNCTION t4remove_ref( T4TAG *t4 )
{
   B4KEY_DATA *my_key_data = (B4KEY_DATA *)u4alloc( t4->header.group_len ) ;
   B4BLOCK *block_on, *del_block, *block_up ;
   long reference ;
   int i ;

   block_on = (B4BLOCK *) t4->blocks.last ;

   #ifdef DEBUG
      if ( block_on->n_keys != 0 )
      e4severe( e4info, "t4remove_ref()", E4_INFO_DEL, (char *) 0 ) ;
   #endif

   memcpy( &(block_on->n_keys), &t4->header.eof, sizeof(t4->header.eof) ) ;
   t4up( t4 ) ;
   block_up = (B4BLOCK *) t4->blocks.last ;

   if ( block_up == 0 )  /* root block only, so reference doesn't exist */
   {
      /* reset the block */
      short offset = ( t4->header.keys_max + 2 +
		     ( (t4->header.keys_max/2)*2 != t4->header.keys_max ) ) * sizeof(short) ;
      for ( i = 0 ; i <= t4->header.keys_max ; i++ )
         block_on->pointers[i] = (short)( t4->header.group_len * i ) + offset ;
      return ;
   }
   else  /* delete the block */
   {
      t4shrink( t4, block_on->file_block ) ;
      del_block = (B4BLOCK *)l4pop(&t4->saved ) ;
      del_block->changed = 0 ;
      b4free( del_block ) ;
   }
   block_on = block_up ;
   if ( block_on->key_on >= block_on->n_keys )
   {
      memcpy( &(my_key_data->num), &( b4key( block_on, block_on->key_on - 1 )->num), sizeof(long) + t4->header.key_len ) ;
   }
   else
      memcpy( &(my_key_data->num), &( b4key( block_on, block_on->key_on )->num), sizeof(long) + t4->header.key_len ) ;

   if ( block_on->n_keys == 1 )
   {
      /* take the branch, and put in place of me, then delete and add me */
      if ( block_on->key_on >= block_on->n_keys )
	  reference = (long) b4key( block_on, 0 )->pointer ;
      else
	  reference = (long) b4key( block_on, 1 )->pointer ;
      memcpy( &(block_on->n_keys), &t4->header.eof, sizeof(t4->header.eof) ) ;
      t4shrink( t4, block_on->file_block ) ;
      t4up( t4 ) ;
      del_block = (B4BLOCK *)l4pop(&t4->saved ) ;
      del_block->changed = 0 ;
      b4free( del_block ) ;
      block_on = (B4BLOCK *) t4->blocks.last ;
      if ( block_on == 0 )  /* just removed root, so make reference root! */
	 t4->header.root = reference ;
      else
      {
         memcpy( &(b4key( block_on, block_on->key_on )->pointer), &reference, sizeof( reference ) )  ;
	 block_on->changed = 1 ;
      }
   }
   else    /* just remove myself, add later */
      b4remove( block_on ) ;

   /* now add the removed reference back into the index */
   t4add( t4, my_key_data->value, my_key_data->num ) ;
   u4free( my_key_data ) ;
}

                                /* Remove the current key */
int S4FUNCTION t4remove_current( T4TAG *t4 ) 
{
   B4BLOCK *block_on ;
   if ( t4->code_base->error_code < 0 )  return -1 ;

   t4->header.version =  t4->header.old_version + 1 ;

   block_on = (B4BLOCK *) t4->blocks.last ;

   #ifdef DEBUG
      if ( b4lastpos( block_on ) == -b4leaf( block_on ) )
         e4severe( e4info, "t4remove_current()", E4_INFO_UNE, (char *) 0 ) ;
   #endif

   switch( t4get_replace_entry( t4, b4key( block_on, block_on->key_on ), block_on ) )
   {
      case 0 :   /* leaf delete */
         b4remove( block_on ) ;
         if ( block_on->n_keys == 0 )  /* last entry deleted! -- remove upward reference */
            t4remove_ref( t4 ) ;
	  break ;

      case 1 :   /* branch delete */
         block_on->changed = 1 ;  /* mark as changed for all entries */
         if( t4block( t4 )->n_keys == 0 )    /* removed the last key of */
            t4remove_ref( t4 ) ;
    	  break ;

      default:
         e4severe( e4info, E4_INFO_INT, (char *) 0 ) ;
         break ;
   }
   return 0 ;
}
#endif  /* ifdef S4CLIPPER  */
#endif  /* ifdef S4NDX  */

int S4FUNCTION t4remove( T4TAG *t4, char *ptr, long rec )
{
   int rc ; 

   rc =  t4go( t4, ptr, rec ) ;  if ( rc < 0 )  return rc ;
   if ( rc != 0 )  return r4entry ;

   return t4remove_current( t4 ) ;
}

int S4FUNCTION t4remove_calc( T4TAG *t4, long rec )
{
   char *ptr ;
   e4key( t4->expr, &ptr ) ;
   return t4remove( t4, ptr, rec ) ;
}
#endif   /* ifdef N4OTHER  */
