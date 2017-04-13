/* c4.c  Conversion Routines  (c)Copyright Sequiter Software Inc., 1991-1992.  All rights reserved. */

#include "d4all.h"
#include "e4error.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

/* c4atod

   Converts a string to a double
*/

int S4FUNCTION c4atod2( char *char_string, int string_len, double *result )
{
   *result = c4atod( char_string, string_len );
   return 0 ;
}

double  S4FUNCTION c4atod( char *char_string, int string_len )
{
   char buffer[50] ;
   int	len ;

   len = (string_len >= 50 ) ? 49 : string_len ; 
   memcpy( buffer, char_string, (size_t) len ) ;
   buffer[len] = '\0' ;
   return( strtod( buffer, (char **) 0)   ) ;    
}

/* c4atoi.c  */
int  S4FUNCTION c4atoi( char *ptr,  int n)
{
   char buf[128] ;
   if ( n >= sizeof(buf) )  n= sizeof(buf) -1 ;
   memcpy(buf, ptr, (size_t) n) ;
   buf[n] = '\0' ;
   return (atoi(buf) ) ;
}

/* c4atol  */

long  S4FUNCTION c4atol( char *ptr, int n)
{
   char buf[128] ;
   if ( n >= sizeof(buf) )  n= sizeof(buf) -1 ;
   memcpy(buf, ptr, (size_t) n) ;
   buf[n] = '\0' ;
   return (atol(buf) ) ;
}

/* c4dtoa45
   - formats a double to a string
   - if there is an overflow, '*' are returned
*/
void  S4FUNCTION c4dtoa45( double doub_val, char *out_buffer, int len, int dec)
{
   int	 dec_val, sign_val ;
   int	 pre_len, post_len, sign_pos ; /* pre and post decimal point lengths */
   char *result ;

   #ifdef S4DEBUG
      if ( len < 0 || len >128 || dec < 0 || dec >= len )
	 e4severe( e4info, "c4dtoa45", (char *) 0 ) ;
   #endif

   memset( out_buffer, (int) '0', (size_t) len) ;

   if (dec > 0)
   {
      post_len =  dec ;
      if (post_len > 15)     post_len =  15 ;
      if (post_len > len-1)  post_len =  len-1 ;
      pre_len  =  len -post_len -1 ;

      out_buffer[ pre_len] = '.' ;
   }
   else
   {
      pre_len  =  len ;
      post_len = 0 ;
   }

   result =  fcvt( doub_val, post_len, &dec_val, &sign_val) ;

   if (dec_val > 0)
      sign_pos =   pre_len-dec_val -1 ;
   else
   {
      sign_pos =   pre_len - 2 ;
      if ( pre_len == 1) sign_pos = 0 ;
   }

   if ( dec_val > pre_len ||  pre_len<0  ||  sign_pos< 0 && sign_val)
   {
      /* overflow */
      memset( out_buffer, (int) '*', (size_t) len) ;
      return ;
   }

   if (dec_val > 0)
   {
      memset( out_buffer, (int) ' ', (size_t) pre_len- dec_val) ;
      memmove( out_buffer+ pre_len- dec_val, result, (size_t) dec_val) ;
   }
   else
   {
      if (pre_len> 0)  memset( out_buffer, (int) ' ', (size_t) (pre_len-1)) ;
   }
   if ( sign_val)  out_buffer[sign_pos] = '-' ;


   out_buffer += pre_len+1 ;
   if (dec_val >= 0)
   {
      result+= dec_val ;
   }
   else
   {
      out_buffer    -= dec_val ;
      post_len += dec_val ;
   }

   if ( post_len > (int) strlen(result) )
      post_len =  (int) strlen( result) ;

   /*  - out_buffer   points to where the digits go to
       - result       points to where the digits are to be copied from
       - post_len     is the number to be copied
   */

   if (post_len > 0)   memmove( out_buffer, result, (size_t) post_len) ;
}

/* c4encode

   - From CCYYMMDD to CCYY.MM.DD

   Ex.	c4encode( to, from, "CCYY.MM.DD", "CCYYMMDD" ) ;
*/

void  S4FUNCTION c4encode( char *to, char *from, char *t_to, char *t_from)
{
   int	pos ;
   char chr ;
   char * chr_pos ;

   strcpy( to, t_to ) ;

   while ( (chr=*t_from++)  != 0)
   {
      if (  (chr_pos= strchr(t_to,chr)) ==0)
      {
	 from++;
	 continue ;
      }

      pos = (int) (chr_pos - t_to) ;
      to[pos++] = *from++ ;

      while (chr== *t_from)
      {
	 if (chr== t_to[pos])
	    to[pos++] = *from ;
	 t_from++ ;
	 from++ ;
      }
   }
}

