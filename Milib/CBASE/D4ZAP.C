/* d4zap.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

S4FUNCTION d4zap( D4DATA *d4, long r1, long r2 )
{
   int rc ; 

   rc =  d4lock_file(d4) ;   if ( rc != 0 )  return rc ;
   rc =  d4lock_index(d4) ;  if ( rc != 0 )  return rc ;
   if ( (rc =  d4flush_record(d4)) != 0 )  return rc ;

   d4zap_data( d4, r1, r2 ) ;
   if ( d4reccount(d4) == 0 )
      d4->bof_flag =  d4->eof_flag =  1 ;
   else
      d4->bof_flag =  d4->eof_flag =  0 ;
   return d4reindex(d4) ;
}

int S4FUNCTION d4zap_data( D4DATA *d4, long start_rec, long end_rec )
{
   long cur_count, i_rec ;
   char *rd_buf, *wr_buf ;
   H4SEQ_READ   rd ;
   H4SEQ_WRITE  wr ;
   unsigned  buf_size ;

   if ( d4->code_base->error_code < 0 )  return -1 ;
   d4->file_changed =  1 ;

   cur_count =  d4reccount(d4) ;
   if ( start_rec > cur_count )  return 0 ;
   if ( end_rec < start_rec )  return 0 ;
   if ( end_rec > cur_count )  end_rec =  cur_count ;

   if ( d4->code_base->error_code < 0 )  return -1 ;

   rd_buf = wr_buf =  0 ;
   buf_size =  d4->code_base->mem_size_buffer ;

   for (; buf_size > d4->record_width; buf_size -= 0x800 )
   {
      if ( rd_buf != 0 ) u4free( rd_buf ) ;
      if ( wr_buf != 0 ) u4free( wr_buf ) ;

      rd_buf =  (char *) u4alloc( buf_size ) ;
      wr_buf =  (char *) u4alloc( buf_size ) ;
      if ( rd_buf != 0  &&  wr_buf != 0 )  break ;
   }
   h4seq_read_init(  &rd, &d4->file, d4rec_pos(d4,end_rec+1), rd_buf, (rd_buf == 0) ? 0 : buf_size ) ;
   h4seq_write_init( &wr, &d4->file, d4rec_pos(d4,start_rec), wr_buf, (wr_buf == 0) ? 0 : buf_size ) ;

   for ( i_rec= end_rec+1L; i_rec <= cur_count; i_rec++ )
   {
      h4seq_read_all( &rd, d4->record, d4->record_width ) ;
      h4seq_write( &wr, d4->record, d4->record_width ) ;
   }

   h4seq_write( &wr, "\032", 1 ) ;
   h4seq_write_flush( &wr ) ;

   if ( rd_buf != 0 )  u4free(rd_buf) ;
   if ( wr_buf != 0 )  u4free(wr_buf) ;

   if ( d4->code_base->error_code < 0 )  return -1 ;

   d4->header.num_recs =  cur_count- (end_rec-start_rec+1) ;
   d4->rec_num =  -1 ;
   d4->rec_num_old =  -1 ;
   memset( d4->record, ' ', d4->record_width ) ;

   return  h4length_set( &d4->file, d4rec_pos(d4,d4->header.num_recs+1)+ 1L ) ;
}


