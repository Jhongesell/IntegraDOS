/* i4init.c   (c)Copyright Sequiter Software Inc., 1990-1991.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifndef N4OTHER

#ifndef S4FOX
int S4CALL t4cmp_doub( S4CMP_PARM data_ptr, S4CMP_PARM search_ptr, size_t len )
{
   double dif ;
   #ifdef S4PORTABLE
      double d1, d2 ;
      memcpy( &d1, data_ptr, sizeof(double) ) ;
      memcpy( &d2, search_ptr, sizeof(double) ) ;
      dif =  d1 - d2 ;
   #else
      dif =  *((double *)data_ptr) - *((double *)search_ptr) ;
   #endif

   if ( dif > E4ACCURACY )  return r4after ;
   if ( dif < -E4ACCURACY ) return -1 ;
   return r4success ;
}

int S4CALL t4desc_cmp_doub( S4CMP_PARM data_ptr, S4CMP_PARM search_ptr, size_t len )
{
   return -1 * t4cmp_doub( data_ptr, search_ptr, 0 ) ;
}

int S4CALL t4desc_bcd_cmp( S4CMP_PARM data_ptr, S4CMP_PARM search_ptr, size_t len )
{
   return -1 * c4bcd_cmp( data_ptr, search_ptr, 0 ) ;
}

int S4CALL t4desc_memcmp( S4CMP_PARM data_ptr, S4CMP_PARM search_ptr, size_t len )
{
   return -1 * memcmp( data_ptr, search_ptr, len ) ;
}

static void t4str_to_date_mdx( char *result, char *input, int dummy )
{
   double d ;
   d =  (double) a4long(input) ;
   memcpy( result, &d, sizeof(double) ) ;
}

static void t4no_change_double( char *result, double d )
{
   memcpy( result, &d, sizeof(double) ) ;
}

static void t4no_change_str( char *a, char *b, int l)
{
   memcpy(a,b,l) ;
}

int S4FUNCTION t4init( T4TAG *t4, I4INDEX *i4, T4DESC *tag_info, long lock_position )
{
   C4CODE *c4 ;
   H4SEQ_READ seqread ;
   char  buffer[1024], garbage_buffer[518], expr_buf[221], *ptr ;
   int   len ;

   c4 =  i4->code_base ;
   if ( c4->error_code < 0 )  return -1 ;

   t4->index =  i4 ;
   t4->code_base =  c4 ;
   t4->unique_error =  c4->default_unique_error ;
   t4->cmp =  u4memcmp ;

   t4->lock_pos =  lock_position ;

   t4->header_offset =  tag_info->header_pos * 512 ;

   h4seq_read_init( &seqread, &i4->file, t4->header_offset, buffer, sizeof(buffer) ) ;
   if ( h4seq_read_all( &seqread, &t4->header,sizeof(t4->header)) < 0 )  return -1 ;
   t4->header.root =  -1 ;

   u4ncpy( t4->alias, tag_info->tag, sizeof(t4->alias) ) ;
   c4trim_n( t4->alias, sizeof(t4->alias) ) ;
   c4upper( t4->alias ) ;

   h4seq_read_all( &seqread, expr_buf, sizeof(expr_buf)-1 ) ;
   c4trim_n( expr_buf, sizeof(expr_buf) ) ;
   t4->expr =  e4parse( i4->data, expr_buf ) ;

   len =  e4key( t4->expr, &ptr ) ;
   if ( len < 0 )  return -1 ;

   if ( t4->header.type !=  (char) e4type(t4->expr) ||
	t4->header.value_len !=  (short) len )
      return e4error( c4, e4index, i4->file.name, (char *) 0 ) ;

   t4init_seek_conv(t4,t4->header.type) ;

   h4seq_read_all( &seqread, garbage_buffer, sizeof(garbage_buffer) ) ;

   h4seq_read_all( &seqread, expr_buf, sizeof(expr_buf)-1 ) ;
   c4trim_n( expr_buf, sizeof(expr_buf) ) ;

   if ( expr_buf[0] != 0 )
   {
      if (garbage_buffer[3] == 1)
         t4->has_keys = 1 ;
      else
         t4->has_keys = 0 ;

      t4->filter =  e4parse( i4->data, expr_buf ) ;
      len = e4key( t4->filter, &ptr ) ;  if ( len < 0 )  return -1 ;
      if ( e4type(t4->filter) != 'L' )
	 return e4error( c4, e4index, E4_INDEX_FIL, i4->file.name, (char *) 0 ) ;
   }
   return 0 ;
}  

void S4FUNCTION t4init_seek_conv( T4TAG *t4, int key_type )
{
   int is_desc ;

   is_desc =  t4->header.type_code & 8 ;

   switch( key_type )
   {
      case t4num_bcd:
	 if ( is_desc )
	    t4->cmp =  t4desc_bcd_cmp ;
	 else
	    t4->cmp =  c4bcd_cmp ;

	 t4->stok =  c4bcd_from_a ;
	 t4->dtok =  c4bcd_from_d ;
         break ;

      case t4date_doub:
         if ( is_desc )
            t4->cmp =  t4desc_cmp_doub ;
         else
            t4->cmp =  t4cmp_doub ;
	 t4->stok =  t4str_to_date_mdx ;
	 t4->dtok =  t4no_change_double ;
         break ;

      case t4str:
         if ( is_desc )
            t4->cmp =  t4desc_memcmp ;
         else
	    t4->cmp =  u4memcmp ;
	 t4->stok =  t4no_change_str ;
	 t4->dtok =  0 ;
         break ;

      default:
	 e4severe( e4info, E4_INFO_INV, (char *) 0 ) ;
   }
}
#endif

#ifdef S4FOX

int S4CALL t4cdx_cmp( S4CMP_PARM data_ptr, S4CMP_PARM search_ptr, size_t len )
{
   unsigned char *data = (unsigned char *)data_ptr ;
   unsigned char *search = (unsigned char *)search_ptr ;
   unsigned on ;
   for( on = 0 ; on < len ; on++ )
   {
      if ( data[on] != search[on] )
      {
         if ( data[on] > search[on] ) return -1 ;  /* gone too far */
         else return on + 1 ;
      }
   }
   return 0 ;
}