/*  c4ltoa45

    Converts a RECNUM to a string.  Fill with '0's rather than blanks if
    'num' is less than zero.
*/

void  S4FUNCTION c4ltoa45( long l_val, char *ptr, int num)
{
   int	 n, num_pos ;
   long  i_long ;

   i_long =  (l_val>0) ? l_val : -l_val ;
   num_pos =  n =  (num > 0) ? num : -num ;

   while (n-- > 0)
   {
      ptr[n] = (char) ('0'+ i_long%10) ;
      i_long = i_long/10 ;
   }

   if ( i_long > 0 )
   {
     memset( ptr, (int) '*', (size_t) num_pos ) ;
     return ;
   }

   num--;
   for (n=0; n<num; n++)
      if (ptr[n]=='0')
	 ptr[n]= ' ';
      else
	 break ;

   if (l_val < 0)
   {
      if ( ptr[0] != ' ' )
      {
	 memset( ptr, (int) '*', (size_t) num_pos ) ;
	 return ;
      }
      for (n=num; n>=0; n--)
	 if (ptr[n]==' ')
	 {
	    ptr[n]= '-' ;
	    break ;
	 }
   }
}

void S4FUNCTION c4trim_n( char *ptr, int num_chars )
{
   int	len ;

   if ( num_chars <= 0 )  return ;

   /* Count the Length */
   len =  0 ;
   while ( len< num_chars )
   {
      len++ ;
      if ( *ptr++ == '\000' )  break ;
   }

   if ( len < num_chars )  num_chars =	len ;


   *(--ptr) =  '\000' ;

   while( --num_chars >= 0 )
   {
      ptr-- ;
      if ( *ptr == '\000' ||  *ptr == ' ' )
	 *ptr =  '\000' ;
      else
	 break ;
   }
}

#ifndef S4LANGUAGE

void S4FUNCTION c4lower( char *str )
{
#ifdef S4NO_STRLWR
   char *ptr ;
   
   ptr =  str ;
   
   while ( *ptr != '\000' )
   {
      if ( *ptr >= 'A' && *ptr <= 'Z' )
       *ptr |=  040 ;
      ptr++ ;
   }
#else
   strlwr( str ) ;
#endif
}

void  S4FUNCTION c4upper( char *str )
{
#ifdef S4NO_STRUPR
   char *ptr ;
   
   ptr =  str ;
   
   while ( *ptr != '\000' )
   {
      if ( *ptr >= 'a'  &&  *ptr <= 'z' )
       *ptr &=  0337 ;
      ptr++ ;
   }
#else
   strupr( str ) ;
#endif
}

#else  /* ifdef S4LANGUAGE  */

#ifdef S4GERMAN

void  S4FUNCTION c4upper( char *str )
{
   char *ptr ;

   ptr =  str ;

   while ( *ptr != '\000' )
   {
      if ( *ptr >= 'a'  &&  *ptr <= 'z' )
         *ptr &=  0337 ;
    
      if ( *ptr >= '\201' )
      {
         switch( *ptr )
         {
      	  case '\204':
      	     *ptr =  '\216' ;
      	     break ;
      	  case '\224':
      	     *ptr =  '\231' ;
      	     break ;
      	  case (char) '\201':
      	     *ptr =  '\232' ;
      	     break ;
         }
      }
      ptr++ ;
   }
}

void S4FUNCTION c4lower( char *str )
{
   char *ptr ;

   ptr =  str ;

   while ( *ptr != '\000' )
   {
      if ( *ptr >= 'A' && *ptr <= 'Z' )
         *ptr |=  040 ;
      if ( *ptr >= '\201' )
      {
         switch( *ptr )
         {
            case '\216':
      	       *ptr = '\204' ;
      	       break ;
            case '\231':
      	       *ptr = '\224' ;
      	       break ;
            case '\232':
      	       *ptr = '\201' ;
      	       break ;
         }
      }
      ptr++ ;
   }
}

#else
#ifdef S4FRENCH

