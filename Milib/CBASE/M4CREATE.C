/* m4create.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved.  */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int m4file_create( M4FILE *m4file, C4CODE *c4, D4DATA *d4, char *name )
{
   M4MEMO_HEADER *header_ptr ;
   char buf[258] ;
   int rc ;

   m4file->data =  d4 ;

   #ifndef S4MNDX
      #ifdef S4DEBUG
         if ( sizeof(M4MEMO_HEADER) > c4->mem_size_block )
	    e4severe( e4info, "m4file_create()", (char *) 0 ) ;
   
         #ifdef S4MFOX
            if ( c4->mem_size_block < 32 ||  c4->mem_size_block > 512*32 )
         #else
            if ( c4->mem_size_block % 512  != 0  ||  c4->mem_size_block > 512*32 )
         #endif
               e4severe( e4info, E4_INFO_C4C, (char *) 0 ) ;
      #endif
   #endif

   if ( (header_ptr = (M4MEMO_HEADER *) u4alloc(c4->mem_size_block)) == 0 )
   {
      e4error( c4, e4memory, (char *) 0 ) ;
      return -1 ;
   }

   #ifdef S4MFOX
      m4file->block_size =  c4->mem_size_block ;
      if ( m4file->block_size > 512 )
         header_ptr->next_block =  x4reverse( 1L ) ;
      else
         header_ptr->next_block =  x4reverse( B4BLOCK_SIZE/m4file->block_size ) ;
      header_ptr->block_size =  x4reverse_short( c4->mem_size_block ) ;
   #else
      #ifdef S4MNDX
         m4file->block_size = M4MEMO_SIZE ;
         header_ptr->next_block =  1 ;
      #else
         header_ptr->next_block =  1 ;
         header_ptr->x102   =  0x102 ;
         m4file->block_size =  header_ptr->block_size =  c4->mem_size_block ;
      #endif
   #endif

   if ( name == 0 )
      h4temp( &m4file->file, c4, 0, 1 ) ;
   else
   {
      u4ncpy( buf, name, sizeof(buf) ) ;

      #ifdef S4MFOX
         u4name_ext( buf, sizeof(buf), "FPT", 1 ) ;
      #else
         u4name_ext( buf, sizeof(buf), "DBT", 1 ) ;
      #endif

      h4create( &m4file->file, c4, name, 1 ) ;
   }

   rc = h4write( &m4file->file, 0L, header_ptr, sizeof(M4MEMO_HEADER) ) ;
   u4free( header_ptr ) ;
   return rc ;
}

int  m4file_dump( M4FILE *m4, long memo_id, char *ptr, unsigned len )
{
   long pos ;
   int  rc ;
   #ifdef S4MNDX
      char one_a = 0x1A ;
   #else
      M4MEMO_BLOCK  memo_block ;

      #ifdef S4MFOX
         memo_block.type = x4reverse( 1L ) ;
         memo_block.num_chars =  x4reverse( len ) ;
      #else
         memo_block.minus_one =  -1 ;
         memo_block.start_pos =  sizeof(long) + 2*sizeof(short) ;
         memo_block.num_chars =  memo_block.start_pos + len ;
      #endif
   #endif

   pos =  memo_id*m4->block_size ;

   #ifndef S4MNDX
      rc = h4write( &m4->file, pos, &memo_block, sizeof(memo_block) ) ;
      if ( rc != 0 ) return rc ;
      #ifdef S4MFOX
         pos += sizeof(memo_block) ;
      #else
         pos +=  memo_block.start_pos ;
      #endif
   #endif

   #ifdef S4MNDX
      rc = h4write( &m4->file, pos, ptr, len ) ;
      if ( rc != 0 ) return rc ;
      return h4write( &m4->file, pos+len, &one_a, 1 ) ;
   #else
      return  h4write( &m4->file, pos, ptr, len ) ;
   #endif
}