static void t4no_change_str( char *a, char *b, int l)
{
   memcpy(a,b,l) ;
}

int S4FUNCTION t4init( T4TAG *t4, I4INDEX *i4, long file_pos, long lock_position, char *name )
{
   C4CODE *c4 ;
   char  buffer[1024], garbage_buffer[518], expr_buf[221], *ptr ;
   int  len ;
   char top_size ;

   c4 =  i4->code_base ;
   if ( c4->error_code )  return -1 ;

   t4->index =  i4 ;
   t4->code_base =  c4 ;
   t4->unique_error =  c4->default_unique_error ;
   t4->lock_pos =  lock_position ;
   t4->header_offset =  file_pos ;
   t4->header.root =  -1 ;
   t4->cmp = t4cdx_cmp ;

   top_size = 2 * sizeof(long) + 4*sizeof(char) + sizeof(short) + 2 * sizeof(unsigned char) ;
   if ( h4read_all( &i4->file, file_pos, &t4->header, top_size ) < 0 ) return 0 ;
   if ( h4read_all( &i4->file, file_pos + (long)top_size + 486L, &t4->header.descending,
        ( 3 * sizeof(char) + 3 * sizeof(short) ) ) < 0 ) return 0 ;

   u4ncpy( t4->alias, name, sizeof(t4->alias) ) ;
   c4trim_n( t4->alias, sizeof(t4->alias) ) ;
   c4upper( t4->alias ) ;

   if ( t4->header.type_code < 0x80 )  /* non-compound header; so expression */
   {
      #ifdef S4DEBUG
         if ( t4->header.expr_len+1 > sizeof( expr_buf ) )
             e4severe( e4info, "t4init()", E4_INFO_EXP, (char *) 0 ) ;
      #endif
      h4read_all( &i4->file, file_pos+B4BLOCK_SIZE, expr_buf, t4->header.expr_len ) ;
      expr_buf[t4->header.expr_len] = '\0' ;
      t4->expr =  e4parse( i4->data, expr_buf ) ;

      len =  e4key( t4->expr, &ptr ) ;
      if ( len < 0 )  return -1 ;

      if ( t4->header.key_len !=  len )
         return e4error( c4, e4index, i4->file.name, (char *) 0 ) ;

      t4init_seek_conv(t4, t4->expr->type ) ;

      if ( t4->header.type_code & 0x08 )   /* For clause (filter) exists */
      {
         h4read_all( &i4->file, file_pos+B4BLOCK_SIZE+t4->header.expr_len, expr_buf, t4->header.filter_len ) ;
         expr_buf[t4->header.filter_len] = '\0' ;

         t4->filter =  e4parse( i4->data, expr_buf ) ;
         len = e4key( t4->filter, &ptr ) ;  if ( len < 0 )  return -1 ;
         if ( e4type(t4->filter) != 'L' )
             return e4error( c4, e4index, E4_INDEX_FIL, i4->file.name, (char *) 0 ) ;
      }
   }
   return 0 ;
}

