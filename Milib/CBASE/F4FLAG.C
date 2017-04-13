/* f4flag.c  (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

int f4flag_init( F4FLAG *f4, C4CODE *c4, long n_flags )
{
   memset( (void *)f4, 0, sizeof(F4FLAG) ) ;

   if ( c4->error_code < 0 )  return -1 ;

   f4->code_base =  c4 ;
   f4->num_flags =  n_flags ;

   if ( n_flags <= 0 )
   {
      if ( n_flags < 0 )
	 return e4error( c4, e4parm, "f4flag_init()", (char *) 0 ) ;
      return 0 ;
   }
   f4->flags =  (unsigned char *)  u4alloc( n_flags/8 +2 ) ;
   if ( f4->flags == 0 )  return e4error( c4, e4memory, (char *) 0 ) ;
   return 0 ;
}

int f4flag_set( F4FLAG *f4, long flag_num )
{
   unsigned low_val, high_val, set_val ;

   if ( f4->code_base->error_code < 0 )
   {
      u4free( f4->flags ) ;
      f4->flags =  0 ;
      return -1 ;
   }

   if ( flag_num < 0 || flag_num > f4->num_flags  ||  f4->flags == 0 )
      return e4error( f4->code_base, e4info, "f4flag_set()", (char *) 0 ) ;

   low_val  =  (unsigned int) (flag_num & 0x7) ;
   high_val =  (unsigned int) (flag_num >> 3) ;
   set_val  =  1 << low_val ;

   f4->flags[high_val] = (unsigned char)  (set_val | (unsigned) f4->flags[high_val]) ;

   return 0 ;
}

int  f4flag_set_range( F4FLAG *f4, long flag_num, long num_flags )
{
   long i_flag ;

   for ( i_flag =  0; i_flag < num_flags; i_flag++ )
      if ( f4flag_set( f4, flag_num + i_flag ) != 0 )  return e4info ;
   return 0 ;
}

int f4flag_is_set( F4FLAG *f4, long flag_num )
{
   unsigned low_val, high_val, ret_val ;

   if ( f4->code_base->error_code < 0 )
   {
      u4free( f4->flags ) ;
      f4->flags =  0 ;
      return -1 ;
   }

   if ( flag_num > f4->num_flags  ||  flag_num < 0  ||  f4->flags == 0 )
      return e4error( f4->code_base, e4info, "f4flag_is_set()", (char *) 0 ) ;

   low_val  =  (unsigned int) (flag_num & 0x7) ;
   high_val =  (unsigned int) (flag_num >> 3) ;
   ret_val =  (1 << low_val) &  (unsigned int) f4->flags[high_val]  ;

   return (int) ret_val ;
}

int f4flag_is_all_set( F4FLAG *f4, long flag_num, long num_flags )
{
   int rc ;
   long i_flag ;

   if ( f4->code_base->error_code < 0 )  num_flags =  1 ;

   for ( i_flag =  flag_num; i_flag <= num_flags; i_flag++ )
   {
      rc = f4flag_is_set( f4, i_flag ) ; 
      if ( rc < 0 )  return -1 ;
      if ( rc == 0 )  return 0 ;
   }
   return 1 ;
}

int f4flag_is_any_set( F4FLAG *f4, long flag_num, long num_flags )
{
   int rc ;
   long i_flag ;

   if ( f4->code_base->error_code < 0 )  num_flags = 1 ;

   for ( i_flag =  flag_num; i_flag <= num_flags; i_flag++ )
      if ( (rc = f4flag_is_set( f4, i_flag )) < 0 )  return rc ;
   return 0 ;
}