void  S4FUNCTION c4upper( char *str )
{
   char *ptr ;

   ptr =  str ;

   while ( *ptr != '\000' )
   {
      if ( *ptr >= 'a'  &&  *ptr <= 'z' )
         *ptr &=  0337 ;

      /* Les accents sont laiss‚ afin d'ˆtre compatible avec toute p‚riph‚rie */ 
      if ( *ptr >= E4C_CED )
      {
         switch( *ptr )
         {
            case E4A_TRE :
            case E4A_GRA :
      	    case E4A_CIR :
      	    case E4A_CI2 :
            case E4A_EGU :
      	       *ptr =  'A' ;        /* A */
      	       break ;
            case E4C_CED :      
               *ptr =  'C' ;        /* C */
               break ;	    	  
      	    case E4E_EGU :
            case E4E_GRA :
      	    case E4E_CIR :
      	    case E4E_TRE :
               *ptr =  'E' ;        /* E */
               break ;
      	    case E4I_TRE :
            case E4I_EGU :
            case E4I_GRA :
      	    case E4I_CIR :
               *ptr =  'I' ;        /* I */
               break ;
      	    case E4U_CIR :
      	    case E4U_TRE :
      	    case E4U_GRA :
      	    case E4U_EGU :
               *ptr =  'O' ;        /* O */
               break ;
      	    case E4O_CIR :
      	    case E4O_GRA :
      	    case E4O_TRE :
      	    case E4O_EGU :
               *ptr =  'U' ;        /* U */
               break ;
      	    case E4Y_TRE :
               *ptr =  'Y' ;        /* Y */
               break ;
         }
      }
      ptr++ ;
   }
}
   
void S4FUNCTION c4lower( char *str )
{
   char *ptr ;

   ptr =  str ;

   while ( *ptr != '\000' )
   {
      if ( *ptr >= 'A' && *ptr <= 'Z' )
         *ptr |=  040 ;

      /* Les accents sont laiss‚ afin d'ˆtre compatible avec toute p‚riph‚rie */
      if ( *ptr >= E4C_CED )
      {
         switch( *ptr )
         {
            case E4CM_CED:
               *ptr =  'c' ;       /* c */
               break ;
            case E4AM_TRE:
            case E4AM_CIR:
               *ptr =  'a' ;       /* a */
               break ;
            case E4EM_EGU:
               *ptr =  'e' ;       /* e */
               break ;
            case E4OM_TRE:
               *ptr =  'o' ;       /* o */
               break ;
            case E4UM_TRE:
               *ptr =  'u' ;       /* u */
               break ;
         }
      }
      ptr++ ;
   }
}

#else
   ERROR:  no language specified
#endif   /*  ifdef S4FRENCH  */
#endif   /*  ifdef S4GERMAN  */

#endif   /* ifndef S4LANGUAGE  */


#ifdef S4CLIPPER
/* Numeric Key Database Output is Converted to Numeric Key Index File format */
int   S4FUNCTION c4clip( char *ptr, int len)
{
   int	 i, negative ;
   char *p ;

   for ( i= negative= 0, p= ptr; i< len; i++, p++ )
   {
      if ( *p == ' ' )
      {
	 *p =  '0' ;
      }
      else
      {
	 if ( *p == '-' )
	 {
	    *p =  '0' ;
	    negative =	1 ;
	 }
	 else
	    break ;
      }
   }

   if ( negative )
   {
      for ( i= 0, p= ptr; i< len; i++, p++ )
	 *p =  (char) 0x5c - *p ;
   }

   return 0 ;
}
#endif

#ifdef S4NO_MEMMOVE
void *memmove(void *dest, void *src, size_t count )
{
   if ( dest < src )
      if ( (char *)dest + count  <= (char *) src )
      {
         memcpy( dest, src, count ) ;
	 return( src ) ;
      }
      else
      {
	 /* Start at beginning of 'src' */
	 int  i ;
	 for ( i=0; i< count; i++ )
	    ((char *) dest)[i] =  ((char *)src)[i] ;
      }

   if ( src < dest )
      if ( (char *) src + count  <=  (char *) dest )
      {
         memcpy( dest, src, count ) ;
	 return( src ) ;
      }
      else
      {
	 /* Start at end of 'src' */
	 for(;count!=0;)
         {
            --count ;
	    ((char *)dest)[count] =  ((char *) src)[count] ;
         }
      }

   return( src ) ;
}
#endif

#ifdef S4NO_STRNICMP
int strnicmp(char *a, char *b, size_t n )
{
   unsigned char  a_char, b_char ;

   for ( ; *a != '\000'  &&  *b != '\000' && n != 0; a++, b++, n-- )
   {
      a_char =  (unsigned char) *a  & 0xDF ;
      b_char =  (unsigned char) *b  & 0xDF ;

      if ( a_char < b_char )  return -1 ;
      if ( a_char > b_char )  return  1 ;
   }
   return 0 ;
}
#endif
