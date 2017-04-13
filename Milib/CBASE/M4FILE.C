/* m4file.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved.  */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int  m4file_open( M4FILE *m4, D4DATA *d4, char *name )
{
   M4MEMO_HEADER  header ;
   int rc ;

   m4->data =  d4 ;
   h4open( &m4->file, d4->code_base, name, 1 ) ;
   rc =  h4read_all( &m4->file, 0L, &header, sizeof(header) ) ;

   #ifdef S4MFOX
      m4->block_size =  x4reverse_short( header.block_size ) ;
   #else
      #ifdef S4MNDX
         m4->block_size =  M4MEMO_SIZE ;
      #else
         m4->block_size =  header.block_size ;
      #endif
   #endif

   return rc ;
}

int  m4file_read( M4FILE *m4, long memo_id, char **ptr_ptr, unsigned *len_ptr )
{
   long  pos ;
   unsigned final_len ;
   #ifdef S4MNDX
      long to_read, amt_read, read_max, len_read ;
      char *t_ptr ;
   #else
      M4MEMO_BLOCK  memo_block ;
   #endif

   if ( memo_id <= 0L )
   {
      *len_ptr =  0 ;
      return 0 ;
   }
    
   pos =  memo_id * m4->block_size ;

   #ifdef S4MNDX
      amt_read = 0 ;
      read_max = *len_ptr ;

      for(;;)
      {
         to_read = M4MEMO_SIZE ;
         if ( to_read > read_max )  /* must increase the memo buffer size */
         {
            t_ptr =  (char *) u4alloc( read_max + to_read ) ;
            if ( t_ptr == 0 )
               return e4error( m4->file.code_base, e4memory, (char *) 0 ) ;
            memcpy( t_ptr, *ptr_ptr, read_max ) ;
            read_max += to_read ;
            u4free( *ptr_ptr ) ;
            *ptr_ptr = t_ptr ;
         }
         to_read = read_max ;

	 len_read = h4read( &m4->file, pos + amt_read, *ptr_ptr + amt_read, to_read ) ;
	 if ( len_read <= 0 ) return -1 ;

         for ( ; amt_read <= read_max && len_read > 0 ; amt_read++, len_read-- ) 
            if ( (*ptr_ptr)[amt_read] == 0x1A ) /* if done */
            {
	       (*ptr_ptr)[amt_read] =  '\000' ;
               *len_ptr = amt_read ;
               return 0 ;
            }
	 read_max -= len_read ;
      }
   #else
      if ( h4read_all( &m4->file, pos, &memo_block, sizeof(M4MEMO_BLOCK) ) < 0)
         return -1 ;

      #ifdef S4MFOX
         memo_block.num_chars =  x4reverse( memo_block.num_chars ) ;
      #endif

      if ( memo_block.num_chars >= USHRT_MAX )
         return e4error( m4->file.code_base, e4info, "m4file_read()", (char *) 0 ) ;

      #ifdef S4MFOX
         if ( memo_block.num_chars > *len_ptr )
         {
            u4free( *ptr_ptr ) ;
            *ptr_ptr =  (char *) u4alloc( memo_block.num_chars + 1 ) ;
            if ( *ptr_ptr == 0 )
               return e4error( m4->file.code_base, e4memory, (char *) 0 ) ;
         }
      
         *len_ptr = ( unsigned )memo_block.num_chars ;
      
         return h4read_all( &m4->file, pos+  2*sizeof(long), *ptr_ptr, memo_block.num_chars ) ;
      #else
         final_len =  (unsigned) memo_block.num_chars - 2*sizeof(short)-sizeof(long) ;
         if ( final_len > *len_ptr )
         {
            u4free( *ptr_ptr ) ;
            *ptr_ptr =  (char *) u4alloc( final_len+1 ) ;
            if ( *ptr_ptr == 0 )
               return e4error( m4->file.code_base, e4memory, (char *) 0 ) ;
         }
         *len_ptr = final_len ;
      
         return h4read_all( &m4->file, pos+ memo_block.start_pos, *ptr_ptr, final_len ) ;
      #endif
   #endif
}
   
