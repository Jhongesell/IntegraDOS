/* u4util.c   (c)Copyright Sequiter Software Inc., 1991-1992. All rights reserved. */

#include "d4all.h"
#ifdef __TURBOC__
   #pragma hdrstop
#endif

#include <time.h>

char   v4buffer[257] ;

/* '*was_old_len' contains the old len and will contain the new.
   'new_len' contains the new length    
*/
int S4FUNCTION u4alloc_again( C4CODE *c4, char **ptr_ptr, unsigned *was_old_len, unsigned new_len )
{
   char *new_ptr ;
   
   new_ptr =  (char *) u4alloc( new_len ) ;
   if ( new_ptr == 0 )
   {
      if ( c4 != 0 )
	 e4error( c4, e4memory, (char *) 0 ) ;
      return e4memory ;
   }

   if ( *ptr_ptr != 0 )
   {
      memcpy( new_ptr, *ptr_ptr, (size_t) *was_old_len ) ;
      u4free( *ptr_ptr ) ;
   }
   *ptr_ptr =  new_ptr ;
   *was_old_len =  new_len ;
   return 0 ;
}

unsigned S4FUNCTION u4ncpy( char *to, char *from, unsigned len )
{
   unsigned i ;

   #ifdef S4DEBUG
      if ( len == 0 )
         e4severe( e4parm, "u4ncpy()", (char *) 0 ) ;
   #endif
   len-- ;
   for ( i = 0;; i++ )
   {
      if ( i >= len )
      {
         to[len] =  0 ;
         return len ;
      }
      to[i] =  from[i] ;
      if ( from[i] == 0 )  return i ;
   }
}

int S4FUNCTION u4ptr_equal( void *p1, void *p2 )
{
   return( p1 == p2 ) ;
}

void S4FUNCTION u4yymmdd( char *yymmdd )
{
   time_t time_val ;
   struct tm *tm_ptr ;

   time( (time_t *) &time_val ) ;
   tm_ptr =   localtime( (time_t *) &time_val ) ;
   yymmdd[0] =  (char) tm_ptr->tm_year ;
   yymmdd[1] =  (char) tm_ptr->tm_mon+ (char) 1 ;
   yymmdd[2] =  (char) tm_ptr->tm_mday ;
}

int  S4FUNCTION u4remove( char *ptr )
{
   #ifdef S4NO_REMOVE
      char  buf[80] ;
      memset( buf, 0, sizeof(buf) ) ;
      strncpy( buf, ptr, sizeof(buf)-1 ) ;
      c4lower( buf ) ;
      return( unlink( buf ) ) ;
   #else
      return remove( ptr ) ;
   #endif
}

#ifndef S4NO_RENAME
   int S4FUNCTION u4rename( char *old_name, char *new_name )
   {
      return rename( old_name, new_name ) ;
   }
#endif

/*   memory comparison routines for foreign languages */

/*  v4map structure :  There are two lines of numbers in each row.
                       The upper line (commented out) represents ascii
                       values.  The lower line is the precedence value
                       of the corresponding ascii value as compared to 
                       the other ascii values.
*/
#ifdef S4LANGUAGE

#ifdef S4GERMAN

   /* This mapping is for German. */
   int v4map[256] =
   {
/*     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 */
       0,137,140,141,142,143,144,145,146,147,148,149,150,151,152,153,

/*    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 */
     154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,

/*    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 */
       1,115,130,121,122,107,120,132, 96, 97,108,105,110,106,109,116,

/*    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 */
       2,  3,  4,  5,  6,  7,  8,  9, 10, 11,112,111,102,104,103,114,

/*    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79 */
     119, 12, 15, 16, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 29, 31,

/*    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95 */
      33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 44, 98,117, 99,133,135,

/*    96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111 */
     131, 45, 51, 52, 54, 55, 60, 61, 62, 63, 68, 69, 70, 71, 72, 74,

/*   112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127 */
      79, 80, 81, 82, 84, 85, 90, 91, 92, 93, 95,100,118,101,134,170,

/*   128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143 */
      17, 86, 56, 49, 46, 48, 50, 53, 58, 59, 57, 67, 66, 65, 13, 14,

/*   144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159 */
      20,128,129, 78, 75, 77, 89, 88, 94, 32, 39,123,124,125,127,126,

/*   160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175 */
      47, 64, 76, 87, 73, 30,136,138,113,171,172,173,174,175,176,177,

/*   176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191 */
     178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,

/*   192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207 */
     194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,

/*   208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223 */
     210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,

/*   224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239 */
     226, 83,227,228,229,230,231,232,233,234,235,236,237,238,239,240,

/*   240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255 */
     241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,
   } ;


   typedef struct
   {
      int ext_char ;
      char *exp_chars ;
   } LANGUAGE_CONVERT ;

   LANGUAGE_CONVERT v4table[] =
   {
      {   0, ""   },   /* A blank entry to make the u4valid work better */
      { 129, "ue" },
      { 130, "e"  },
      { 131, "a"  },
      { 132, "ae" },
      { 133, "a"  },
      { 134, "a"  },
      { 136, "e"  },
      { 137, "e"  },
      { 138, "e"  },
      { 140, "i"  },
      { 141, "i"  },
#ifdef S4CLIPPER
      { 142, "AE" },
#else
      { 142, "Ae" },
#endif
      { 143, "A" },
/* The angstrom is not indexed in German correctly, so this is used instead*/
      { 148, "oe" },
      { 147, "o"  },
      { 149, "o"  },
      { 150, "u"  },
      { 151, "u"  },
      { 152, "y"  },
#ifdef S4CLIPPER
      { 153, "OE" },
      { 154, "UE" },
#else
      { 153, "Oe" },
      { 154, "Ue" },
#endif
      { 160, "a"  },
      { 161, "i"  },
      { 162, "o"  },
      { 163, "u"  },
      { 164, "n"  },
      { 225, "ss" }
   };
