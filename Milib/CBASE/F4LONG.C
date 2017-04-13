/* f4long.c (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4data.h"

long  S4FUNCTION f4long( F4FIELD *f4 )
{
   if ( f4->type == 'D' )
      return a4long( f4ptr(f4) ) ;

   return  c4atol( f4ptr(f4), f4->len ) ;
}

void S4FUNCTION f4assign_long( F4FIELD *f4, long l_value )
{
   if ( f4->type == 'D' )
      a4assign( f4assign_ptr(f4), l_value ) ;
   else
   {
      if ( f4->dec == 0 )
	 c4ltoa45( l_value, f4assign_ptr(f4), f4->len ) ;
      else
	 f4assign_double( f4, (double) l_value ) ;
   }
}
