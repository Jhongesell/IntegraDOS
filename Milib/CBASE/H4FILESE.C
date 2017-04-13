/* h4filese.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifdef S4WINDOWS
   #include <windows.h>
#endif

void  S4FUNCTION h4seq_read_init( H4SEQ_READ *q4, H4FILE *h4, long start_offset, void *ptr, unsigned ptr_len )
{
   memset( (void *)q4, 0, sizeof(H4SEQ_READ) ) ;

   q4->total =  ptr_len & 0xFC00 ;  /* Make it a multiple of 1K */
   q4->pos   =  start_offset ;
   q4->buffer=  (char *) ptr ;
   q4->next_read_len =  q4->total - (unsigned)(start_offset % 0x400) ;
   q4->file  =  h4 ;
}

unsigned S4FUNCTION h4seq_read( H4SEQ_READ *q4, void *ptr, unsigned ptr_len )
{
   C4CODE *c4 ;
   unsigned urc, buffer_i, copy_bytes ;
   long  lrc ;

   c4 =  q4->file->code_base ;
   if ( c4->error_code < 0 ) return -1 ;

   #ifdef S4DEBUG
      if ( q4->file == 0 ) e4severe( e4info, "h4seq_read_read", (char *) 0);
   #endif

   if ( q4->buffer == 0 )
   {
      urc =  h4read( q4->file, q4->pos, ptr, ptr_len ) ;
      q4->pos +=  urc ;
      return urc ;
   }

   if ( q4->avail == 0 )
   {
      #ifdef S4DEBUG
         if ( q4->pos < 0 )
            e4severe( e4result, "h4seq_read()", (char *) 0 ) ;
      #endif

      #ifdef S4WINDOWS
	 lrc =  _llseek( q4->file->hand, q4->pos, 0 ) ;
      #else
         lrc =  lseek( q4->file->hand, q4->pos, 0 ) ;
      #endif
      if ( lrc < 0 )
      {
         h4read_error( q4->file ) ;
         return 0 ;
      }

      #ifdef S4WINDOWS
         q4->avail =  q4->working =  (unsigned) _lread( q4->file->hand, 
                 q4->buffer, q4->next_read_len ) ;
      #else
         q4->avail =  q4->working =  (unsigned) read( q4->file->hand, 
                 q4->buffer, q4->next_read_len ) ;
      #endif

      if ( q4->working == UINT_MAX )
      {
         h4read_error( q4->file ) ;
         return 0 ;
      }

      #ifdef S4DEBUG
	 /* Make sure reading is aligned correctly for maximum speed */
	 if ( (q4->pos+q4->next_read_len) % 0x400  != 0  &&  q4->avail != 0 )
	    e4severe( e4result, "h4seq_read()", (char *) 0 ) ;
      #endif
      q4->pos +=  q4->working ;
      q4->next_read_len =  q4->total ;
   }

   if ( q4->avail >= ptr_len )
   {

      buffer_i =  q4->working - q4->avail ;
      memcpy( ptr, q4->buffer+ buffer_i, ptr_len ) ;
      q4->avail -=  ptr_len ;
      return  ptr_len ;
   }
   else
   {
      if ( q4->avail == 0 )
         return 0 ;

      buffer_i =  q4->working - q4->avail ;
      memcpy( ptr, q4->buffer+ buffer_i, q4->avail ) ;

      copy_bytes =  q4->avail ;
      q4->avail =  0 ;

      urc =  h4seq_read( q4, (char *) ptr + copy_bytes, ptr_len - copy_bytes);
      if ( c4->error_code < 0 )  return 0 ;

      return  urc + copy_bytes ;
   }
}

int S4FUNCTION h4seq_read_all( H4SEQ_READ *q4, void *ptr, unsigned ptr_len )
{
   unsigned len_read ;

   len_read =  h4seq_read( q4, ptr, ptr_len ) ;
   if ( q4->file->code_base->error_code < 0 )  return -1 ;

   if ( len_read != ptr_len )  return h4read_error( q4->file ) ;
   return 0 ;
}

