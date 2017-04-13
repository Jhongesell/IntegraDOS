/* m4memo.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved.  */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int S4FUNCTION d4memo_compress( D4DATA *d4 )
{
   char *rd_buf, *wr_buf, *ptr ;
   H4SEQ_READ   rd ;
   H4SEQ_WRITE  wr ;
   M4FILE new_file ;
   unsigned  buf_size, ptr_len ;
   long cur_count, i_rec, new_id ;
   int  rc, i ;
   F4FIELD *field ;

   if ( d4->code_base->error_code < 0 )  return -1 ;
   if ( d4->n_fields_memo == 0 )  return 0 ;

   if ( (rc = d4lock_file(d4)) != 0 )  return rc ;
   if ( (rc = d4flush_all(d4)) != 0 )  return rc ;

   if ( m4file_create( &new_file, d4->code_base, d4, 0 ) < 0 )  return -1 ;

   rd_buf   =  wr_buf =  0 ;
   buf_size =  d4->code_base->mem_size_buffer ;

   for (; buf_size > d4->record_width; buf_size -= 0x800 )
   {
      if ( rd_buf != 0 )  u4free( rd_buf ) ;
      if ( wr_buf != 0 )  u4free( wr_buf ) ;

      rd_buf =  (char *) u4alloc( buf_size ) ;
      if ( rd_buf == 0 )  continue ;

      wr_buf =  (char *) u4alloc( buf_size ) ;
      if ( wr_buf != 0 )  break ;
   }

   h4seq_read_init(  &rd, &d4->file, d4rec_pos(d4,1L), rd_buf, buf_size ) ;
   h4seq_write_init( &wr, &d4->file, d4rec_pos(d4,1L), wr_buf, buf_size ) ;

   cur_count =  d4reccount(d4) ;

   ptr =  0 ;
   ptr_len =  0 ;

   for ( i_rec= 1L; i_rec <= cur_count; i_rec++ )
   {
      if ( h4seq_read_all( &rd, d4->record, d4->record_width ) < 0 )  break ;

      for ( i = 0; i < d4->n_fields_memo; i++ )
      {
	 field =  d4->fields_memo[i].field ;
	 m4file_read( &d4->memo_file, f4long(field), &ptr, &ptr_len ) ;

	 new_id =  0L ;
	 m4file_write( &new_file, &new_id, ptr, ptr_len ) ;

	 c4ltoa45( new_id, f4ptr(field), - field->len ) ;
      }

      h4seq_write( &wr, d4->record, d4->record_width ) ;
   }

   h4seq_write_flush(&wr) ;

   u4free( ptr ) ;
   u4free( rd_buf ) ;
   u4free( wr_buf ) ;

   return h4replace( &d4->memo_file.file, &new_file.file ) ;
}
                          /* Make the memo file entries current */
int S4FUNCTION d4validate_memo_ids( D4DATA *d4 )  
{
   int rc, i ;
   char *from_ptr ;

   if ( d4->memo_validated )  return 0 ;

   if ( d4->rec_num > 0 )
   {
      rc =  d4lock( d4, d4->rec_num ) ;
      if ( rc != 0 )  return rc ;
   }
   if ( d4read_old(d4,d4->rec_num) < 0 )  return -1 ;

   for ( i = 0; i < d4->n_fields_memo; i++ )
   {
      from_ptr =  d4->record_old + d4->fields_memo[i].field->offset ;
      memcpy( f4ptr(d4->fields_memo[i].field), from_ptr, 10 ) ;
   }
   d4->memo_validated = 1 ;
   return 0 ;
}

#ifndef S4MFOX
#ifndef S4MNDX

int m4file_chain_flush( M4FILE *m4, M4CHAIN_ENTRY *chain )
{
   if ( chain->to_disk )
   {
      chain->to_disk =  0 ;
      return h4write( &m4->file, chain->block_no * m4->block_size, chain, 2*sizeof(long) ) ;
   }
   return 0 ;
}

int m4file_chain_skip( M4FILE *m4, M4CHAIN_ENTRY *chain )
{
   unsigned len_read ;

   chain->to_disk  =  0 ;
   chain->block_no =  chain->next ;

   if ( chain->next < 0 )
      len_read =  0 ;
   else
      len_read =  h4read( &m4->file, chain->next*m4->block_size, chain, sizeof(chain->next)+sizeof(chain->num) ) ;
   if ( m4->data->code_base->error_code < 0 )  return -1 ;
   if ( len_read == 0 )
   {
      chain->num  =  -1 ;
      chain->next =  -1 ;
      return 0 ;
   }
   if ( len_read != sizeof(chain->next)+sizeof(chain->num) )
      return h4read_error( &m4->file ) ;
   return 0 ;
}
#endif
#endif
