/* f4true.c (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved.

   Returns a true or false.
*/

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int S4FUNCTION f4true( F4FIELD *f4 )
{
   char char_value ;

   char_value =  *f4ptr(f4) ;
   if ( char_value == 'Y'  ||  char_value == 'y'  ||
         char_value == 'T'  ||  char_value == 't'  )
      return( 1 ) ;
   else
      return( 0 ) ;
}
