/* i4index.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
    
#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif


#ifndef N4OTHER

int S4FUNCTION i4close( I4INDEX *i4 )
{
   int final_rc, save_wait ;
   C4CODE *c4 ;
   T4TAG *tag_on ;

   if ( i4 == 0 )  return -1 ;

   c4 =  i4->code_base ;

   final_rc =  c4->error_code ;
   save_wait =  c4->wait ;
   c4->wait =  1 ;

   if ( i4->data != 0 )
      if ( d4flush_all( i4->data ) < 0 )
         final_rc =  e4error_set( c4, 0 ) ;

   if ( i4unlock(i4) < 0 )
      final_rc =  e4error_set( c4, 0 ) ;

   #ifdef S4FOX
      if ( i4->tag_index->header.type_code >= 64 )  /* compound index */
   #endif

   while( tag_on =  (T4TAG *) l4pop(&i4->tags) )
   {
      if ( t4free_all(tag_on) < 0 )
      {
	 final_rc =  e4error_set( c4, 0) ;
         break ;
      }

      e4free( tag_on->expr ) ;
      e4free( tag_on->filter ) ;
      y4free( c4->tag_memory, tag_on ) ;
   }

   #ifdef S4FOX
      if ( t4free_all(i4->tag_index) < 0 )
         final_rc =  e4error_set( c4, 0) ;
      else
      {
         e4free( i4->tag_index->expr ) ;
         e4free( i4->tag_index->filter ) ;
         y4free( c4->tag_memory, i4->tag_index ) ;
      }
   #endif

   y4release( i4->block_memory ) ;

   if ( h4open_test(&i4->file) )
   {
      if ( i4->data != 0 )
	 l4remove( &i4->data->indexes, i4 ) ;
      if ( h4close(&i4->file) < 0 )  final_rc = e4error_set( c4, 0) ;
   }

   y4free( i4->code_base->index_memory, i4 ) ;
   c4->wait =  save_wait ;
   e4error_set( c4, final_rc ) ;
   return  final_rc ;
}

long S4FUNCTION i4extend( I4INDEX *i4 )
{
   long old_eof ;
   unsigned len ;

   if ( i4->code_base->error_code < 0 )  return -1 ;

   #ifndef S4FOX
      #ifdef S4DEBUG
         if ( i4->header.version == i4->version_old )
            e4severe( e4info, "i4extend()", (char *) 0 ) ;
      #endif
   
      old_eof = i4->header.free_list ;
   
      if( i4->header.free_list == 0L )  /* case where no free list */
      {
         old_eof = i4->header.eof ;
         i4->header.eof = i4->header.eof + i4->header.block_rw/I4MULTIPLY ;
      }
      else
      {
         len =  h4read( &i4->file, i4->header.free_list*I4MULTIPLY + sizeof(long), 
	         (char *) &i4->header.free_list, sizeof(i4->header.free_list)) ;
         if ( i4->code_base->error_code < 0 )  return -1 ;
   
         switch( len )
         {
            case 0:
               #ifdef S4DEBUG
                  e4severe( e4info, "i4extend()", (char *) 0 ) ;
               #endif
   
               /* else fix up */
               i4->header.free_list = 0L ;
	       old_eof = i4->header.eof ;
	       i4->header.eof = i4->header.eof + i4->header.block_rw/I4MULTIPLY ;
               break ;
   
            case sizeof(i4->header.free_list):
               break ;
   
            default:
               return h4read_error( &i4->file ) ;
         }
      }
   #endif

   #ifdef S4FOX
      #ifdef S4DEBUG
         if ( i4->tag_index->header.version == i4->version_old )
            e4severe( e4info, "i4extend()", (char *) 0 ) ;
      #endif
   
      old_eof = i4->tag_index->header.free_list ;
   
      if( old_eof == 0L )  /* case where no free list */
      {
         old_eof = i4->eof ;
         i4->eof += B4BLOCK_SIZE ;
      }
      else
      {
         len =  h4read( &i4->file, i4->tag_index->header.free_list*I4MULTIPLY, 
                 (char *) &i4->tag_index->header.free_list, sizeof(i4->tag_index->header.free_list)) ;
         if ( i4->code_base->error_code < 0 )  return -1 ;
   
         switch( len )
         {
            case 0:
               #ifdef S4DEBUG
                  e4severe( e4info, "i4extend()", (char *) 0 ) ;
               #endif
   
               /* else fix up */
               i4->tag_index->header.free_list = 0L ;
	        old_eof = i4->eof ;
               i4->eof += B4BLOCK_SIZE ;
               break ;
   
            case sizeof(i4->tag_index->header.free_list):
               break ;
   
            default:
               return h4read_error( &i4->file ) ;
         }
      }
   #endif

   return old_eof ;
}

