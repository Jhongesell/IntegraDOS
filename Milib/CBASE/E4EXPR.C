/* e4expr.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifdef S4CLIPPER
static int e4string( E4EXPR *e4, char *result )
{
   int dig_len, is_neg, zeros_len, i, dec_pos, dig_pos, dec_len, result_len ;
   double index_date ;
   long l ;
   char *str, *ptr ;
   int result_type = e4->type ;

   if ( result_type == t4num_doub )
   {
      result_len = e4->data->code_base->numeric_str_len ;
      if ( e4->num_decimals > 0 ) result_len += ( 1 + e4->num_decimals ) ;
      result[result_len] =  0 ;
      str = fcvt( *((double *) result), e4->num_decimals, &dig_len, &is_neg) ;

      zeros_len = result_len - dig_len - 1 - e4->num_decimals ;

      memset( result, '0', zeros_len ) ;

      if ( e4->num_decimals > 0 )
      {
         dec_pos =  result_len - e4->num_decimals - 1 ;
         memset( result + dec_pos, '.', 1) ;
      }
      else
         dec_pos =  result_len ;

      ptr =  result + zeros_len ;

      if ( dig_len >= 0 )
      {
         if ( (dec_pos - dig_len) < 0 )
         {
            memset( ptr, (int) '*', (size_t) result_len) ;
            return t4num_str ;
	 }
	 memcpy( ptr, str, (size_t) dig_len ) ;
         ptr += dig_len ;
         *ptr = '.' ;
	 memcpy( ++ptr, str+dig_len, (size_t) e4->num_decimals ) ;
      }
      else
      {
         dec_len = e4->num_decimals + dig_len ;
         if ( dec_len > 0 )
            memcpy( ptr - dig_len, str, dec_len ) ;
      }

      if ( is_neg )
      {
         for ( i=0; i< result_len; i++ )
           ptr[i] = (char) 0x5c - ptr[i] ;
      }
      return t4num_str ;
   }

   if ( result_type == t4date_doub )
   {
      memcpy( &index_date, result, sizeof(double) ) ;
      l = index_date ;
      a4assign( result, l ) ;
      return  t4date_str ;
   }

   if ( result_type == t4log )
   {
      int *i_ptr =  (int *) result ;
      if ( *i_ptr )
         memset( result, 'Y', 1 ) ;
      else
	 memset( result, 'N', 1 ) ;
   }

   return( result_type ) ;
}
#endif

void S4FUNCTION e4free( E4EXPR *e4 )
{
   if ( e4 == 0 )  return ;
   u4free( e4 ) ;
}

int S4FUNCTION e4key( E4EXPR *e4, char **ptr_ptr )
{
   int result_len ;
   double  d ;
   #ifdef S4MDX
      C4BCD  bcd ;
   #else
      #ifdef S4CLIPPER
         double  index_date ;
         long l ;
      #endif
   #endif
   #ifndef S4NOTHER
      double *d_ptr ;
   #endif

   result_len =  e4vary( e4, ptr_ptr ) ;
   if ( result_len < 0 )  return -1 ;

   switch( e4->type )
   {
      #ifdef S4FOX
         case t4num_str:
            d = c4atod( *ptr_ptr, result_len ) ;
            t4dbl_to_fox( *ptr_ptr, d ) ;
            e4->type =  t4num_fox ;
	    return (int) sizeof(double) ;
         case t4date_str:
            d =  (double) a4long( *ptr_ptr ) ;
            t4dbl_to_fox( *ptr_ptr, d ) ;
            e4->type =  t4date_fox ;
            return (int) sizeof(double) ;

         case t4num_doub:
            d_ptr = (double *) (*ptr_ptr) ;
            t4dbl_to_fox( *ptr_ptr, *d_ptr ) ;
            e4->type =  t4num_fox ;
            return (int) sizeof(double) ;
      #else
      #ifdef S4NDX
         case t4num_str:
	    d = c4atod( *ptr_ptr, result_len ) ;
            memcpy( *ptr_ptr, &d, sizeof( d ) ) ;
	    e4->type = t4num_doub ;
            return (int) sizeof(double) ;

         case t4num_doub:
            break ;
         case t4date_str:
            d = (double) a4long( *ptr_ptr ) ;
            memcpy( *ptr_ptr, &d, sizeof(double) ) ;
            e4->type =  t4date_doub ;
            break ;
      #else
      #ifdef S4CLIPPER
         case t4num_str:
	    c4clip( *ptr_ptr, result_len ) ;
	    e4->type = t4num_clip ;
            break ;
         case t4num_clip:
   	    break ;
         case t4num_doub:
	    if ( e4string( e4, *ptr_ptr ) != t4num_str )
	       e4severe( e4info, E4_INFO_EVA, (char *) 0 ) ;
	    e4->type = t4num_clip ;
	    break ;
	 case t4date_doub:
	    memcpy( &index_date, *ptr_ptr, sizeof(double) ) ;
	    l =  index_date ;
	    a4assign( *ptr_ptr, l ) ;
	    e4->type = t4date_str ;
            break ;
      #else     /*  ifdef S4MDX  */
         case t4num_str:
            c4bcd_from_a( (char *) &bcd, *ptr_ptr, result_len ) ;
            memcpy( *ptr_ptr, (void *)&bcd, sizeof(C4BCD) ) ;
            e4->type =  t4num_bcd ;
            return (int) sizeof(C4BCD) ;

         case t4num_doub:
            d_ptr =  (double *) (*ptr_ptr) ;
            c4bcd_from_d( (char *) &bcd, *d_ptr ) ;
            memcpy( *ptr_ptr, (void *)&bcd, sizeof(C4BCD) ) ;
            e4->type =  t4num_bcd ;
            return (int) sizeof(C4BCD) ;

         case t4date_str:
            d =  (double) a4long( *ptr_ptr ) ;
            memcpy( *ptr_ptr, (void *)&d, sizeof(double) ) ;
            e4->type =  t4date_doub ;
            return (int) sizeof(double) ;
      #endif  /*  ifdef S4CLIPPER  */
      #endif  /*  ifdef S4NDX      */
      #endif  /*  ifdef S4FOX      */
   }

   return result_len ;
}

