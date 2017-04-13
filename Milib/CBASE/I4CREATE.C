/* i4create.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#include "r4reinde.h"


#ifndef N4OTHER

I4INDEX *S4FUNCTION i4create( D4DATA *d4, char *file_name, T4TAG_INFO *tag_data )
{
   I4INDEX *i4 ;
   C4CODE *c4 ;
   T4TAG *tag_ptr ;
   char buf[258] ;
   int i, rc ;

   c4 =  d4->code_base ;
   if ( c4->error_code < 0 )  return 0 ;
   c4->error_code =  0 ;  /* Make sure it is not 'r4unique' or 'r4no_create'. */
   if ( d4lock_file( d4 ) != 0 )  return 0 ;

   i4 =  (I4INDEX *) y4memory_type_alloc( &c4->index_memory, c4->mem_start_index,
               sizeof(I4INDEX), c4->mem_expand_index, 0 ) ;
   if ( i4 == 0 )
   {
      e4error( c4, e4memory, (char *) 0 ) ;
      return 0 ;
   }

   i4->code_base =  c4 ;
   i4->data =  d4 ;

   memset(buf,0,sizeof(buf));
   if (  file_name != 0 )
      u4ncpy( buf, file_name, sizeof(buf) ) ;
   else
      u4ncpy( buf, d4->file.name, sizeof(buf) ) ;

   #ifdef S4FOX
      u4name_ext( buf, sizeof(buf), "CDX", 1 ) ;
   #else
      u4name_ext( buf, sizeof(buf), "MDX", 1 ) ;
   #endif
   c4upper( buf ) ;

   rc = h4create( &i4->file, c4, buf, 1 ) ;
   if ( rc != 0 )
   {
      if ( rc > 0 )
	 c4->error_code =  rc ;

      i4close(i4) ;
      return 0 ;
   }

   l4add( &d4->indexes, i4 ) ;

   #ifdef S4FOX
      i4->block_memory = y4memory_type( c4->mem_start_block,
           sizeof(B4BLOCK) + B4BLOCK_SIZE - sizeof(B4STD_HEADER) - sizeof(B4NODE_HEADER),
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
   
      i4->tag_index = (T4TAG *) y4alloc( c4->tag_memory ) ;
      if ( i4->tag_index == 0 )
         e4error(  c4, e4memory, (char *) 0 ) ;
   
      i4->tag_index->code_base = d4->code_base ;
      i4->tag_index->index = i4 ;
      i4->tag_index->header.type_code = 0xE0 ;  /* compound, compact */
      i4->tag_index->header.filter_len = 1 ;
      i4->tag_index->header.filter_pos = 1 ;
      i4->tag_index->header.expr_len = 1 ;
      i4->tag_index->header.expr_pos = 0 ;
      i4->tag_index->header.key_len = 10 ;
   
      u4name_piece( i4->tag_index->alias, sizeof(i4->tag_index->alias), buf, 0, 0 ) ;
   
      for ( i = 0; tag_data[i].name; i++ )
      {
         tag_ptr =  (T4TAG *) y4alloc( c4->tag_memory ) ;
         if ( tag_ptr == 0 )
            e4error(  c4, e4memory, (char *) 0 ) ;
   
         l4add( &i4->tags, tag_ptr ) ;
         tag_ptr->code_base =  d4->code_base ;
         tag_ptr->index = i4 ;
   
         u4ncpy( tag_ptr->alias, tag_data[i].name, sizeof(tag_ptr->alias) ) ;
         c4upper( tag_ptr->alias ) ;
   
         tag_ptr->header.type_code  =  0x60 ;  /* compact */
         if ( tag_data[i].unique )
         {
            tag_ptr->header.type_code +=  0x01 ;
            tag_ptr->unique_error =  tag_data[i].unique ;
               
            #ifdef S4DEBUG
               if ( tag_data[i].unique != e4unique &&
                    tag_data[i].unique != r4unique &&
                    tag_data[i].unique != r4unique_continue )
                  e4severe( e4parm, "i4create()", E4_PARM_UNI, (char *) 0 ) ;
            #endif
         }
         if ( tag_data[i].descending)
         {
            tag_ptr->header.descending = 1 ;
            #ifdef S4DEBUG
               if ( tag_data[i].descending != r4descending )
                  e4severe( e4parm, "i4create()", E4_PARM_FLA, (char *) 0 ) ;
            #endif
         }
   
         #ifdef S4DEBUG
            if ( tag_data[i].expression == 0 )
               e4severe( e4parm, "i4create()", E4_PARM_TAG, (char *) 0 ) ;
         #endif
   
         tag_ptr->expr =  e4parse( d4, tag_data[i].expression ) ;
         tag_ptr->header.expr_len =  strlen( tag_ptr->expr->source ) + 1 ;
         if ( tag_data[i].filter != 0 )
            if ( *(tag_data[i].filter) != '\0' )
            {
               tag_ptr->header.type_code +=  0x08 ;
               tag_ptr->filter =  e4parse( d4, tag_data[i].filter ) ;
               tag_ptr->header.filter_len =  strlen( tag_ptr->filter->source ) ;
            }
         tag_ptr->header.filter_len++ ;  /* minimum of 1, for the '\0' */
         tag_ptr->header.filter_pos = tag_ptr->header.expr_len ;
   
         tag_ptr->lock_pos =  L4LOCK_POS + i ;
         if ( c4->error_code < 0 )  break ;
      }
   #else                /* if not S4FOX   */
      i4->header.two =  2 ;
      u4yymmdd( i4->header.create_date ) ;
   
      if ( file_name == 0 )
         i4->header.is_production =  1 ;
   
      i4->header.num_slots = 0x30 ;
      i4->header.slot_size = 0x20 ;
   
      u4name_piece( i4->header.data_name, sizeof(i4->header.data_name), buf, 0, 0);
      i4->header.block_chunks =  c4->mem_size_block/512 ;
   
      #ifdef S4DEBUG
         if ( i4->header.block_chunks < 2 )   /* disallowed for compatibility reasons */
            e4severe( e4info, E4_INFO_BLO, E4_INFO_BLS, (char *) 0 ) ;
      #endif
   
      i4->header.block_rw =  i4->header.block_chunks*I4MULTIPLY ;
   
      i4->block_memory = y4memory_type( c4->mem_start_block,
              sizeof(B4BLOCK) + i4->header.block_rw -
              sizeof(B4KEY_DATA) - sizeof(short) - sizeof(char[6]),
              c4->mem_expand_block, 0 ) ;
      if ( i4->block_memory == 0 )
      {
         e4error( c4, e4memory, (char *) 0 ) ;
         i4close(i4) ;
         return 0 ;
      }
   
      for ( i = 0; tag_data[i].name; i++ )
      {
         i4->header.num_tags++ ;
   
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
   
         tag_ptr =  (T4TAG *) y4alloc( c4->tag_memory ) ;
         if ( tag_ptr == 0 )
            e4error(  c4, e4memory, (char *) 0 ) ;

         memset( tag_ptr,0, sizeof(T4TAG) ) ; 
   
         l4add( &i4->tags, tag_ptr ) ;
         tag_ptr->code_base =  d4->code_base ;
         tag_ptr->index = i4 ;
   
   
         u4ncpy( tag_ptr->alias, tag_data[i].name, sizeof(tag_ptr->alias) ) ;
         c4upper( tag_ptr->alias ) ;
   
         tag_ptr->header.type_code  =  0x10 ;
         if ( tag_data[i].unique )
         {
            tag_ptr->header.type_code +=  0x40 ;
            tag_ptr->header.unique     =  0x4000 ;
            tag_ptr->unique_error =  tag_data[i].unique ;
   
            #ifdef S4DEBUG
               if ( tag_data[i].unique != e4unique &&
                    tag_data[i].unique != r4unique &&
                    tag_data[i].unique != r4unique_continue )
                  e4severe( e4parm, "i4create()", E4_PARM_UNI, (char *) 0 ) ;
            #endif
         }
         if ( tag_data[i].descending)
         {
            tag_ptr->header.type_code += 0x08 ;
            #ifdef S4DEBUG
               if ( tag_data[i].descending != r4descending )
                  e4severe( e4parm, "i4create()", E4_PARM_FLA, (char *) 0 ) ;
            #endif
         }
   
         #ifdef S4DEBUG
            if ( tag_data[i].expression == 0 )
               e4severe( e4parm, "i4create()", E4_PARM_TAG, (char *) 0 ) ;
         #endif
   
         tag_ptr->expr =  e4parse( d4, tag_data[i].expression ) ;
         if ( tag_data[i].filter != 0 )
            if ( *(tag_data[i].filter) != '\0' )
               tag_ptr->filter =  e4parse( d4, tag_data[i].filter ) ;
   
         tag_ptr->lock_pos =  L4LOCK_POS + i ;
         if ( c4->error_code < 0 )  break ;
      }
   
      #ifdef S4DEBUG
         if ( i4->header.num_tags > 47 )
            e4severe( e4parm, "i4create()", E4_PARM_TOO, (char *) 0 ) ;
      #endif
   #endif

   if ( i4reindex(i4) == r4unique )
   {
      c4->error_code =  r4unique ;
      i4close( i4 ) ;
      return 0 ;
   }
   if ( file_name == 0 )
   {
      d4->has_mdx =  1 ;
      h4write( &d4->file, sizeof(D4DATA_HEADER)+sizeof(short)+sizeof(char[16]),
        &i4->data->has_mdx,sizeof(i4->data->has_mdx)) ;
   }
   if ( c4->error_code < 0 || c4->error_code == r4unique )
   {
      i4close( i4 ) ;
      return 0 ;
   }
   return i4 ;
}

