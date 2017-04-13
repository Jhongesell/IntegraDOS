/* d4field.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */
        
#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

F4FIELD *S4FUNCTION d4field( D4DATA *d4, char *field_name )
{
   int i ;

   i =  d4field_number(d4,field_name)-1 ;
   if ( i < 0 )  return 0 ;

   return d4->fields+ i ;
}

int S4FUNCTION d4field_number( D4DATA *d4, char *field_name )
{
   char buf[256] ;
   int i ;

   if ( d4 == 0 )
      e4severe( e4parm, E4_PARM_NUL, (char *) 0 ) ;
   if ( d4->code_base->error_code < 0 )  return -1 ;

   if ( field_name != 0 )
   {
      u4ncpy( buf, field_name, sizeof(buf) ) ;
      c4trim_n( buf, sizeof(buf) ) ;
      c4upper( buf ) ;

      for ( i = 0; i< d4->n_fields; i++ )
         if ( strcmp( buf, d4->fields[i].name ) == 0 )
            return i+1 ;
   }

   if ( d4->code_base->field_name_error )
      e4error( d4->code_base, e4field_name, field_name, (char *) 0 ) ;

   return -1 ;
}

F4FIELD *S4FUNCTION d4field_j( D4DATA *d4, int j_field )
{
   if ( d4 == 0 )
      e4severe( e4parm, E4_PARM_NUL, (char *) 0 ) ;
   if ( d4->code_base->error_code < 0 )  return 0 ;
   #ifdef S4DEBUG
      if ( d4->fields == 0 || j_field > d4->n_fields || j_field <= 0 )
      e4severe( e4info, "d4field_j()", (char *) 0 ) ;
   #endif
   return  d4->fields+j_field-1 ;
}