int S4FUNCTION h4seq_write_flush( H4SEQ_WRITE *q4 )
{
   unsigned to_write ;

   if ( q4->file->code_base->error_code < 0 )  return -1 ;
   if ( q4->buffer == 0 )  return 0 ;

   #ifdef S4DEBUG
      if( q4->pos < 0 )
	 e4severe( e4result, (char *) 0 ) ;
   #endif

   #ifdef S4WINDOWS
      if ( _llseek( q4->file->hand, q4->pos, 0 ) != q4->pos )
   #else
		if ( lseek( q4->file->hand, q4->pos, 0) != q4->pos )
   #endif
	 return e4error( q4->file->code_base, e4write, q4->file->name, (char *) 0 ) ;

   to_write =  q4->working - q4->avail ;

   #ifdef S4WINDOWS
      if ((unsigned) _lwrite( q4->file->hand, q4->buffer, to_write) != to_write)
   #else
      if ((unsigned) write( q4->file->hand, q4->buffer, to_write) != to_write)
   #endif
	 return e4error( q4->file->code_base, e4write, q4->file->name, (char *) 0 ) ;

   q4->pos +=  to_write ;

   q4->avail =  q4->working =  q4->total ;
   return 0 ;
}

void  S4FUNCTION h4seq_write_init( H4SEQ_WRITE *q4, H4FILE *h4, long start_offset, void *ptr, unsigned ptr_len )
{
   memset( (void *)q4, 0, sizeof(H4SEQ_WRITE) ) ;

   q4->file =  h4 ;

   q4->total =  ptr_len & 0xFC00 ;  /* Make it a multiple of 1K */
   q4->pos =  start_offset ;

   q4->buffer =  (char *) ptr ;
   q4->avail =  q4->working =  q4->total - (unsigned)(start_offset % 0x400) ;
}

int  S4FUNCTION h4seq_write_repeat( H4SEQ_WRITE *q4, long n, char ch )
{
   char buf[512] ;
   memset( (void *)buf, ch, sizeof(buf) ) ;

   while ( n > sizeof(buf) )
   {
      if ( h4seq_write( q4, buf, (unsigned) sizeof(buf)) < 0 )  return -1 ;
      n -= sizeof(buf) ;
   }

   return h4seq_write( q4, buf, (unsigned) n ) ;
}

int  S4FUNCTION h4seq_write( H4SEQ_WRITE *q4, void *ptr, unsigned ptr_len )
{
   int rc ;
   unsigned  first_len ;

   if ( q4->file->code_base->error_code < 0 )  return -1 ;

   if ( q4->buffer == 0 )
   {
      rc =  h4write( q4->file, q4->pos, ptr, ptr_len ) ;
      q4->pos +=  ptr_len ;
      return rc ;
   }

   if ( q4->avail == 0 )
      if ( h4seq_write_flush(q4) < 0 )  return -1 ;

   #ifdef S4DEBUG
      if ( q4->avail <= 0 )
         e4severe( e4info, "h4seq_write()", (char *) 0 ) ;
   #endif

   if ( q4->avail >= ptr_len )
   {
      #ifdef S4DEBUG
	 if ( q4->working < q4->avail ||  
              q4->working - q4->avail+ptr_len  >  q4->total )
	    e4severe( e4result, "h4seq_write()", (char *) 0 ) ;
      #endif
      memcpy( q4->buffer+ (q4->working-q4->avail), ptr, ptr_len ) ;
      q4->avail -= ptr_len ;
      return 0 ;
   }
   else
   {
      first_len =  q4->avail ;

      #ifdef S4DEBUG
	 if ( q4->working < q4->avail || q4->working > q4->total )
	    e4severe( e4result, "h4seq_write()", (char *) 0 ) ;
      #endif
      memcpy( q4->buffer+ (q4->working-q4->avail), ptr, q4->avail ) ;
      q4->avail =  0 ;

      return h4seq_write( q4, (char *) ptr+first_len, ptr_len-first_len ) ;
   }
}
