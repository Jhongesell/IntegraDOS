/* f4field.c (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

D4DATA *S4FUNCTION f4data( F4FIELD *f4 )
{
   return f4->data ;
}

int S4FUNCTION f4decimals( F4FIELD *f4 )
{
   return f4->dec ;
}

void S4FUNCTION f4blank( F4FIELD *f4 )
{
   memset( f4assign_ptr(f4), ' ', f4->len ) ;
}

char * S4FUNCTION f4name( F4FIELD *f4 )
{
   return f4->name ;
}

int  S4FUNCTION f4type( F4FIELD *f4 )
{
   return (int) f4->type ;
}

unsigned  S4FUNCTION f4len( F4FIELD *f4 )
{
   return f4->len ;
}
