/* f4char.c (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int  S4FUNCTION f4char( F4FIELD *f4 )
{
   /* Return the first character of the record buffer. */
   return( *f4ptr(f4) ) ;
}

void S4FUNCTION f4assign_char( F4FIELD *f4, int chr )
{
   f4blank( f4 ) ;
   *f4assign_ptr(f4) =  (char) chr ;
}
