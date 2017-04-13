/* f4ptr.c   (c)Copyright Sequiter Software Inc., 1991-1992, all rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

char *S4FUNCTION f4ptr( F4FIELD *f4 )
{
   #ifdef S4DEBUG
      if ( f4 == 0 )
         e4severe( e4parm, "f4ptr()", (char *) 0 ) ;
   #endif

   return f4->data->record + f4->offset ;
}


char *S4FUNCTION f4assign_ptr( F4FIELD *f4 )
{
   #ifdef S4DEBUG
      if ( f4 == 0 )
         e4severe( e4parm, "f4ptr()", (char *) 0 ) ;
   #endif

   f4->data->record_changed  =  1 ;

   return f4->data->record + f4->offset ;
}
