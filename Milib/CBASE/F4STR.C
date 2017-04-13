/* f4str.c (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

/* Returns a pointer to static string corresponding to the field.
   This string will end in a NULL character.
*/

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

char *S4FUNCTION f4str( F4FIELD *f4 )
{
   unsigned l ;

   l =  f4->len ;
   if ( l >= sizeof(v4buffer) )
      l = sizeof(v4buffer) - 1 ;

   memcpy( v4buffer, f4ptr(f4), l ) ;
   v4buffer[l] =  0 ;
   return v4buffer ;
}

unsigned  S4FUNCTION f4ncpy( F4FIELD *f4, char *ptr, unsigned n )
{
   unsigned  num_cpy ;

   num_cpy =  f4->len ;
   if ( n < num_cpy )  num_cpy =  n ;

   /* 'f4ptr' returns a pointer to the field within the database record buffer. */
   memcpy( ptr, f4ptr(f4), (size_t) num_cpy ) ;

   if ( num_cpy < n )  ptr[num_cpy] =  '\000' ;

   return( num_cpy ) ;
}

void  S4FUNCTION f4assign( F4FIELD *f4, char *str )
{
   f4assign_n( f4, str, (unsigned) strlen(str) ) ;
}

void  S4FUNCTION f4assign_n( F4FIELD *f4, char *str, unsigned copy_bytes )
{
   char     *f_ptr ;

   f_ptr =  f4assign_ptr(f4) ;

   if ( copy_bytes > f4->len )  copy_bytes =  f4->len ;

   /* Copy the data into the record buffer. */
   memcpy( f_ptr, str, (size_t) copy_bytes ) ;

   /* Make the rest of the field blank. */
   memset( f_ptr+copy_bytes, (int) ' ', (size_t) (f4->len-copy_bytes) ) ;
}