void S4FUNCTION t4init_seek_conv( T4TAG *t4, int type )
{
   t4->cmp =  t4cdx_cmp ;

   switch( type )
   {
      case t4date_fox:
         t4->stok =  t4dtstr_to_fox ;
	 t4->dtok =  t4dbl_to_fox ;
         t4->p_char = '\0' ;
         break ;
      case t4num_fox:
         t4->stok =  t4str_to_fox ;
	 t4->dtok =  t4dbl_to_fox ;
         t4->p_char = '\0' ;
         break ;
      case t4str:
         t4->stok =  t4no_change_str ;
         t4->dtok =  0 ;
         t4->p_char = ' ' ;
         break ;
      default:
	 e4severe( e4info, E4_INFO_INV, (char *) 0 ) ;
   }
}
#endif

#endif   /*  ifndef N4OTHER  */

#ifdef N4OTHER

int S4CALL t4cmp_doub( S4CMP_PARM data_ptr, S4CMP_PARM search_ptr, size_t len )
{
   #ifdef S4PORTABLE
      double d1, d2 ;
      memcpy( &d1, data_ptr, sizeof(double) ) ;
      memcpy( &d2, search_ptr, sizeof(double) ) ;
      double dif =  d1 - d2 ;
   #else
      double dif =  *((double *)data_ptr) - *((double *)search_ptr) ;
   #endif

   if ( dif > E4ACCURACY )  return r4after ;
   if ( dif < -E4ACCURACY ) return -1 ;
   return r4success ;
}

static void  t4str_to_doub( char *result, char *input, int dummy )
{
   double d ;
   d = c4atod( input, strlen( input )) ;
   memcpy( result, &d, sizeof(double) ) ;
}

static void t4str_to_date_mdx( char *result, char *input, int dummy )
{
   double d =  (double) a4long(input) ;
   memcpy( result, &d, sizeof(double) ) ;
}

static void t4no_change_double( char *result, double d )
{
   memcpy( result, &d, sizeof(double) ) ;
}

static void t4no_change_str( char *a, char *b, int l)
{
   memcpy(a,b,l) ;
}

#ifdef S4CLIPPER
static void  t4str_to_clip( char *result, char *input )
{
   memcpy ( result, input, strlen( input ) ) ;
   c4clip( result, strlen( result ) ) ;
}
#endif

static void  t4date_doub_to_str( char *result, double d )
{
   long  l =  d ;
   a4assign( result, l ) ;
}

void S4FUNCTION t4init_seek_conv( T4TAG *t4, int key_type )
{
   switch( key_type )
   {
      case t4num_doub:
         t4->cmp  =  t4cmp_doub ;
         t4->stok =  t4str_to_doub ;
	 #ifdef S4NDX
	    t4->dtok =  t4no_change_double ;
	 #else
	    t4->dtok =  0 ;
	 #endif
         break ;

      #ifdef S4NDX
	 case t4date_doub:
	    t4->cmp  =  t4cmp_doub ;
	    t4->stok =  t4str_to_date_mdx ;
	    t4->dtok =  t4no_change_double ;
	    break ;
      #else
         case t4date_str:
            t4->cmp  =  u4memcmp ;
            t4->stok =  t4no_change_str ;
            t4->dtok =  t4date_doub_to_str ;
            break ;

         case t4num_clip:
            t4->cmp  =  u4memcmp ;
            t4->stok =  t4no_change_str ;
            t4->dtok =  0 ;
            break ;
      #endif

      case t4str:
         t4->cmp  =  u4memcmp ;
         t4->stok =  t4no_change_str ;
         t4->dtok =  0 ;
         break ;

      default:
         e4severe( e4info, E4_INFO_INV, (char *) 0 ) ;
   }
}
#endif   /*  ifdef N4OTHER  */