int  m4file_write( M4FILE *m4, long *memo_id_ptr, char *ptr, unsigned ptr_len )
{
   M4MEMO_HEADER mh ;
   unsigned len_read ;
   int  str_written, rc, str_num_blocks ;
   long pos, file_len, extra_len ;
   #ifdef S4MNDX
      char  buf[M4MEMO_SIZE] ;
      int   read_size, i ;
   #else
      M4MEMO_BLOCK old_memo_block ;
      #ifdef S4MFOX
	 long block_no ;
	 unsigned n_entry_blks ;
      #else
	 M4CHAIN_ENTRY new_entry, cur, prev ;
	 long prev_prev_entry, prev_prev_num ;
      #endif
   #endif

   #ifdef S4DEBUG
      if ( memo_id_ptr == 0 )
         e4severe( e4parm, "m4file_write()", (char *) 0 ) ;
   #endif

   #ifdef S4MNDX
      if ( ptr_len == 0 )
      {
         *memo_id_ptr =  0L ;
         return 0 ;
      }

      str_num_blocks =  (ptr_len + m4->block_size-1) / m4->block_size ;

      if ( *memo_id_ptr <= 0L )
	 *memo_id_ptr =  0L ;
      else    /* read in old record to see if new entry can fit */
      {
	 read_size =  0 ;
	 pos =  *memo_id_ptr * m4->block_size ;

	 do
	 {
	    read_size +=  M4MEMO_SIZE ;

	    len_read = h4read( &m4->file, pos, buf, M4MEMO_SIZE ) ;
	    if ( len_read <= 0 )
	       return h4read_error( &m4->file ) ;

	    for ( i=0 ; i < len_read ; i++ )
	       if ( buf[i] == (char) 0x1A )  break ;

	    #ifdef S4DEBUG
	       if ( buf[i] != (char) 0x1A && len_read != M4MEMO_SIZE )
		  return e4error( m4->file.code_base, e4info, E4_INFO_CMF, (char *) 0 ) ;
	    #endif

	    pos += M4MEMO_SIZE ;
	 } while ( i >= M4MEMO_SIZE && buf[i] != (char) 0x1A ) ;  /* Continue if Esc is not located */

	 if ( read_size <= ptr_len )   /* there is not room */
	    *memo_id_ptr = 0 ;
      }

      if ( *memo_id_ptr == 0 )   /* add entry at eof */
      {
         len_read =  h4read( &m4->file, 0, &mh, sizeof( mh ) ) ;
         if ( m4->data->code_base->error_code < 0 )
            return -1 ;
         if ( len_read != sizeof( mh ) )
            return h4read_error( &m4->file ) ;
	 *memo_id_ptr =  mh.next_block ;
         mh.next_block = *memo_id_ptr + str_num_blocks ;
         h4write( &m4->file, 0, &mh, sizeof( mh ) ) ;
      }

      if ( m4file_dump( m4, *memo_id_ptr, ptr, ptr_len ) < 0 )
         return -1 ;
            
      return 0 ;
   #else
      #ifdef S4MFOX
         if ( ptr_len == 0 )
         {
            *memo_id_ptr =  0L ;
            return 0 ;
         }
      
         str_num_blocks =  (ptr_len + sizeof(M4MEMO_BLOCK) + m4->block_size-1) / m4->block_size ;
      
         if ( *memo_id_ptr <= 0L )
            block_no =  0L ;
         else
         {
            block_no =  *memo_id_ptr ;
            pos =  block_no * m4->block_size ;
            h4read_all( &m4->file, pos, (char *) &old_memo_block, sizeof(old_memo_block) ) ;
            n_entry_blks = ((unsigned)old_memo_block.num_chars + m4->block_size-1)/ m4->block_size ;
      
            if ( n_entry_blks != str_num_blocks  &&  m4->data != 0 )
            {
               int rc =  d4lock_append( m4->data ) ;
               if ( rc != 0  ) return rc ;
            }
         }
      
         if ( n_entry_blks >= str_num_blocks && block_no )  /* write to existing position */
            *memo_id_ptr =  block_no ;
         else  /* read in header record */
         {
            len_read =  h4read( &m4->file, 0, &mh, sizeof( mh ) ) ;
            if ( m4->data->code_base->error_code < 0 )
               return -1 ;
            if ( len_read != sizeof( mh ) )
               return h4read_error( &m4->file ) ;
            *memo_id_ptr = x4reverse( mh.next_block ) ;
            mh.next_block = x4reverse( *memo_id_ptr + str_num_blocks ) ;
            h4write( &m4->file, 0, &mh, sizeof( mh ) ) ;
         }
      
         if ( m4file_dump( m4, *memo_id_ptr, ptr, ptr_len ) < 0 )
            return -1 ;
   
         return 0 ;
   
      #else       /*  if not S4MFOX  */
   
         memset( (void *)&new_entry, 0, sizeof(new_entry) ) ;
         new_entry.block_no =  *memo_id_ptr ;
      
         str_written =  0 ;
         if ( ptr_len == 0 )
         {
            str_written =  1 ;
            *memo_id_ptr = 0 ;
         }
      
         /* Initialize information about the old memo entry */
         if ( new_entry.block_no <= 0L )
         {
            if ( str_written )
            {
               *memo_id_ptr =  0L ;
               return 0 ;
            }
            new_entry.num =  0 ;
         }
         else
         {
            pos =  new_entry.block_no * m4->block_size ;
            h4read_all( &m4->file, pos, (char *) &old_memo_block, sizeof(old_memo_block) ) ;
            new_entry.num  = ((unsigned)old_memo_block.num_chars + m4->block_size-1)/ m4->block_size ;
         }
      
         str_num_blocks =  (ptr_len+2*sizeof(short)+sizeof(long)+ m4->block_size-1) / m4->block_size ;
      
         if ( new_entry.num != str_num_blocks  &&  m4->data != 0 )
         {
            rc =  d4lock_append( m4->data ) ;
            if ( rc != 0  ) return rc ;
         }
      
         if ( new_entry.num >= str_num_blocks  &&  ! str_written )
         {
            *memo_id_ptr =  new_entry.block_no + new_entry.num - str_num_blocks ;
            if ( m4file_dump( m4, *memo_id_ptr, ptr, ptr_len ) < 0 )  return -1 ;
            str_written =  1 ;
            if ( new_entry.num == str_num_blocks )  return 0 ;
            new_entry.num -=  str_num_blocks ;
         }
      
         /* Initialize 'chain' */
         memset( (void *)&cur, 0, sizeof(cur) ) ;
         memset( (void *)&prev, 0, sizeof(prev) ) ;
      
         for(;;)
         {
            if ( m4->data->code_base->error_code < 0 )  return -1 ;
      
            m4file_chain_flush( m4, &prev ) ;
            prev_prev_entry = prev.block_no ;
            prev_prev_num = prev.num ;
      
            memcpy( (void *)&prev, (void *)&cur, sizeof(prev) ) ;
      
            if ( new_entry.block_no > 0  &&  prev.next > new_entry.block_no )
            {
               /* See if the new entry fits in here */
               memcpy( (void *)&cur, (void *)&new_entry, sizeof(cur) ) ;
               new_entry.block_no =  0 ;
               cur.next  =  prev.next ;
               prev.next =  cur.block_no ;
               cur.to_disk =  prev.to_disk =  1 ;
            }
            else
               m4file_chain_skip( m4, &cur ) ;
      
            /* See if the entries can be combined ! */
            if ( prev.block_no + prev.num == cur.block_no  &&  prev.num != 0 )
            {
               /* 'cur' becomes the combined groups. */
               prev.to_disk =  0 ;
               cur.to_disk  =  1 ;
      
               cur.block_no =  prev.block_no ;
               if ( cur.num >= 0 )
                  cur.num  +=  prev.num ;
               prev.block_no = prev_prev_entry ;
               prev.num = prev_prev_num ;
            }
      
            if ( str_written )
            {
               if ( new_entry.block_no == 0 )
               {
                  m4file_chain_flush( m4, &prev ) ;
                  m4file_chain_flush( m4, &cur ) ;
                  return 0 ;
               }
            }
            else  /* 'str' is not yet written, try the current entry */
            {
               if ( cur.next == -1 )  /* End of file */
                  cur.num =  str_num_blocks ;
      
               if ( cur.num >= str_num_blocks )
               { 
                  cur.num -=  str_num_blocks ;
                  m4file_dump( m4, *memo_id_ptr =  cur.block_no+cur.num, ptr, ptr_len ) ;
                  if ( cur.next == -1 ) /* if end of file */
                  {
                     /* For dBASE IV compatibility */
                     file_len  =  h4length( &m4->file ) ;
                     extra_len =  m4->block_size -  file_len % m4->block_size ;
                     if ( extra_len != m4->block_size )
                        h4length_set( &m4->file, file_len+extra_len ) ;
                  }
      
                  str_written =  1 ;
      
                  if ( cur.num == 0 )
                  {
                     if ( cur.next == -1 ) /* End of file */
                        prev.next =  cur.block_no + str_num_blocks ;
                     else
                        prev.next =  cur.next ;
                     prev.to_disk =  1 ;
                     cur.to_disk =  0 ;
                  }
                  else
                     cur.to_disk =  1 ;
               }
            }
         }
      #endif      
   #endif      
}