#define NUM_EXTENDED 27


static unsigned char *u4valid( unsigned char S4PTR* parm, int non_ext )
{
   char x = NUM_EXTENDED ;

   if( non_ext)  return parm ;

   for( ; x; x-- )
   {
      if( v4table[x].ext_char == *parm )
      {
         return (unsigned char *)v4table[x].exp_chars ;
      }
   }
   return parm ;
}

int S4CALL u4memcmp( unsigned char S4PTR *s1, unsigned char S4PTR *s2, size_t len )
{
   int  i, equal = 0 ;
   int  s1_ext, s2_ext ;
   unsigned char *replace1, *replace2 ;
   unsigned char hold1, hold2 ;
   unsigned char *s3, *s4 ;
   size_t length ;

   length = len ;

   s3 = s1;
   s4 = s2 ;
   hold1 = *(s1+len) ; 
   hold2 = *(s2+len) ;

   *(s1+len) = '\0' ;
   *(s2+len) = '\0' ;

   while( len-- )
   {
      s1_ext = 0 ; s2_ext = 0 ;

      if( *s1 != *s2 )
      {
         /* The characters are not equal.  Check for extended characters
            as in German the extended characters are equivalent to
            expanded characters  */
 
         s1_ext = ( *s1 < 127 ) ;
         s2_ext = ( *s2 < 127 ) ;
 
         if( !s1_ext ^ !s2_ext )
         {
         /* Only one is an  extended character. Check to see if valid and
            expand to full length */

            replace1 = u4valid( s1, s1_ext ) ;
            replace2 = u4valid( s2, s2_ext ) ;

            for( i = 2; i; i-- )
            {
               /* Loop two times, since currently extended characters
               can only be expanded to two characters. */

               if( *replace1 != *replace2 )
               {
                  if( !*replace1 || !*replace2 )
                  {
                  	s1_ext = 0 ; s2_ext = 0 ;

                     /* a single character equilivant was encountered */

                     if ( v4map[*s1] < v4map[*s2] ) equal = -1 ;
                     else equal = 1 ;
                     break ;
                  }

                  if ( v4map[*replace1] < v4map[*replace2] )
                  {
                     *(s3+length) = hold1 ; *(s4+length) = hold2 ;
                     return -1 ;
                  }
                  *(s3+length) = hold1 ; *(s4+length) = hold2 ;
                  return 1 ;
               }
               replace1++; replace2++ ;
            }
         }
         else
         {
            /* Neither character is extended so return according to
               v4map[].  */

            if ( v4map[*s1] < v4map[*s2] )
            {
               *(s3+length) = hold1 ; *(s4+length) = hold2 ;
               return -1 ;
            }
            *(s3+length) = hold1 ; *(s4+length) = hold2 ;
            return 1 ;
         }
      }
      /* Characters are equal. Increment the pointers and loop again. */

      s1+=(++s1_ext) ; s2+=(++s2_ext) ;
   }
   /* Length is 0 so the characters were equal to `len' characters.
      return a 0. */

   *(s3+length) = hold1 ; *(s4+length) = hold2 ;
   return equal ;
}

#endif  /*  ifdef S4GERMAN  */


#ifdef S4FRENCH

   /* This mapping is for French. */
   int v4map[256] =
   {
/*     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 */
     224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,

/*    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 */
     240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,  

/*    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 */
       0,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,

/*    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 */
      87, 88, 89, 90, 91, 92, 93, 94, 95, 96,145,160,161,162,163,164,

/*    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79 */
     165,  2, 13, 15, 19, 21, 28, 30, 32, 34, 40, 42, 44, 46, 48, 52,

/*    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95 */
      59, 61, 63, 65, 67, 69, 76, 78, 80, 82, 85,166,167,168,169,170,

/*    96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111 */
     171,  6, 14, 17, 20, 23, 29, 31, 33, 35, 41, 43, 45, 47, 50, 54,

/*   112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127 */
      60, 62, 64, 66, 68, 71, 77, 79, 81, 83, 86,172,173,174,175,146,

/*   128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143 */
      16, 74, 24,  8,  9,  7, 10, 18, 26, 27, 25, 37, 36, 38,  3,  4,

/*   144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159 */
      22, 12,  5, 55, 56, 57, 73, 72, 84, 53, 70,147,148,149,150,151,

/*   160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175 */
      11, 39, 58, 75, 51, 49, 97, 98,152,153,154,155,156,157,158,159,

/*   176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191 */
     222,223,176,177,178,179,180,181,182,183,184,185,186,187,188,189,

/*   192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207 */
     190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,

/*   208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223 */
     206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,

/*   224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239 */
      99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,

/*   240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255 */
     115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,  1,
   } ;

int S4CALL u4memcmp( unsigned char S4PTR *s1, unsigned char S4PTR *s2, size_t len )
{
   int i ;

   for (i=0; i<len; i++)
      if ( s1[i] != s2[i] )
      {
         if ( v4map[s1[i]] < v4map[s2[i]] )  return -1 ;
         return 1 ;
      }
   return 0 ;
}

#endif  /*   ifdef S4FRENCH  */
#endif  /*  ifdef S4LANGUAGE  */