int S4FUNCTION i4flush( I4INDEX *i4 )
{
   T4TAG *tag_on ;

   if ( i4->file_locked )
   {
      if ( i4update_header(i4) < 0 )  return -1 ;

      #ifdef S4FOX
         if ( t4flush(i4->tag_index) < 0 )  return -1 ;
         if ( i4->tag_index->header.type_code >= 64 )  /* compound index */
      #endif

      for ( tag_on = 0; tag_on =  (T4TAG *) l4next(&i4->tags,tag_on); )
      {
	 if ( t4flush(tag_on) < 0 )  return -1 ;
	 tag_on->header.root =  -1L ;
      }
   }
   return 0 ;
}

int S4FUNCTION i4lock( I4INDEX *i4 )
{
   int rc ;
   #ifdef S4DEBUG
      I4INDEX *index_on ;
   #endif

   if ( i4->code_base->error_code < 0 )  return -1 ;
   if ( i4->file_locked )  return 0 ;    /* Already locked */
   if ( i4unlock(i4) < 0 )  return -1 ;  /* Unlock any locked tag */

   #ifdef S4DEBUG
      for ( index_on = i4; index_on = (I4INDEX *) l4next(&i4->data->indexes,index_on);)
         if ( index_on->file_locked || index_on->tag_locked )
            e4severe( e4result, E4_RESULT_LOC, E4_RESULT_INC, i4->file.name, (char *) 0 ) ;
   #endif

   rc =  h4lock( &i4->file, L4LOCK_POS, L4LOCK_POS ) ;
   if ( rc != 0 )  return rc ;

   if ( i4version_check(i4) < 0 )
   {
      h4unlock( &i4->file, L4LOCK_POS, L4LOCK_POS ) ;
      return -1 ;
   }

   i4->file_locked =  1 ;
   return 0 ;
}