int S4FUNCTION e4true( E4EXPR *e4 )
{
   int result_len ;
   int *i_ptr ;

   result_len =  e4vary( e4, (char **) &i_ptr ) ;
   if ( result_len < 0 )  return -1 ;

   if ( e4type(e4) != t4log )
      return e4error( e4->data->code_base, e4result, "e4true()", E4_RESULT_EXP, (char *) 0 ) ;

   return *i_ptr ;
}

/*   1.  Get function type and parameter information.
     2.  Recursively call to get the parameters.
     3.  Call function with the parameters.
     4.  Return the result.

     result_ptr   points to where the value should be returned.
     return_len   is the length of the available space.

     If the function returns a value of a specific length, the
     parameter space will not overlap the last parameter.  Otherwise,
     64 bytes of space will be allocated for the return value.  If
     more is returned, the expression evaluation function return
     will overlap the last parameter and perhaps other parameters as well.

     Returns
    -1    Error
   >= 0   The length of the return value.
*/

int  S4FUNCTION e4vary( E4EXPR *e4, char **ptr_ptr )
{
   char    *compile_ptr ;
   E4PARMS  list ;
   int      f_code, string_len ;
   C4CODE  *c4 ;
   #ifdef S4CLIPPER
      int old_dec = e4->data->code_base->decimals ;
   #endif

   c4 =  e4->data->code_base ;

   if ( c4->error_code < 0 )  return -1 ;

   compile_ptr =  e4->parsed ;
   e4parms_init( &list, e4->data ) ;

   for (;;)
   {
      memcpy( (void *)&f_code, compile_ptr, sizeof(int) ) ;
      compile_ptr +=  sizeof(int) ;

      if ( f_code > I4LAST_IMMEDIATE )
      {
         if ( e4parms_pop( &list, v4functions[f_code].num_parms ) < 0 ) return -1 ;
         (*v4functions[f_code].function_ptr)( &list ) ;
         if ( c4->error_code < 0 )  return -1 ;
         e4parms_push( &list ) ;
         continue ;
      }

      if ( f_code < 0 )
      {
         if ( e4parms_pop( &list, 1) < 0 )  return -1 ;

         #ifdef S4DEBUG
            if ( list.on_parm != 0  &&  list.parms[list.on_parm].pos != 0 )
               e4severe( e4info, "e4vary()", (char *) 0 ) ;
         #endif

         e4buf[list.parms[0].len] = 0;
         *ptr_ptr  =  e4buf ;
         e4->type = list.parms[0].type;
         #ifdef S4CLIPPER
            c4->decimals = old_dec ;
         #endif
         return list.parms[0].len ;
      }

      /* F4FIELD or Immediate Data */

      if ( f_code <= LAST_I4FIELD )
      {
         F4FIELD *field ;
         memcpy( (void *)&field, compile_ptr, sizeof(field) ) ;
         compile_ptr +=  sizeof(field) ;

         #ifdef S4CLIPPER
	    e4->num_decimals = f4decimals( field ) ;
	    if ( e4->num_decimals == 0 )
	       e4->num_decimals = c4->decimals ;
            else if ( c4->decimals == 0 )
               c4->decimals = 2 ;
         #endif

         switch( f_code )
         {
            case I4FIELD_STR:
            case I4FIELD_NUM_S:
            case I4FIELD_DATE_S:
               e4parms_set( &list, f4ptr(field), field->len, field->type);
               if ( f_code == I4FIELD_NUM_S )
                  if ( field->dec > 0 )
                     e4buf[ list.parms[list.on_parm].pos +
                        field->len - field->dec - 1 ]  =  '.' ;
               break ;

            case I4FIELD_NUM_D:
               e4parms_set_double( &list, f4double(field) ) ;
               break ;

            case I4FIELD_DATE_D:
               e4parms_set_double( &list, (double) a4long(f4ptr(field)) ) ;
               break ;

            case I4FIELD_LOG:
               e4parms_set_logical( &list, f4true(field) ) ;
               break ;
         }

         list.parms[list.on_parm].type =  v4functions[f_code].return_type ;
         if ( e4parms_push( &list ) < 0 )  return -1 ;
         continue ;
      }

      /* Immediate Data */
      switch( f_code )
      {
         case  I4STRING:
            memcpy( (void *)&string_len, compile_ptr, sizeof(int) ) ;
            compile_ptr +=  sizeof(int) ;
            e4parms_set( &list, compile_ptr, string_len, t4str ) ;
            compile_ptr +=  string_len ;
            break ;

         case  I4DOUBLE:
            e4parms_pos_align( &list ) ;
            e4parms_set( &list, compile_ptr, sizeof(double), t4num_doub ) ;
            compile_ptr +=  sizeof(double) ;
            break ;
      }

      if (  e4parms_push( &list ) < 0 )  return -1 ;
      continue ;
   }
}

double S4FUNCTION e4double( E4EXPR *e4 )
{
   char *ptr ;
   int   len ;

   len =  e4vary( e4, &ptr ) ;
   if ( len < 0 )  return 0.0 ;

   switch( e4type(e4) )
   {
      case t4num_doub:
      case t4date_doub:
         return  *((double *) ptr) ;

      case t4num_str:
      case t4str:
         return c4atod( ptr, len ) ;

      case t4date_str:
         return (double) a4long(ptr) ;
   }

   return 0.0 ;
}

char *S4FUNCTION e4source( E4EXPR *e4 )
{
   return e4->source ;
}

S4FUNCTION e4type( E4EXPR *e4 )
{
   return e4->type ;
}

