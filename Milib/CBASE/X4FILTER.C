/* x4filter.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

void S4FUNCTION x4init_work( X4FILTER *filter, D4DATA *data, X4FUNCTION *routine, void *routine_data )
{
   filter->data      =  data ;
   filter->routine   =  routine ;
   filter->routine_data =  routine_data ;
}

int  S4FUNCTION x4bottom( X4FILTER *x4 )
{
   int rc ;
   D4DATA *d4 ;

   d4 =  x4->data ;

   if ( (rc = d4bottom(d4)) != 0 )  return rc ;
   rc = x4find_good(x4,-1) ;
   if ( rc == r4keep )
      return 0 ;
   if ( rc == r4bof )
   {
      d4->bof_flag =  1 ;
      return x4filter_go_eof(x4) ;
   }
   return rc ;
}

int  S4FUNCTION x4filter_test( X4FILTER *x4 )
{
   #ifdef S4DEBUG
      #if ! (defined __ZTC__ && defined S4MEDIUM)
	 if (u4ptr_equal( x4->routine, 0))
            e4severe( e4info, "x4filter_test()", (char *) 0);
      #endif
   #endif
   return (*x4->routine)( x4->routine_data ) ;
}

int  x4find_good( X4FILTER *x4, int sign )
{
   int rc ;
   for(;;)
   {
      rc = x4filter_test(x4) ;
      if ( rc != r4ignore )   return rc ;
      if ( (rc = d4skip(x4->data, (long) sign)) != 0 )  return rc ;
   }
}

int  x4filter_go_eof( X4FILTER *x4 )
{
   int rc ;
   rc =  d4go_eof(x4->data) ;
   if ( rc != r4eof )  return rc ;
   rc = x4filter_test(x4) ;
   if ( rc != r4keep  &&  rc != r4ignore )  return rc ;
   return r4eof ;
}

int  S4FUNCTION x4go( X4FILTER *x4, long new_rec )
{
   int rc ;

   rc =  d4go( x4->data, new_rec) ;
   if ( rc != 0 )  return rc ;

   rc =  (*x4->routine)( x4->routine_data ) ;
   if ( rc == r4ignore )
      return r4entry ;
   if ( rc == r4keep )
      return 0 ;
   return rc ;
}

int  S4FUNCTION x4seek_double( X4FILTER *x4, double d )
{
   int rc ;
   double d_on, diff ;
   T4TAG *tag_ptr ;

   rc =  x4seek_do( x4, d4seek_double(x4->data,d) ) ;
   if ( rc != 0 )  return rc ;

   /* 0 may become r4after */
   tag_ptr =  d4tag_default(x4->data) ;

   d_on =  e4double( tag_ptr->expr ) ;
   if ( x4->data->code_base->error_code < 0 )  return -1 ;

   diff = d_on - d ;
   if ( diff < -E4ACCURACY || diff > E4ACCURACY )
      return r4after ;

   return 0 ;
}

int  S4FUNCTION x4seek( X4FILTER *x4, char *s )
{
   int rc, len, compare_len ;
   #ifdef S4MDX
      char bcd[sizeof(C4BCD)] ;
   #endif
   char *key ;
   double d, d_on, diff ;
   T4TAG *tag_ptr ;

   rc =  x4seek_do( x4, d4seek( x4->data, s) ) ;
   if ( rc != 0 )  return rc ;

   /* 0 return may become r4after. */
   tag_ptr =  d4tag_default(x4->data) ;
   len =  strlen(s) ;

   /* do the search according to the tag type */
   switch ( t4type(tag_ptr) )
   {
      #ifndef S4MDX
         case t4num_doub:
   	    d    =  c4atod( s, len ) ;
	    d_on =  e4double(tag_ptr->expr) ;
	    if ( x4->data->code_base->error_code < 0 )  return -1 ;

	    diff = d_on - d ;
	    if ( diff < -E4ACCURACY || diff > E4ACCURACY )
	       return r4after ;

	    return 0 ;
      #else
            case t4num_doub:
            d    =  c4atod( s, len ) ;
            d_on =  e4double(tag_ptr->expr) ;
            if ( x4->data->code_base->error_code < 0 )  return -1 ;

            diff = d_on - d ;
            if ( diff < -E4ACCURACY || diff > E4ACCURACY )
               return r4after ;

            return 0 ;

         case t4num_bcd:
            c4bcd_from_a( bcd, s, len ) ;

            if ( e4key( tag_ptr->expr, &key ) < 0 )
               return -1 ;

            if ( memcmp( bcd, key, sizeof(C4BCD)) == 0 )
               return 0 ;
            break ;
      #endif

      default :
         if ( (compare_len = e4key( tag_ptr->expr, &key)) < 0 )
            return -1 ;

         if ( len < compare_len )
            compare_len =  len ;

         if ( memcmp( key, s, compare_len ) == 0 )
            return 0 ;
         break ;
   }

   return r4after ;
}

int  x4seek_do( X4FILTER *x4, int seek_rc )
{
   int rc ;

   if ( seek_rc != r4after  &&  seek_rc != 0 )
      return seek_rc ;

   rc =  x4find_good( x4, 1) ;
   if ( rc != r4keep )  return rc ;

   return seek_rc ;
}

int  S4FUNCTION x4skip( X4FILTER *x4, long n )
{
   C4CODE *c4 ;
   D4DATA *d4 ;
   int  sign, rc, save_flag ;
   long good_rec ; 

   d4 =  x4->data ;
   c4 =  d4->code_base ;
   if ( c4->error_code < 0 )  return -1 ;

   if ( n < 0 )
   {
      n = -n ;
      sign = -1 ;
   }
   else
      sign =  1 ;

   while ( n-- )  /* skip a record */
   {
      good_rec =  d4recno( d4 ) ;
      d4skip( x4->data, sign) ;
      rc = x4find_good(x4, sign) ;

      if ( rc != r4keep )
      {
         if ( rc != r4eof && rc != r4bof )  return rc ;

         if ( sign > 0 )
         {
            if ( (rc = x4filter_go_eof(x4)) != r4eof )  return rc ;
            return r4eof ;
         }
   
         save_flag =  c4->go_error ;
         c4->go_error =  0 ;
         rc =  d4go( x4->data, good_rec ) ;
         c4->go_error =  save_flag ;
         d4->bof_flag =  1 ;
         if ( rc != 0 )  return rc ;
         return r4bof ;
      }
   }
   if ( sign < 0 )
   {
      if ( d4->bof_flag )
         return r4bof ;
   }
   else
   {
      if ( d4->eof_flag )
         return r4eof ;
   }
   return 0 ;
}

int  S4FUNCTION x4top( X4FILTER *x4 )
{
   int        rc ;

   if ( (rc = d4top(x4->data)) != 0 )  return rc ;
   rc =  x4find_good(x4, 1) ;
   if ( rc == r4keep )
      return 0 ;
   if ( rc == r4eof )
   {
      x4->data->bof_flag =  1 ;
      if ( (rc = x4filter_go_eof(x4)) != r4eof )  return rc ;
      return r4eof ;
   }
   return rc ;
}