#endif   /*  ifndef N4OTHER  */

#ifdef N4OTHER

I4INDEX *S4FUNCTION i4create( D4DATA *d4, char *file_name, T4TAG_INFO *tag_data )
{
   I4INDEX *i4 ;
   C4CODE *c4 ;
   char buf[258] ;
   int i, rc, num_files, len ;
   long pos ;

   c4 =  d4->code_base ;
   if ( c4->error_code < 0 )  return 0 ;
   c4->error_code =  0 ;  /* Make sure it is not 'r4unique' or 'r4no_create'. */
   if ( d4lock_file( d4 ) != 0 )  return 0 ;

   i4 = (I4INDEX *) y4memory_type_alloc( &c4->index_memory, c4->mem_start_index,
	       sizeof(I4INDEX), c4->mem_expand_index, 0 ) ;

   if ( i4 == 0 )
   {
      e4error( c4, e4memory, (char *) 0 ) ;
      return 0 ;
   }

   i4->code_base = c4 ;
   i4->data = d4 ;

   memset(buf,0,sizeof(buf));
   if (  file_name != 0 )
      u4ncpy( buf, file_name, sizeof(buf) ) ;
   else
      u4ncpy( buf, d4->file.name, sizeof(buf) ) ;

   u4ncpy( i4->alias, buf, sizeof( i4->alias ) ) ;
   c4trim_n( i4->alias, sizeof( i4->alias ) ) ;
   c4upper( i4->alias ) ;

   u4name_ext( buf, sizeof(buf), "CGP", 1 ) ;
   c4upper( buf ) ;

   rc = h4create( &i4->file, c4, buf, 1 ) ;
   if ( rc != 0 )
   {
      if ( rc > 0 )
	 c4->error_code =  rc ;

      h4close( &i4->file ) ;
      return 0 ;
   }

   rc = h4lock( &i4->file, L4LOCK_POS, L4LOCK_POS ) ;  /* lock the group file */
   if ( rc != 0 )
   {
      if ( rc > 0 )
	 c4->error_code = rc ;

      h4close( &i4->file ) ;
      return 0 ;
   }

   /* calculate # of files */
   for ( num_files = 0; tag_data[num_files].name; num_files++ ) ;

   pos = 0L ;
   h4write( &i4->file, pos, &num_files, sizeof( num_files ) ) ;
   pos += sizeof( num_files ) ;

   /* create the group file */
   for ( i = 0; tag_data[i].name; i++ )
   {
      len = strlen( tag_data[i].name ) ;
      h4write( &i4->file, pos, &len, sizeof( len ) ) ;
      pos += sizeof( len ) ;
      h4write( &i4->file, pos, tag_data[i].name, len ) ;
      pos += len ;
   }

   rc = h4unlock( &i4->file, L4LOCK_POS, L4LOCK_POS ) ;
   h4close ( &i4->file ) ;

   if ( rc != 0 )
   {
      if ( rc > 0 )
	 c4->error_code =  rc ;
      return 0 ;
   }

   i4->path = buf ;

   /* now create the actual tag files */
   for ( i = 0; tag_data[i].name; i++ )
      t4create( d4, &tag_data[i], i4 ) ;

   if ( c4->error_code < 0 || c4->error_code == r4unique )
   {
      i4close( i4 ) ;
      return 0 ;
   }

   l4add( &d4->indexes, i4 ) ;

   if ( i4reindex( i4 ) == r4unique )
   {
      i4close( i4 ) ;
      return 0 ;
   }

   return i4 ;
}

