/* d4create.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

typedef struct
{
   D4DATA_HEADER h ;
   unsigned short  record_len ;
   char filler[20] ;
} D4CREATE_HEADER ;

D4DATA *S4FUNCTION d4create( C4CODE *c4, char *name, F4FIELD_INFO *field_data, T4TAG_INFO *tag_info )
{
   unsigned n_flds ;
   int      is_memo, i ;
   long     calc_record_len, lheader_len ;
   char     buf[258], buffer[0x800] ;
   H4SEQ_WRITE seq_write ;
   D4DATA  *d4 ;
   D4CREATE_HEADER create_header ;
   F4FIELD_IMAGE   create_field_image ;
   H4FILE   file ;
   #ifdef S4DEBUG
      char t ;
      int  is_illegal, len, dec ;
   #endif

   is_memo = 0 ;
   calc_record_len = 1L ;
   n_flds =  0 ;

   for (; field_data[n_flds].name; n_flds++ )
   {
      if ( field_data[n_flds].type == 'M' )  is_memo =  1 ;
      calc_record_len +=  field_data[n_flds].len ;
   }

   if ( calc_record_len >= USHRT_MAX ) /* Increment for deletion flag. */
   {
      e4error( c4, e4record_len, name, (char *) 0 ) ;
      return 0 ;
   }

   lheader_len =  (long)n_flds * 32  + 34 ;
   if ( lheader_len >= USHRT_MAX )
   {
      e4error( c4, e4create, E4_CREATE_TOO, name, (char *) 0 ) ;
      return 0 ;
   }

   u4ncpy( buf, name, sizeof(buf) ) ;
   u4name_ext( buf, sizeof(buf), "DBF", 0 ) ;
   c4upper( buf ) ;

   if ( h4create( &file, c4, buf, 1 ) != 0 )  return 0 ;

   h4seq_write_init( &seq_write, &file, 0L, buffer, sizeof(buffer) ) ;

   /* Write the header */
   memset( (void *)&create_header, 0, sizeof(create_header) ) ;
   if ( is_memo )
      #ifdef S4MFOX
         create_header.h.version =  0xF5 ;
      #else
         #ifdef S4MNDX
            create_header.h.version =  0x83 ;
         #else
            create_header.h.version =  0x8B ;
         #endif
      #endif
   else
      create_header.h.version =  0x03 ;

   u4yymmdd( &create_header.h.yy ) ;
   create_header.h.header_len =  (unsigned short) (32*(n_flds+1) + 1) ;
   create_header.record_len =  (unsigned short) calc_record_len ;

   h4seq_write( &seq_write, (char *) &create_header, sizeof(create_header) ) ;

   for ( i = 0; i < (int) n_flds; i++ )
   {
      memset( (void *)&create_field_image, 0, sizeof(create_field_image) ) ;
      u4ncpy( create_field_image.name, field_data[i].name, sizeof(create_field_image.name));
      c4trim_n( create_field_image.name, sizeof(create_field_image.name) ) ;
      c4upper( create_field_image.name ) ;

      create_field_image.type =  field_data[i].type ;
      c4upper( &create_field_image.type ) ;

      if ( create_field_image.type == 'C' )
      {
          create_field_image.len =  (unsigned char) (field_data[i].len & 0xFF) ;
          create_field_image.dec =  (unsigned char) (field_data[i].len>>8) ;
      }
      else
      {
         create_field_image.len =  (unsigned char) field_data[i].len ;
         create_field_image.dec =  (unsigned char) field_data[i].dec ;
      }

      #ifdef S4DEBUG
         t =  create_field_image.type ;
         is_illegal =  0 ;
         if ( t != 'D' && t != 'L' && t != 'N' && t != 'M' && t != 'F'  && t != 'C' )  is_illegal =  1 ;
         if ( create_field_image.type == 'L' && create_field_image.len != 1  ||
              create_field_image.type == 'D' && create_field_image.len != 8  ||
              create_field_image.type == 'M' && create_field_image.len != 10 )
            is_illegal =  1 ;


         if ( create_field_image.type == 'N' || create_field_image.type == 'F' )
         {
            len =  field_data[i].len ;
            dec =  field_data[i].dec ;
            if ( len > 19 || len < 1 || len <= 2 && dec != 0 || dec < 0) is_illegal =  1 ;
            if ( dec >= len-1  && dec > 0 )  is_illegal = 1 ;
         }
         else

            if ( is_illegal )
               e4severe( e4data, E4_DATA_ILL, (char *) 0 ) ;
      #endif

      if ( h4seq_write( &seq_write, &create_field_image, sizeof(create_field_image)) < 0 )  break ;
   }

   h4seq_write( &seq_write, "\015\032", 2 ) ;
   h4seq_write_flush( &seq_write ) ;

   h4close( &file ) ;

   if ( create_header.h.version & 0x80 )
   {
      M4FILE m4file ;

      #ifdef S4MFOX
         u4name_ext( buf, sizeof(buf), "FPT", 1 ) ;
      #else
         u4name_ext( buf, sizeof(buf), "DBT", 1 ) ;
      #endif
      m4file_create( &m4file, c4, 0, buf) ;
      h4close( &m4file.file ) ;
   }

   #ifdef N4OTHER
      i = c4->auto_open ;
      c4->auto_open = 0 ;   /* don't open the index files */
   #endif
   if ( (d4 = d4open(c4,name)) == 0 )  return 0 ;
   #ifdef N4OTHER
      c4->auto_open = i ;   /* reset the auto_open flag */
   #endif

   if ( tag_info != 0 )
   {

      if ( i4create( d4, 0, tag_info ) == (I4INDEX *) 0 )
      {
         d4close(d4) ;
         return 0 ;
      }


      #ifdef S4DEBUG
         if ( c4->index_memory == 0 )
         e4severe( e4data, "d4create()", (char *) 0 ) ;
      #endif
   }

   c4->create_error = 0;
   return d4 ;
}