I4INDEX *S4FUNCTION i4open( D4DATA *d4, char *file_name )
{
   I4INDEX *i4 ;
   C4CODE  *c4 ;
   char   buf[258] ;
   int    rc, i_tag ;
   T4TAG *tag_ptr ;
   #ifndef S4FOX
      T4DESC tag_info[47] ;
   #endif

   #ifdef S4DEBUG
   I4INDEX *i4_ptr ;

      if ( d4 == 0 )
         e4severe( e4parm, E4_OPEN_NOD, (char *) 0 ) ;
   #endif

   c4  =  d4->code_base ;

   if ( c4->index_memory == 0 )
      c4->index_memory =  y4memory_type( c4->mem_start_index, sizeof(I4INDEX),
				   c4->mem_expand_index, 0 ) ;

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

   i4 =  (I4INDEX *) y4alloc( c4->index_memory ) ;
   if ( i4 == 0 )
   {
      e4error( c4, e4memory, (char *) 0 ) ;
      return 0 ;
   }

   i4->data =  d4 ;
   i4->code_base =  c4 =  d4->code_base ;

   #ifdef S4FOX
      if ( file_name == 0 )
      {
         u4ncpy( buf, d4->file.name, sizeof(buf) ) ;
         u4name_ext( buf, sizeof(buf), "CDX", 1 ) ;
      }
      else
      {
         u4ncpy( buf, file_name, sizeof(buf) ) ;
         c4upper(buf) ;
         u4name_ext( buf, sizeof(buf), "CDX", 0 ) ;
      }

      #ifdef S4DEBUG
         for ( i4_ptr = 0; i4_ptr =  (I4INDEX *) l4next(&d4->indexes, i4_ptr); )
            if ( !memcmp(i4_ptr->file.name, buf, (size_t) strlen(buf) ) )
               e4severe( e4parm, E4_PARM_IND, (char *) 0 ) ;
      #endif

      rc =  h4open( &i4->file, c4, buf, 1 ) ;  if ( rc != 0 )  return 0 ;
      l4add( &d4->indexes, i4 ) ;

      i4->eof = h4length( &i4->file ) ;

      i4->tag_index =  (T4TAG *) y4alloc( c4->tag_memory ) ;
      if ( i4->tag_index == 0 )
      {
         e4error( c4, e4memory, (char *) 0 ) ;
         return 0 ;
      }

      i_tag = 0 ;
      if ( file_name == 0 )
      {
         if ( t4init( i4->tag_index, i4, 0L, L4LOCK_POS+i_tag, "") < 0 )
         {
            i4close( i4 ) ;
            return 0 ;
         }
      }
      else
      {
         u4name_piece( buf, 258, file_name, 0, 0 ) ;  /* get the tag_name based on the file_name */
         if ( t4init( i4->tag_index, i4, 0L, L4LOCK_POS+i_tag, buf) < 0 )
         {
            i4close( i4 ) ;
            return 0 ;
         }
      }

      /* Perform some checks */
      if ( i4->tag_index->header.root <= 0L ||
           i4->tag_index->header.type_code < 32 )
      {
         e4error( c4, e4index, buf, (char *) 0 ) ;
         return 0 ;
      }

      i4->block_memory = y4memory_type( c4->mem_start_block,
           sizeof(B4BLOCK) + B4BLOCK_SIZE - sizeof(B4STD_HEADER) - sizeof(B4NODE_HEADER),
           c4->mem_expand_block, 0 ) ;
      if ( i4->block_memory == 0 )
      {
         i4close(i4) ;
         return 0 ;
      }

      t4top( i4->tag_index ) ;
      i4unlock( i4 ) ;

      /* if we have a compound index, then load the tags, otherwise this is the only tag */
      if ( i4->tag_index->header.type_code >= 64 )
      {
         if ( t4block( i4->tag_index )->header.n_keys )
            do
         {
            tag_ptr =  (T4TAG *) y4alloc( c4->tag_memory ) ;
            if ( tag_ptr == 0 )
            {
               e4error( c4, e4memory, (char *) 0 ) ;
               return 0 ;
            }

            i_tag++ ;
            if ( t4init( tag_ptr, i4, b4recno( t4block(i4->tag_index) ), L4LOCK_POS+i_tag, t4key( i4->tag_index )->value ) < 0 )
            {
               i4close( i4 ) ;
               return 0 ;
            }

            l4add( &i4->tags, tag_ptr ) ;
          } while ( t4skip( i4->tag_index, 1L ) == 1L ) ;
       }
      else
      {
         #ifdef S4DEBUG
            if ( file_name == 0 )
            {
               i4close( i4 ) ;
              e4error( c4, e4index, E4_INDEX_EXP, (char *) 0 ) ;
            }
         #endif
         l4add( &i4->tags, i4->tag_index ) ;   /* if an .idx, add single tag */
      }
   #endif

   #ifndef S4FOX
      if ( file_name == 0 )
      {
         u4ncpy( buf, d4->file.name, sizeof(buf) ) ;
         u4name_ext( buf, sizeof(buf), "MDX", 1 ) ;
      }
      else
      {
         u4ncpy( buf, file_name, sizeof(buf) ) ;
         c4upper(buf) ;
         u4name_ext( buf, sizeof(buf), "MDX", 0 ) ;
      }

      #ifdef S4DEBUG
        for ( i4_ptr = 0; i4_ptr =  (I4INDEX *) l4next(&d4->indexes, i4_ptr); )
            if ( !memcmp(i4_ptr->file.name, buf, (size_t) strlen(buf) ) )
               e4severe( e4parm, E4_PARM_IND, (char *) 0 ) ;
      #endif

      rc =  h4open( &i4->file, c4, buf, 1 ) ;  if ( rc != 0 )  return 0 ;
      l4add( &d4->indexes, i4 ) ;

      if ( h4read_all( &i4->file, 0L, &i4->header, sizeof(i4->header) ) < 0 ) return 0 ;

      /* Perform some checks */
      if ( i4->header.block_rw != i4->header.block_chunks*512  ||
           i4->header.block_chunks <= 0 ||
           i4->header.block_chunks > 32 ||
           i4->header.num_tags < 0  || i4->header.num_tags > 47 ||
           i4->header.eof <= 0L )
      {
         e4error( c4, e4index, buf, (char *) 0 ) ;
         return 0 ;
      }

      if ( h4read_all( &i4->file, 544L, &tag_info, sizeof(tag_info)) < 0 )  return 0 ;

      for ( i_tag = 0; i_tag < (int) i4->header.num_tags; i_tag++ )
      {
         tag_ptr =  (T4TAG *) y4alloc( c4->tag_memory ) ;
         if ( tag_ptr == 0 )
         {
	    e4error( c4, e4memory, (char *) 0 ) ;
	    return 0 ;
         }

         l4add( &i4->tags, tag_ptr ) ;

         if ( t4init( tag_ptr, i4, tag_info+i_tag, L4LOCK_POS+i_tag) < 0 )
         {
            i4close( i4 ) ;
            return 0 ;
         }
      }

      i4->block_memory = y4memory_type( c4->mem_start_block,
	      sizeof(B4BLOCK) + i4->header.block_rw -
	      sizeof(B4KEY_DATA) - sizeof(short) - sizeof(char[6]),
	      c4->mem_expand_block, 0 ) ;
      if ( i4->block_memory == 0 )
      {
         i4close(i4) ;
         return 0 ;
      }
   #endif

   return i4 ;
}