/* this function does not reindex if an 'i4ndx' is passed as a parameter */
/* this allows several creations before an actual reindex must occur */
T4TAG *S4FUNCTION t4create( D4DATA *d4, T4TAG_INFO *tag_data, I4INDEX *i4ndx )
{
   C4CODE  *c4 ;
   I4INDEX *i4 ;
   char   buf[258] ;
   T4TAG *t4 ;
   int rc ;

   c4 =  d4->code_base ;

   #ifdef S4DEBUG
      if( (tag_data->filter != 0 && *tag_data->filter != '\0')
          || tag_data->descending != 0 )
	 e4severe( e4parm, E4_PARM_FOR, (char *) 0 ) ;
   #endif

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
      l4add( &i4->data->indexes, i4 ) ;

   }
   else
      i4 = i4ndx ;

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

   if ( i4->block_memory == 0 )
      i4->block_memory = y4memory_type( c4->mem_start_block,
	      sizeof(B4BLOCK) + B4BLOCK_SIZE -
	      sizeof(B4KEY_DATA) - sizeof(short) - sizeof(char[2]),
	      c4->mem_expand_block, 0 ) ;

   if ( i4->block_memory == 0 )
   {
      e4error( c4, e4memory, (char *) 0 ) ;
      y4free( c4->tag_memory, t4 ) ;
      return 0 ;
   }

   if ( i4ndx != 0 && i4->path != 0)
   {
      rc = u4name_path( buf, sizeof(buf), i4ndx->path ) ;
      u4ncpy( buf+rc, tag_data->name, sizeof(buf)-rc ) ;
   }
   else
      u4ncpy( buf, tag_data->name, sizeof(buf) ) ;

   c4upper(buf) ;

   #ifdef S4NDX
      u4name_ext( buf, sizeof(buf), "NDX", 0 ) ;
   #else
      #ifdef S4CLIPPER
         u4name_ext( buf, sizeof(buf), "NTX", 0 ) ;
      #endif
   #endif

   u4name_piece( t4->alias, sizeof( t4->alias ), tag_data->name, 0, 0 ) ;
   c4upper( t4->alias ) ;

   rc =  h4create( &t4->file, c4, buf, 1 ) ;
   if ( rc != 0 )
   {
      y4free( c4->tag_memory, t4 ) ;
      return 0 ;
   }

   #ifdef S4NDX
      t4->header.eof = 2 ;
      t4->header.root = 1 ;
   #else
      #ifdef S4CLIPPER
         t4->header.eof = 0 ;
         t4->header.root = 1024 ;
      #endif
   #endif

   if ( tag_data->unique )
   {
      #ifdef S4NDX
	 t4->header.unique     =  0x4000 ;
      #else
         #ifdef S4CLIPPER
	    t4->header.unique     =  0x01 ;
         #endif
      #endif

      t4->unique_error =  tag_data->unique ;

      #ifdef S4DEBUG
	 if ( tag_data->unique != e4unique &&
	      tag_data->unique != r4unique &&
	      tag_data->unique != r4unique_continue )
	    e4severe( e4parm, "t4create()", E4_PARM_UNI, (char *) 0 ) ;
      #endif
   }

   #ifdef S4DEBUG
      if ( tag_data->expression == 0 )
	 e4severe( e4parm, "t4create()", E4_PARM_TAG, (char *) 0 ) ;
   #endif

   t4->expr =  e4parse( d4, tag_data->expression ) ;

   t4->lock_pos =  L4LOCK_POS ;
   if ( c4->error_code < 0 )
   {
      y4free( c4->tag_memory, t4 ) ;
      return 0 ;
   }

   t4->code_base = d4->code_base ;
   t4->index = i4 ;

   /* add the tag to the index list */
   l4add( &i4->tags, t4 ) ;

   if ( i4ndx == 0 )   /* single create, so reindex now */
      if ( t4reindex( t4 ) == r4unique )
      {
         c4->error_code =  r4unique ;
         y4free( c4->tag_memory, t4 ) ;
         return 0 ;
      }

   return t4 ;
}
#endif   /*  ifdef N4OTHER  */
