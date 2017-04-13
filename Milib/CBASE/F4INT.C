/* f4int.c (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int  S4FUNCTION f4int( F4FIELD *f4 )
{
   /* Convert the field data into a 'int' */
   return  c4atoi( f4ptr(f4), f4->len ) ;
}

void S4FUNCTION f4assign_int( F4FIELD *f4, int i_value )
{
   if ( f4->dec == 0 )
      c4ltoa45( (long) i_value, f4assign_ptr(f4), f4->len ) ;
   else
      f4assign_double( f4, (double) i_value ) ;
}
