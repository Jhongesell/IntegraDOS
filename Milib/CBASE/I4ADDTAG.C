/* i4addtag.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifndef N4OTHER

int S4FUNCTION t4add( T4TAG *t4, char *key_info, long rec )
{
   C4CODE *c4 ;
   I4INDEX *i4 ;
   B4BLOCK *old_block, *root_block, *new_block ;
   int rc, is_branch, key_on ;
   long  old_file_block, extend_block ;
   #ifdef S4FOX
      long rec1 = 0l ;
      long rec2 = 0l ;
      char *temp_key ;
   #endif

   c4 =  t4->code_base ;
   i4 =  t4->index ;

   if ( c4->error_code < 0 )  return -1 ;

   rc =  t4go( t4, key_info, rec ) ;   if ( rc < 0 )  return -1 ;

   if ( rc == 0 )
   {
      e4error( c4, e4unique, i4->file.name, (char *) 0 ) ;
      return e4unique ;
   }

   #ifdef S4FOX
   if ( (t4->header.type_code & 0x01) && rc == r4found )
   #else
   if ( t4->header.unique && rc == r4found )
   #endif
   {
      switch ( t4->unique_error )
      {
         case e4unique:
	    return e4error( c4, e4unique, i4->file.name, (char *) 0 ) ;

         case r4unique:
            return r4unique ;

         case r4unique_continue:
            return r4unique_continue ;
      }
   }

   if ( t4->filter && !t4->has_keys )
   {
      h4write(&t4->index->file, t4->header_offset+sizeof(t4->header)+222,
            (char *) "\0", (int) 1 ) ;

      t4->has_keys = (char) 1 ;
   }

   old_block =  t4block(t4) ;
   old_file_block =  0 ;

   #ifdef S4FOX
      i4->tag_index->header.version =  i4->version_old+1 ;
   
      temp_key = (char *)u4alloc( t4->header.key_len ) ;
   
      for(;;)
      {
         if ( old_block == 0 )
         {
            /* Must create a new root block */
            extend_block =  i4extend(i4) ;
            if ( extend_block < 0 )
            {
               u4free( temp_key ) ;
               return (int) extend_block ;
            }
   
            root_block =  b4alloc( t4, extend_block) ;
            if ( root_block == 0 )
            {
               u4free( temp_key ) ;
               return -1 ;
            }
   
            root_block->header.left_node = -1 ;
            root_block->header.right_node = -1 ;
            root_block->header.node_attribute = 1 ;
   
            l4add( &t4->blocks, root_block ) ;
   
            b4top( root_block ) ;
            b4insert( root_block, temp_key, rec, rec2, 1 ) ;
            b4insert( root_block, key_info, old_file_block, rec1, 1 ) ;
            rec1 = 0l ;
            rec2 = 0l ;
            t4->header.root =  root_block->file_block ;
            t4->root_write  =  1 ;
            u4free( temp_key ) ;
            return 0 ;
         }
   
         if ( (rc = b4insert( old_block, key_info, rec, rec1, 0 )) != 1 )
         {
            u4free( temp_key ) ;
            if ( rc == 0 && b4leaf(old_block) )  /* insert succeeded on leaf, so update upper blocks if nec. */
            {
               int count = 0 ;
               while( old_block->key_on == old_block->header.n_keys - 1 )
               {
                  l4add( &t4->saved, l4pop( &t4->blocks ) ) ;
                  count++ ;
	          old_block = (B4BLOCK *)l4last(&t4->blocks) ;
                  if ( old_block == 0 ) break ;  /* done */
                  b4br_replace( old_block, key_info, rec ) ;
               }
               for( ; count ; count-- )
                  l4add( &t4->blocks, l4pop( &t4->saved ) ) ;
            }
            return rc ;  /* either succeeded, or error */
         }
   
         l4pop( &t4->blocks ) ;
   
         key_on = old_block->key_on ;
   
         /* NNNNOOOO  N - New, O - Old */
         /* The new block's end key gets added to the block just up */
         new_block=  t4split( t4, old_block ) ;
         if ( new_block == 0 )
         {
            u4free( temp_key ) ;
            return -1 ;
         }
   
         l4add( &t4->saved, old_block ) ;
   
         if ( key_on < old_block->header.n_keys )
         {
             b4go( old_block, key_on ) ;
             b4insert( old_block, key_info, rec, rec1, 0 ) ;
         }
         else
         {
             b4go( new_block, key_on - old_block->header.n_keys ) ;
             b4insert( new_block, key_info, rec, rec1, 0 ) ;
         }
   
         /* Now add to the block just up */ 
         b4go( old_block, old_block->header.n_keys - 1 ) ;
   
         key_info =  b4key_key( old_block, old_block->key_on ) ;
         old_file_block = old_block->file_block ;
         rec1 = b4recno( old_block ) ;
   
         rec =  new_block->file_block ;
         if ( b4flush(new_block) < 0 )
         {
            u4free( temp_key ) ;
            return -1 ;
         }
   
         b4go( new_block, new_block->header.n_keys - 1 ) ;
         memcpy( temp_key, b4key_key( new_block, new_block->key_on ), t4->header.key_len ) ;
         rec2 = b4recno( new_block ) ;
         b4free( new_block ) ;
         old_block =  (B4BLOCK *) t4->blocks.last ;
      }
   #else              /* if not S4FOX  */
      i4->header.version =  i4->version_old+1 ;
   
      for(;;)
      {
         if ( old_block == 0 )
         {
            /* Must create a new root block */
            extend_block =  i4extend(i4) ;
            if ( extend_block < 0 )  return (int) extend_block ;
   
	    root_block =  b4alloc( t4, extend_block) ;
            if ( root_block == 0 )  return -1 ;
   
	    l4add( &t4->blocks, root_block ) ;
   
            b4insert( root_block, key_info, old_file_block ) ;
	    b4insert( root_block, key_info, rec ) ;
	    root_block->n_keys-- ;
	    t4->header.root =  root_block->file_block ;
            t4->root_write  =  1 ;
	    return 0 ;
         }
   
         if ( old_block->n_keys < old_block->tag->header.keys_max )
         {
            b4insert( old_block, key_info, rec ) ;
            return 0 ;
         }
      
         l4pop( &t4->blocks ) ;
   
         is_branch  =  b4leaf( old_block )  ?  0 : 1 ;
   
         /* NNNNOOOO  N - New, O - Old */
         /* The new block's end key gets added to the block just up */
         new_block=  t4split( t4, old_block ) ; 
         if ( new_block == 0 )  return -1 ;
   
         l4add( &t4->saved, new_block ) ;
   
         new_block->n_keys -=  is_branch ;
         if ( new_block->key_on < (new_block->n_keys+is_branch) )
	    b4insert( new_block, key_info, rec ) ;
         else
            b4insert( old_block, key_info, rec ) ;
   
         /* Now add to the block just up */
         new_block->key_on =  b4lastpos(new_block) ;
   
         key_info =  b4key_key( new_block, new_block->key_on ) ;
         rec      =  new_block->file_block ;
   
         old_file_block =  old_block->file_block ;
         if ( b4flush(old_block) < 0 )  return -1 ;
   
         b4free( old_block ) ;
         old_block =  (B4BLOCK *) t4->blocks.last ;
      }
   #endif
}