int S4FUNCTION i4shrink( I4INDEX *i4, long block_no )
{
   #ifdef S4FOX
      if ( h4write( &i4->file, block_no, (char *)&i4->tag_index->header.free_list,
	   sizeof(i4->tag_index->header.free_list)) < 0) return -1 ;
      i4->tag_index->header.free_list =  block_no ;
   #else
      if ( h4write( &i4->file, block_no*I4MULTIPLY + sizeof(long), 
           (char *) &i4->header.free_list, sizeof(i4->header.free_list)) < 0) return -1 ;
      i4->header.free_list =  block_no ;
   #endif

   return 0 ;
}

T4TAG *S4FUNCTION i4tag( I4INDEX *i4, char *tag_name )
{
   char tag_lookup[11] ;
   T4TAG *tag_on ;

   if ( i4 == 0 )  return 0 ;

   u4ncpy( tag_lookup, tag_name, sizeof(tag_lookup) ) ;
   c4upper( tag_lookup ) ;

   for( tag_on = 0; tag_on = (T4TAG *)l4next( &i4->tags, 0); )
   {
      if ( strcmp( tag_on->alias, tag_lookup) == 0 )
         return tag_on ;
   }

   if ( i4->data->code_base->tag_name_error )
      e4error( i4->data->code_base, e4tag_name, tag_name, (char *) 0 ) ;
   return 0 ;
}

int S4FUNCTION i4unlock( I4INDEX *i4 )
{
   if ( i4->file_locked )
   {
      if ( i4flush(i4) < 0 )  return -1 ;

      if ( h4unlock( &i4->file, L4LOCK_POS, L4LOCK_POS ) < 0 )  return -1 ;
      i4->file_locked =  0 ;
      return 0 ;
   }

   if ( i4->tag_locked != 0 )
   {
      /* Must lock entire index to update, consequently, it was not changed */
      i4->tag_locked->header.root =  -1 ;
      if ( h4unlock( &i4->file, i4->tag_locked->lock_pos, 1L) < 0 )  return -1 ;
      i4->tag_locked =  0 ;
   }
   return 0 ;
}

