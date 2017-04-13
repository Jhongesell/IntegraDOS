/*  c4bcd.c   (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#ifndef N4OTHER

int S4CALL c4bcd_cmp( S4CMP_PARM a_ptr, S4CMP_PARM b_ptr, size_t dummy_len )
{
   int a_sign, b_sign, a_len, b_len, a_sig_dig, b_sig_dig, compare_len, rc ;

   if ( ((C4BCD *)a_ptr)->sign )
      a_sign =  -1 ;
   else
      a_sign =   1 ;

   if ( ((C4BCD *)b_ptr)->sign )
      b_sign =  -1 ;
   else
      b_sign =   1 ;

   if ( a_sign != b_sign )
      return a_sign ;

   a_len =  ((C4BCD *)a_ptr)->len ;
   b_len =  ((C4BCD *)b_ptr)->len ;

   if ( a_len == 0 )
      a_sig_dig =  0 ;
   else
      a_sig_dig =  ((C4BCD *) a_ptr)->sig_dig ;

   if ( b_len == 0 )
      b_sig_dig =  0 ;
   else
      b_sig_dig =  ((C4BCD *) b_ptr)->sig_dig ;

   if ( a_sig_dig != b_sig_dig )
   {
      if ( a_sig_dig < b_sig_dig )
	 return -a_sign ;
      else
	 return a_sign ;
   }

   compare_len =  (a_len < b_len) ? b_len : a_len ;  /* Use Max */

   compare_len =  (compare_len+1)/2 ;

   rc =  memcmp( ((C4BCD *)a_ptr)->bcd, ((C4BCD *)b_ptr)->bcd, compare_len ) ;
   if ( a_sign < 0 )  return -rc ;

   return rc ;
}

void c4bcd_from_a( char *result, char *input_ptr, int input_ptr_len )
{
   char  *ptr ;
   int   n, last_pos, pos, is_before_dec, zero_count ;

   memset( result, 0, sizeof(C4BCD) ) ;

   last_pos = input_ptr_len - 1 ;
   pos = 0 ;
   for ( ; pos <= last_pos; pos++ )
      if ( input_ptr[pos] != ' ' )  break ;

   if ( pos <= last_pos )
   {
      if ( input_ptr[pos] == '-' )
      {
	 ((C4BCD *)result)->sign =  1 ;
	 pos++ ;
      }
      else
      {
	 if ( input_ptr[pos] == '+' )  pos++ ;
      }
   }

   for ( ; pos <= last_pos; pos++ )
      if ( input_ptr[pos] != ' ' && input_ptr[pos] != '0' )  break ;

   is_before_dec =  1 ;

   ((C4BCD *)result)->sig_dig =  0x34 ;
   if ( pos <= last_pos )
      if ( input_ptr[pos] == '.' )
      {
	 is_before_dec =  0 ;
	 pos++ ;
	 for ( ; pos <= last_pos; pos++ )
	 {
	    if ( input_ptr[pos] != '0' )  break ;
	    ((C4BCD *)result)->sig_dig-- ;
	 }
      }

   ptr =  (char *) ((C4BCD *)result)->bcd ; ;
   zero_count =  0 ;

   for ( n=0; pos <= last_pos; pos++ )
   {
      if ( input_ptr[pos] >= '0' && input_ptr[pos] <= '9' )
      {
	 if ( input_ptr[pos] == '0' )
	    zero_count++ ;
	 else
	    zero_count =  0 ;
	 if ( n >= 20 )  break ;
	 if ( n & 1 )
	    *ptr++ |=  input_ptr[pos] - '0' ;
	 else
	    *ptr  +=  (unsigned char)(input_ptr[pos]-'0') << 4 ;
      }
      else
      {
	 if ( input_ptr[pos] != '.'  ||  ! is_before_dec )
	    break ;

	 is_before_dec =  0 ;
	 continue ;
      }
      if ( is_before_dec )
	 ((C4BCD *)result)->sig_dig++ ;

      n++ ;
   }

   ((C4BCD *)result)->one =  1 ;
   ((C4BCD *)result)->len =  n - zero_count ;
   if ( ((C4BCD *)result)->len == 0 )
      ((C4BCD *)result)->sign =  0 ;
}

void  c4bcd_from_d( char *result, double doub )
{
   char  temp_str[258], *ptr ;
   int   sign, dec, len, pos ;

   ptr = ecvt( doub, E4ACCURACY_DIGITS, &dec, &sign ) ;
   if ( sign )
   {
      pos =  1 ;
      temp_str[0] = '-' ;
   }
   else
      pos =  0 ;

   if ( dec < 0 )
   {
      dec =  -dec ;
      len =  dec+1+pos ;
      memcpy( temp_str+len, ptr, E4ACCURACY_DIGITS ) ;
      memset( temp_str+pos, '0', len-pos ) ;
      temp_str[pos] =  '.' ;

      c4bcd_from_a( result, temp_str, E4ACCURACY_DIGITS + len ) ;
   }
   else
   {
      memcpy( temp_str+pos, ptr, dec ) ;
      pos +=  dec ;
      if ( dec < E4ACCURACY_DIGITS )
      {
         temp_str[pos++] =  '.' ;

	 len =  E4ACCURACY_DIGITS - dec ;
         memcpy( temp_str+pos, ptr+dec, len ) ;
         pos +=  len ;
      }
      c4bcd_from_a( result, temp_str, pos ) ;
   }
}

#endif   /*  ifndef  N4OTHER  */