int S4FUNCTION t4add_calc( T4TAG *t4, long rec )
{
   int len ;
   char *ptr ;

   if (t4->filter )
      if ( ! e4true(t4->filter) ) return 0;

   if ( (len =  e4key( t4->expr, &ptr )) < 0 )  return -1 ;
   #ifdef S4DEBUG
      #ifdef S4FOX
         if ( len != t4->header.key_len )
            e4severe( e4result, "t4add_calc()", (char *) 0 ) ;
      #else
         if ( len != t4->header.value_len )
            e4severe( e4result, "t4add_calc()", (char *) 0 ) ;
      #endif
   #endif

   return t4add( t4, ptr, rec ) ;
}

#endif  /*  ifndef N4OTHER  */


#ifdef N4OTHER

#ifdef S4NDX
int S4FUNCTION i4get_last_key( T4TAG *t4, char *key_data, long for_block )
{
   int rc = 0 ;
   B4BLOCK *temp_block ;

   #ifdef DEBUG
      if ( for_block <= 0 )
	 e4severe( e4info, "i4get_last_key()", (char *) 0 ) ;
   #endif

   /* must get block for_block, then find the last key */
   temp_block =  b4alloc( t4, for_block ) ;
   if ( temp_block == 0 )  return -1 ;

   if ( h4read_all( &t4->file, I4MULTIPLY * for_block, &temp_block->n_keys,
	B4BLOCK_SIZE ) < 0 ) return -1 ;

   b4go_eof( temp_block ) ;
   if ( b4leaf( temp_block ) )  rc = 2 ;
   else rc = b4get_last_key( temp_block, key_data ) ;

   b4free( temp_block ) ;

   return rc ;
}
#endif