/* Updates the header if the version has changed */
int S4FUNCTION i4update_header( I4INDEX *i4 )
{
   if ( i4->code_base->error_code < 0 )  return -1 ;

   #ifdef S4FOX
      if ( i4->version_old != i4->tag_index->header.version )
      {
         if (h4write(&i4->file,0L, (char *)&i4->tag_index->header, T4HEADER_WR_LEN) < 0) return -1;
         if (h4write(&i4->file,VERSION_POS, (char *)&i4->tag_index->header.version, sizeof(long)) < 0) return -1;
         i4->version_old =  i4->tag_index->header.version ;
      }
   #else
      if ( i4->version_old != i4->header.version )
      {
         if (h4write(&i4->file,0L, (char *)&i4->header, sizeof(i4->header)) < 0) return -1;
         i4->version_old =  i4->header.version ;
      }
   #endif


   return 0 ;
}

/* Reads the header, checks the version to see if the blocks need to be freed. */
int S4FUNCTION i4version_check( I4INDEX *i4 )
{
   T4TAG *tag_on ;

   #ifdef S4FOX
      if ( h4read_all( &i4->file, 0L, &i4->tag_index->header, T4HEADER_WR_LEN) < 0 ) return -1 ;
      if ( h4read_all( &i4->file, VERSION_POS, &i4->tag_index->header.version, sizeof(long)) < 0 ) return -1 ;
      if ( i4->tag_index->header.version == i4->version_old )
         return 0 ;
   
      i4->version_old =  i4->tag_index->header.version ;
   #else
      if ( h4read_all( &i4->file, 0L, &i4->header, sizeof(i4->header)) < 0 ) return -1 ;
      if ( i4->header.version == i4->version_old )
         return 0 ;
   
      i4->version_old =  i4->header.version ;
   #endif

   for ( tag_on = 0; tag_on = (T4TAG *) l4next(&i4->tags,tag_on); )
      if ( t4free_all(tag_on) < 0 )  /* Should be a memory operation only */
         e4severe( e4result, "i4version_check()", (char *) 0 ) ;

   return 0 ;
}

#endif  /*  ifndef  N4OTHER  */


#ifdef N4OTHER

/* This function closes all the tags corresponding with the index */
int S4FUNCTION i4close( I4INDEX *i4 )
{
   int final_rc, save_wait ;
   C4CODE *c4 ;
   T4TAG *tag_on ;

   c4 =  i4->code_base ;

   final_rc =  c4->error_code ;
   save_wait =  c4->wait ;
   c4->wait =  1 ;

   if ( i4->data != 0 )
      if ( d4flush_all( i4->data ) < 0 )
         final_rc =  e4error_set( c4, 0 ) ;

   if ( i4unlock(i4) < 0 )
      final_rc =  e4error_set( c4, 0 ) ;

   while( tag_on =  (T4TAG *) l4pop(&i4->tags) )
   {
      t4close( tag_on ) ;
      if ( t4free_all(tag_on) < 0 )
      {
	 final_rc =  e4error_set( c4, 0) ;
         break ;
      }

      e4free( tag_on->expr ) ;
      e4free( tag_on->filter ) ;
      y4free( c4->tag_memory, tag_on ) ;
   }

   y4release( i4->block_memory ) ;
   i4->block_memory =  0 ;

   if ( i4->data != 0 )
      l4remove( &i4->data->indexes, i4 ) ;

   y4free( i4->code_base->index_memory, i4 ) ;
   c4->wait =  save_wait ;
   e4error_set( c4, final_rc ) ;
   return  final_rc ;
}

/* This function flushes all the tags corresponding with the index */
int S4FUNCTION i4flush( I4INDEX *i4 )
{
   T4TAG *tag_on ;

   if ( i4->file_locked )
   {
      for ( tag_on = 0; tag_on =  (T4TAG *) l4next(&i4->tags,tag_on); )
      {
	 if ( t4flush(tag_on) < 0 )  return -1 ;
	 tag_on->header.root =  -1L ;
      }
   }
   return 0 ;
}

