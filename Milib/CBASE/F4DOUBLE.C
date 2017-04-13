/* f4double.c (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

/* Returns the value of the corresponding field as a double.
   Only defined for 'Numeric' fields and 'Character' fields
   containing numeric data.
*/

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int S4FUNCTION f4double2( F4FIELD *f4, double *result )
{
   *result = f4double( f4 );
   return 0 ;
}

double  S4FUNCTION f4double( F4FIELD *f4 )
{
   if ( f4->type == 'D' )
      return (double) a4long( f4ptr(f4) ) ;

   /* Convert the field data into a 'double' */
   return( c4atod( f4ptr(f4), f4->len ) ) ;
}

void S4FUNCTION f4assign_double( F4FIELD *f4, double d_value )
{
   if ( f4type(f4) == 'D' )
      a4assign( f4assign_ptr(f4), (long) d_value ) ;
   else
      /* Convert the 'double' to ASCII and then copy it into the record buffer. */
      c4dtoa45( d_value, f4assign_ptr(f4), f4->len, f4->dec ) ;
}

