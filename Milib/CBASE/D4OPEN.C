/* d4open.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

D4DATA * S4FUNCTION d4open( C4CODE *c4, char *name )
{
   int rc, i_fields, i_memo ;
   unsigned field_data_len, count ;
   char name_buf[258], field_buf[2], *info ;
   D4DATA *d4 ;
   D4DATA_HEADER_FULL full_header ;
   I4INDEX *i4 ;

   #ifdef S4DEBUG
      if ( c4->debug_int !=  0x5281 )
         e4severe( e4result, E4_RESULT_D4I, (char *) 0 ) ;

      if ( d4data( c4, name ) != 0 )
	 e4severe( e4info, E4_INFO_DAO, (char *) 0 ) ;
   #endif

   if ( c4->data_memory == 0 )
   {
      c4->data_memory =  y4memory_type( c4->mem_start_data, sizeof(D4DATA),
                                                  c4->mem_expand_data, 0 ) ;
      if ( c4->data_memory == 0 )
      {
         e4error( c4, e4memory, (char *) 0 ) ;
         return 0 ;
      }
   }
   d4 =  (D4DATA *) y4alloc( c4->data_memory ) ;
   if ( d4 == 0 )
      e4error(  c4, e4memory, (char *) 0 ) ;

   d4->code_base =  c4 ;
   d4->locks =  &d4->locked_record ;
   d4->n_locks = 1 ;

   u4ncpy( name_buf, name, sizeof(name_buf) ) ;
   u4name_ext( name_buf, sizeof(name_buf), "DBF", 0 ) ;
   c4upper( name_buf ) ;

   rc =  h4open( &d4->file, c4, name_buf, 1 ) ;
   if ( rc != 0 )
   {
      d4close( d4 ) ;
      return 0 ;
   }
   l4add( &c4->data_list, &d4->link ) ;

   u4name_piece( d4->alias, sizeof(d4->alias), name_buf, 0,0 ) ;

   if ( h4read_all( &d4->file, 0L, &full_header,sizeof(full_header)) < 0 )
   {
      d4close(d4) ;
      return 0 ;
   }

   memcpy( (void *)&d4->header, (void *)&full_header.h, sizeof(d4->header) ) ;
   d4->header.num_recs =  -1L ;
   d4->has_mdx =  full_header.has_mdx ;

   field_data_len =  full_header.h.header_len-sizeof(full_header) ;
   if ( full_header.h.header_len <= sizeof(full_header) )
   {
      e4error( d4->code_base, e4data, d4->file.name, (char *) 0 ) ;
      d4close(d4) ;
      return 0 ;
   }

   info =  (char *) u4alloc(field_data_len) ;
   if ( info == 0 )
   {
      e4error( d4->code_base, e4memory, (char *) 0 ) ;
      d4close(d4) ;
      return 0 ;
   }

   if ( h4read_all( &d4->file, (long) sizeof(full_header), info,field_data_len) < 0 )
   {
      u4free(info) ;
      e4error( d4->code_base, e4data, (char *) 0 ) ;
      d4close(d4) ;
      return 0 ;
   }

   /* count the number of fields */
   for (count=0; count < field_data_len; count+= 32 )
      if ( info[count] ==  0xD )  break ;
   d4->n_fields =  (int) (count/32) ;
   if ( field_data_len/32 < (unsigned) d4->n_fields )
   {
      u4free(info) ;
      e4error( d4->code_base, e4data, d4->file.name, (char *) 0 ) ;
      d4close(d4) ;
      return 0 ;
   }

   d4->fields =  (F4FIELD *) u4alloc( sizeof(F4FIELD) * (long) d4->n_fields ) ;
   if ( d4->fields == 0 )  e4error( d4->code_base, e4memory, (char *) 0 ) ;

   d4->record_width = 1 ;

   if ( ! (d4->code_base->error_code < 0)  )
      for ( i_fields =  0; i_fields < d4->n_fields; i_fields++ )
      {
	 F4FIELD_IMAGE *image = (F4FIELD_IMAGE *) (info+ i_fields*32) ;
         u4ncpy( d4->fields[i_fields].name, image->name, sizeof(d4->fields->name) ) ;

	 u4ncpy( field_buf, &image->type, 2 ) ;
	 c4upper( field_buf ) ;
	 d4->fields[i_fields].type =  *field_buf ;

         if ( d4->fields[i_fields].type == 'N' || d4->fields[i_fields].type == 'F')
         {
            d4->fields[i_fields].len =  image->len ;
            d4->fields[i_fields].dec =  image->dec ;
         }
         else
           if ( d4->fields[i_fields].type == 'L' || d4->fields[i_fields].type == 'D')
              d4->fields[i_fields].len =  image->len ;
           else
              d4->fields[i_fields].len =  image->len +  (image->dec << 8) ;

         if ( d4->fields[i_fields].type == 'M' )
            d4->n_fields_memo++ ;

         d4->fields[i_fields].offset =  d4->record_width ;
         d4->record_width +=  d4->fields[i_fields].len ;
         d4->fields[i_fields].data =  d4 ;
      }

   u4free( info ) ;

   if ( d4->n_fields_memo > 0  &&  ! (d4->code_base->error_code< 0) )
   {
      i_memo =  0 ;

      d4->fields_memo =  (F4MEMO *) u4alloc( sizeof(F4MEMO) * d4->n_fields_memo ) ;
      if ( d4->fields_memo == 0 )
         e4error( d4->code_base, e4memory, (char *) 0 ) ;
      else
         for ( i_fields = 0; i_fields < d4->n_fields; i_fields++ )
            if ( d4->fields[i_fields].type == 'M' )
            {
               d4->fields[i_fields].memo =  d4->fields_memo+i_memo ;
               d4->fields_memo[i_memo].status =  1 ;
               d4->fields_memo[i_memo].field  =  d4->fields+i_fields ;
               i_memo++ ;
            }
   }

   if ( d4->record_width != full_header.record_len  &&  ! (d4->code_base->error_code<0) )
      e4error( c4, e4data, d4->file.name, (char *) 0 ) ;

   if ( c4->error_code < 0 )
   {
      d4close(d4) ;
      return 0 ;
   }

   d4->record =  (char *) u4alloc( d4->record_width+1 ) ;
   d4->record_old =  (char *) u4alloc( d4->record_width+1 ) ;
   if ( d4->record == 0  ||  d4->record_old == 0 )
   {
      e4error( d4->code_base, e4memory, (char *) 0 ) ;
      d4close(d4) ;
      return 0 ;
   }

   memset( d4->record, ' ', d4->record_width ) ;
   memset( d4->record_old, ' ', d4->record_width ) ;

   d4->rec_num =  d4->rec_num_old =  -1 ;

   #ifdef N4OTHER
      if ( d4->code_base->auto_open )
      {
         i4 = i4open( d4, 0 ) ;
      }
   #else
      if ( d4->has_mdx && d4->code_base->auto_open )
      {
         i4 = i4open( d4, 0 ) ;
         if ( i4 == 0 )
         {
            d4close(d4) ;
            return 0 ;
         }
         #ifdef S4MDX
            if ( !i4->header.is_production )
               i4close(i4);
         #endif
      }
   #endif

   if ( d4->header.version & 0x80 )
   {
      #ifdef S4MFOX
         u4name_ext( name_buf, sizeof(name_buf), "FPT", 1 ) ;
      #else
         u4name_ext( name_buf, sizeof(name_buf), "DBT", 1 ) ;
      #endif
      if ( m4file_open( &d4->memo_file, d4, name_buf ) < 0 )
      {
         d4close(d4) ;
         return 0 ;
      }
   }

   return d4 ;
}