/* locks all the corresponding tag files  -  if a lock fails on any tag, no locking is done */
int S4FUNCTION i4lock( I4INDEX *i4 )
{
   int rc ;
   T4TAG *tag_on ;
   #ifdef S4DEBUG
      I4INDEX *index_on ;
   #endif

   if ( i4->code_base->error_code < 0 )  return -1 ;
   if ( i4->file_locked )  return 0 ;    /* Already locked */
   if ( i4unlock( i4 ) < 0 )  return -1 ;  /* Unlock any locked tag */

   #ifdef S4DEBUG
      for ( index_on = i4; index_on = (I4INDEX *) l4next(&i4->data->indexes,index_on);)
         if ( index_on->file_locked || index_on->tag_locked )
            e4severe( e4result, E4_RESULT_LOC, E4_RESULT_INC, i4->file.name, (char *) 0 ) ;
   #endif

   for( tag_on = 0; tag_on = (T4TAG *)l4next( &i4->tags, tag_on); )
   {
      rc = h4lock( &tag_on->file, L4LOCK_POS, L4LOCK_POS ) ;
      if ( rc != 0 )
         break ;
      tag_on->file_locked = 1 ;
   }

   for( tag_on = 0; tag_on = (T4TAG *)l4next( &i4->tags, tag_on ); )
      if ( rc == 0 )
         rc = t4version_check( tag_on ) ;

   if ( rc != 0 )   /* unlock all previously locked tags */
   {
      for( tag_on = 0; tag_on = (T4TAG *)l4next( &i4->tags, tag_on ); )
         if ( tag_on->file_locked )
         {
            if ( h4unlock( &tag_on->file, L4LOCK_POS, L4LOCK_POS ) == 0 )
               tag_on->file_locked = 0 ;
         }
      return rc ;
   }

   i4->file_locked =  1 ;
   return 0 ;
}

I4INDEX *S4FUNCTION i4open( D4DATA *d4, char *file_name )
{
   I4INDEX *i4 ;
   C4CODE  *c4 ;
   int len, rc, i ;
   long pos ;
   char buf[258], tag_buf[258] ;
   int num_files ;

   c4  =  d4->code_base ;

   if ( c4->index_memory == 0 )
      c4->index_memory =  y4memory_type( c4->mem_start_index, sizeof(I4INDEX),
				   c4->mem_expand_index, 0 ) ;

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

   i4 =  (I4INDEX *) y4alloc( c4->index_memory ) ;
   if ( i4 == 0 )
   {
      e4error( c4, e4memory, (char *) 0 ) ;
      return 0 ;
   }

   i4->data =  d4 ;
   i4->code_base =  c4 =  d4->code_base ;

   if ( file_name == 0 )
   {
      u4ncpy( buf, d4->file.name, sizeof(buf) ) ;
      u4ncpy( i4->alias, buf, sizeof( i4->alias ) ) ;
      u4name_ext( buf, sizeof(buf), "CGP", 1 ) ;
   }
   else
   {

      u4ncpy( buf, file_name, sizeof(buf) ) ;
      c4upper(buf) ;
      u4ncpy( i4->alias, buf, sizeof( i4->alias ) ) ;
      u4name_ext( buf, sizeof(buf), "CGP", 1 ) ;
   }

   c4trim_n( i4->alias, sizeof( i4->alias ) ) ;

   rc = h4open( &i4->file, i4->code_base, buf ,1 ) ;
   if ( rc > 0 )
   {
      c4->error_code =  rc ;

      i4close(i4) ;
      return 0 ;
   }

   rc = h4lock( &i4->file, L4LOCK_POS, L4LOCK_POS ) ;  /* lock the group file */
   if ( rc != 0 )
   {
      e4error( c4, e4info, E4_INFO_LOC, (char *) 0 ) ;
      return 0 ;
   }

   pos = 0L ;
   rc = h4read_all( &i4->file, pos, &num_files, sizeof( num_files ) ) ;
   if ( rc != 0 )
   {
      e4error( c4, e4info, E4_INFO_REA, (char *) 0 ) ;
      return 0 ;
   }
   pos += sizeof( num_files ) ;

   for ( i = 0 ; i < num_files ; i++ )
   {
      rc = h4read_all( &i4->file, pos, &len, sizeof( len ) ) ;
      if ( rc != 0 )
      {
	 i4close( i4 ) ;
	 e4error( c4, e4info, E4_INFO_REA, (char *) 0 ) ;
	 return 0 ;
      }
      pos += sizeof( len ) ;
      rc = u4name_path( tag_buf, sizeof( tag_buf ), buf ) ;
      tag_buf[rc+len] = '\0' ;
      if ( sizeof( tag_buf ) > rc + len ) /* make sure room to read */
         rc = h4read_all( &i4->file, pos, tag_buf+rc, len ) ;
      else
         rc = -1 ;

      if ( rc != 0 )
      {
	 i4close( i4 ) ;
	 e4error( c4, e4info, E4_INFO_REA, (char *) 0 ) ;
	 return 0 ;
      }
      pos += len ;
      t4open( i4->data, i4, tag_buf ) ;
   }

   rc = h4unlock( &i4->file, L4LOCK_POS, L4LOCK_POS ) ;
   if ( h4close ( &i4->file ) != 0 )
   {
      e4error( c4, e4info, E4_INFO_CLO, (char *) 0 ) ;
      return 0 ;
   }

   if ( rc < 0 )
   {
      e4error( c4, e4info, E4_INFO_UNL, (char *) 0 ) ;
      return 0 ;
   }

   l4add( &d4->indexes, i4 ) ;
   return i4 ;
}