#ifdef S4NDX
int S4FUNCTION t4add( T4TAG *t4, char *key_info, long rec )
{
   C4CODE *c4 ;
   I4INDEX *i4 ;
   B4BLOCK *old_block, *root_block, *new_block, *wch_block ;
   int rc, is_branch ;
   long  old_file_block, extend_block ;
   static char key_data[101] ;    /* temporary storage for the key data (max size 100) */
   static char key2_data[101] ;

   c4 =  t4->code_base ;
   i4 =  t4->index ;

   if ( c4->error_code < 0 )  return -1 ;

   rc =  t4go( t4, key_info, rec ) ;   if ( rc < 0 )  return -1 ;
   if ( rc == 0 )
   {
      e4error( c4, e4unique, i4->file.name, (char *) 0 ) ;
      return e4unique ;
   }

   if ( t4->header.unique && rc == r4found )
   {
      switch ( t4->unique_error )
      {
	 case e4unique:
	    return e4error( c4, e4unique, i4->file.name, (char *) 0 ) ;

	 case r4unique:
	    return r4unique ;

	 case r4unique_continue:
            return r4unique_continue ;
      }
   }

   old_block =  t4block(t4) ;
   old_file_block =  0 ;

   t4->header.version =  t4->header.old_version+1 ;

   for(;;)
   {
      if ( old_block == 0 )
      {
         /* Must create a new root block */
	 extend_block =  t4extend( t4 ) ;
         if ( extend_block < 0 )  return (int) extend_block ;

	 root_block =  b4alloc( t4, extend_block) ;
         if ( root_block == 0 ) return -1 ;

	 l4add( &t4->blocks, root_block ) ;

	 i4get_last_key( t4, key_data, old_file_block ) ;

         b4insert( root_block, key_data, 0L, old_file_block ) ;
         b4append( root_block, rec ) ;
	 t4->header.root =  root_block->file_block ;
         t4->root_write  =  1 ;
         return 0 ;
      }

      if ( b4room( old_block ) )
      {
	 if ( b4leaf( old_block ) )
            b4insert( old_block, key_info, rec, 0L ) ;
         else
         {
            b4get_last_key( old_block, key_data ) ;
            if ( old_block->key_on >= old_block->n_keys  )  /*insert at end done different */
            {
               b4insert( old_block, key_data, 0L, old_file_block ) ;
               b4append( old_block, rec ) ;
            }
            else
            {
               b4remove( old_block ) ;
               b4insert( old_block, key_info, 0L, rec ) ;
               b4insert( old_block, key_data, 0L, old_file_block ) ;
            }
         }
         return 0 ;
      }
   
      l4pop( &t4->blocks ) ;
      is_branch  =  b4leaf( old_block )  ?  0 : 1 ;

      /* NNNNOOOO  N - New, O - Old */
      /* The new block's end key gets added to the block just up */
      new_block=  t4split( t4, old_block ) ; 
      if ( new_block == 0 )  return -1 ;

      l4add( &t4->saved, new_block ) ;

      /* which block should do the insertion ? */
      if ( old_block->key_on < (old_block->n_keys + is_branch) )
         wch_block = old_block ;
      else wch_block = new_block ;

      if ( b4leaf( wch_block ) )
         b4insert( wch_block, key_info, rec, 0L ) ;
      else
      {
         b4get_last_key( wch_block, key_data ) ;
         if ( wch_block->key_on >= wch_block->n_keys )  /* insert at end done different */
         {
            b4insert( wch_block, key_data, 0L, old_file_block ) ;
            b4append( wch_block, rec ) ;
         }
         else
         {
            b4remove( wch_block ) ;
            b4insert( wch_block, key_info, 0L, rec ) ;
            b4insert( wch_block, key_data, 0L, old_file_block ) ;
         }
      }

      /* Now add to the block just up */
      new_block->key_on =  b4lastpos(new_block) ;

      key_info =  b4key_key( new_block, new_block->key_on ) ;
      rec      =  new_block->file_block ;

      if( !b4leaf( new_block ) )
         if ( b4get_last_key( new_block, key2_data ) != 4)
            key_info = key2_data ;

      old_block->key_on = b4lastpos( old_block ) ;
      memcpy( key_data, b4key_key( old_block, old_block->key_on ), t4->header.key_len ) ;

      old_file_block =  old_block->file_block ;
      if ( b4flush( old_block ) < 0 )  return -1 ;
      b4free( old_block ) ;
      old_block =  (B4BLOCK *) t4->blocks.last ;
   }
}
#else
#ifdef S4CLIPPER
int S4FUNCTION t4add( T4TAG *t4, char *key_info, long rec )
{
   C4CODE *c4 ;
   I4INDEX *i4 ;
   B4BLOCK *old_block, *root_block, *new_block, *wch_block ;
   int rc, is_branch, key_on, add_new ;
   long  old_file_block, extend_block, new_file_block ;
   B4KEY_DATA *at_new ;
   char old_key_ptr[ I4MAX_KEY_SIZE ] ;

   c4 =  t4->code_base ;
   i4 =  t4->index ;

   if ( c4->error_code < 0 )  return -1 ;


   rc =  t4go( t4, key_info, rec ) ;   if ( rc < 0 )  return -1 ;
   if ( rc == 0 )
   {
      e4error( c4, e4unique, i4->file.name, (char *) 0 ) ;
      return e4unique ;
   }

   if ( t4->header.unique && rc == r4found )
   {
      switch ( t4->unique_error )
      {
         case e4unique:
	    return e4error( c4, e4unique, i4->file.name, (char *) 0 ) ;

         case r4unique:
            return r4unique ;

         case r4unique_continue:
            return r4unique_continue ;
      }
   }

   old_block =  t4block(t4) ;
   old_file_block =  0 ;
   new_file_block =  0 ;

   t4->header.version =  i4->version_old+1 ;

   while( !b4leaf( old_block ) )
   {
      if( t4down( t4 ) != 0 ) return -1 ;
      old_block = t4block( t4 ) ;
      if ( b4leaf( old_block ) )
         old_block->key_on = b4lastpos( old_block ) + 1 ;
      else
         old_block->key_on = b4lastpos( old_block ) ;
   }

   for(;;)
   {
      if ( old_block == 0 )
      {
	 /* Must create a new root block */
	 extend_block =  t4extend( t4 ) ;
         if ( extend_block < 0 )  return (int) extend_block ;

	 root_block =  b4alloc( t4, extend_block) ;
	 if ( root_block == 0 ) return -1 ;

	 l4add( &t4->blocks, root_block ) ;

	 b4insert( root_block, key_info, rec, old_file_block ) ;
         b4append( root_block, new_file_block ) ;
	 t4->header.root =  root_block->file_block * 512 ;

         t4->root_write  =  1 ;
         return 0 ;
      }

      if ( b4room( old_block ) )
      {
         if ( b4leaf( old_block ) )
	    b4insert( old_block, key_info, rec, 0L ) ;
         else   /* update the current pointer, add the new branch */
         {
	    at_new = b4key( old_block, old_block->key_on ) ;
	    at_new->pointer = new_file_block * 512 ;
	    b4insert( old_block, key_info, rec, old_file_block ) ;
         }
	 return 0 ;
      }
   
      l4pop( &t4->blocks ) ;
      is_branch  =  b4leaf( old_block )  ?  0 : 1 ;

      /* NNNNOOOO  N - New, O - Old */
      /* The new block's end key gets added to the block just up */
      new_block=  t4split( t4, old_block ) ; 
      if ( new_block == 0 )  return -1 ;

      /* which block should do the insertion ? */
      if ( old_block->key_on < (old_block->n_keys + is_branch) )
         wch_block = old_block ;
      else wch_block = new_block ;

      if ( b4leaf( wch_block ) )
	 b4insert( wch_block, key_info, rec, 0L ) ;
      else
      {
          if ( wch_block->n_keys == 0 )
          {
	    b4insert( wch_block, key_info, rec, old_file_block ) ;
            at_new = b4key( wch_block, 1 ) ;
	    at_new->pointer = new_file_block * 512 ;
          }
          else
          {
	    at_new = b4key( wch_block, wch_block->key_on ) ;
	    at_new->pointer = new_file_block * 512 ;
	    b4insert( wch_block, key_info, rec, old_file_block ) ;
          }
      }

      add_new = 1 ;

      #ifdef DEBUG
         if( new_block == 0 )
         e4severe( e4info, "t4add()", E4_INFO_CIS, (char *) 0 ) ;
      #endif

      if( b4leaf( new_block ) )
      {
         if ( new_block->n_keys == 1 )   /* add a key from the old block!, must have info in new_block because old_block gets deleted below */
         {
            #ifdef DEBUG
            if ( old_block->n_keys == 1 )  /* impossible */
               e4severe( e4info, "t4add()", E4_INFO_CIF, (char *) 0 ) ;
            #endif
            old_block->key_on = old_block->n_keys - 1 ;
            key_info = b4key_key( old_block, old_block->key_on ) ;
	    rec = b4key( old_block, old_block->key_on )->num ;
            b4remove( old_block ) ;
            new_block->key_on = 0 ;
            b4insert( new_block, key_info, rec, 0 ) ;
         }
         new_block->key_on = 0 ;
         key_info = b4key_key( new_block, new_block->key_on ) ;
	 rec = b4key( new_block, new_block->key_on )->num ;
         b4remove( new_block ) ;
      }
      else
      {
         if ( new_block->n_keys == 0 )   /* dummy branch, so delete it */
         {
	    new_file_block = ( long )(b4key_key( new_block, 0 ))/512 ;
	    t4shrink( t4, new_block->file_block ) ;
            new_block->changed = 0 ;
            b4free( new_block ) ;
            add_new = 0 ;
         }
         /* now get the key to place upwards */
	 memcpy( old_key_ptr, b4key_key( old_block, b4lastpos( old_block )) , t4->header.key_len ) ;
	 key_info = old_key_ptr ;
	 rec = b4key( old_block, b4lastpos( old_block ) )->num ;
      }

      if ( add_new )
      {
         l4add( &t4->saved, new_block ) ;
	 new_file_block = new_block->file_block ;
      }
      old_file_block =  old_block->file_block ;
      if ( b4flush( old_block ) < 0 )  return -1 ;
      b4free( old_block ) ;
      old_block =  (B4BLOCK *) t4->blocks.last ;
   }
}
#endif  /* ifdef S4CLIPPER   */
#endif  /* ifdef S4NDX    */

int S4FUNCTION t4add_calc( T4TAG *t4, long rec )
{
   char *ptr ;

   if ( e4key( t4->expr, &ptr ) < 0 )  return -1 ;
   #ifdef S4CLIPPER
      #ifdef DEBUG
	 if ( ( e4type( t4->expr ) == t4num_clip ) &&  ( t4->r_type != t4num_clip ) )
	     e4severe( e4result, "t4add_calc()", E4_RESULT_CLI, (char *) 0 ) ;
      #endif
   #endif
   return t4add( t4, ptr, rec ) ;
}

#endif  /* ifdef N4OTHER  */