T4TAG *S4FUNCTION i4tag( I4INDEX *i4, char *tag_name )
{
   char tag_lookup[11] ;
   T4TAG *tag_on ;

   if ( i4 == 0 )  return 0 ;

   u4ncpy( tag_lookup, tag_name, sizeof(tag_lookup) ) ;
   c4upper( tag_lookup ) ;

   for( tag_on = 0; tag_on = (T4TAG *)l4next( &i4->tags, 0); )
   {
      if ( strcmp( tag_on->alias, tag_lookup) == 0 )
         return tag_on ;
   }

   if ( i4->data->code_base->tag_name_error )
      e4error( i4->data->code_base, e4tag_name, tag_name, (char *) 0 ) ;
   return 0 ;
}

/* unlocks all the corresponding tag files */
int S4FUNCTION i4unlock( I4INDEX *i4 )
{
   int rc, save_rc = 0 ;
   T4TAG *tag_on ;

   if ( i4->file_locked )
   {
      #ifdef S4DEBUG
         if ( i4->tag_locked != 0 )
	    e4severe( e4info, "i4unlock()", E4_INFO_ILL, (char *) 0 ) ;
      #endif
      if ( i4flush(i4) < 0 )  return -1 ;

      for( tag_on = 0; tag_on = (T4TAG *)l4next( &i4->tags, tag_on); )
      {
         rc = h4unlock( &tag_on->file, L4LOCK_POS, L4LOCK_POS ) ;
         if ( rc != 0 )
            save_rc = rc ;
         else
            tag_on->file_locked = 0 ;
      }

      if ( save_rc == 0 )
         i4->file_locked =  0 ;
   }

   if ( i4->tag_locked != 0 )
   {
      save_rc = h4unlock( &i4->tag_locked->file, L4LOCK_POS, L4LOCK_POS ) ;
      i4->tag_locked->file_locked = 0 ;
      i4->tag_locked =  0 ;
      #ifdef S4DEBUG
         /* all tags should be unlocked in index file, do check */
	 for( tag_on = 0; tag_on = (T4TAG *)l4next( &i4->tags, tag_on); )
            if ( tag_on->file_locked )
            e4severe( e4info, E4_INFO_UNE, i4->file.name, 
		      E4_INFO_FOR, tag_on->alias, (char *) 0 ) ;
      #endif
   }

   return save_rc ;
}
#endif   /*  ifdef N4OTHER  */
